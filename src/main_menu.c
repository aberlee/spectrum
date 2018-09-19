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

typedef enum {
    ITEM_USE,
    ITEM_MOVE,
    ITEM_DROP,
    ITEM_CANCEL,
} ITEM_MENU_OPTION;

static MENU ItemMenu = {
    .Option = {
        [ITEM_USE]      = "Use",
        [ITEM_MOVE]     = "Move",
        [ITEM_DROP]     = "Drop",
        [ITEM_CANCEL]   = "Cancel",
    },
    .Control = {
        .IndexMax       = 3,
    },
};

/// @brief Wait data for when the main menu opens an overlay.
static WAIT Overlay = WAIT_INITIALIZER(KEY_DENY);

/**********************************************************//**
 * @enum SAVE_PHASE
 * @brief Informs the save system of how the save is progressing.
 **************************************************************/
typedef enum {
    SAVE_BEFORE,
    SAVE_DURING,
    SAVE_AFTER,
} SAVE_PHASE;

/// @brief Current save system phase.
static SAVE_PHASE SavePhase;

/// @brief Whether the last save succeeded or failed.
static bool SaveStatus = false;

static bool ItemUseInProgress = false;

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
            return true;
        } else {
            Output("There was no effect...");
            return false;
        }
    }
}

/**********************************************************//**
 * @brief Sets up the main menu when it's opened.
 **************************************************************/
void InitializeMainMenu(void) {
    for (int i=0; i<PARTY_SIZE && Player->Spectra[i].Species; i++) {
        PartyControl()->IndexMax = i;
    }
    for (int i=0; i<INVENTORY_SIZE && Player->Inventory[i]; i++) {
        ItemsControl()->IndexMax = i;
    }
    ResetControl(&MainMenu.Control);
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
                DrawSpectraDisplay(&Player->Spectra[ControlItem(PartyControl())]);
            }
            break;

        case MENU_ITEMS:
            DrawAt(4, 92);
            DrawItems();
            DrawAt(4, 216);
            DrawItemDisplay(Player->Inventory[ControlItem(ItemsControl())]);
            if (ItemsControl()->State == CONTROL_CONFIRM) {
                DrawAt(12, 100);
                DrawParty();
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
            case SAVE_DURING:
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

/**********************************************************//**
 * @brief Updates the main menu and any of its descendants.
 **************************************************************/
void UpdateMainMenu(void) {
    // If item use is ongoing, resolve that through first
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
    // Cases where a submenu is currently open:
    // Party, Items, Player, Info, and Save screens.
    case CONTROL_CONFIRM:
        switch (MenuItem(&MainMenu)) {
        case MENU_PARTY:
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
            break;
        
        case MENU_ITEMS:
            switch (ItemsControl()->State) {
            case CONTROL_CONFIRM:
                // Select a spectra to use the item on
                switch (PartyControl()->State) {
                case CONTROL_CONFIRM: {
                        const ITEM_ID id = Player->Inventory[ControlItem(ItemsControl())];
                        SPECTRA *spectra = &Player->Spectra[ControlItem(PartyControl())];
                        UseItem(id, spectra);
                    }
                    break;
                case CONTROL_CANCEL:
                    ItemsControl()->State = CONTROL_IDLE;
                    break;
                case CONTROL_IDLE:
                    UpdateControl(PartyControl());
                    break;
                }
                break;
            case CONTROL_CANCEL:
                MainMenu.Control.State = CONTROL_IDLE;
                break;
            case CONTROL_IDLE:
                UpdateControl(ItemsControl());
                // Initialize spectra choice submenu
                if (ItemsControl()->State == CONTROL_CONFIRM) {
                    // Ensure the item can be used from the menu
                    const ITEM_ID id = Player->Inventory[ControlItem(ItemsControl())];
                    const ITEM *item = ItemByID(id);
                    if (item->Flags & MENU_ONLY) {
                        if (item->Effect == EFFECT_SPECIAL) {
                            ItemsControl()->State = CONTROL_IDLE;
                            if (UseItem(id, NULL)) {
                                MainMenu.Control.State = CONTROL_CANCEL;
                            }
                        } else {
                            ResetControl(PartyControl());
                        }
                    } else {
                        ItemsControl()->State = CONTROL_IDLE;
                    }
                }
                break;
            }
            break;
        
        case MENU_PLAYER:
        case MENU_INFO:
            UpdateWait(&Overlay);
            if (!IsWaiting(&Overlay)) {
                MainMenu.Control.State = CONTROL_IDLE;
            }
            break;
        
        case MENU_SAVE:
            switch (SavePhase) {
            case SAVE_BEFORE:
                SavePhase = SAVE_DURING;
                break;
            case SAVE_DURING:
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
            break;
        }
        break;
     
     // The player can use the main menu currently.
     case CONTROL_IDLE:
        // Update menu and initialize new submenu if needed
        UpdateMenu(&MainMenu);
        if (MainMenu.Control.State == CONTROL_CONFIRM) {
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
                // Immediately cancel out of menu
                MainMenu.Control.State = CONTROL_CANCEL;
                break;
            }
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
