/**********************************************************//**
 * @file battle.c
 * @brief Implements battle system.
 * @author Rena Shinomiya
 * @date May 06, 2018
 **************************************************************/

#include <stdio.h>              // snprintf
#include <math.h>               // fmod

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "random.h"             // randint
#include "assets.h"             // MiscImage
#include "species.h"            // SPECTRA
#include "technique.h"          // TECHNIQUE
#include "menu.h"               // MENU
#include "player.h"             // Player
#include "battler.h"            // BATTLER
#include "battle.h"             // TEAM
#include "type.h"               // TYPE
#include "output.h"             // Output

/**************************************************************/
/// @brief Number of battlers in the whole battle.
#define BATTLE_SIZE (TEAM_SIZE+TEAM_SIZE)

/**************************************************************/
/// @brief Battlers in the player's team.
static TEAM PlayerTeam;

/// @brief Battlers in the enemy team.
static TEAM EnemyTeam;

/// @brief Buffer for enemy spectra.
static SPECTRA EnemySpectra[TEAM_SIZE];

/// @brief The type of encounter occurring.
static ENCOUNTER_TYPE EncounterType;

/**********************************************************//**
 * @enum TURN_STATE
 * @brief Represents the state of each active turn.
 **************************************************************/
typedef enum {
    TURN_INACTIVE       = 0,
    TURN_PENDING        = 1,
    TURN_ACTIVE         = 2,
    TURN_RESULT         = 3,
    TURN_DONE           = 4,
} TURN_STATE;

/**********************************************************//**
 * @struct TURN
 * @brief Represents all information required to perform
 * one battler's turn.
 **************************************************************/
typedef struct {
    TURN_STATE State;
    int User;
    TECHNIQUE_ID Technique;
    int Target;
} TURN;

/// @brief Buffer for actions the battlers will take on this turn.
static TURN Turns[BATTLE_SIZE];

/**********************************************************//**
 * @struct PLAYER_MENU
 * @brief Temporary buffer for the menus needed for the
 * player's spectra.
 **************************************************************/
typedef struct {
    MENU TechniqueMenu;
    MENU CostMenu;
    char CostString[MOVESET_SIZE][4];
} PLAYER_MENU;

/// @brief Buffers the player's spectra's battle menus.
static PLAYER_MENU PlayerMenu[TEAM_SIZE];

/// @brief Temporary buffer for target menu.
static MENU TargetMenu;

/// @brief Temporary buffer for strings used on the TargetMenu.
static char EnemyNames[TEAM_SIZE][21];

/// @brief ID of each enemy on the TargetMenu.
static int TargetID[BATTLE_SIZE];

/**********************************************************//**
 * @enum BATTLE_MENU_OPTION
 * @brief Enumerates options in the battle menu.
 **************************************************************/
typedef enum {
    BATTLE_ATTACK,
    BATTLE_DEFEND,
    BATTLE_TECHNIQUE,
    BATTLE_ITEM,
    BATTLE_CANCEL,
} BATTLE_MENU_OPTION;

/// @brief Defines the battle menu and its control structure.
static MENU BattleMenu = {
    .Option = {
        [BATTLE_ATTACK]         = "Attack",
        [BATTLE_DEFEND]         = "Defend",
        [BATTLE_TECHNIQUE]      = "Technique",
        [BATTLE_ITEM]           = "Item",
        // This item periodically changes between
        // "Escape" and "Cancel" depending on whose
        // turn it is.
        [BATTLE_CANCEL]         = "Escape",
    },
    .Control = {
        .IndexMax               = 4,
    },
};

/**************************************************************/
/// @brief Used in battle menu code to denote whose turn
/// is being inputted by the user.
static int CurrentUser = 0;

/// @brief Turn being executed currently.
static TURN *CurrentTurn = NULL;


/// @brief Centered coordinates where battlers should be drawn.static const COORDINATE BattlerPosition[] = {
    // User's team
    [0] = { 60, 260},
    [1] = {110, 240},
    [2] = {160, 220},
    
    // Enemy's team
    [3] = {320, 220},
    [4] = {370, 240},
    [5] = {420, 260},
};

