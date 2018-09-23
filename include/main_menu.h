/**********************************************************//**
 * @file main_menu.h
 * @brief Defines the way the user interacts with the main
 * game menu, as well as rendering.
 * @author Rena Shinomiya
 * @date September 18th, 2018
 **************************************************************/

#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

/**************************************************************/
extern void InitializeMainMenu(void);
extern void DrawMainMenu(void);
extern void UpdateMainMenu(void);
extern bool MainMenuClosed(void);

/**************************************************************/
#endif // _MAIN_MENU_H_
