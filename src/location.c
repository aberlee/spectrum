/**********************************************************//**
 * @file location.c
 * @brief Functionality for the overworld map processing.
 * @author Rena Shinomiya
 * @date April 15, 2018
 **************************************************************/

// World Coordinates refer to pixel positions on the map, and
// Tile Coordinates refer to individual tiles on the map (tile
// size is 16*16 pixels).

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <stdbool.h>            // bool

#include "location.h"           // MAP_ID, LOCATION
#include "game.h"               // KEY
#include "assets.h"             // MapImage, SensorImage
#include "event.h"              // EVENT, Events
#include "player.h"             // Player
#include "debug.h"              // eprintf

#include "location.i"           // LOCATION_DATA

/**************************************************************/
/// @brief The user's walking speed in pixels/second.
#define WALK_SPEED 120

/**************************************************************/
/// @brief The identity of the current map being displayed
/// (associated with the current location).
static MAP_ID CurrentMap;

/// @brief Event array for event callbacks from the current
/// map, associated with the current map.
static const EVENT *CurrentEvents;

/// @brief Sensor associated to the current map.
static SENSOR CurrentSensor;

/// @brief Bounding box on the overworld map. Going outside
/// these bounds on the overworld map will update the current
/// location.
static const COORDINATE *CurrentBounds;

#ifdef DEBUG
/// @brief Turns on map debugging features - displays sensor,
/// position, tile position, and interact position.
static bool DebugMap = true;
#endif

/// @brief Set if the main menu is open on the map.
static bool MainMenuOpen = false;

static int PlayerWalkFrame = 0;

/**********************************************************//**
 * @brief Gets LOCATION data from its ID.
 * @param id: Identity of the location.
 * @return Pointer to the constant location data.
 **************************************************************/
const LOCATION *Location(LOCATION_ID id) {
    return &LOCATION_DATA[id];
}

/**********************************************************//**
 * @brief Checks if the tile coordinate is in bounds,
 * according to the current sensor.
 * @param x: Tile X-position.
 * @param y: Tile Y-position.
 * @return True if (x, y) is in bounds.
 **************************************************************/
static inline bool TileInBounds(int x, int y) {
    return 0 <= x && x < CurrentSensor.Width && 0 <= y && y < CurrentSensor.Height;
}

/**********************************************************//**
 * @brief Checks if the world coordinate is in bounds.
 * @param bounds: The bounds to use. Should be an array of 2
 * COORDINATE structs, with [0] being the upper left, and [1]
 * being the lower right.
 * @param x: World X-coordinate to check.
 * @param y: World Y-coordinate to check.
 * @return True if (x, y) is in bounds.
 **************************************************************/
static inline bool WorldInBounds(const COORDINATE *bounds, int x, int y) {
    return bounds[0].X <= x && x < bounds[1].X && bounds[0].Y <= y && y < bounds[1].Y;
}

/**********************************************************//**
 * @brief Convert from Tile to World coordinates.
 * @param n: Tile coordinate.
 * @return World coordinate, centered on the tile.
 **************************************************************/
static inline int TileToWorld(int n) {
    return n*16+8;
}

/**********************************************************//**
 * @brief Convert from World to Tile coordinates.
 * @param n: World coordinate. World coordinates less than 0
 * are illegal.
 * @return Tile coordinate, containing the world coordinate.
 * Negative numbers are returned on invalid input, such as if
 * the world coordinate is out of bounds.
 **************************************************************/
static inline int WorldToTile(int n) {
    return (n<0)? (n/16)-1: (n/16);
}

/**********************************************************//**
 * @brief Maps from the special OVERWORLD keyword down to a
 * LOCATION on the overworld map whose bounding box contains
 * (x, y).
 * @param x: World X-coordinate.
 * @param y: World Y-coordinate.
 **************************************************************/
