/**********************************************************//**
 * @file main_menu.c
 * @brief Implements main menu processing.
 * @author Rena Shinomiya
 * @date September 18th, 2018
 **************************************************************/

#include <stdio.h>              // snprintf
#include <math.h>               // sin

#include "menu.h"               // MENU
#include "species.h"            // SPECTRA
#include "technique.h"          // TECHNIQUE_ID
#include "wait.h"               // WAIT
#include "item.h"               // ITEM_ID
#include "player.h"             // Player
#include "output.h"             // Output
#include "assets.h"             // WindowImage

/**************************************************************/
/// @brief Wait data for when the main menu opens an overlay.
static WAIT Overlay = WAIT_INITIALIZER(KEY_DENY);

/// @brief Whether the last save succeeded or failed.
static bool SaveStatus = false;

/// @brief Whether an item is being used currently and main
/// menu processing can't proceed as usual.
static bool ItemUseInProgress = false;

/**********************************************************//**
 * @enum SAVE_PHASE
 * @brief Informs the save system of how the save is progressing.
 **************************************************************/
typedef enum {
    SAVE_BEFORE,
    SAVE_AFTER,
} SAVE_PHASE;

/// @brief Current save system phase.
static SAVE_PHASE SavePhase;

/**********************************************************//**
 * @enum MAIN_MENU_OPTION
 * @brief Enumerates options in the main menu.
 **************************************************************/
typedef enum {
    MENU_PARTY,
    MENU_ITEMS,
    MENU_PLAYER,
    MENU_INFO,
    MENU_SAVE,
    MENU_EXIT,
} MAIN_MENU_OPTION;

/**********************************************************//**
 * @enum ITEM_MENU_OPTION
 * @brief Enumerates options in the item submenu.
 **************************************************************/
typedef enum {
    ITEM_USE,
    ITEM_DROP,
    ITEM_CANCEL,
} ITEM_MENU_OPTION;

/**************************************************************/
/// @brief Defines the main menu and its control structure.
static MENU MainMenu = {
    .Option = {
        [MENU_PARTY]    = "Party",
        [MENU_ITEMS]    = "Items",
        [MENU_PLAYER]   = "Player",
        [MENU_INFO]     = "Options",
        [MENU_SAVE]     = "Save",
        [MENU_EXIT]     = "Exit",
    },
    .Control = {
        .IndexMax       = 5,
    },
};

/// @brief Defines the items submenu.
static MENU ItemMenu = {
    .Option = {
        [ITEM_USE]      = "Use",
        [ITEM_DROP]     = "Drop",
        [ITEM_CANCEL]   = "Cancel",
    },
    .Control = {
        .IndexMax       = 2,
    },
};

static MENU YesNo = {
    .Option = {
        "Yes",
        "No",
    },
    .Control = {
        .IndexMax       = 1,
    },
};

/**********************************************************//**
 * @brief Use an item from the main menu system.
 * @param id: Item to use.
 * @param spectra: Target of the item.
 * @return True if the item was used, else false.
 **************************************************************/
static bool UseItem(ITEM_ID id, SPECTRA *spectra) {
    ItemUseInProgress = true;
    const ITEM *item = ItemByID(id);
    if (item->Effect == EFFECT_SPECIAL) {
        if (UseMapItem(id)) {
            return true;
        } else {
            Output("That can't be used right now!");
            return false;
        }
    } else {
        if (ApplyEffectInMenu(item->Effect, spectra, item->Argument)) {
            if (!(item->Flags&REUSABLE)) {
                DropItem(id);
            }
            return true;
        } else {
            Output("There was no effect...");
            return false;
        }
    }
}

static inline void InitializePartyMenu(void) {
    for (int i=0; i<PARTY_SIZE && Player->Spectra[i].Species; i++) {
        PartyControl()->IndexMax = i;
    }
}

static inline void InitializeItemsMenu(void) {
    for (int i=0; i<INVENTORY_SIZE && Player->Inventory[i]; i++) {
        ItemsControl()->IndexMax = i;
    }
}

/**********************************************************//**
 * @brief Sets up the main menu when it's opened.
 **************************************************************/
void InitializeMainMenu(void) {
    InitializePartyMenu();
    InitializeItemsMenu();
    ResetControl(&MainMenu.Control);
}

static ITEM_ID SelectedItemID(void) {
    int index = ControlItem(ItemsControl());
    return Player->Inventory[index];
}

