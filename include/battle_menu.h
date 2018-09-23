/**********************************************************//**
 * @file battle_menu.h
 * @brief Implements the battle menu interface.
 * @author Rena Shinomiya
 * @date September 23, 2018
 **************************************************************/

#ifndef _BATTLE_MENU_H_
#define _BATTLE_MENU_H_
 
/**************************************************************/
extern void InitializeBattleMenu(void);
extern void DrawBattleMenu(void);
extern void UpdateBattleMenu(void);

/**************************************************************/
extern bool BattleMenuDone(void);
extern int BattleMenuCurrentUserID(void);
extern int BattleMenuCurrentTargetID(void);

/**************************************************************/
#endif // _BATTLE_MENU_H_