static void SetOverworldLocation(int x, int y) {
    for (int i=1; i < N_LOCATION; i++) {
        const LOCATION *location = Location(i);
        if (location->Map == MAP_OVERWORLD && WorldInBounds(location->Bounds, x, y)) {
            CurrentBounds = location->Bounds;
            Player->Location = (LOCATION_ID)i;
            return;
        }
    }
    eprintf("Invalid overworld location: (%d, %d)\n", x, y);
}

/**********************************************************//**
 * @brief Updates the overworld LOCATION if the user has gone
 * out of the previous LOCATION's bounds.
 **************************************************************/
static void UpdateOverworldLocation(void) {
    if (CurrentMap == MAP_OVERWORLD && !WorldInBounds(CurrentBounds, Player->Position.X, Player->Position.Y)) {
        SetOverworldLocation(Player->Position.X, Player->Position.Y);
    }
}

/**********************************************************//**
 * @brief Alias for a tile in the current sensor. This
 * can act as an lvalue or an rvalue.
 * @param x: Tile X position.
 * @param y: Tile Y-position.
 * @return The tile at x,y.
 * @details No error checking is done.
 **************************************************************/
#define Tile(x, y) CurrentSensor.Sensor[y*CurrentSensor.Width+x]

/**********************************************************//**
 * @brief Installs the sensor for the given map ID. This
 * controls how tiles on the map behave.
 * @param id: The map identity.
 **************************************************************/
static void UseSensor(MAP_ID id) {
    // Get rid of old sensor if it exists
    if (CurrentSensor.Sensor) {
        free(CurrentSensor.Sensor);
    }

    // Load the sensor image
    ALLEGRO_BITMAP *sensorImage = SensorImage(CurrentMap);
    al_lock_bitmap(sensorImage, al_get_bitmap_format(sensorImage), ALLEGRO_LOCK_READONLY);
    CurrentSensor.Height = al_get_bitmap_height(sensorImage);
    CurrentSensor.Width = al_get_bitmap_width(sensorImage);
    CurrentSensor.Sensor = (TILE *)malloc(sizeof(TILE)*CurrentSensor.Height*CurrentSensor.Width);
    if (!CurrentSensor.Sensor) {
        eprintf("Failed to load sensor for map %d\n", id);
        return;
    }
    
    // Load the tile information
    int argument = 1;
    for (int y = 0; y < CurrentSensor.Height; y++) {
        for (int x = 0; x < CurrentSensor.Width; x++) {
            // Get the pixel color
            ALLEGRO_COLOR color = al_get_pixel(sensorImage, x, y);
            unsigned char r, g, b;
            al_unmap_rgb(color, &r, &g, &b);
            
            // Determine the tile flags
            TILE_FLAGS flags = 0;
            if (r==239 && g==239 && b==239) {
                flags = 0;
            } else if (r==132 && g==183 && b==244) {
                flags = TILE_SOLID;
            } else if (r==24 && g==119 && b==235) {
                flags = TILE_WATER;
            } else if (r==0 && g==0 && b==0) {
                flags = TILE_EVENT;
            } else if (r==128 && g==128 && b==128) {
                flags = TILE_EVENT;
            } else {
                eprintf("Invalid color in sensor %d at %d,%d\n", id, x, y);
            }
            
            // Load the tile
            Tile(x, y).Flags = flags;
            Tile(x, y).Argument = 0;
            if (flags == TILE_EVENT) {
                Tile(x, y).Argument = argument++;
            }
        }
    }
    al_unlock_bitmap(sensorImage);
}

/**********************************************************//**
 * @brief Warps to another LOCATION immediately, updating all
 * static resources and sensors.
 * @param id: The new LOCATION.
 * @param x: World X-coordinate on the new location.
 * @param y: World Y-coordinate on the new location.
 **************************************************************/
