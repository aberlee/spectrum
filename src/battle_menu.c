/**********************************************************//**
 * @file battle_menu.c
 * @brief Implements the battle menu interface.
 * @author Rena Shinomiya
 * @date September 23, 2018
 **************************************************************/

#include <stdio.h>          // snprintf

#include "battle.h"         // BattlerByID
#include "menu.h"           // MENU
#include "player.h"         // Player

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

/**************************************************************/
/// @brief Buffers the player's spectra's battle menus.
static PLAYER_MENU PlayerMenu[TEAM_SIZE];

/// @brief Temporary buffer for target menu.
static MENU TargetMenu;

/// @brief ID of each enemy on the TargetMenu.
static int TargetID[BATTLE_SIZE];

/// @brief Used in battle menu code to denote whose turn
/// is being inputted by the user.
static int CurrentUser = 0;

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

/// @brief Stores a dynamic menu for the player's items.
static MENU ItemMenu;

/// @brief Defines an item index as unlocked.
#define NOT_LOCKED (-1)

/// @brief Stores items that are already queued for use
/// by another battler.
static int LockedItemIndices[TEAM_SIZE] = {NOT_LOCKED};

/**********************************************************//**
 * @brief Initializes technique menus for each battler.
 **************************************************************/
static void InitializePlayerMenus(void) {
    for (int id=0; id<TEAM_SIZE; id++) {
        SPECTRA *spectra = &Player->Spectra[id];
        PLAYER_MENU *menu = &PlayerMenu[id];
        if (spectra->Species) {
            // Initialize menu data
            int t;
            for (t=0; spectra->Moveset[t]; t++) {
                const TECHNIQUE *technique = TechniqueByID(spectra->Moveset[t]);
                menu->TechniqueMenu.Option[t] = technique->Name;
                snprintf(menu->CostString[t], 3, "%d", technique->Cost);
                menu->CostMenu.Option[t] = menu->CostString[t];
            }
            menu->TechniqueMenu.Option[t] = NULL;
            menu->CostMenu.Option[t] = NULL;
            ResetMenu(&menu->TechniqueMenu);
            ResetMenu(&menu->CostMenu);
            InitializeMenuScroll(&menu->TechniqueMenu, MENU_COLUMN);
            InitializeMenuScroll(&menu->CostMenu, MENU_COLUMN);
        }
    }
}

/**********************************************************//**
 * @brief Initializes the player's items menu for this turn.
 **************************************************************/
static void InitializeItemsMenu(void) {
    // Set up the names of items
    for (int i=0; i<INVENTORY_SIZE; i++) {
        ITEM_ID id = Player->Inventory[i];
        const ITEM *item = ItemByID(id);
        ItemMenu.Option[i] = item->Name;
    }
    ResetMenu(&ItemMenu);
    InitializeMenuScroll(&ItemMenu, MENU_OPTION);
    
    // Unlock all items
    for (int i=0; i<TEAM_SIZE; i++) {
        LockedItemIndices[i] = -1;
    }
}

/**********************************************************//**
 * @brief Check if the user is done inputting turns.
 * @return True if the user is finished.
 **************************************************************/
bool BattleMenuDone(void) {
    return CurrentUser>=TEAM_SIZE || !BattlerIsAlive(BattlerByID(CurrentUser));
}

int BattleMenuCurrentUserID(void) {
    return CurrentUser;
}

int BattleMenuCurrentTargetID(void) {
    if (!BattleMenuDone() && MenuIdle(&TargetMenu)) {
        int index = MenuItem(&TargetMenu);
        return TargetID[index];
    } else {
        return -1;
    }
}

/**********************************************************//**
 * @brief Gets the index of the first user to pick a turn for.
 * @return User ID.
 **************************************************************/
static int FirstUser(void) {
    int id = 0;
    while (id<TEAM_SIZE) {
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsAlive(battler)) {
            id++;
        } else {
            break;
        }
    }
    return id;
}

/**********************************************************//**
 * @brief Goes to the next user who requires turn select.
 **************************************************************/
static void JumpToNextUser(void) {
    CurrentUser++;
    while (CurrentUser<TEAM_SIZE) {
        BATTLER *battler = BattlerByID(CurrentUser);
        if (!BattlerIsAlive(battler)) {
            CurrentUser++;
        } else {
            break;
        }
    }
}

