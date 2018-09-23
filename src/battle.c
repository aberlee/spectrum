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

/**********************************************************//**
 * @enum BATTLE_STATE
 * @brief Controls how the battle is finished.
 **************************************************************/
typedef enum {
    BATTLE_STATE_INTRO,
    BATTLE_STATE_ACTIVE,
    BATTLE_STATE_WIN,
    BATTLE_STATE_LOSE,
    BATTLE_STATE_ESCAPE,
    BATTLE_STATE_NO_ESCAPE,
} BATTLE_STATE;

/// @brief The state of the current battle.
static BATTLE_STATE BattleState;

/**************************************************************/
/// @brief Battlers in the player's team.
static TEAM PlayerTeam;

/// @brief Battlers in the enemy team.
static TEAM EnemyTeam;

/// @brief Buffer for enemy spectra.
static SPECTRA EnemySpectra[TEAM_SIZE];

static BATTLER *Captured = NULL;

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
    ITEM_ID Item;
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

static int LockedItemIndices[TEAM_SIZE];

static MENU ItemMenu;

static void InitializeItems(void) {
    int count = 0;
    for (int i=0; i<INVENTORY_SIZE; i++) {
        ITEM_ID id = Player->Inventory[i];
        const ITEM *item = ItemByID(id);
        ItemMenu.Option[i] = item->Name;
        if (id) {
            count = i;
        }
    }
    
    // Set up indices
    ItemMenu.Control.Index = 0;
    ItemMenu.Control.IndexMax = (count<6)? count: 6;
    ItemMenu.Control.Scroll = 0;
    ItemMenu.Control.ScrollMax = (count<6)? 0: count-6;
    ItemMenu.Control.Jump = 5;
    ItemMenu.Control.State = CONTROL_IDLE;
    
    // Unlock all items
    for (int i=0; i<TEAM_SIZE; i++) {
        LockedItemIndices[i] = -1;
    }
}

/**************************************************************/
/// @brief Used in battle menu code to denote whose turn
/// is being inputted by the user.
static int CurrentUser = 0;

/// @brief Turn being executed currently.
static TURN *CurrentTurn = NULL;

