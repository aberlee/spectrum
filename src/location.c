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

#include "random.h"             // uniform, randint
#include "location.h"           // MAP_ID, LOCATION
#include "game.h"               // KEY
#include "assets.h"             // MapImage, SensorImage
#include "event.h"              // EVENT, Events
#include "player.h"             // Player
#include "output.h"             // Output
#include "main_menu.h"          // MainMenu
#include "shop.h"               // SHOP
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

/// @brief The current frame of motion the player's walk cycle
/// is on.
static int PlayerWalkFrame = 0;

/**************************************************************/
/// @brief The al_get_time() at which the location popup was
/// last activated.
static double LocationPopupTime = 0.0;

/// @brief Y-position of the location popup.
static float LocationPopupY = -20;

/**************************************************************/
/// @brief Buffered screenshot of the image to fade out
/// while warping to another location.
static ALLEGRO_BITMAP *WarpPreimage = NULL;

/// @brief al_get_time() at which the player was last warped
/// to another location.
static double TimeOfLastWarp = -2;

/**********************************************************//**
 * @struct PERSON_TEMP
 * @brief Stores temporary data for each PERSON event on the
 * current map.
 **************************************************************/
typedef struct {
    DIRECTION Direction;        ///< Direction the person faces.
} PERSON_TEMP;

/**********************************************************//**
 * @struct RUNTIME_EVENT_TEMP_DATA
 * @brief Stores event data that's only needed at runtime,
 * that isn't saved anywhere.
 **************************************************************/
typedef struct {
    int EventID;
    int EventX;
    int EventY;
    union {
        PERSON_TEMP Person;
    } Union;
} RUNTIME_EVENT_DATA;

/// @brief Number of events that can be buffered in the
/// RuntimeMapTiles cache.
#define N_RUNTIME_EVENT 256

/// @brief Cache for all events that need to be rendered on
/// the map at every frame.
static RUNTIME_EVENT_DATA RuntimeEventData[N_RUNTIME_EVENT+1];

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
 * @brief Convert from Tile to World coordinates, and align
 * to the upper left corner of the tile.
 * @param n: Tile coordinate.
 * @return World coordinate, on the upper left corner.
 **************************************************************/
static inline int TileToWorld(int n) {
    return n*16;
}

/**********************************************************//**
 * @brief Convert from Tile to World coordinates, and center
 * the world coordinates within the tile.
 * @param n: Tile coordinate.
 * @return World coordinate, centered on the tile.
 **************************************************************/