/**********************************************************//**
 * @brief Gets the team the BATTLER belongs to.
 * @param id: BATTLER's unique ID.
 * @return Pointer to the BATTLER's team.
 **************************************************************/
static inline TEAM *TeamOf(int id) {
    return (id<TEAM_SIZE)? &PlayerTeam: &EnemyTeam;
}

/**********************************************************//**
 * @brief Gets the BATTLER data from its ID.
 * @param id: Unique ID of the BATTLER.
 * @return Pointer to the BATTLER's data.
 **************************************************************/
static inline BATTLER *BattlerByID(int id) {
    if (id<TEAM_SIZE) {
        return &PlayerTeam.Member[id];
    } else {
        return &EnemyTeam.Member[id-TEAM_SIZE];
    }
}

/**********************************************************//**
 * @brief Shortcut to get the name of a battler for output.
 * @param id: ID of the battler.
 * @return The battler's name.
 **************************************************************/
static inline const char *BattlerNameByID(int id) {
    return BattlerName(BattlerByID(id));
}

/**********************************************************//**
 * @brief Sets up the player's team.
 **************************************************************/
static void InitializePlayerTeam(void) {
    for (int id=0; id<TEAM_SIZE; id++) {
        // Collect data that we'll use to set up the battler.
        SPECTRA *spectra = &Player->Spectra[id];
        PLAYER_MENU *menu = &PlayerMenu[id];
        BATTLER *battler = &PlayerTeam.Member[id];
        
        // Initialize battler data
        if (spectra->Species) {
            InitializeBattler(battler, spectra);
            
            // Initialize menu data
            int t;
            for (t=0; spectra->Moveset[t]; t++) {
                const TECHNIQUE *technique = TechniqueByID(spectra->Moveset[t]);
                menu->TechniqueMenu.Option[t] = technique->Name;
                snprintf(menu->CostString[t], 3, "%d", technique->Cost);
                menu->CostMenu.Option[t] = menu->CostString[t];
            }
            ResetControl(&menu->TechniqueMenu.Control);
            ResetControl(&menu->CostMenu.Control);
            menu->TechniqueMenu.Control.IndexMax = t-1;
            menu->TechniqueMenu.Control.ScrollMax = 0;
            menu->CostMenu.Control.IndexMax = t-1;
            menu->CostMenu.Control.ScrollMax = 0;
        } else {
            InitializeBattlerAsInactive(battler);
        }
    }
}

/**********************************************************//**
 * @brief Generates a random ENEMY based on the encounters
 * defined for the map.
 * @param enemy: ENEMY to generate.
 * @param encounters: ENCOUNTER data from the map.
 **************************************************************/
static void RandomEnemy(ENEMY *enemy, const ENCOUNTER *encounters) {
    int chance = randint(0, 99);
    const ENCOUNTER *encounter = NULL;
    for (int i=0, total=0; chance<total; total+=encounters[i++].Chance) {
        encounter = &encounters[i];
    }
    if (encounter) {
        enemy->Species = encounter->Spectra;
        enemy->Level = randint(encounter->LevelRange[0], encounter->LevelRange[1]);
    } else {
        // Summoned enemy for location where no enemies are defined.
        // Make do with this easter egg.
        enemy->Species = PRGMERROR;
        enemy->Level = 80;
    }
}

/**********************************************************//**
 * @brief Set up the enemy's team.
 * @param enemies: Basic ENEMY data to set up.
 **************************************************************/
static void InitializeEnemyTeam(const ENEMY *enemies) {
    for (int id=0; id<TEAM_SIZE; id++) {
        BATTLER *enemy = &EnemyTeam.Member[id];
        if (enemies[id].Species) {
            CreateSpectra(&EnemySpectra[id], enemies[id].Species, enemies[id].Level);
            InitializeBattler(enemy, &EnemySpectra[id]);
        } else {
            EnemySpectra[id].Species = 0;
            InitializeBattlerAsInactive(enemy);
        }
    }
}

