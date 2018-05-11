/**********************************************************//**
 * @file game.c
 * @brief Controls the entire game.
 * @author Rena Shinomiya
 * @date April 24, 2018
 **************************************************************/

#include <stddef.h>             // NULL
#include <stdbool.h>            // bool
#include <assert.h>             // assert

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "game.h"               // KEY
#include "assets.h"             // LoadAssets, DestroyAssets
#include "debug.h"              // assert

// Included for debugging purposes - not final
#include "menu.h"
#include "battle.h"
#include "output.h"

/**************************************************************/
/// @brief Time spent in last frame, used for animation.
static double LastFrameTimeElapsed = 0.0;

/**********************************************************//**
 * @brief Gets the tile spent on the last frame.
 * @return Time spent on the last frame.
 **************************************************************/
double LastFrameTime(void) {
    return LastFrameTimeElapsed;
}

/**************************************************************/
/// @brief Main game display object.
static ALLEGRO_DISPLAY *Display;

/// @brief Bitmap for scaling pixel art to different screen
/// sizes and resolutions (namely, fullscreen mode).
static ALLEGRO_BITMAP *ScaleBuffer;

/// @brief Whether the game should be terminated. Causes
/// the game to end on the next frame.
static bool StopGame = false;

// ScaleBuffer data
static int ScaleX;      ///< X offset for ScaleBuffer.
static int ScaleY;      ///< Y offset for ScaleBuffer.
static int ScaleW;      ///< Width scaling in ScaleBuffer.
static int ScaleH;      ///< Height scaling in ScaleBuffer.

/// @brief Main event queue for the game - tied to frame
/// timer, key events, etc...
static ALLEGRO_EVENT_QUEUE *EventQueue;

/// @brief Ensures an event is generated at rate equal to
/// the FRAME_RATE parameter from game.h.
static ALLEGRO_TIMER *FrameRateTimer;

/// @brief True if the game is in fullscreen-window mode.
static bool Fullscreen = false;

/**********************************************************//**
 * @brief Updates the ScaleBuffer when the screen changes.
 **************************************************************/