static inline int TileToWorldCenter(int n) {
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
 * @brief Get the world coordinate of the center of the map.
 * This aligns to the player's position on the screen.
 * @return Map center X-coordinate.
 **************************************************************/
static inline int MapCenterX(void) {
    return DISPLAY_WIDTH/2-Player->Position.X;
}

/**********************************************************//**
 * @brief Get the world coordinate of the center of the map.
 * This aligns to the player's position on the screen.
 * @return Map center Y-coordinate.
 **************************************************************/
static inline int MapCenterY(void) {
    return DISPLAY_HEIGHT/2-Player->Position.Y;
}

/**********************************************************//**
 * @brief Align drawing to the map. Anything drawn here will
 * appear to scroll with the map when it moves. This aligns
 * to the upper left corner of the map (tile 0, 0).
 **************************************************************/
static inline void DrawAtMapCenter() {
    DrawAt(MapCenterX(), MapCenterY());
}

/**********************************************************//**
 * @brief Align drawing to a tile within the map. This
 * applies an offset to DrawMapAtCenter, and aligns to the
 * tile's upper left corner.
 * @param x: Tile X-coordinate.
 * @param y: Tile Y-coordinate.
 **************************************************************/
static inline void DrawAtTile(int x, int y) {
    DrawAt(MapCenterX()+TileToWorld(x), MapCenterY()+TileToWorld(y));
}

/**********************************************************//**
 * @brief Align drawing to a tile within the map. This
 * applies an offset to DrawMapAtCenter, and aligns to the
 * center of the tile.
 * @param x: Tile X-coordinate.
 * @param y: Tile Y-coordinate.
 **************************************************************/
static inline void DrawAtTileCenter(int x, int y) {
    DrawAt(MapCenterX()+TileToWorldCenter(x), MapCenterY()+TileToWorldCenter(y));
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
    if (CurrentMap == MAP_OVERWORLD) {
        if(!CurrentBounds || !WorldInBounds(CurrentBounds, Player->Position.X, Player->Position.Y)) {
            // Compare location names to see if we need to trigger
            // the location popup.
            const char *oldLocation = Location(Player->Location)->Name;
            SetOverworldLocation(Player->Position.X, Player->Position.Y);
            if (strcmp(oldLocation, Location(Player->Location)->Name)) {
                LocationPopupTime = al_get_time();
            }
        }
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
    int eventID = 1;
    int runtimeID = 1;
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
                // Warp
                flags = TILE_EVENT;
            } else if (r==128 && g==128 && b==128) {
                // Sign
                flags = TILE_EVENT;
            } else if (r==255 && g==135 && b==139) {
                // Gift
                flags = TILE_EVENT;
            } else if (r==0 && g==255 && b==0) {
                // Person
                flags = TILE_EVENT;
            } else {
                eprintf("Invalid color in sensor %d at %d,%d\n", id, x, y);
            }
            
            // Load and cache tile events
            Tile(x, y).Flags = flags;
            Tile(x, y).EventID = 0;
            Tile(x, y).RuntimeID = 0;
            if (flags == TILE_EVENT) {
                Tile(x, y).EventID = eventID;
                const EVENT *event = &CurrentEvents[eventID];
                RUNTIME_EVENT_DATA *data = &RuntimeEventData[runtimeID];
                if (runtimeID >= N_RUNTIME_EVENT) {
                    eprintf("Runtime event data overflow.\n");
                    return;
                }
                
                // Store generic temp data
                switch (event->Type) {
                case EVENT_PRESENT:
                case EVENT_PERSON:
                    data->EventID = eventID;
                    data->EventX = x;
                    data->EventY = y;
                    Tile(x, y).RuntimeID = runtimeID++;
                    break;
                    
                default:
                    break;
                }
                
                // Store specialized temp data
                switch (event->Type) {
                case EVENT_PERSON:
                    data->Union.Person.Direction = event->Union.Person.Direction;
                    break;
                
                default:
                    break;
                }
                
                // Advance
                eventID++;
            }
        }
    }
    // Null terminator
    RuntimeEventData[runtimeID].EventID = 0;
    al_unlock_bitmap(sensorImage);
}

/**********************************************************//**
 * @brief Sets up location and mapping data for a location
 * the player has already been transported to. This is
 * typically used to start the game, or load a save file.
 **************************************************************/
void InitializeLocation(void) {
    CurrentMap = Location(Player->Location)->Map;
    CurrentEvents = Events(CurrentMap);
    LocationPopupTime = al_get_time();
    UseSensor(CurrentMap);
    UpdateOverworldLocation();
}

/**********************************************************//**
 * @brief Warps to another LOCATION immediately, updating all
 * static resources and sensors.
 * @param id: The new LOCATION.
 * @param x: Tile X-coordinate on the new location.
 * @param y: Tile Y-coordinate on the new location.
 **************************************************************/
void Warp(LOCATION_ID id, int x, int y, DIRECTION direction) {
    // Old location name
    const char *oldLocation = NULL;
    if (Player->Location) {
        oldLocation = Location(Player->Location)->Name;
    }
    
    // Set the current map and position
    if (id == OVERWORLD) {
        // Special mapping for OVERWORLD because it's one large map
        // fragmented into multiple sub-locations.
        SetOverworldLocation(TileToWorldCenter(x), TileToWorldCenter(y));
    } else {
        Player->Location = id;
    }
    CurrentMap = Location(Player->Location)->Map;
    CurrentEvents = Events(CurrentMap);
    
    // Location entry popup - display if the name is new.
    if (!oldLocation || strcmp(oldLocation, Location(Player->Location)->Name)) {
        LocationPopupY = -20.0;
        LocationPopupTime = al_get_time();
    }
    
    // Offset to center from tile
    Player->Position.X = TileToWorldCenter(x);
    Player->Position.Y = TileToWorldCenter(y);
    Player->Direction = direction;
    
    // Load the sensor at the map
    UseSensor(CurrentMap);
    
    // Set up warp - ensure we don't leak image resources
    if (WarpPreimage) {
        al_destroy_bitmap(WarpPreimage);
    }
    WarpPreimage = Screenshot();
    TimeOfLastWarp = al_get_time();
}