/**********************************************************//**
 * @brief Initializes setup for the current round of battle.
 **************************************************************/
static void InitializeRound(void) {
    CurrentUser = 0;
    CurrentTurn = NULL;
    ResetControl(&BattleMenu.Control);
}

/**********************************************************//**
 * @brief Set up a new random encounter with spectra on the
 * overworld (normal or fishing).
 * @param count: Number of enemies.
 * @param type: Type of random encounter.
 **************************************************************/
void InitializeRandomEncounter(int count, ENCOUNTER_TYPE type) {
    // Get the encounter table
    const ENCOUNTER *encounters = NULL;
    switch (type) {
    case ENCOUNTER_FISHING:
        encounters = Location(Player->Location)->Fishing;
        break;
    case ENCOUNTER_OVERWORLD:
        encounters = Location(Player->Location)->Encounters;
        break;
    default:
        break;
    }
    
    // Get random enemies
    ENEMY enemies[TEAM_SIZE];
    for (int i=0; i<TEAM_SIZE; i++) {
        if (i < count) {
            RandomEnemy(&enemies[i], encounters);
        } else {
            enemies[i].Species = 0;
        }
    }
    InitializeEnemyTeam(enemies);
    InitializePlayerTeam();
    EncounterType = type;
    InitializeRound();
}

/**********************************************************//**
 * @brief Set up a non-random encounter with spectra.
 * @param bosses: Boss enemies to generate.
 **************************************************************/
void InitializeBossEncounter(const BOSS *bosses) {
    InitializeEnemyTeam(bosses->Boss);
    InitializePlayerTeam();
    EncounterType = ENCOUNTER_BOSS;
    InitializeRound();
}

/**********************************************************//**
 * @brief Gets a list of target IDs for the given type.
 * @param targets: Location to store the list of targets.
 * @param user: User ID
 * @param type: Targetting type
 * @return Number of targets loaded.
 **************************************************************/
static int GetTargets(int *targets, int user, TARGET_TYPE type) {
    int i = 0;
    for (int id=0; id<BATTLE_SIZE; id++) {
        const BATTLER *battler = BattlerByID(id);
        // Can't target gone or incapacitated battlers
        if (!IsAlive(battler) || !battler->Spectra) {
            continue;
        } else if (type&TARGET_USER && id==user) {
            // Targetting yourself
            targets[i++] = id;
        } else if (type&TARGET_ALLY && TeamOf(id)==TeamOf(user)) {
            // Targetting any ally
            targets[i++] = id;
        } else if (type&TARGET_ENEMY && TeamOf(id)!=TeamOf(user)) {
            // Targetting any enemy
            targets[i++] = id;
        }
    }
    return i;
}

/**********************************************************//**
 * @brief Sets up the target menu for the current user.
 * @param type: What the range of the technique is.
 **************************************************************/
static void LoadTargetMenu(TARGET_TYPE type) {
    // Only one group ever, so no menu needed.
    if (type&TARGET_GROUP) {
        if (type&TARGET_ALLY&TARGET_ENEMY) {
            TargetMenu.Option[0] = "Everyone";
        } else if (type&TARGET_ALLY) {
            TargetMenu.Option[0] = "Allies";
        } else if (type&TARGET_ENEMY) {
            TargetMenu.Option[0] = "Enemies";
        }
        TargetID[0] = -1;
        TargetMenu.Control.IndexMax = 1;
        TargetMenu.Control.ScrollMax = 0;
        return;
    }
    
    // Select targets
    int nTargets = GetTargets(TargetID, CurrentUser, type);
    int i;
    for (i=0; i<nTargets; i++) {
        int id = TargetID[i];
        if (id==CurrentUser) {
            TargetMenu.Option[i] = "Yourself";
        } else if (TargetID[i]>=TEAM_SIZE && type&TARGET_ALLY) {
            // Disambiguate enemies and users if necessary
            snprintf(EnemyNames[id-TEAM_SIZE], 20, "Enemy %s", BattlerNameByID(id));
            TargetMenu.Option[i] = EnemyNames[id-TEAM_SIZE];
        } else {
            TargetMenu.Option[i] = BattlerNameByID(id);
        }
    }
    
    // Reset
    TargetMenu.Control.IndexMax = i-1;
    while (i < 6) {
        TargetMenu.Option[i++] = NULL;
    }
    ResetControl(&TargetMenu.Control);
    TargetMenu.Control.ScrollMax = 0;
}

