
// C standard library
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Main Allegro5 platform
#include <allegro5/allegro.h>

// Allegro5 graphics processing
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// Allegro5 audio processing
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

// Allegro5 font processing
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// This project
#include "game.h"
#include "assets.h"
#include "menu.h"
#include "output.h"
#include "debug.h"

const char INTRODUCTION[] = "MY GRANDMOTHER ONCE TOLD ME A STORY,/LONG AGO.*IT WAS ABOUT A GREAT KING WHO/BECAME BLINDED BY HIS POWER.*HIS NAME WAS VARAN.*VARAN WAS THE KING OF THE SPECTRA.*IN THE STORY, A LONE WARRIOR/HAD TO FACE THE GREAT KING VARAN,*WHO HAD ONCE RULED FAIRLY AND/PEACEFULLY, BUT HE BECAME*OBSESSED WITH HIS POWER.*HE BEGAN TO USE IT WRONGLY./HE WANTED ONLY MORE.*VARAN'S POWER GREW SO MUCH, THAT/HIS INFLUENCE BEGAN TO AFFECT LIFE.*GOOD HUSBANDS TURNED ON THEIR FAMILIES./EVERYONE RAN AMOK.*MANY LIVES WERE LOST, MANY WERE KILLED,/BY PEOPLE AND SPECTRA WHO WERE*TOUCHED BY VARAN'S EVIL AURA.*HIS POWER DID NOT AFFECT EVERYONE./SOME PEOPLE WERE STRONGER.*OTHERS WERE ABLE TO BREAK OUT FROM/VARAN'S INFLUENCE.*THESE PEOPLE ROSE FROM THE DEVASTATION/CAUSED BY THE REST.*THEY CALLED THEMSELVES THE REBELLION.*THE LEADER OF THE REBELLION WAS A/BEAUTIFUL WOMAN.*SHE LED HER REBEL ARMY TO THE/PALACE WHERE VARAN LIVED.*AND THEY FOUGHT A GREAT WAR.*MANY PEOPLE DIED IN THAT WAR.*AND THAT IS WHERE THE STORY ENDED.*MY GRANDMA NEVER TOLD ME THE END./SHE SAID I WOULD FIND OUT LATER.*I WAS ALWAYS AFRAID SHE DIED.";

// Allegro global variables
ALLEGRO_DISPLAY *Display;
ALLEGRO_BITMAP *ScaleBuffer;
double LastFrameTimeElapsed = 0.0;
double TotalTimeElapsed = 0.0;
bool StopGame = false;

static int ScaleX, ScaleY, ScaleW, ScaleH;

// Allegro static variables
static ALLEGRO_EVENT_QUEUE *EventQueue;
static ALLEGRO_TIMER *FrameRateTimer;
static ALLEGRO_KEYBOARD_STATE KeyboardState;

/**********************************************************//**
 * @brief Determines if the key is pressed.
 * @param key: The KEY to check.
 * @return Whether they key is being pressed on this frame.
 **************************************************************/
bool KeyDown(KEY key) {
    return al_key_down(&KeyboardState, key);
}

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

static void GameInitialize(void) {
    // Set up the display
    al_set_new_display_option(ALLEGRO_COLOR_SIZE, 24, ALLEGRO_REQUIRE);
    al_set_new_window_title("Spectrum Legacy");
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    Display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    al_inhibit_screensaver(true);
    
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
    
    NewGame();
    Output("Hello world!");
    Output("Multi-line text output\ntext!");
    OutputF("Formatted output text: %d", 5);
}

static void MenuTestUpdate(void) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_translate_transform(&trans, 10, 10);
    al_use_transform(&trans);
    MENU choice = {
        {"Yes", "No", "Third", "Fourth", "Fifth", "Cancel"},
        {.IndexMax=5},
    };
    DrawChoice(&choice);
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 60, 10);
    al_use_transform(&trans);
    DrawAlert("This is a test alert for the game window system!");
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 120, 10);
    al_use_transform(&trans);
    DrawWarning("This is a test warning for the game window system!");
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 10, 60);
    al_use_transform(&trans);
    DrawOption(&choice);
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 10, 140);
    al_use_transform(&trans);
    DrawColumn(&choice, &choice);
    
    SPECTRA spectra;
    CreateSpectra(&spectra, GLACIALITH, 100);
    spectra.Health /= 2;
    spectra.Power /= 4;
    spectra.Ailment = AFLAME;
    al_identity_transform(&trans);
    al_translate_transform(&trans, 200, 80);
    al_use_transform(&trans);
    DrawSpectraDisplay(&spectra);
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 200, 40);
    al_use_transform(&trans);
    DrawHudUser(&spectra);
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 200, 20);
    al_use_transform(&trans);
    DrawHudEnemy(&spectra);
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 10, 200);
    al_use_transform(&trans);
    DrawTechniqueDisplay(INFERNO);
    
    al_identity_transform(&trans);
    al_use_transform(&trans);
    UpdateOutput();
    DrawOutputMap();
    
    al_identity_transform(&trans);
    al_translate_transform(&trans, 10, 10);
    al_use_transform(&trans);
    DrawPlayerDisplay();
}

static void Update(void) {
/*     MenuTestUpdate(); */
/*     static int yes = 1;
    if (yes) {
        Warp(MAP_OVERWORLD, 24*16, 43*16);
        yes = 0;
    }
    UpdateMap();
    DrawMap(); */
    UpdateMainMenu();
    DrawMainMenu();
}

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
            TotalTimeElapsed = currentTime - startTime;
            LastFrameTimeElapsed = currentTime - lastFrameTime;
            lastFrameTime = currentTime;
            
            // Frame rendering
            if (!paused) {
                al_set_target_bitmap(ScaleBuffer);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                Update();
                al_set_target_backbuffer(Display);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_scaled_bitmap(ScaleBuffer, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, ScaleX, ScaleY, ScaleW, ScaleH, 0);
                al_flip_display();
            }
            break;
        
        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_KEY_CHAR:
        case ALLEGRO_EVENT_KEY_UP:
            al_get_keyboard_state(&KeyboardState);
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