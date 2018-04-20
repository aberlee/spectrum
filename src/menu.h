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

// TODO split out wait, make OPTIONS named MENU, make
// OPTIONS be a n Options*[] with a CONTROLLER abstraction

typedef enum {
    WAIT_BEFORE,
    WAIT_DURING,
    WAIT_AFTER,
} WAIT_STATE;

typedef struct {
    KEY Key;
    WAIT_STATE State;
} WAIT;

typedef enum {
    MENU_IDLE           = 0,
    MENU_CONFIRM        = 1,
    MENU_CANCEL         = 2,
    MENU_MOVE           = 3,
} MENU_STATE;

/**********************************************************//**
 * @struct OPTIONS
 * @brief Contains text for displaying choices on a menu.
 **************************************************************/
typedef struct {
    const char *Option[16];
    int Index;
    int Scroll;
    int IndexMax;
    int ScrollMax;
    int Jump;
    MENU_STATE State;
} OPTIONS;

/**************************************************************/
extern void DrawChoice(const OPTIONS *choice);
extern void DrawOption(const OPTIONS *choice);
extern void DrawColumn(const OPTIONS *first, const OPTIONS *second);
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

extern void UpdateMenu(OPTIONS *options);

extern void DrawMainMenu(void);
extern void UpdateMainMenu(void);

/**************************************************************/
#endif // _MENU_H_