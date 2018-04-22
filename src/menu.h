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

/**********************************************************//**
 * @enum WINDOW_ID
 * @brief Defines constants for each different window template.
 **************************************************************/
typedef enum {
    // Warnings
    ALERT=1,
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
    OUTPUT_BATTLE,
    OUTPUT_MENU,
    OUTPUT_MAP,
} WINDOW_ID;

/// The number of different window templates defined in WINDOW_ID.
#define N_WINDOW (OUTPUT_MAP+1)

typedef enum {
    CONTROL_IDLE        = 0,
    CONTROL_CONFIRM     = 1,
    CONTROL_CANCEL      = 2,
    CONTROL_BUSY        = 3,
} CONTROL_STATE;

typedef struct {
    int Index;
    int IndexMax;
    int Scroll;
    int ScrollMax;
    int Jump;
    CONTROL_STATE State;
} CONTROL;

static inline int ControlItem(const CONTROL *control) {
    return control->Index + control->Scroll;
}

static inline void ControlDown(CONTROL *control) {
    if (control->Index < control->IndexMax) {
        control->Index++;
    } else if (control->Scroll < control->ScrollMax) {
        control->Scroll++;
    }
}

static inline void ControlUp(CONTROL *control) {
    if (control->Index > 0) {
        control->Index--;
    } else if (control->Scroll > 0) {
        control->Scroll--;
    }
}

extern void UpdateControl(CONTROL *control);

/**********************************************************//**
 * @struct MENU
 * @brief Contains text for displaying choices on a menu.
 **************************************************************/
typedef struct {
    const char *Option[16];
    CONTROL Control;
} MENU;

static inline void UpdateMenu(MENU *menu) {
    UpdateControl(&menu->Control);
}

static inline int MenuItem(const MENU *menu) {
    return ControlItem(&menu->Control);
}

/**************************************************************/
extern void DrawChoice(const MENU *choice);
extern void DrawOption(const MENU *choice);
extern void DrawColumn(const MENU *first, const MENU *second);
extern void DrawAlert(const char *text);
extern void DrawWarning(const char *text);
extern void DrawHudUser(const SPECTRA *spectra);
extern void DrawHudEnemy(const SPECTRA *spectra);
extern void DrawSpectraDisplay(const SPECTRA *spectra);
extern void DrawTechniqueDisplay(TECHNIQUE_ID id);
extern void DrawOutputBattle(void);
extern void DrawOutputMenu(void);
extern void DrawOutputMap(void);
extern void DrawPlayerDisplay(void);

extern void DrawMainMenu(void);
extern void UpdateMainMenu(void);

/**************************************************************/
#endif // _MENU_H_