static SPECTRA *SelectedSpectra(void) {
    int index = ControlItem(PartyControl());
    return &Player->Spectra[index];
}

/**********************************************************//**
 * @brief Draws the main menu and any of its descendants
 * on the screen.
 **************************************************************/
void DrawMainMenu(void) {
    // Draw main menu
    DrawAt(326, 92);
    DrawOption(&MainMenu);
    
    // Draw sub-menus
    if (MainMenu.Control.State == CONTROL_CONFIRM) {
        switch (MenuItem(&MainMenu)) {
        case MENU_PARTY:
            DrawAt(18, 92);
            DrawParty();
            if (PartyControl()->State == CONTROL_CONFIRM) {
                DrawAt(26, 100);
                DrawSpectraDisplay(SelectedSpectra());
            }
            break;

        case MENU_ITEMS:
            DrawAt(4, 92);
            DrawItems();
            DrawAt(4, 216);
            DrawItemDisplay(SelectedItemID());
            if (ItemsControl()->State == CONTROL_CONFIRM) {
                DrawAt(12, 100);
                DrawOption(&ItemMenu);
                if (ItemMenu.Control.State == CONTROL_CONFIRM) {
                    switch (MenuItem(&ItemMenu)) {
                    case ITEM_USE:
                        DrawAt(20, 108);
                        DrawParty();
                        break;
                    case ITEM_DROP:
                        DrawAt(20, 108);
                        DrawChoice(&YesNo);
                        break;
                    default:
                        break;
                    }
                }
            }
            break;

        case MENU_PLAYER:
            DrawAt(179, 92);
            DrawPlayerDisplay();
            break;

        case MENU_SAVE:
            DrawAt(194, 92);
            switch (SavePhase) {
            case SAVE_BEFORE:
                DrawAlert("Now saving...");
                break;
            case SAVE_AFTER:
                DrawAlert(SaveStatus? "Now saving...\nComplete!": "Now saving...\nFailed.");
                break;
            }
            break;
        }
    }
    
    // Items being used?
    if (ItemUseInProgress) {
        DrawAt(0, 0);
        DrawOutput();
    }
}

static void ExitMainMenu(void) {
    MainMenu.Control.State = CONTROL_CANCEL;
}

static void UpdateItemSubmenuUse(void) {
    switch (PartyControl()->State) {
    case CONTROL_CONFIRM:
        UseItem(SelectedItemID(), SelectedSpectra());
        InitializeItemsMenu();
        break;
        
    case CONTROL_IDLE:
        UpdateControl(PartyControl());
        break;
        
    case CONTROL_CANCEL:
        ItemMenu.Control.State = CONTROL_IDLE;
        break;
    }
}

static void UpdateItemSubmenuDrop(void) {
    switch (YesNo.Control.State) {
    case CONTROL_CONFIRM:
        if (MenuItem(&YesNo) == 0) {
            DropItem(SelectedItemID());
        }
        InitializeItemsMenu();
        ItemsControl()->State = CONTROL_IDLE;
        break;
    
    case CONTROL_IDLE:
        UpdateMenu(&YesNo);
        break;
     
    case CONTROL_CANCEL:
        // Return to main items list
        ItemsControl()->State = CONTROL_IDLE;
        break;
    }
}

static void UpdateItemSubmenuOnConfirm(void) {
    ITEM_ID id = SelectedItemID();
    const ITEM *item = ItemByID(id);

    switch (MenuItem(&ItemMenu)) {
    case ITEM_USE:
        if (item->Flags&MENU_ONLY) {
            if (item->Effect==EFFECT_SPECIAL) {
                // Immediately activate overworld effect and exit out of
                // the main menu system.
                ItemsControl()->State = CONTROL_IDLE;
                if (UseItem(id, NULL)) {
                    ExitMainMenu();
                }
            } else {
                // Prepare to select a target.
                ResetControl(PartyControl());
            }
        } else {
            Output("This can't be used right now!");
            ItemMenu.Control.State = CONTROL_IDLE;
        }
        break;
    
    case ITEM_DROP:
        if (item->Flags&IMPORTANT) {
            Output("This is too important to throw out!");
            ItemsControl()->State = CONTROL_IDLE;
        } else {
            // Default to "No"
            ResetControl(&YesNo.Control);
            YesNo.Control.Index = 1;
        }
        break;
    
    case ITEM_CANCEL:
        break;
    }
}