void Warp(LOCATION_ID id, int x, int y) {
    // Set the current map and position
    if (id == OVERWORLD) {
        // Special mapping for OVERWORLD because it's one large map
        // fragmented into multiple sub-locations.
        SetOverworldLocation(x, y);
    } else {
        Player->Location = id;
    }
    CurrentMap = Location(Player->Location)->Map;
    CurrentEvents = Events(CurrentMap);
    
    // Offset to center from tile
    Player->Position.X = x;
    Player->Position.Y = y;
    
    // Load the sensor at the map
    UseSensor(CurrentMap);
}

/**********************************************************//**
 * @brief Gets the tile position the user can currently
 * interact with.
 * @return 
 **************************************************************/
static inline COORDINATE InteractPosition(void) {
    int ix = WorldToTile(Player->Position.X);
    int iy = WorldToTile(Player->Position.Y);
    switch (Player->Direction) {
    case UP:
        iy--;
        break;
    case DOWN:
        iy++;
        break;
    case LEFT:
        ix--;
        break;
    case RIGHT:
        ix++;
        break;
    }
    return (COORDINATE){ix, iy};
}

/**********************************************************//**
 * @brief Interacts with events on the map at the current
 * interaction position.
 **************************************************************/
static void Interact(void) {
    COORDINATE interact = InteractPosition();
    if (!TileInBounds(interact.X, interact.Y)) {
        return;
    }
    
    // Get the tile properties that map to an event.
    const TILE *tile = &Tile(interact.X, interact.Y);
    if (tile->Flags & TILE_EVENT) {
        const EVENT *event = &CurrentEvents[tile->Argument];
        
        // Redirection events (to avoid code duplication)
        while (event->Type == EVENT_REDIRECT) {
            event = &CurrentEvents[event->Union.Redirect];
        }
        
        // Normal events
        const WARP *warp = NULL;
        switch (event->Type) {
        case EVENT_WARP:
            warp = &event->Union.Warp;
            Warp(warp->Location, TileToWorld(warp->Destination.X), TileToWorld(warp->Destination.Y));
            break;
        
        case EVENT_TEXT:
            // TODO
            break;
        case EVENT_SHOP:
            // TODO
            break;
        case EVENT_BOSS:
            // TODO
            break;
        default:
            eprintf("Invalid event type: %d\n", event->Type);
            break;
        }
    }
}

#ifdef DEBUG
/**********************************************************//**
 * @brief Shades a tile-sized square on the map for debugging.
 * @param x: World X-coordinate.
 * @param y: World Y-coordinate.
 * @param color: The color to shade.
 **************************************************************/
static inline void ShadeTile(int x, int y, ALLEGRO_COLOR color) {
    al_draw_filled_rectangle(
        DISPLAY_WIDTH/2-Player->Position.X+x,
        DISPLAY_HEIGHT/2-Player->Position.Y+y,
        DISPLAY_WIDTH/2-Player->Position.X+x+16,
        DISPLAY_HEIGHT/2-Player->Position.Y+y+16,
        color
    );
}
#endif

#ifdef DEBUG
/**********************************************************//**
 * @brief Draws debugging information on the map - shows the
 * sensor, current position, current tile, and interact tile.
 **************************************************************/
void DrawDebugInformation(void) {
    // Sensor visualization
    int centerX = DISPLAY_WIDTH/2-Player->Position.X;
    int centerY = DISPLAY_HEIGHT/2-Player->Position.Y;
    ALLEGRO_BITMAP *sensor = SensorImage(CurrentMap);
    al_draw_tinted_scaled_bitmap(
        sensor,
        al_map_rgba_f(0.5, 0.5, 0.5, 0.5),
        0, 0, al_get_bitmap_width(sensor), al_get_bitmap_height(sensor),
        centerX, centerY, al_get_bitmap_width(sensor)*16, al_get_bitmap_height(sensor)*16,
        0
    );
    
    // Position visualization
    int x = WorldToTile(Player->Position.X);
    int y = WorldToTile(Player->Position.Y);
    ShadeTile(x*16, y*16, al_map_rgb(128, 128, 255));
    
    // Interact Position
    COORDINATE interact = InteractPosition();
    ShadeTile(interact.X*16, interact.Y*16, al_map_rgb(255, 255, 128));
    
    // Current position
    ShadeTile(Player->Position.X-8, Player->Position.Y-8, al_map_rgb(255, 128, 128));
}
#endif