/**********************************************************//**
 * @brief Check if the user is done inputting turns.
 * @return True if the user is finished.
 **************************************************************/
static inline bool BattleMenuDone(void) {
    return CurrentUser>=TEAM_SIZE || !PlayerTeam.Member[CurrentUser].Spectra;
}

/**********************************************************//**
 * @brief Draws the main battle menu system on the screen.
 **************************************************************/
static void DrawBattleMenu(void) {
    // Escape/Cancel display
    BattleMenu.Option[BATTLE_CANCEL] = CurrentUser? "Cancel": "Escape";
    DrawAt(4, 275);
    DrawOption(&BattleMenu);
    if (BattleMenu.Control.State == CONTROL_CONFIRM) {
        switch (MenuItem(&BattleMenu)) {
        case BATTLE_ATTACK:
        case BATTLE_DEFEND:
            // Shortcut techniques...
            DrawAt(108, 275);
            DrawOption(&TargetMenu);
            break;
        
        case BATTLE_TECHNIQUE:
            // Technique menu is open
            DrawAt(108, 249);
            DrawColumn(&PlayerMenu[CurrentUser].TechniqueMenu, &PlayerMenu[CurrentUser].CostMenu);
            if (PlayerMenu[CurrentUser].TechniqueMenu.Control.State == CONTROL_CONFIRM) {
                DrawAt(254, 275);
                DrawOption(&TargetMenu);
            }
            break;
        
        case BATTLE_ITEM:
            // TODO
            break;
        
        case BATTLE_CANCEL:
            // Only if first user - escape
            if (CurrentUser == 0) {
                DrawAt(108, 275);
                DrawOption(&TargetMenu);
            }
            break;
        }
    }
}

/**********************************************************//**
 * @brief Updates the targetting menu.
 **************************************************************/
static void UpdateTargetMenu(void) {
    TURN *turn = &Turns[CurrentUser];
    switch (TargetMenu.Control.State) {
    case CONTROL_CONFIRM:
        // Confirm the entire turn
        turn->Target = TargetID[MenuItem(&TargetMenu)];
        turn->State = TURN_PENDING;
        
        // Maybe move to the next battler
        CurrentUser++;
        if (!BattleMenuDone()) {
            ResetControl(&PlayerMenu[CurrentUser].TechniqueMenu.Control);
            ResetControl(&BattleMenu.Control);
        }
        break;

    case CONTROL_CANCEL:
        // Revert to the previous menu.        if (PlayerMenu[CurrentUser].TechniqueMenu.Control.State) {
            PlayerMenu[CurrentUser].TechniqueMenu.Control.State = CONTROL_IDLE;
        } else {
            BattleMenu.Control.State = CONTROL_IDLE;
        }
        break;

    case CONTROL_IDLE:
        UpdateControl(&TargetMenu.Control);
        break;
    }
}

/**********************************************************//**
 * @brief Updates the battle menu for the user to input turns.
 **************************************************************/
