/**********************************************************//**
 * @file game.h
 * @brief Header for basic game information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>            // bool
#include <allegro5/allegro.h>   // ALLEGRO_KEY_*

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
extern double LastFrameTimeElapsed;
extern double TotalTimeElapsed;

/**************************************************************/
extern bool KeyDown(KEY key);

/**************************************************************/
#endif // _GAME_H_