void WarpToLastHospital(void) {
    // Coordinates to warp to within each hospital.
    static const COORDINATE DeathWarp[] = {
        [SAPLING_HOSPITAL]      = {10,  3},
        [ROYAL_HOSPITAL]        = {10,  3},
        [SOLAR_HOSPITAL]        = {10,  3},
        [ANDORA_HOSPITAL]       = {10,  3},
        [GRANITE_HOSPITAL]      = { 1,  3},
    };
    
    LOCATION_ID hospital = Player->LastHospital;
    const COORDINATE *coordinate = &DeathWarp[hospital];
    Warp(hospital, coordinate->X, coordinate->Y, DOWN);
}

/**********************************************************//**
 * @brief Check if the game is animating a warp.
 * @return True if a warp is ongiong.
 **************************************************************/
static inline bool WarpInProgress(void) {
    return al_get_time()-TimeOfLastWarp < 1;
}

#define INTERACT_REACH 8

/**********************************************************//**
 * @brief Gets the tile position the user can currently
 * interact with.
 * @return COORDINATE at which the player can interact.
 **************************************************************/
static inline COORDINATE InteractPosition(void) {
    int x = Player->Position.X;
    int y = Player->Position.Y;
    switch (Player->Direction) {
    case UP:
        y -= INTERACT_REACH;
        break;
    case DOWN:
        y += INTERACT_REACH;
        break;
    case LEFT:
        x -= INTERACT_REACH;
        break;
    case RIGHT:
        x += INTERACT_REACH;
        break;
    }
    return (COORDINATE){WorldToTile(x), WorldToTile(y)};
}

static inline bool FishingAvailable(void) {
    COORDINATE interact = InteractPosition();
    bool isWater = Tile(interact.X, interact.Y).Flags & TILE_WATER;
    return isWater && HasItem(FISHING_ROD);
}

typedef enum {
    FISHING_DONE,
    FISHING_CAST,
    FISHING_WAIT,
    FISHING_REEL,
} FISHING_PHASE;

static FISHING_PHASE FishingPhase = FISHING_DONE;

static void UpdateFishing(void) {
    static int Persist = 1;
    switch (FishingPhase) {
    case FISHING_CAST:
        OutputSplitByCR("...\r......\r..........\r");
        FishingPhase = FISHING_WAIT;
        break;
    
    case FISHING_WAIT:
        UpdateOutput();
        if (OutputDone()) {
            float found = uniform(0.0, 1.0/Persist) < 0.1;
            if (found) {
                Persist = 1;
                Output("Something's on the line!");
                FishingPhase = FISHING_REEL;
            } else {
                Persist++;
            }
        }
        break;
    
    case FISHING_REEL:
        UpdateOutput();
        if (OutputDone()) {
            InitializeRandomEncounter(1, ENCOUNTER_FISHING);
            SetMode(MODE_BATTLE);
            FishingPhase = FISHING_DONE;
        }
        break;
    
    case FISHING_DONE:
    default:
        break;
    }
}


/**********************************************************//**
 * @brief Applies any redirects to the event ID.
 * @param argument: Event ID to redirect.
 * @return Pointer to a non-redirect event.
 **************************************************************/
static inline const EVENT *GetEvent(int argument) {
    const EVENT *event = &CurrentEvents[argument];
    while (event->Type == EVENT_REDIRECT) {
        event = &CurrentEvents[event->Union.Redirect];
    }
    return event;
}

/**********************************************************//**
 * @brief Interacts with events on the map at the current
 * interaction position. These occur when the user presses
 * a button to interact.
 **************************************************************/