static void UpdateBattleMenu(void) {
    // Set up the current turn
    TURN *turn = &Turns[CurrentUser];
    turn->User = CurrentUser;
    
    // Update battle menu system
    switch (BattleMenu.Control.State) {
    case CONTROL_CONFIRM:
        switch (MenuItem(&BattleMenu)) {
        case BATTLE_ATTACK:
            UpdateTargetMenu();
            break;
            
        case BATTLE_DEFEND:
            UpdateTargetMenu();
            break;
        
        case BATTLE_TECHNIQUE:
            // Technique menu is open
            switch (PlayerMenu[CurrentUser].TechniqueMenu.Control.State) {
            case CONTROL_CONFIRM:
                UpdateTargetMenu();
                break;
            
            case CONTROL_CANCEL:
                BattleMenu.Control.State = CONTROL_IDLE;
                break;
            
            case CONTROL_IDLE:
                UpdateControl(&PlayerMenu[CurrentUser].TechniqueMenu.Control);
                if (PlayerMenu[CurrentUser].TechniqueMenu.Control.State == CONTROL_CONFIRM) {
                    turn->Technique = BattlerByID(CurrentUser)->Spectra->Moveset[MenuItem(&PlayerMenu[CurrentUser].TechniqueMenu)];
                    LoadTargetMenu(TechniqueByID(turn->Technique)->Target);
                }
                break;
            }
            break;
        
        case BATTLE_ITEM:
            // TODO
            break;
        
        case BATTLE_CANCEL:
            // Go back to the previous user, or try to escape the battle.
            if (CurrentUser > 0) {
                CurrentUser--;
                ResetControl(&PlayerMenu[CurrentUser].TechniqueMenu.Control);
            } else if (CurrentUser == 0) {
                UpdateTargetMenu();
            }
            break;
        }
        break;
    
    case CONTROL_IDLE:
        // Using battle control menu...
        UpdateControl(&BattleMenu.Control);
        if (BattleMenu.Control.State == CONTROL_CONFIRM) {
            // Initialize submenu
            switch (MenuItem(&BattleMenu)) {
            case BATTLE_ATTACK:
                turn->Technique = DEFAULT_ATTACK,
                LoadTargetMenu(TechniqueByID(turn->Technique)->Target);
                break;
            case BATTLE_DEFEND:
                turn->Technique = DEFAULT_DEFEND,
                LoadTargetMenu(TechniqueByID(turn->Technique)->Target);
                break;
            case BATTLE_TECHNIQUE:
                ResetControl(&PlayerMenu[CurrentUser].TechniqueMenu.Control);
                break;
            case BATTLE_ITEM:
                // TODO
                break;
            case BATTLE_CANCEL:
                if (CurrentUser == 0) {
                    turn->Technique = DEFAULT_ESCAPE,
                    LoadTargetMenu(TechniqueByID(turn->Technique)->Target);
                }
                break;
            }
        }
        break;
    
    case CONTROL_CANCEL:
        if (CurrentUser > 0) {
            CurrentUser--;
            ResetControl(&BattleMenu.Control);
        }
        break;
    }
}

/**********************************************************//**
 * @brief Randomly generates turns for all the enemies in
 * the current battle.
 **************************************************************/
static void LoadEnemyTurns(void) {
    for (int id=TEAM_SIZE; id<BATTLE_SIZE; id++) {
        BATTLER *enemy = BattlerByID(id);
        TURN *turn = &Turns[id];
        if (enemy->Spectra && IsAlive(enemy)) {
            turn->State = TURN_PENDING;
            turn->User = id;
            
            // Get a list of legal techniques to use here
            TECHNIQUE_ID usableTechniques[MOVESET_SIZE+2];
            usableTechniques[0] = DEFAULT_ATTACK;
            usableTechniques[1] = DEFAULT_DEFEND;
            int u = 2;
            for (int t=0; t<enemy->Spectra->MovesetSize; t++) {
                const TECHNIQUE *technique = TechniqueByID(enemy->Spectra->Moveset[t]);
                if (technique->Cost > enemy->Spectra->Power) {
                    continue;
                }
                usableTechniques[u++] = enemy->Spectra->Moveset[t];
            }
            turn->Technique = usableTechniques[randint(0,u)];
            
            // Get the primary target, if applicable
            TARGET_TYPE type = TechniqueByID(turn->Technique)->Target;
            if (type&TARGET_GROUP) {
                turn->Target = -1;
            } else {
                int targets[BATTLE_SIZE];
                int nTargets = GetTargets(targets, id, type);
                turn->Target = targets[randint(0,nTargets)];
            }
        } else {
            // Turn skipped
            turn->State = TURN_DONE;
        }
    }
}