/**********************************************************//**
 * @brief Draws the current map (based on static data).
 **************************************************************/
void DrawMap(void) {
    // Draw the map
    ALLEGRO_BITMAP *mapImage = MapImage(CurrentMap);
    int centerX = DISPLAY_WIDTH/2-Player->Position.X;
    int centerY = DISPLAY_HEIGHT/2-Player->Position.Y;
    DrawAt(centerX, centerY);
    al_draw_bitmap(mapImage, 0, 0, 0);
    
#ifdef FUCK
    // Debug overlays
    DrawAt(0, 0);
    DrawDebugInformation();
#endif
    
    // Draw the player moving
    DrawAt(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    DrawPlayer(PlayerWalkFrame/8%4);
    
    // Main menu overlay
    if (MainMenuOpen) {
        DrawAt(0, 0);
        DrawMainMenu();
    }
}

/**********************************************************//**
 * @brief Checks if the user can walk through the given tile.
 * @param x: Tile X-coordinate.
 * @param y: Tile Y-coordinate.
 * @return True if the user can walk on the tile.
 **************************************************************/
static inline bool Passable(int x, int y) {
    return TileInBounds(x, y) && !Tile(x, y).Flags;
}

/**********************************************************//**
 * @brief Parses the user's keyboard input to update the map
 * position and activate any events.
 **************************************************************/
void UpdateMap(void) {
    // Main menu initialization
    if (!MainMenuOpen && KeyJustUp(KEY_MENU)) {
        MainMenuOpen = true;
        InitializeMainMenu();
        return;
    }
    
    // Main menu processing pre-empts map processing
    if (MainMenuOpen) {
        UpdateMainMenu();
        if (MainMenuClosed()) {
            MainMenuOpen = false;
        }
        return;
    }
    
    // Key press reading
    float dx = (KeyDown(KEY_RIGHT)-KeyDown(KEY_LEFT))*WALK_SPEED*LastFrameTimeElapsed;
    float dy = (KeyDown(KEY_DOWN)-KeyDown(KEY_UP))*WALK_SPEED*LastFrameTimeElapsed;
    
    // Set the player's direction if any motion is
    // REQUESTED (not if it's possible).
    // Prefer UP/DOWN on diagonal motion.
    bool motion = true;
    if (dy > 0) {
        Player->Direction = DOWN;
    } else if (dy < 0) {
        Player->Direction = UP;
    } else if (dx > 0) {
        Player->Direction = RIGHT;
    } else if (dx < 0) {
        Player->Direction = LEFT;
    } else {
        motion = false;
    }
    
    // Update player walk frame
    if (motion) {
        PlayerWalkFrame++;
    } else {
        PlayerWalkFrame = 0;
    }

    // Collision checking
    int x = WorldToTile(Player->Position.X);
    int y = WorldToTile(Player->Position.Y);
    int xf = WorldToTile(Player->Position.X+dx);
    int yf = WorldToTile(Player->Position.Y+dy);
    if (Passable(xf, yf)) {
        // AX // Make sure we can't clip across a corner like this,
        // XB // going from A to B, with X solid.
        if (!Passable(x, yf) && !Passable(xf, y)) {
            dx = 0;
            dy = 0;
        }
    } else if (Passable(x, yf)) {
        dx = 0;
    } else if (Passable(xf, y)) {
        dy = 0;
    } else {
        dx = 0;
        dy = 0;
    }
    
    // Update position
    Player->Position.X += dx;
    Player->Position.Y += dy;
    
    // Event checking
    Interact();
    //UpdateOverworldLocation();
}

/**************************************************************/
