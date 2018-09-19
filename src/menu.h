/**********************************************************//**
 * @file menu.h
 * @brief Defines screens that appear in the menu, battle,
 * and game system to show information to the player.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

#include "game.h"               // KEY
#include "species.h"            // SPECTRA
#include "item.h"               // ITEM_ID

/**********************************************************//**
 * @enum WINDOW_ID
 * @brief Defines constants for each different window template.
 **************************************************************/
typedef enum {
    // Warnings
    ALERT               =  1,
    WARNING,
    
    // User options
    MENU_CHOICE,
    MENU_OPTION,
    MENU_COLUMN,
    
    // Heads-Up Displays
    HUD_ENEMY,
    HUD_USER,
    
    // Stat screens
    PLAYER_DISPLAY,
    SPECTRA_DISPLAY,
    TECHNIQUE_DISPLAY,
    ITEM_DISPLAY,
    
    // Lists
    SPECTRA_LIST,
    ITEM_LIST,
    
    // Output frames
    OUTPUT,
    
    // Popups
    POPUP_WINDOW,
    POPUP_BAR,
    POPUP_DETAIL,
} WINDOW_ID;

/// The number of different window templates defined in WINDOW_ID.
#define N_WINDOW (POPUP_DETAIL+1)

/**********************************************************//**
 * @enum CONTROL_STATE
 * @brief Defines constants for each state a CONTROL can be.
 **************************************************************/
typedef enum {
    CONTROL_IDLE        = 0,    ///< Control isn't doing anything.
    CONTROL_CONFIRM     = 1,    ///< Current item has been selected.
    CONTROL_CANCEL      = 2,    ///< User wants to cancel control.
} CONTROL_STATE;

/**********************************************************//**
 * @struct CONTROL
 * @brief Defines information for a menu control - supports
 * scrolling around through a list of information.
 **************************************************************/
typedef struct {
    int Index;                  ///< Current position on visible items.
    int IndexMax;               ///< Index <= IndexMax
    int Scroll;                 ///< Current scroll position of items.
    int ScrollMax;              ///< Scroll <= ScrollMax
    int Jump;                   ///< Number of items to jump using Left/Right.
    CONTROL_STATE State;        ///< What the control is doing.
} CONTROL;

/**********************************************************//**
 * @brief Gets the item selected with the control.
 * @param control: CONTROL structure to read.
 * @return Integer from 0 to Index+Scroll.
 **************************************************************/
static inline int ControlItem(const CONTROL *control) {
    return control->Index + control->Scroll;
}

/**********************************************************//**
 * @brief Move the control down one item, if possible.
 * @param control: CONTROL to edit.
 **************************************************************/
static inline void ControlDown(CONTROL *control) {
    if (control->Index < control->IndexMax) {
        control->Index++;
    } else if (control->Scroll < control->ScrollMax) {
        control->Scroll++;
    }
}

/**********************************************************//**
 * @brief Move the control up one item, if possible.
 * @param control: CONTROL to edit.
 **************************************************************/
static inline void ControlUp(CONTROL *control) {
    if (control->Index > 0) {
        control->Index--;
    } else if (control->Scroll > 0) {
        control->Scroll--;
    }
}

/**********************************************************//**
 * @brief Reset a CONTROL to the initial state.
 * @param control: CONTROL to reset.
 **************************************************************/
static inline void ResetControl(CONTROL *control) {
    control->Index = 0;
    control->Scroll = 0;
    control->State = CONTROL_IDLE;
}

/**************************************************************/
extern void UpdateControl(CONTROL *control);

/**********************************************************//**
 * @struct MENU
 * @brief Contains text for displaying choices on a menu.
 **************************************************************/
typedef struct {
    const char *Option[16];
    CONTROL Control;
} MENU;

/**********************************************************//**
 * @brief Updates the MENU's CONTROL with user input.
 * @param menu: MENU to update.
 **************************************************************/
static inline void UpdateMenu(MENU *menu) {
    UpdateControl(&menu->Control);
}

/**********************************************************//**
 * @brief Gets the item currently selected on the menu.
 * @param menu: MENU to read.
 * @return Index of item selected.
 **************************************************************/
static inline int MenuItem(const MENU *menu) {
    return ControlItem(&menu->Control);
}

/**************************************************************/
extern void DrawAt(int x, int y);
extern void DrawChoice(const MENU *choice);
extern void DrawOption(const MENU *choice);
extern void DrawColumn(const MENU *first, const MENU *second);
extern void DrawAlert(const char *text);
extern void DrawWarning(const char *text);
extern void DrawHudUser(const SPECTRA *spectra);
extern void DrawHudEnemy(const SPECTRA *spectra);
extern void DrawSpectraDisplay(const SPECTRA *spectra);
extern void DrawTechniqueDisplay(TECHNIQUE_ID id);
extern void DrawItemDisplay(ITEM_ID id);
extern void DrawOutput(void);
extern void DrawPlayerDisplay(void);
extern void DrawPopupBar(const char *text);

/**************************************************************/
extern void DrawParty(void);
extern void DrawItems(void);
extern CONTROL *PartyControl(void);
extern CONTROL *ItemsControl(void);

/**************************************************************/
#endif // _MENU_H_