/**********************************************************//**
 * @brief Performs one turn during battle.
 * @param turn: Turn to execute.
 **************************************************************/
static void ExecuteTurn(const TURN *turn) {
    // Get the targets of the technique
    int Targets[BATTLE_SIZE];
    TARGET_TYPE targetType = TechniqueByID(turn->Technique)->Target;
    int nTargets;
    if (targetType & TARGET_GROUP) {
        nTargets = GetTargets(Targets, turn->User, targetType);
    } else {
        Targets[0] = turn->Target;
        nTargets = 1;
    }
    
    // Perform the turn
    BATTLER *user = BattlerByID(turn->User);
    const TECHNIQUE *technique = TechniqueByID(turn->Technique);
    for (int i=0; i<nTargets; i++) {
        BATTLER *target = BattlerByID(Targets[i]);
        
        // Inflict damage
        int damage = 0;
        if (technique->Power) {
            float ratio = (float)BattlerAttack(user)/BattlerDefend(target);
            float scale = (float)user->Spectra->Level/target->Spectra->Level;
            const TYPE_ID *targetType = SpeciesByID(target->Spectra->Species)->Type;
            float matchup = TypeMatchup(technique->Type, targetType[0]);
            if (targetType[1]) {
                matchup *= TypeMatchup(technique->Type, targetType[1]);
            }
            damage = 1 + technique->Power*ratio*scale*matchup;
            target->Spectra->Health -= damage;
            if (target->Spectra->Health < 0) {
                target->Spectra->Health = 0;
            }
            
            // Messages
            if (damage) {
                OutputF("%s took %d damage!", BattlerName(target), damage);
            } else {
                OutputF("%s didn't take any damage!", BattlerName(target));
            }
        }
        
        // Apply effect
        if (ShouldEffectActivate(technique->Effect, technique->Argument)) {
            ApplyEffectInBattle(technique->Effect, user, target, technique->Argument);
        } else if (!technique->Power) {
            // Technique doesn't do damage, and effect missed.
            OutputF("%s avoided the attack!", BattlerName(target));
        }
    }
}

/**********************************************************//**
 * @brief Updates a step of the battle system during each
 * frame of rendering.
 **************************************************************/
static void UpdateBattleExecution(void) {
    // Update to the next turn; if needed.
    if (!CurrentTurn || CurrentTurn->State==TURN_DONE) {
        CurrentTurn = NULL;
        int maxPriority = 0;
        for (int i=0; i<BATTLE_SIZE; i++) {
            if (Turns[i].State == TURN_PENDING) {
                int priority = BattlerEvade(BattlerByID(i));
                if (priority > maxPriority || !CurrentTurn) {
                    maxPriority = priority;
                    CurrentTurn = &Turns[i];
                }
            }
        }
        if (!CurrentTurn) {
            return;
        }
    }
    
    // Execute the current turn
    switch (CurrentTurn->State) {
    case TURN_PENDING:
        CurrentTurn->State = TURN_ACTIVE;
        const char *technique = TechniqueByID(CurrentTurn->Technique)->Name;
        OutputF("%s used %s!", BattlerNameByID(CurrentTurn->User), technique);
        break;
    
    case TURN_ACTIVE:
        // Await previous phase completion
        UpdateOutput();
        if (OutputDone()) {
            CurrentTurn->State = TURN_RESULT;
            ExecuteTurn(CurrentTurn);
        }
        break;
    
    case TURN_RESULT:
        // Await previous phase completion
        UpdateOutput();
        if (OutputDone()) {
            CurrentTurn->State = TURN_DONE;
        }
        break;
    default:
        break;
    }
}

/**********************************************************//**
 * @brief Determines if the battle execution step has finished.
 * @return True if battle execution is done and we should go
 * to the next turn.
 **************************************************************/
