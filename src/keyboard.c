/**********************************************************//**
 * @file keyboard.c
 * @brief Implements keyboard information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#include <allegro5/allegro.h>
 
#include "keyboard.h"

static KEY KeyMap[ALLEGRO_KEY_MAX] = {
    // Arrow Keys
    [ALLEGRO_KEY_LEFT]      = KEY_LEFT,
    [ALLEGRO_KEY_RIGHT]     = KEY_RIGHT,
    [ALLEGRO_KEY_UP]        = KEY_UP,
    [ALLEGRO_KEY_DOWN]      = KEY_DOWN,
    
    // WASD Keys
    [ALLEGRO_KEY_A]         = KEY_LEFT,
    [ALLEGRO_KEY_D]         = KEY_RIGHT,
    [ALLEGRO_KEY_W]         = KEY_UP,
    [ALLEGRO_KEY_S]         = KEY_DOWN,
    
    // Legacy Controls
    [ALLEGRO_KEY_M]         = KEY_MENU,
    [ALLEGRO_KEY_SPACE]     = KEY_CONFIRM,
    [ALLEGRO_KEY_V]         = KEY_DENY,
    
    // Intelligent Controls
    [ALLEGRO_KEY_E]         = KEY_MENU,
    
    // Debug Controls
    [ALLEGRO_KEY_ESCAPE]    = KEY_DEBUG,
    [ALLEGRO_KEY_F5]        = KEY_DEBUG,
};

static bool Pressed[N_KEY] = {false};

void PressKey(int keycode) {
    KEY key = KeyMap[keycode];
    if (key) {
        Pressed[key] = true;
    }
}

void ReleaseKey(int keycode) {
    KEY key = KeyMap[keycode];
    if (key) {
        Pressed[key] = false;
    }
}

void ResetKeys(void) {
    for (int i = 1; i < N_KEY; i++) {
        Pressed[i] = false;
    }
}

bool KeyDown(KEY key) {
    return Pressed[key];
}