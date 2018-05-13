/**********************************************************//**
 * @file battle.c
 * @brief Implements battle system.
 * @author Rena Shinomiya
 * @date May 06, 2018
 **************************************************************/

#include <stdio.h>              // snprintf

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "random.h"             // randint
#include "assets.h"             // MiscImage
#include "species.h"            // SPECTRA
#include "technique.h"          // TECHNIQUE
#include "menu.h"               // MENU
#include "player.h"             // Player
#include "battle.h"             // BATTLER, TEAM, TURN

/**************************************************************/
/// @brief Battlers in the player's team.
static TEAM PlayerTeam;

/// @brief Battlers in the enemy team.
static TEAM EnemyTeam;

/// @brief Buffer for enemy spectra.
static SPECTRA EnemySpectra[TEAM_SIZE];

/// @brief Buffer for actions the battlers will take
/// on this turn.
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

/// @brief BATTLER_ID of each enemy on the TargetMenu.
static BATTLER_ID TargetID[BATTLE_SIZE];

/**********************************************************//**
 * @brief Gets the team the BATTLER belongs to.
 * @param id: BATTLER's unique ID.
 * @return Pointer to the BATTLER's team.
 **************************************************************/
static inline TEAM *TeamOf(BATTLER_ID id) {
    return (id<TEAM_SIZE)? &PlayerTeam: &EnemyTeam;
}

/**********************************************************//**
 * @brief Gets the opposite team in the battle.
 * @param team: A team.
 * @return Pointer to team's enemy.
 **************************************************************/
static inline TEAM *OppositeTeam(const TEAM *team) {
    return (team==&PlayerTeam)? &EnemyTeam: &PlayerTeam;
}

/**********************************************************//**
 * @brief Gets the BATTLER data from its ID.
 * @param id: Unique ID of the BATTLER.
 * @return Pointer to the BATTLER's data.
 **************************************************************/
static inline BATTLER *BattlerByID(BATTLER_ID id) {
    if (id < TEAM_SIZE) {
        return &PlayerTeam.Member[id];
    } else {
        return &EnemyTeam.Member[id-TEAM_SIZE];
    }
}

/**********************************************************//**
 * @brief Gets the name of the BATTLER.
 * @param id: BATTLER's unique ID.
 * @return The BATTLER's name.
 **************************************************************/
static inline const char *NameOf(BATTLER_ID id) {
    return SpeciesByID(BattlerByID(id)->Spectra->Species)->Name;
}

/**********************************************************//**
 * @brief Set up a BATTLER for a SPECTRA.
 * @param battler: BATTLER to set up.
 * @param spectra: SPECTRA that's joining the battle.
 **************************************************************/
static void InitializeBattler(BATTLER *battler, SPECTRA *spectra) {
    battler->Spectra = spectra;
    battler->AttackBoost = 0;
    battler->DefendBoost = 0;
    battler->EvadeBoost = 0;
    battler->LuckBoost = 0;
}

/**********************************************************//**
 * @brief Sets up the player's team.
 **************************************************************/
static void InitializePlayerTeam(void) {
    for (int i=0; i<TEAM_SIZE; i++) {
        // Collect data that we'll use to set up the battler.
        SPECTRA *spectra = &Player->Spectra[i];
        PLAYER_MENU *menu = &PlayerMenu[i];
        BATTLER *battler = &PlayerTeam.Member[i];
        
        // Initialize battler data
        battler->ID = UserID(i);
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
            InitializeBattler(battler, NULL);
        }
    }
}

/**********************************************************//**
 * @brief Generates a random ENEMY based on the encounters
 * defined for the map.
 * @param enemy: ENEMY to generate.
 * @param encounters: ENCOUNTER data from the map.
 **************************************************************/
