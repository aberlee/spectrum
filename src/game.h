/**********************************************************//**
 * @file game.h
 * @brief Header for basic game information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>            // bool
#include <allegro5/allegro.h>

/**************************************************************/
/// Size of the game window in pixels.
#define DISPLAY_WIDTH 480

/// Height of the game window in pixels.
#define DISPLAY_HEIGHT 360

/// Maximum frame rate of the game in FPS.
#define FRAME_RATE 60.0

/**********************************************************//**
 * @enum KEY
 * @brief Constants mapping each Spectrum key function to a
 * keycode recognized by Allegro.
 **************************************************************/
typedef enum {
    KEY_LEFT        = ALLEGRO_KEY_LEFT,
    KEY_RIGHT       = ALLEGRO_KEY_RIGHT,
    KEY_UP          = ALLEGRO_KEY_UP,
    KEY_DOWN        = ALLEGRO_KEY_DOWN,
    KEY_MENU        = ALLEGRO_KEY_M,
    KEY_CONFIRM     = ALLEGRO_KEY_SPACE,
    KEY_DENY        = ALLEGRO_KEY_V,
#ifdef DEBUG
    KEY_DEBUG       = ALLEGRO_KEY_D,
#endif
} KEY;

/**************************************************************/
extern double LastFrameTime(void);

/**************************************************************/
extern bool KeyDown(KEY key);
extern bool KeyJustDown(KEY key);
extern bool KeyJustUp(KEY key);

/**********************************************************//**
 * @brief Checks if the key is not being pressed.
 * @param key: They key to check.
 * @return True if the key isn't pressed.
 **************************************************************/
#define KeyUp(key) (!KeyDown(key))

/**************************************************************/
extern ALLEGRO_BITMAP *Screenshot(void);

/**********************************************************//**
 * @enum MODE_ID
 * @brief Defines identifiers for each unique game mode.
 **************************************************************/
typedef enum {
    MODE_BATTLE     = 1,
    MODE_MAP        = 2,
} MODE_ID;

/**************************************************************/
extern MODE_ID GetMode(void);
extern void SetMode(MODE_ID mode);

/**************************************************************/
#endif // _GAME_H_