/// @brief Centered coordinates where battlers should be drawn.
static const COORDINATE BattlerPosition[] = {
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
    int chance = randint(1, 100);
    const ENCOUNTER *encounter = NULL;
    for (int i=0, total=0; total<chance; total+=encounters[i++].Chance) {
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

static int FirstUser(void) {
    int id = 0;
    while (id<TEAM_SIZE) {
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsActive(battler) || !IsAlive(battler)) {
            id++;
        } else {
            break;
        }
    }
    return id;
}

static void JumpToNextUser(void) {
    CurrentUser++;
    while (CurrentUser<TEAM_SIZE) {
        BATTLER *battler = BattlerByID(CurrentUser);
        if (!BattlerIsActive(battler) || !IsAlive(battler)) {
            CurrentUser++;
        } else {
            break;
        }
    }
}
static void JumpToPreviousUser(void) {
    CurrentUser--;
    while (CurrentUser>0) {
        BATTLER *battler = BattlerByID(CurrentUser);
        if (!BattlerIsActive(battler) || !IsAlive(battler)) {
            CurrentUser--;
        } else {
            break;
        }
    }
}

/**********************************************************//**
 * @brief Initializes setup for the current round of battle.
 **************************************************************/
static void InitializeRound(void) {
    CurrentUser = FirstUser();
    CurrentTurn = NULL;
    ResetControl(&BattleMenu.Control);
    BattleState = BATTLE_STATE_ACTIVE;
    for (int i=0; i<BATTLE_SIZE; i++) {
        Turns[i].State = TURN_INACTIVE;
    }
    InitializeItems();
}

static void IntroduceBattle(ENCOUNTER_TYPE type) {
    BattleState = BATTLE_STATE_INTRO;
    int count = 0;
    const BATTLER *leader = NULL;
    for (int i=0; i<TEAM_SIZE; i++) {
        const BATTLER *battler = &EnemyTeam.Member[i];
        if (BattlerIsActive(battler)) {
            if (!leader) {
                leader = battler;
            }
            count++;
        }
    }
    
    // Determine if the sentence should be "A [name]" or "An [name]"
    const char *name = BattlerName(leader);
    const char *an = NULL;
    switch (name[0]) {
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
    case 'Y':
        an = "An";
        break;
    default:
        an = "A";
        break;
    }
    
    // Determine how many enemies
    const char *others = NULL;
    switch (count) {
    case 1:
        others = NULL;
        break;
    case 2:
        others = "and its cohort";
        break;
    default:
        others = "and its cohorts";
        break;
    }
    
    // Determine how the enemy are appearing
    const char *description = NULL;
    switch (type) {
    case ENCOUNTER_FISHING:
        description = (count>1)? "were hooked": "was hooked";
    default:
        description = (count>1)? "draw near": "draws near";
        break;
    }
    
    // Finalize sentence
    if (others) {
        OutputF("%s %s %s %s!", an, name, others, description);
    } else {
        OutputF("%s %s %s!", an, name, description);
    }
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
    IntroduceBattle(type);
}

/**********************************************************//**
 * @brief Set up a non-random encounter with spectra.
 * @param bosses: Boss enemies to generate.
 **************************************************************/
void InitializeBossEncounter(const BOSS *bosses) {
    InitializeEnemyTeam(bosses->Boss);
    InitializePlayerTeam();
    IntroduceBattle(ENCOUNTER_BOSS);
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
        if (!BattlerIsActive(battler) || !IsAlive(battler)) {
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

static void EscapeBattle(void) {
    // Get the escape chance
    int ally = 0;
    int enemy = 0;
    for (int id=0; id<BATTLE_SIZE; id++) {
        const BATTLER *battler = BattlerByID(id);
        if (BattlerIsActive(battler) && IsAlive(battler)) {
            if (id<TEAM_SIZE) {
                ally += BattlerEvade(battler)+BattlerLuck(battler);
            } else {
                enemy += BattlerEvade(battler);
            }
        }
    }
    assert(ally && enemy);
    float chance = (float)ally / (ally+enemy);
    
    // Maybe escape
    Output("The party tries to escape...");
    if (uniform(0.0, 1.0) < chance) {
        Output("And succeeds!");
        BattleState = BATTLE_STATE_ESCAPE;
    } else {
        Output("And fails...");
        
        // Fast-forward to enemy turn select, and invalidate all
        // of the user's turns.
        for (int id=0; id<TEAM_SIZE; id++) {
            Turns[id].State = TURN_DONE;
        }
        CurrentUser = TEAM_SIZE;
        BattleState = BATTLE_STATE_NO_ESCAPE;
    }
}

/**********************************************************//**
 * @brief Draws the main battle menu system on the screen.
 **************************************************************/
static void DrawBattleMenu(void) {
    // Escape/Cancel display
    BattleMenu.Option[BATTLE_CANCEL] = CurrentUser==FirstUser()? "Escape": "Cancel";
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
            DrawAt(108, 275);
            DrawOption(&ItemMenu);
            if (ItemMenu.Control.State == CONTROL_CONFIRM) {
                DrawAt(212, 275);
                DrawOption(&TargetMenu);
            }
            break;
        
        case BATTLE_CANCEL:
            // Only if first user - escape
            if (CurrentUser==FirstUser()) {
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
        JumpToNextUser();
        if (!BattleMenuDone()) {
            ResetControl(&PlayerMenu[CurrentUser].TechniqueMenu.Control);
            ResetControl(&BattleMenu.Control);
        }
        break;

    case CONTROL_CANCEL:
        // Revert to the previous menu.
        if (PlayerMenu[CurrentUser].TechniqueMenu.Control.State) {
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

static inline bool SelectedItemLocked(void) {
    int index = MenuItem(&ItemMenu);
    for (int i=0; i<TEAM_SIZE; i++) {
        if (LockedItemIndices[i]==index) {
            return true;
        }
    }
    return false;
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
            switch (ItemMenu.Control.State) {
            case CONTROL_CONFIRM:
                UpdateTargetMenu();
                break;
                
            case CONTROL_CANCEL:
                BattleMenu.Control.State = CONTROL_IDLE;
                LockedItemIndices[CurrentUser] = -1;
                break;

            case CONTROL_IDLE:
                UpdateMenu(&ItemMenu);
                if (SelectedItemLocked()) {
                    ItemMenu.Control.State = CONTROL_IDLE;
                } else if (ItemMenu.Control.State == CONTROL_CONFIRM) {
                    turn->Technique = DEFAULT_ITEM;
                    turn->Item = Player->Inventory[MenuItem(&ItemMenu)];
                    LockedItemIndices[CurrentUser] = MenuItem(&ItemMenu);
                    LoadTargetMenu(ALLY);
                }
                break;
            }
            break;
        
        case BATTLE_CANCEL:
            // Go back to the previous user, or try to escape the battle.
            if (CurrentUser > FirstUser()) {
                JumpToPreviousUser();
                ResetControl(&PlayerMenu[CurrentUser].TechniqueMenu.Control);
            } else if (CurrentUser==FirstUser()) {
                EscapeBattle();
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
                ResetControl(&ItemMenu.Control);
                break;
            case BATTLE_CANCEL:
                if (CurrentUser==FirstUser()) {
                    turn->Technique = DEFAULT_ESCAPE,
                    LoadTargetMenu(TechniqueByID(turn->Technique)->Target);
                }
                break;
            }
        }
        break;
    
    case CONTROL_CANCEL:
        if (CurrentUser > FirstUser()) {
            JumpToPreviousUser();
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
            turn->Technique = usableTechniques[randint(0,u-1)];
            
            // Get the primary target, if applicable
            TARGET_TYPE type = TechniqueByID(turn->Technique)->Target;
            if (type&TARGET_GROUP) {
                turn->Target = -1;
            } else {
                int targets[BATTLE_SIZE];
                int nTargets = GetTargets(targets, id, type);
                turn->Target = targets[randint(0,nTargets-1)];
            }
        } else {
            // Turn skipped
            turn->State = TURN_INACTIVE;
        }
    }
}

static bool ExecuteCapture(BATTLER *battler) {
    // Determine if capture succeeds
    int rate = BattlerSpecies(battler)->CatchRate;
    float percent = (float)BattlerHealth(battler)/BattlerMaxHealth(battler);
    
    // Increased rate if ailed
    switch (battler->Spectra->Ailment) {
    case ASLEEP:
    case BURIED:
        rate += 10;
        break;
    case SHOCKED:
        rate += 5;
        break;
    default:
        break;
    }
    
    // Test rate
    OutputSplitByCR("...\r......\r.........");
    int test = randint(0, 99);
    int threshold = rate + (100-rate)*(1-percent)*(1-percent);
    if (test < threshold) {
        if (Capture(battler->Spectra)) {
            Output("The capture succeeded!");
            Captured = battler;
            return true;
        } else {
            Output("You can't capture any more!");
        }
    } else if (test-10 < threshold) {
        Output("It just got away...");
    } else if (test-20 < threshold) {
        Output("It managed to break free!");
    } else {
        Output("The capture failed!");
    }
    return false;
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
    
    // Maybe fail if there are status conditions
    BATTLER *user = BattlerByID(turn->User);
    const TECHNIQUE *technique = TechniqueByID(turn->Technique);
    switch (user->Spectra->Ailment) {
    case SHOCKED:
        if (uniform(0.0,1.0)<0.5) {
            OutputF("%s can't move...", BattlerName(user));
            return;
        }
        break;
    
    case BURIED:
        OutputF("%s is buried in the ground...", BattlerName(user));
        user->Spectra->Ailment = 0;
        return;
    
    case ASLEEP:
        OutputF("%s is fast asleep...", BattlerName(user));
        return;
    
    default:
        break;
    }
    
    // Perform the turn
    bool allInvalid = true;
    for (int i=0; i<nTargets; i++) {
        BATTLER *target = BattlerByID(Targets[i]);
        if (!BattlerIsActive(target) || !IsAlive(target)) {
            continue;
        }
        allInvalid = false;
        
        // Inflict damage
        int damage = 0;
        if (technique->Power) {
            float ratio = (float)BattlerAttack(user)/BattlerDefend(target);
            float scale = (float)user->Spectra->Level/LEVEL_MAX;
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
                if (!IsAlive(target)) {
                    OutputF("%s passed out!", BattlerName(target));
                }
            } else {
                OutputF("%s didn't take any damage!", BattlerName(target));
            }
        }
        
        // Apply effect if applicable
        if (IsAlive(target) && !(technique->Flags&TECHNIQUE_EFFECT_ONCE)) {
            const ITEM *item;
            switch (turn->Technique) {
            case DEFAULT_ITEM:
                item = ItemByID(turn->Item);
                if (item->Flags&BATTLE_ONLY) {
                    if (ApplyEffectInBattle(item->Effect, user, target, item->Argument)) {
                        DropItem(turn->Item);
                    } else {
                        Output("There was no effect...");
                    }
                } else {
                    Output("That's not useful right now!");
                }
                
                break;
            
            case CAPTURE:
                ExecuteCapture(target);
                break;
            
            default:
                if (ShouldEffectActivate(technique->Effect, technique->Argument)) {
                    ApplyEffectInBattle(technique->Effect, user, target, technique->Argument);
                } else if (!technique->Power) {
                // Technique doesn't do damage, and effect missed.
                OutputF("%s avoided the attack!", BattlerName(target));
                }
                break;
            }
        }
    }
    
    // All invalid targets?
    if (allInvalid) {
        Output("There was no target...");
    }
    
    // Perform effect if it activates after all targets are hit
    if (technique->Flags&TECHNIQUE_EFFECT_ONCE) {
        if (ShouldEffectActivate(technique->Effect, technique->Argument)) {
            ApplyEffectInBattle(technique->Effect, user, NULL, technique->Argument);
        }
    }
}

static void MaybeUpdateBattleState(void) {
    // Clean up any dead battlers
    bool win = true;
    bool lose = true;
    for (int id=0; id<BATTLE_SIZE; id++) {
        BATTLER *battler = BattlerByID(id);
        if (BattlerIsActive(battler)){
            if(IsAlive(battler)) {
                if (TeamOf(id)==&PlayerTeam) {
                    lose = false;
                } else {
                    win = false;
                }
            } else {
                InitializeBattlerAsInactive(battler);
            }
        }
    }
    
    // If it's a tie, you lose.
    if (lose) {
        BattleState = BATTLE_STATE_LOSE;
        Output("You lost!");
    } else if (win) {
        BattleState = BATTLE_STATE_WIN;
        Output("You won!");
    }
}

static inline int Priority(const BATTLER *battler) {
    int evade = BattlerEvade(battler);
    if (battler->Spectra->Ailment==SHOCKED) {
        return evade/2;
    } else {
        return evade;
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
        for (int id=0; id<BATTLE_SIZE; id++) {
            BATTLER *battler = BattlerByID(id);
            if (Turns[id].State == TURN_PENDING) {
                if (BattlerIsActive(battler) && IsAlive(battler)) {
                    int priority = Priority(BattlerByID(id));
                    if (priority > maxPriority || !CurrentTurn) {
                        maxPriority = priority;
                        CurrentTurn = &Turns[id];
                    }
                } else {
                    // Inactivate turns for dead battlers
                    Turns[id].State = TURN_INACTIVE;
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
        if (CurrentTurn->Technique==DEFAULT_ITEM) {
            const ITEM *item = ItemByID(CurrentTurn->Item);
            OutputF("%s used %s!", BattlerNameByID(CurrentTurn->User), item->Name);
        } else {
            const TECHNIQUE *technique = TechniqueByID(CurrentTurn->Technique);
            OutputF("%s used %s!", BattlerNameByID(CurrentTurn->User), technique->Name);
        }
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
            if (Captured) {
                Captured->Spectra = 0;
                Captured = NULL;
            }
            CurrentTurn->State = TURN_DONE;
            MaybeUpdateBattleState();
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
        switch (Turns[i].State) {
        case TURN_PENDING:
        case TURN_ACTIVE:
        case TURN_RESULT:
            return false;
        case TURN_INACTIVE:
        case TURN_DONE:
        default:
            break;
        }
    }
    return true;
}

static void ApplyEndOfRoundEffects(void) {
    for (int id=0; id<BATTLE_SIZE; id++) {
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsActive(battler) || !IsAlive(battler)) {
            continue;
        }
        
        // Unset the battler's flags.
        battler->Flags = 0;
        
        // Apply status ailments
        int damage;
        switch (battler->Spectra->Ailment) {
        case ASLEEP:
            if (uniform(00.0,1.0)<0.5) {
                OutputF("%s woke up!", BattlerName(battler));
                battler->Spectra->Ailment =0;
            }
            break;
        
        case POISONED:
            damage = 1 + BattlerMaxHealth(battler)/8;
            battler->Spectra->Health -= damage;
            if (battler->Spectra->Health < 0) {
                battler->Spectra->Health = 0;
            }
            OutputF("%s took %d damage from poison!", BattlerName(battler), damage);
            if (!IsAlive(battler)) {
                OutputF("%s passed out!", BattlerName(battler));
            }
            break;  
            
        case AFLAME:
            damage = 1 + BattlerMaxHealth(battler)/8;
            battler->Spectra->Health -= damage;
            if (battler->Spectra->Health < 0) {
                battler->Spectra->Health = 0;
            }
            OutputF("%s took %d damage from fire!", BattlerName(battler), damage);
            if (!IsAlive(battler)) {
                OutputF("%s passed out!", BattlerName(battler));
            }
            break;
            
        default:
            break;
        }
        
        // Check win and lose conditions
        MaybeUpdateBattleState();
        if (BattleState!=BATTLE_STATE_ACTIVE) {
            return;
        }
    }
}

/**********************************************************//**
 * @brief Updates one step of the battle system.
 **************************************************************/
void UpdateBattle(void) {
    switch (BattleState) {
    case BATTLE_STATE_INTRO:
        // Happens once at the start of each battle
        UpdateOutput();
        if (OutputDone()) {
            InitializeRound();
        }
        break;

    case BATTLE_STATE_ACTIVE:
        // Typical ongoing battle
        if (!BattleMenuDone()) {
            UpdateBattleMenu();
            // Get enemy turns
            if (BattleMenuDone()) {
                LoadEnemyTurns();
            }
        } else if (!BattleExecutionDone()) {
            UpdateBattleExecution();
            if (BattleExecutionDone()) {
                ApplyEndOfRoundEffects();
                if (OutputDone()) {
                    InitializeRound();
                }
            }
        } else if (!OutputDone()) {
            UpdateOutput();
            if (OutputDone()) {
                InitializeRound();
            }
        }
       break;
    
    case BATTLE_STATE_NO_ESCAPE:
        UpdateOutput();
        if (OutputDone()) {
            BattleState = BATTLE_STATE_ACTIVE;
        }
        break;
    
    // End conditions
    case BATTLE_STATE_WIN:
    case BATTLE_STATE_LOSE:
    case BATTLE_STATE_ESCAPE:
        UpdateOutput();
        if (OutputDone()) {
            SetMode(MODE_MAP);
        }
        break;
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
        if (!BattlerIsActive(BattlerByID(id))) {
            continue;
        }
        
        // Different colors for seletion
        ALLEGRO_COLOR color = al_map_rgba(0, 0, 0, 60);
        if (!BattleMenuDone()) {
            if (id == CurrentUser && id < TEAM_SIZE) {
                color = al_map_rgba(0, 127, 255, 200);
            } else if (id == target) {
                color = al_map_rgba(255, 20, 0, 200);
            }
        }
        al_draw_filled_ellipse(center->X, center->Y, 40, 10, color);
    }
    
    // Draw existing spectra in layer order
    static const int Order[] = {3, 2, 4, 1, 5, 0};
    for (int i=0; i<6; i++) {
        int id = Order[i];
        if (!BattlerIsActive(BattlerByID(id))) {
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
        
        // Flip enemies
        if (id<TEAM_SIZE) {
            al_draw_bitmap(image, center->X-offset->X, center->Y-offset->Y, 0);
        } else {
            int flipped = al_get_bitmap_width(image) - offset->X;
            al_draw_bitmap(image, center->X-flipped, center->Y-offset->Y, ALLEGRO_FLIP_HORIZONTAL);
        }
    }
}

/**********************************************************//**
 * @brief Draws all the battle HUDs on the screen.
 **************************************************************/
static void DrawHUDs(void) {
    // User HUDs
    for (int id=0, y=4; id<TEAM_SIZE; id++) {
        if (!BattlerIsActive(BattlerByID(id))) {
            continue;
        }
        DrawAt(4, y);
        DrawHudUser(PlayerTeam.Member[id].Spectra);
        // Hud tag
        if (!BattleMenuDone()) {
            if (id==CurrentUser) {
                al_draw_bitmap(MiscImage(HUD_UP), 200, 5, 0);
            } else if (id<CurrentUser) {
                al_draw_bitmap(MiscImage(HUD_OK), 200, 5, 0);
            }
        }
        y += 29;
    }
    
    // Enemy HUDs
    for (int id=TEAM_SIZE, y=4; id<BATTLE_SIZE; id++) {
        if (!BattlerIsActive(BattlerByID(id))) {
            continue;
        }
        DrawAt(275, y);
        DrawHudEnemy(EnemyTeam.Member[id-TEAM_SIZE].Spectra);
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
    if (!BattleMenuDone() && BattleState==BATTLE_STATE_ACTIVE) {
        DrawBattleMenu();
    } else {
        DrawAt(0, 0);
        DrawOutput();
    }
}

/**************************************************************/