static inline void RandomEnemy(ENEMY *enemy, const ENCOUNTER *encounters) {
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
static inline void InitializeEnemyTeam(const ENEMY *enemies) {
    for (int i=0; i<TEAM_SIZE; i++) {
        if (enemies[i].Species) {
            CreateSpectra(&EnemySpectra[i], enemies[i].Species, enemies[i].Level);
            InitializeBattler(&EnemyTeam.Member[i], &EnemySpectra[i]);
        } else {
            EnemySpectra[i].Species = 0;
            InitializeBattler(&EnemyTeam.Member[i], NULL);
        }
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
    const ENCOUNTER *encounters;
    switch (type) {
    case ENCOUNTER_FISHING:
        encounters = Location(Player->Location)->Fishing;
        break;
    case ENCOUNTER_OVERWORLD:
    default:
        encounters = Location(Player->Location)->Encounters;
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
}

/**********************************************************//**
 * @brief Set up a non-random encounter with spectra.
 * @param bosses: Boss enemies to generate.
 **************************************************************/
void InitializeBossEncounter(const BOSS *bosses) {
    InitializeEnemyTeam(bosses->Boss);
    InitializePlayerTeam();
}

static const COORDINATE BattlerPosition[] = {
    // User's team
    [0] = { 60, 260},
    [1] = {110, 240},
    [2] = {160, 220},
    
    // Enemy's team
    [3] = {420, 260},
    [4] = {370, 240},
    [5] = {320, 220},
};

static void DrawBattlers(void) {
    // Draw drop shadows
    for (BATTLER_ID id=0; id<BATTLE_SIZE; id++) {
        if (!BattlerByID(id)) {
            continue;
        }
        const COORDINATE *center = &BattlerPosition[id];
        al_draw_filled_ellipse(center->X, center->Y, 40, 10, al_map_rgba(0, 0, 0, 60));
    }
    
    // Draw existing spectra in layer order
    const BATTLER_ID order[] = {5, 2, 4, 1, 3, 0};
    for (int i=0; i<BATTLE_SIZE; i++) {
        BATTLER_ID id = order[i];
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

/**************************************************************/
/// @brief Used in battle menu code to denote whose turn
/// is being inputted by the user.
static BATTLER_ID CurrentUser;

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
        TargetMenu.Control.IndexMax = 1;
        TargetMenu.Control.ScrollMax = 0;
        return;
    }
    
    // Select from users/allies/both
    int i = 0;
    for (BATTLER_ID id=0; id<BATTLE_SIZE; id++) {
        if (type&TARGET_USER && id==CurrentUser) {
            // Targetting yourself - specify that!
            TargetID[i] = id;
            TargetMenu.Option[i++] = "Yourself";
        } else if (type&TARGET_ALLY && id<TEAM_SIZE) {
            // Targetting any ally
            TargetID[i] = id;
            TargetMenu.Option[i++] = NameOf(id);
        } else if (type&TARGET_ENEMY && id>=TEAM_SIZE) {
            if (type&TARGET_ALLY) {
                // Targetting all - specify who an enemy is!
                snprintf(EnemyNames[id-TEAM_SIZE], 20, "Enemy %s", NameOf(id));
                TargetID[i] = id;
                TargetMenu.Option[i++] = EnemyNames[id-TEAM_SIZE];
            } else {
                // Targetting enemies - just give names.
                TargetID[i] = id;
                TargetMenu.Option[i++] = NameOf(id);
            }
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
 * @brief Draws all the battle HUDs on the screen.
 **************************************************************/
static void DrawHUDs(void) {
    // User HUDs
    int y = 4;
    for (BATTLER_ID id=0; id<3 && PlayerTeam.Member[id].Spectra; id++) {
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
    for (BATTLER_ID id=3; id<6 && EnemyTeam.Member[id-3].Spectra; id++) {
        DrawAt(275, y);
        DrawHudEnemy(EnemyTeam.Member[id-3].Spectra);
        y += 29;
    }
}

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

/**************************************************************/
/// @brief Defines the battle menu and its control structure.
static MENU BattleMenu = {
    .Option = {
        [BATTLE_ATTACK]         = "Attack",
        [BATTLE_DEFEND]         = "Defend",
        [BATTLE_TECHNIQUE]      = "Technique",
        [BATTLE_ITEM]           = "Item",
        // This item periodically chaanges between
        // "Escape" and "Cancel" depending on whose
        // turn it is.
        [BATTLE_CANCEL]         = "Escape",
    },
    .Control = {
        .IndexMax               = 4,
    },
};

/**********************************************************//**
 * @brief Check if the user is done inputting turns.
 * @return True if the user is finished.
 **************************************************************/
static inline bool BattleMenuDone(void) {
    return CurrentUser>=TEAM_SIZE || !PlayerTeam.Member[CurrentUser].Spectra;
}

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
                    turn->TargetType = TechniqueByID(turn->Technique)->Target;
                    LoadTargetMenu(turn->TargetType);
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
                turn->TargetType = TechniqueByID(turn->Technique)->Target;
                LoadTargetMenu(turn->TargetType);
                break;
            case BATTLE_DEFEND:
                turn->Technique = DEFAULT_DEFEND,
                turn->TargetType = TechniqueByID(turn->Technique)->Target;
                LoadTargetMenu(turn->TargetType);
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
                    turn->TargetType = TechniqueByID(turn->Technique)->Target;
                    LoadTargetMenu(turn->TargetType);
                }
                break;
            }
        }
        break;
    
    case CONTROL_CANCEL:
        break;
    }
}

void DrawBattle(void) {
    // Draw background image
    DrawAt(0, 0);
    BACKGROUND_ID background = Location(Player->Location)->Background;
    al_draw_bitmap(BackgroundImage(background? background: CHARCOAL), 0, 0, 0);
    DrawBattlers();
    DrawHUDs();
    DrawBattleMenu();
}

void UpdateBattle(void) {
    UpdateBattleMenu();
}

/**************************************************************/