static void ResizeScreen(void) {
    if (ScaleBuffer) {
        al_destroy_bitmap(ScaleBuffer);
    }
    
    // Set up scaling backbuffer
    ScaleBuffer = al_create_bitmap(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    int windowWidth = al_get_display_width(Display);
    int windowHeight = al_get_display_height(Display);
    int sx = windowWidth / (float)DISPLAY_WIDTH;
    int sy = windowHeight / (float)DISPLAY_HEIGHT;
    int scale = (sx>sy)? sy: sx;
    
    // Calculate scaling
    ScaleW = DISPLAY_WIDTH*scale;
    ScaleH = DISPLAY_HEIGHT*scale;
    ScaleX = (windowWidth-ScaleW)/2;
    ScaleY = (windowHeight-ScaleH)/2;
}

/**********************************************************//**
 * @enum KEY_STATE
 * @brief Used to determine if a key is up, down, and if that
 * change happened on this frame or not.
 **************************************************************/
typedef enum {
    KEY_STATE_UP              = 0x00,
    KEY_STATE_DOWN            = 0x01,
    KEY_STATE_JUST_UP         = 0x10,
    KEY_STATE_JUST_DOWN       = 0x11,
} KEY_STATE;

/// @brief Buffer for keyboard states.
static KEY_STATE KeyboardState[ALLEGRO_KEY_MAX];

/**********************************************************//**
 * @brief Determines if the key is pressed.
 * @param key: The KEY to check.
 * @return Whether they key is being pressed on this frame.
 **************************************************************/
bool KeyDown(KEY key) {
    return KeyboardState[key] & KEY_STATE_DOWN;
}

/**********************************************************//**
 * @brief Determines if the key was just pressed this frame.
 * @param key: The KEY to check.
 * @return True if the key was just pressed this frame.
 **************************************************************/
bool KeyJustDown(KEY key) {
    return KeyboardState[key] == KEY_STATE_JUST_DOWN;
}

/**********************************************************//**
 * @brief Determines if the key was just released this frame.
 * @param key: The KEY to check.
 * @return True if the key was just released this frame.
 **************************************************************/
bool KeyJustUp(KEY key) {
    return KeyboardState[key] == KEY_STATE_JUST_UP;
}

/**********************************************************//**
 * @brief Copy the results of the last frame into a new
 * bitmap.
 * @return Pointer to the new screenshot image.
 **************************************************************/
ALLEGRO_BITMAP *Screenshot(void) {
    return al_clone_bitmap(al_get_backbuffer(Display));
}

/**********************************************************//**
 * @brief Initializes the Allegro5 platform.
 **************************************************************/
static void AllegroInstall(void) {
    // Initialize Allegro5 platform
    assert(al_init());
    assert(al_install_keyboard());
    
    // Initialize addons
    assert(al_init_image_addon());
    assert(al_init_primitives_addon());
    assert(al_install_audio());
    assert(al_init_acodec_addon());
    assert(al_init_font_addon());
    assert(al_init_ttf_addon());
}

/**********************************************************//**
 * @brief Initializes the game before the main loop begins.
 **************************************************************/
static void GameInitialize(void) {
    // Set up the display
    al_set_new_display_option(ALLEGRO_COLOR_SIZE, 24, ALLEGRO_REQUIRE);
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    al_set_new_window_title("Spectrum Legacy");
    if (Fullscreen) {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    }
    Display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    al_inhibit_screensaver(true);
    ResizeScreen();
    
    // Blender
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    
    // Force display to render black screen; this eliminates
    // garbage display data.
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    
    // Set up the event queue
    EventQueue = al_create_event_queue();
    FrameRateTimer = al_create_timer(1.0 / FRAME_RATE);
    al_register_event_source(EventQueue, al_get_display_event_source(Display));
    al_register_event_source(EventQueue, al_get_keyboard_event_source());
    al_register_event_source(EventQueue, al_get_timer_event_source(FrameRateTimer));

    // Load game assets
    LoadAssets();
    
    // TODO Debug information goes here... Remove!
    if (!LoadGame()) {
        NewGame();

        CreateSpectra(&Player->Spectra[1], GLACIALITH, 100);
        CreateSpectra(&Player->Spectra[2], KARDA, 100);
        CreateSpectra(&Player->Spectra[3], FLOPJELLY, 100);
        CreateSpectra(&Player->Spectra[4], COALSHARK, 100);
        
        Player->Inventory[0] = WATER_BOTTLE;
        Player->Inventory[1] = FISHING_ROD;
        Player->Inventory[2] = POTION;
        Player->Inventory[3] = TOUGH_HERB;
        Player->Inventory[4] = SERUM;
        
        InitializeRandomEncounter(3, ENCOUNTER_OVERWORLD);
    }
}

/**********************************************************//**
 * @brief Updates the screen on one frame.
 **************************************************************/
static void Update(void) {
    UpdateBattle();
}

/**********************************************************//**
 * @brief Renders the screen on one frame.
 **************************************************************/
static void Draw(void) {
    DrawBattle();
}

/**********************************************************//**
 * @brief Executes the main game loop until the game stops.
 **************************************************************/
static void GameMainLoop(void) {
    // State variables
    ALLEGRO_EVENT event;
    StopGame = false;
    bool paused = false;
    
    // Timing variables
    const double startTime = al_get_time();
    double currentTime;
    double lastFrameTime = startTime;
    
    // Main game loop
    al_start_timer(FrameRateTimer);
    while (!StopGame) {
        // Event management
        al_wait_for_event(EventQueue, &event);
        switch (event.type) {
        case ALLEGRO_EVENT_TIMER:
            // The frame rate timer was activated.
            // No other timers should be registered (this will
            // fail otherwise).
            assert(event.timer.source == FrameRateTimer);
            
            // Timing log
            currentTime = al_get_time();
            LastFrameTimeElapsed = currentTime - lastFrameTime;
            lastFrameTime = currentTime;
            
            // Frame rendering
            if (!paused) {
                al_set_target_bitmap(ScaleBuffer);
                Update();
                al_clear_to_color(al_map_rgb(0, 0, 0));
                Draw();
                al_set_target_backbuffer(Display);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_scaled_bitmap(ScaleBuffer, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, ScaleX, ScaleY, ScaleW, ScaleH, 0);
                al_flip_display();
            }
            
            // Remove "Just" events
            for (int i=0; i<ALLEGRO_KEY_MAX; i++) {
                KeyboardState[i] &= 0x01;
            }
            break;
        
        case ALLEGRO_EVENT_KEY_DOWN:
            // Game control keys
            if (event.keyboard.keycode == ALLEGRO_KEY_F11) {
                if (al_set_display_flag(Display, ALLEGRO_FULLSCREEN_WINDOW, !Fullscreen)) {
                    Fullscreen = !Fullscreen;
                    ResizeScreen();
                }
            } else {
                KeyboardState[event.keyboard.keycode] = KEY_STATE_JUST_DOWN;
            }
            break;
        
        case ALLEGRO_EVENT_KEY_CHAR:
            KeyboardState[event.keyboard.keycode] = KEY_STATE_JUST_DOWN;
            break;
            
        case ALLEGRO_EVENT_KEY_UP:
            KeyboardState[event.keyboard.keycode] = KEY_STATE_JUST_UP;
            break;
            
        case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING:
            paused = true;
            al_stop_timer(FrameRateTimer);
            al_acknowledge_drawing_halt(Display);
            break;
        
        case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING:
            paused = false;
            al_resume_timer(FrameRateTimer);
            al_acknowledge_drawing_resume(Display);
            break;
        
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            StopGame = true;
            break;
        }
    }
}

/**********************************************************//**
 * @brief Get rid of stuff the game created in preparation
 * for shutting down.
 **************************************************************/
static void GameDestroy(void) {
    // Get rid of the assets
    DestroyAssets();
    
    // Destroy the event queue
    al_destroy_timer(FrameRateTimer);
    al_destroy_event_queue(EventQueue);
    
    // Destroy the display
    al_inhibit_screensaver(false);
    al_destroy_display(Display);
}

/**********************************************************//**
 * @brief Get rid of Allegro5 in preparation for shutting
 * down the game.
 **************************************************************/
static void AllegroUninstall(void) {
    // Remove addons
    al_shutdown_font_addon();
    al_uninstall_audio();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    
    // Shut down Allegro5 platform
    al_uninstall_keyboard();
    al_uninstall_system();
}

/**********************************************************//**
 * @brief Main game function and program entry point.
 * @param argc: Command-line argument count.
 * @param argv: Command-line argument strings.
 * @return Exit code.
 **************************************************************/
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    AllegroInstall();
    GameInitialize();
    GameMainLoop();
    GameDestroy();
    AllegroUninstall();
    return 0;
}

/**************************************************************/