static bool BattleExecutionDone(void) {
    for (int i=0; i<BATTLE_SIZE; i++) {
        if (Turns[i].State != TURN_DONE) {
            return false;
        }
    }
    return true;
}

/**********************************************************//**
 * @brief Updates one step of the battle system.
 **************************************************************/
void UpdateBattle(void) {
    if (!BattleMenuDone()) {
        UpdateBattleMenu();
        // Get enemy turns
        if (BattleMenuDone()) {
            LoadEnemyTurns();
        }
    } else if (!BattleExecutionDone()) {
        UpdateBattleExecution();
        if (BattleExecutionDone()) {
            InitializeRound();
        }
    }
}

/**********************************************************//**
 * @brief Draws each battler's sprite on the screen.
 **************************************************************/
static void DrawBattlers(void) {
    // Temp target?
    int target = -1;
    if (TargetMenu.Control.State == CONTROL_IDLE) {
        target = TargetID[MenuItem(&TargetMenu)];
    }

    // Draw drop shadows
    for (int id=0; id<BATTLE_SIZE; id++) {
        const COORDINATE *center = &BattlerPosition[id];
        if (!BattlerByID(id)) {
            continue;
        }
        
        // Different colors for seletion
        ALLEGRO_COLOR color = al_map_rgba(0, 0, 0, 60);
        if (id == CurrentUser) {
            color = al_map_rgba(0, 127, 255, 200);
        } else if (id == target) {
            color = al_map_rgba(255, 20, 0, 200);
        }
        al_draw_filled_ellipse(center->X, center->Y, 40, 10, color);
    }
    
    // Draw existing spectra in layer order
    static const int Order[] = {3, 2, 4, 1, 5, 0};
    for (int i=0; i<6; i++) {
        int id = Order[i];
        if (!BattlerByID(id)) {
            continue;
        }
        SPECIES_ID speciesID = BattlerByID(id)->Spectra->Species;
        const SPECIES *species = SpeciesByID(speciesID);
        const COORDINATE *center = &BattlerPosition[id];
        const COORDINATE *offset = &species->Offset;
        ALLEGRO_BITMAP *image = NULL;
        if (speciesID == AMY) {
            image = CostumeImage(Player->Costume);
        } else {
            image = SpeciesImage(speciesID);
        }
        al_draw_bitmap(image, center->X-offset->X, center->Y-offset->Y, 0);
    }
}

/**********************************************************//**
 * @brief Draws all the battle HUDs on the screen.
 **************************************************************/
static void DrawHUDs(void) {
    // User HUDs
    int y = 4;
    for (int id=0; id<3 && PlayerTeam.Member[id].Spectra; id++) {
        DrawAt(4, y);
        DrawHudUser(PlayerTeam.Member[id].Spectra);
        // Hud tag
        if (id==CurrentUser) {
            al_draw_bitmap(MiscImage(HUD_UP), 200, 5, 0);
        } else if (id<CurrentUser) {
            al_draw_bitmap(MiscImage(HUD_OK), 200, 5, 0);
        }
        y += 29;
    }
    
    // Enemy HUDs
    y = 4;
    for (int id=0; id<3 && EnemyTeam.Member[id].Spectra; id++) {
        DrawAt(275, y);
        DrawHudEnemy(EnemyTeam.Member[id].Spectra);
        y += 29;
    }
}

/**********************************************************//**
 * @brief Renders the current state of the battle system on
 * the screen.
 **************************************************************/
void DrawBattle(void) {
    // Draw background image
    DrawAt(0, 0);
    BACKGROUND_ID background = Location(Player->Location)->Background;
    al_draw_bitmap(BackgroundImage(background? background: CHARCOAL), 0, 0, 0);
    DrawBattlers();
    DrawHUDs();
    
    // Battle menu phase
    if (!BattleMenuDone()) {
        DrawBattleMenu();
    } else {
        DrawAt(0, 0);
        DrawOutput();
    }
}

/**************************************************************/