static void UpdateItemSubmenu(void) {
    switch (ItemMenu.Control.State) {
    // Confirm pressed on items submenu.
    case CONTROL_CONFIRM:
        switch (MenuItem(&ItemMenu)) {
        case ITEM_USE:
            UpdateItemSubmenuUse();
            break;
        case ITEM_DROP:
            UpdateItemSubmenuDrop();
            break;
        case ITEM_CANCEL:
            ItemsControl()->State = CONTROL_IDLE;
            break;
        }
        break;

    // Items submenu being used to select something
    case CONTROL_IDLE:
        UpdateMenu(&ItemMenu);
        if (ItemMenu.Control.State==CONTROL_CONFIRM) {
            UpdateItemSubmenuOnConfirm();
        }
        break;
    
    // Items submenu was cancelled
    case CONTROL_CANCEL:
        ItemsControl()->State = CONTROL_IDLE;
        break;
    }
}

void UpdatePartyMenu(void) {
    switch (PartyControl()->State) {
    case CONTROL_CONFIRM:
        if (KeyJustUp(KEY_DENY)) {
            PartyControl()->State = CONTROL_IDLE;
        }
        break;

    case CONTROL_CANCEL:
        MainMenu.Control.State = CONTROL_IDLE;
        break;

    case CONTROL_IDLE:
        UpdateControl(PartyControl());
        break;
    }
}

void UpdateItemsMenu(void) {
    switch (ItemsControl()->State) {
    case CONTROL_CONFIRM:
        UpdateItemSubmenu();
        break;

    case CONTROL_CANCEL:
        // Return to the main menu
        MainMenu.Control.State = CONTROL_IDLE;
        break;

    case CONTROL_IDLE:
        UpdateControl(ItemsControl());
        if (ItemsControl()->State == CONTROL_CONFIRM) {
            ResetControl(&ItemMenu.Control);
        }
        break;
    }
}

void UpdateSave(void) {
    switch (SavePhase) {
    case SAVE_BEFORE:
        SaveStatus = SaveGame();
        SavePhase = SAVE_AFTER;
        break;

    case SAVE_AFTER:
        UpdateWait(&Overlay);
        if (!IsWaiting(&Overlay)) {
            MainMenu.Control.State = CONTROL_IDLE;
        }
        break;
    }
}

void UpdateMainMenuOnConfirm(void) {
    switch (MenuItem(&MainMenu)) {
    case MENU_PARTY:
        ResetControl(PartyControl());
        break;

    case MENU_ITEMS:
        ResetControl(ItemsControl());
        break;

    case MENU_PLAYER:
    case MENU_INFO:
        ResetWait(&Overlay);
        break;

    case MENU_SAVE:
        ResetWait(&Overlay);
        SavePhase = SAVE_BEFORE;
        break;

    case MENU_EXIT:
        ExitMainMenu();
        break;
    }
}

/**********************************************************//**
 * @brief Updates the main menu and any of its descendants.
 **************************************************************/
void UpdateMainMenu(void) {
    // Pre-empted by output
    if (ItemUseInProgress) {
        UpdateOutput();
        if (OutputDone()) {
            ItemUseInProgress = false;
            PartyControl()->State = CONTROL_CANCEL;
            ItemsControl()->State = CONTROL_IDLE;
        }
        return;
    }
    
    switch (MainMenu.Control.State) {
    // Main menu selection confirmed
    case CONTROL_CONFIRM:
        switch (MenuItem(&MainMenu)) {
        case MENU_PARTY:
            UpdatePartyMenu();
            break;
        
        case MENU_ITEMS:
            UpdateItemsMenu();
            break;
        
        case MENU_PLAYER:
        case MENU_INFO:
            UpdateWait(&Overlay);
            if (!IsWaiting(&Overlay)) {
                MainMenu.Control.State = CONTROL_IDLE;
            }
            break;
        
        case MENU_SAVE:
            UpdateSave();
            break;
        }
        break;
     
     // The player can use the main menu currently.
     case CONTROL_IDLE:
        UpdateMenu(&MainMenu);
        if (MainMenu.Control.State == CONTROL_CONFIRM) {
            UpdateMainMenuOnConfirm();
        }
        break;
    
    // The menu has been cancelled.
    case CONTROL_CANCEL:
        break;
    }
}

/**********************************************************//**
 * @brief Check if the main menu was cancelled and processing
 * should return to whatever invoked the menu.
 * @return True if the main menu is closed.
 **************************************************************/
bool MainMenuClosed(void) {
    return MainMenu.Control.State == CONTROL_CANCEL;
}

/**************************************************************/