/**********************************************************//**
 * @brief Returns to the previous user's turn to select again.
 **************************************************************/
static void JumpToPreviousUser(void) {
    CurrentUser--;
    while (CurrentUser>0) {
        BATTLER *battler = BattlerByID(CurrentUser);
        if (!BattlerIsAlive(battler)) {
            CurrentUser--;
        } else {
            break;
        }
    }
}

/**********************************************************//**
 * @brief Initializes the battle menu at the start of each
 * round of a battle.
 **************************************************************/
void InitializeBattleMenu(void) {
    InitializePlayerMenus();
    InitializeItemsMenu();
    ResetMenu(&BattleMenu);
    CurrentUser = FirstUser();
}

/**********************************************************//**
 * @brief Sets up the target menu for the current user.
 * @param type: What the range of the technique is.
 **************************************************************/
static void LoadTargetMenu(TARGET_TYPE type) {
    // Temporary buffer for strings used on the TargetMenu.
    static char EnemyNames[TEAM_SIZE][21];
    
    if (type&TARGET_GROUP) {
        // Only one group ever, so no menu needed.
        if (type&TARGET_ALLY&TARGET_ENEMY) {
            TargetMenu.Option[0] = "Everyone";
        } else if (type&TARGET_ALLY) {
            TargetMenu.Option[0] = "Allies";
        } else if (type&TARGET_ENEMY) {
            TargetMenu.Option[0] = "Enemies";
        }
        TargetMenu.Option[1] = NULL;
        TargetID[0] = -1;

    } else {
        // Menu needs to list each name
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
        TargetMenu.Option[i] = NULL;
    }
    
    // Reset the menu after creation
    InitializeMenuScroll(&TargetMenu, MENU_OPTION);
    ResetMenu(&TargetMenu);
}

/**********************************************************//**
 * @brief Updates the targetting menu.
 **************************************************************/
static void UpdateTargetMenu(void) {
    TURN *turn = TurnByID(CurrentUser);
    switch (MenuState(&TargetMenu)) {
    case CONTROL_CONFIRM:
        // Confirm the entire turn
        turn->Target = TargetID[MenuItem(&TargetMenu)];
        turn->State = TURN_PENDING;
        
        // Maybe move to the next battler
        JumpToNextUser();
        if (!BattleMenuDone()) {
            ResetMenu(&PlayerMenu[CurrentUser].TechniqueMenu);
            ResetMenu(&BattleMenu);
        }
        break;

    case CONTROL_CANCEL:
        // Revert to the previous menu.
        if (!MenuIdle(&PlayerMenu[CurrentUser].TechniqueMenu)) {
            ResetMenuToIdle(&PlayerMenu[CurrentUser].TechniqueMenu);
        } else {
            ResetMenuToIdle(&BattleMenu);
        }
        break;

    case CONTROL_IDLE:
        UpdateMenu(&TargetMenu);
        break;
    }
}

/**********************************************************//**
 * @brief Draws the main battle menu system on the screen.
 **************************************************************/