static void InteractUser(void) {
    COORDINATE interact = InteractPosition();
    if (!TileInBounds(interact.X, interact.Y)) {
        return;
    }
    
    // Get the tile properties that map to an event.
    const TILE *tile = &Tile(interact.X, interact.Y);
    if (tile->Flags & TILE_EVENT) {
        const EVENT *event = GetEvent(tile->EventID);
        switch (event->Type) {
        case EVENT_TEXT:
            OutputSplitByCR(event->Union.Text);
            break;

        case EVENT_BOSS:
            // TODO
            break;

        case EVENT_WARP:
            // Warps processed by InteractAutomatic
            break;

        case EVENT_PRESENT:
            // Present
            if (Switch(event->Union.Present.Switch)) {
                Output("It's empty...");
            } else if (GetItem(event->Union.Present.Item)) {
                OutputF("Amy found %s!", ItemByID(event->Union.Present.Item)->Name);
                Switch(event->Union.Present.Switch) = true;
            } else {
                Output("You can't carry anything else!");
            }
            break;

        case EVENT_PERSON:
            // Make person face the player
            RuntimeEventData[tile->RuntimeID].Union.Person.Direction = OppositeDirection(Player->Direction);
            switch (event->Union.Person.Type) {
            case PERSON_SPEECH:
                OutputSplitByCR(event->Union.Person.Speech);
                break;
            case PERSON_HOSPITAL:
                OutputSplitByCR(event->Union.Person.Speech);
                Player->LastHospital = Player->Location;
                RecoverParty();
                break;
            case PERSON_SHOP:
                OutputSplitByCR(event->Union.Person.Speech);
                InitializeShop(event->Union.Person.Shop);
                break;
            default:
                break;
            }
            break;

        default:
            eprintf("Invalid event type: %d\n", event->Type);
            break;
        }
    } else if (tile->Flags & TILE_WATER) {
        if (FishingAvailable()) {
            FishingPhase = FISHING_CAST;
        } else {
            Output("The water is crystal clear...");
        }
    }
}

/**********************************************************//**
 * @brief Interacts with events on the map at the current
 * interaction position. These occur automatically, without
 * the user needing to do anything.
 * @return True if interaction is happening.
 **************************************************************/
