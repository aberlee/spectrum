/**********************************************************//**
 * @file menu.h
 * @brief Defines screens that appear in the menu, battle,
 * and game system to show information to the player.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

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

/**********************************************************//**
 * @struct OPTIONS
 * @brief Contains text for displaying choices on a menu.
 **************************************************************/
typedef struct {
    char **Option;
    int Index;
} OPTIONS;

/**************************************************************/
extern void DrawChoice(const OPTIONS *choice);
extern void DrawOption(const OPTIONS *choice);
extern void DrawColumn(const OPTIONS *choice);
extern void DrawAlert(const char *text);
extern void DrawWarning(const char *text);
extern void DrawBar(float percent, int x, int y);
extern void DrawSpectraDisplay(const SPECTRA *spectra);
extern void DrawHudUser(const SPECTRA *spectra);
extern void DrawHudEnemy(const SPECTRA *spectra);

/**************************************************************/
#endif // _MENU_H_