void DrawBattleMenu(void) {
    // Escape/Cancel display
    DrawAt(4, 275);
    DrawOption(&BattleMenu);
    if (MenuConfirmed(&BattleMenu)) {
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
            if (MenuConfirmed(&PlayerMenu[CurrentUser].TechniqueMenu)) {
                DrawAt(254, 275);
                DrawOption(&TargetMenu);
            }
            break;
        
        case BATTLE_ITEM:
            DrawAt(108, 275);
            DrawOption(&ItemMenu);
            if (MenuConfirmed(&ItemMenu)) {
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
 * @brief Determine if the item is already queued for another
 * turn.
 * @return True if the item is locked.
 **************************************************************/
static inline bool SelectedItemLocked(void) {
    int index = MenuItem(&ItemMenu);
    for (int i=0; i<TEAM_SIZE; i++) {
        if (LockedItemIndices[i]==index) {
            return true;
        }
    }
    return false;
}

static inline TECHNIQUE_ID SelectedTechniqueID(void) {
    int index = MenuItem(&PlayerMenu[CurrentUser].TechniqueMenu);
    return BattlerByID(CurrentUser)->Spectra->Moveset[index];
}

static inline const TECHNIQUE *SelectedTechnique(void) {
    TECHNIQUE_ID id = SelectedTechniqueID();
    return TechniqueByID(id);
}

static inline ITEM_ID SelectedItemID(void) {
    int index = MenuItem(&ItemMenu);
    return Player->Inventory[index];
}

static inline TURN *CurrentTurn(void) {
    return TurnByID(CurrentUser);
}

static void UpdateTechniqueMenu(void) {
    switch (MenuState(&PlayerMenu[CurrentUser].TechniqueMenu)) {
    case CONTROL_CONFIRM:
        UpdateTargetMenu();
        break;
    
    case CONTROL_CANCEL:
        ResetMenuToIdle(&BattleMenu);
        break;
    
    case CONTROL_IDLE:
        UpdateMenu(&PlayerMenu[CurrentUser].TechniqueMenu);
        if (MenuConfirmed(&PlayerMenu[CurrentUser].TechniqueMenu)) {
            CurrentTurn()->Technique = SelectedTechniqueID();
            LoadTargetMenu(SelectedTechnique()->Target);
        }
        break;
    }
}

static void UpdateItemsMenu(void) {
    TURN *turn = CurrentTurn();
    switch (ItemMenu.Control.State) {
    case CONTROL_CONFIRM:
        UpdateTargetMenu();
        break;
        
    case CONTROL_CANCEL:
        ResetMenuToIdle(&BattleMenu);
        LockedItemIndices[CurrentUser] = NOT_LOCKED;
        break;

    case CONTROL_IDLE:
        UpdateMenu(&ItemMenu);
        if (MenuConfirmed(&ItemMenu)) {
            if (SelectedItemLocked()) {
                ResetMenuToIdle(&ItemMenu);
            } else {
                turn->Technique = DEFAULT_ITEM;
                turn->Item = SelectedItemID();
                LockedItemIndices[CurrentUser] = MenuItem(&ItemMenu);
                LoadTargetMenu(ALLY);
            }
        }
        break;
    }
}

void UpdateBattleMenuOnConfirm(void) {
    TURN *turn = CurrentTurn();
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
        ResetMenu(&PlayerMenu[CurrentUser].TechniqueMenu);
        break;
        
    case BATTLE_ITEM:
        ResetMenu(&ItemMenu);
        break;
        
    case BATTLE_CANCEL:
        if (CurrentUser==FirstUser()) {
            turn->Technique = DEFAULT_ESCAPE,
            LoadTargetMenu(TechniqueByID(turn->Technique)->Target);
        }
        break;
    }
}

/**********************************************************//**
 * @brief Updates the battle menu for the user to input turns.
 **************************************************************/
void UpdateBattleMenu(void) {
    // Set up the current turn
    CurrentTurn()->User = CurrentUser;
    
    // Update battle menu system
    BattleMenu.Option[BATTLE_CANCEL] = CurrentUser==FirstUser()? "Escape": "Cancel";
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
            UpdateTechniqueMenu();
            break;
        case BATTLE_ITEM:
            UpdateItemsMenu();
            break;
        case BATTLE_CANCEL:
            if (CurrentUser == FirstUser()) {
                // Fast-forward out of the battle menu
                if (EscapeBattle()) {
                    for (int id=0; id<TEAM_SIZE; id++) {
                        TurnByID(id)->State = TURN_INACTIVE;
                    }
                    CurrentUser = TEAM_SIZE;
                }
            } else {
                JumpToPreviousUser();
                ResetMenu(&PlayerMenu[CurrentUser].TechniqueMenu);
            }
            break;
        }
        break;
    
    case CONTROL_IDLE:
        UpdateMenu(&BattleMenu);
        if (MenuConfirmed(&BattleMenu)) {
            UpdateBattleMenuOnConfirm();
        }
        break;
    
    case CONTROL_CANCEL:
        if (CurrentUser > FirstUser()) {
            JumpToPreviousUser();
        }
        ResetMenu(&BattleMenu);        
        break;
    }
}

/**************************************************************/