static bool InteractAutomatic(void) {
    COORDINATE interact = InteractPosition();
    if (!TileInBounds(interact.X, interact.Y)) {
        return false;
    }
    
    // Get the tile properties that map to an event.
    const TILE *tile = &Tile(interact.X, interact.Y);
    if (tile->Flags & TILE_EVENT) {
        const EVENT *event = GetEvent(tile->EventID);
        if (event->Type == EVENT_WARP) {
            const WARP *warp = &event->Union.Warp;
            Warp(warp->Location, warp->Destination.X, warp->Destination.Y, warp->Direction);
            return true;
        }
    }
    return false;
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

/**********************************************************//**
 * @brief Draws debugging information on the map - shows the
 * sensor, current position, current tile, and interact tile.
 **************************************************************/
static void DrawDebugInformation(void) {
    DrawAt(0, 0);
    
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
 * @brief Draws the location popup on the screen, if it's
 * been less than 2 seconds since location names changed.
 **************************************************************/
void DrawLocationPopup(void) {
    double popup = al_get_time()-LocationPopupTime;
    if (popup < 2 && !WarpInProgress()) {
        LocationPopupY += LastFrameTime()*80;
        if (LocationPopupY > 4) {
            LocationPopupY = 4;
        }
    } else {
        LocationPopupY -= LastFrameTime()*80;
        if (LocationPopupY < -20) {
            LocationPopupY = -20;
        }
    }
    if (LocationPopupY > -20) {
        DrawAt(4, LocationPopupY);
        DrawPopupBar(Location(Player->Location)->Name);
    }
}

/**********************************************************//**
 * @brief Fade out the entire screen to the given opacity.
 * @param opacity: From 0 to 1, how opaque the screen is.
 **************************************************************/
static void DrawScreenFade(float opacity) {
    DrawAt(0, 0);
    al_draw_filled_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgba_f(0, 0, 0, opacity));
}


/**********************************************************//**
 * @enum EVENT_DRAW_RANGE
 * @brief Used by DrawPeople to determine which people to draw.
 **************************************************************/
typedef enum {
    ABOVE           = 0x0001,
    BELOW           = 0x0002,
} EVENT_DRAW_RANGE;

/**********************************************************//**
 * @brief Draws any events that need graphics.
 **************************************************************/
static void DrawRuntimeEvents(EVENT_DRAW_RANGE range) {
    int playerY = WorldToTile(Player->Position.Y);
    for (int i=1; i<N_RUNTIME_EVENT && RuntimeEventData[i].EventID; i++) {
        // Get position
        const RUNTIME_EVENT_DATA *data = &RuntimeEventData[i];
        int eventID = data->EventID;
        int eventX = data->EventX;
        int eventY = data->EventY;
        
        // Check range
        bool above = playerY>=eventY && range&ABOVE;
        bool below = playerY<eventY && range&BELOW;
        if (!above && !below) {
            continue;
        }
        
        // Draw the event
        const EVENT *event = &CurrentEvents[eventID];
        switch (event->Type) {
        case EVENT_PRESENT: {
            DrawAtTile(eventX, eventY);
            bool open = Switch(event->Union.Present.Switch);
            al_draw_bitmap(MiscImage(open? GIFT_OPEN: GIFT_CLOSED), 0, 0, 0);
            break;
        }
        case EVENT_PERSON:
            DrawAtTileCenter(eventX, eventY);
            DrawPerson(event->Union.Person.Person, data->Union.Person.Direction);
            break;
        
        default:
            break;
        }
    }
}

/**********************************************************//**
 * @brief Draws the current map (based on static data).
 **************************************************************/
void DrawMap(void) {
    // Warp pre-processing
    double warpTime = al_get_time()-TimeOfLastWarp;
    if (warpTime < 0.5) {
        DrawAt(0, 0);
        al_draw_bitmap(WarpPreimage, 0, 0, 0);
        DrawScreenFade(2*warpTime);
        return;
    }
    
    // Draw items not influenced by the player.
    ALLEGRO_BITMAP *mapImage = MapImage(CurrentMap);
    DrawAtMapCenter();
    al_draw_bitmap(mapImage, 0, 0, 0);
    DrawRuntimeEvents(ABOVE);
    
    // Draw the player
    DrawAt(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    DrawPlayer(PlayerWalkFrame/8%4);
    
    // Draw things that can overlap the player.
    DrawAtMapCenter();
    DrawRuntimeEvents(BELOW);
    
    // Location popup
    if (!MainMenuOpen) {
        DrawLocationPopup();
    }
    
    // Warp post-processing
    if (warpTime < 1) {
        DrawScreenFade(2-warpTime*2);
        return;
    }
    
    // Output
    if (!OutputDone()) {
        DrawAt(0, 0);
        DrawOutput();
    }

    // Main menu overlay
    if (MainMenuOpen) {
        DrawAt(0, 0);
        DrawMainMenu();
    }
    
    // Shop menu overlay
    if (!ShopDone()) {
        DrawShop();
    }
}

/**********************************************************//**
 * @brief Checks if the user can walk through the given tile.
 * @param x: Tile X-coordinate.
 * @param y: Tile Y-coordinate.
 * @return True if the user can walk on the tile.
 **************************************************************/
static inline bool TilePassable(int x, int y) {
    return TileInBounds(x, y) && !Tile(x, y).Flags;
}

static inline bool WorldPassable(int x, int y) {
    return TilePassable(WorldToTile(x), WorldToTile(y));
}

#define COLLISION_PADDING 6

static inline bool WorldPassableWithPadding(int x, int y) {
    return WorldPassable(x-COLLISION_PADDING, y-COLLISION_PADDING) &&\
    WorldPassable(x-COLLISION_PADDING, y+COLLISION_PADDING) &&\
    WorldPassable(x+COLLISION_PADDING, y-COLLISION_PADDING) &&\
    WorldPassable(x+COLLISION_PADDING, y+COLLISION_PADDING);
}

static bool RandomEncounter(void) {
    const LOCATION *location = Location(Player->Location);
    if (!location->Encounters || !location->EncounterRate) {
        return false;
    } else {
        // Get the encounter rate
        float rate = 0.0;
        switch (location->EncounterRate) {
        case RARE:
            rate = 0.05;
            break;
        case UNCOMMON:
            rate = 0.10;
            break;
        case COMMON:
            rate = 0.20;
            break;
        default:
            rate = 0.0;
            break;
        }
        
        // Randomly generate encounter at "rate" per second
        // of motion on the map.
        return uniform(0.0,1.0) < rate*LastFrameTime();
    }
}

/**********************************************************//**
 * @brief Parses the user's keyboard input to update the map
 * position and activate any events.
 **************************************************************/
void UpdateMap(void) {
    if (FishingPhase != FISHING_DONE) {
        // Fishing mode ongoing
        UpdateFishing();

    } else if (!OutputDone()) {
        // Output processing pre-empts all
        UpdateOutput();

    } else if (WarpInProgress()) {
        // Nothing to update while waiting for the warp
        (void)0;
        
    } else if (!ShopDone()) {
        // Shop mode ongoing
        UpdateShop();

    } else if (!MainMenuOpen && KeyJustUp(KEY_MENU)) {
        // User opened the main menu
        MainMenuOpen = true;
        LocationPopupY = -20;
        InitializeMainMenu();

    } else if (MainMenuOpen) {
        // Main menu processing is ongoing
        UpdateMainMenu();
        if (MainMenuClosed()) {
            MainMenuOpen = false;
        }

    } else if (KeyJustUp(KEY_CONFIRM)) {
        // User interacting with an overworld object
        InteractUser();

    } else {
        // Normal map processing
        float dx = (KeyDown(KEY_RIGHT)-KeyDown(KEY_LEFT))*WALK_SPEED*LastFrameTime();
        float dy = (KeyDown(KEY_DOWN)-KeyDown(KEY_UP))*WALK_SPEED*LastFrameTime();
        
        // Set the player's direction if any motion is
        // REQUESTED (not if it's possible).
        // Prefer UP/DOWN on diagonal motion.
        if (dy > 0) {
            Player->Direction = DOWN;
        } else if (dy < 0) {
            Player->Direction = UP;
        } else if (dx > 0) {
            Player->Direction = RIGHT;
        } else if (dx < 0) {
            Player->Direction = LEFT;
        }
        
        // Normalize diagonal motion
        // Approximately sqrt(2)/2
        if (dx && dy) {
            dx *= 0.7;
            dy *= 0.7;
        }

        // Collision checking
        int x = Player->Position.X;
        int y = Player->Position.Y;
        int xf = Player->Position.X+dx;
        int yf = Player->Position.Y+dy;
        int dxv = dx? (dx>0)? 1: -1: 0;
        int dyv = dy? (dy>0)? 1: -1: 0;
        while (xf!=x && !WorldPassableWithPadding(xf, y)) {
            xf -= dxv;
        }
        while (yf!=y && !WorldPassableWithPadding(x, yf)) {
            yf -= dyv;
        }
        while (xf!=x && yf!=y && !WorldPassableWithPadding(xf, yf)) {
            if (xf!=x) {
                xf -= dxv;
            }
            if (yf!=y) {
                yf -= dyv;
            }
        }
        assert(WorldPassableWithPadding(xf, yf));
        
        // Update walk frame
        if (x!=xf || y != yf) {
            PlayerWalkFrame++;
            Player->Position.X = xf;
            Player->Position.Y = yf;
        
            // Maybe enter battle if no interaction is needed,
            // and a random encounter is triggered.
            if (!InteractAutomatic() && RandomEncounter()) {
                InitializeRandomEncounter(randint(1, 3), ENCOUNTER_OVERWORLD);
                SetMode(MODE_BATTLE);
            } else {
                UpdateOverworldLocation();
            }
        } else {
            PlayerWalkFrame = 0;
        }
    }
}

bool UseMapItem(ITEM_ID id) {
    switch (id) {
    case FISHING_ROD:
        if (FishingAvailable()) {
            FishingPhase = FISHING_CAST;
            return true;
        }
        return false;
    
    default:
        eprintf("Incorrectly using %d as a map item.\n", id);
        return false;
    }
}

/**************************************************************/
