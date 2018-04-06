/**********************************************************//**
 * @file keyboard.h
 * @brief Header for basic keyboard information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdbool.h>

typedef enum {
    KEY_LEFT=1,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_MENU,
    KEY_CONFIRM,
    KEY_DENY,
    KEY_DEBUG,
} KEY;

#define N_KEY (KEY_DEBUG+1)

extern bool KeyDown(KEY key);
extern void PressKey(int keycode);
extern void ReleaseKey(int keycode);
extern void ResetKeys(void);

/**************************************************************/
#endif // _KEYBOARD_H_
