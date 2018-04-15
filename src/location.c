
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "assets.h"
#include "game.h"
#include "debug.h"
#include "item.h"
#include "location.h"
#include "shop.h"
#include "event.h"

#include "location.i"
#include "shop.i"

static MAP_ID CurrentMap = 0;
static LOCATION_ID CurrentLocation = 0;
static const EVENT *CurrentEvents = NULL;
static SENSOR CurrentSensor = {0, 0, NULL};
static COORDINATE Position = {0, 0};
static DIRECTION Direction = DOWN;
static const COORDINATE *CurrentBounds = NULL;

#ifdef DEBUG
static bool DebugMap = true;
#endif

#define Tile(x, y) CurrentSensor.Sensor[y*CurrentSensor.Width+x]

static bool TileInBounds(int x, int y) {
    return (0 <= x && x < CurrentSensor.Width && 0 <= y && y < CurrentSensor.Height);
}

const LOCATION *Location(LOCATION_ID id) {
    return &LOCATION_DATA[id];
}

static inline bool InBounds(const COORDINATE *bounds, int x, int y) {
    return bounds[0].X <= x && x < bounds[1].X && bounds[0].Y <= y && y < bounds[1].Y;
}

static void SetOverworldLocation(int x, int y) {
    for (int i=1; i < N_LOCATION; i++) {
        const LOCATION *location = Location(i);
        if (location->Map == MAP_OVERWORLD && InBounds(location->Bounds, x, y)) {
            CurrentBounds = location->Bounds;
            CurrentLocation = (LOCATION_ID)i;
            return;
        }
    }
    eprintf("Invalid overworld location: (%d, %d)\n", x, y);
}

static inline int TileToWorld(int n) {
    return n*16+8;
}

static inline int WorldToTile(int n) {
    return (n<0)? (n/16)-1: (n/16);
}

static inline void CheckCurrentBounds(void) {
    if (CurrentMap == MAP_OVERWORLD && !InBounds(CurrentBounds, Position.X, Position.Y)) {
        SetOverworldLocation(Position.X, Position.Y);
    }
}

static void UseSensor(MAP_ID id) {
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
    
    // Done
    al_unlock_bitmap(sensorImage);
}

void Warp(LOCATION_ID id, int x, int y) {
    // Set the current map and position
    if (id == OVERWORLD) {
        // Special mapping for OVERWORLD because it's one large map
        // fragmented into multiple sub-locations.
        SetOverworldLocation(x, y);
    } else {
        CurrentLocation = id;
    }
    CurrentMap = Location(CurrentLocation)->Map;
    CurrentEvents = Events(CurrentMap);
    
    // Offset to center from tile
    Position.X = x;
    Position.Y = y;
    
    // Load the sensor at the map
    UseSensor(CurrentMap);
}

void Interact(void) {
    int x = WorldToTile(Position.X);
    int y = WorldToTile(Position.Y);
    
    // Interact Position
    int ix = x;
    int iy = y;
    switch (Direction) {
    case UP:
        iy = y-1;
        break;
    case DOWN:
        iy = y+1;
        break;
    case LEFT:
        ix = x-1;
        break;
    case RIGHT:
        ix = x+1;
        break;
    }
    
    // Activate any events
    if (TileInBounds(ix, iy)) {
        const TILE *tile = &Tile(ix, iy);
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
                Warp(
                    warp->Location,
                    TileToWorld(warp->Destination.X),
                    TileToWorld(warp->Destination.Y)
                );
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
}

#define ShadeTile(x, y, color) {\
    al_draw_filled_rectangle(\
        DISPLAY_WIDTH/2-Position.X+x,\
        DISPLAY_HEIGHT/2-Position.Y+y,\
        DISPLAY_WIDTH/2-Position.X+x+16,\
        DISPLAY_HEIGHT/2-Position.Y+y+16,\
        color\
    );\
}

#ifdef DEBUG
void DrawDebugInformation(void) {
    // Sensor visualization
    int centerX = DISPLAY_WIDTH/2-Position.X;
    int centerY = DISPLAY_HEIGHT/2-Position.Y;
    ALLEGRO_BITMAP *sensor = SensorImage(CurrentMap);
    al_draw_tinted_scaled_bitmap(
        sensor,
        al_map_rgba_f(0.5, 0.5, 0.5, 0.5),
        0, 0, al_get_bitmap_width(sensor), al_get_bitmap_height(sensor),
        centerX, centerY, al_get_bitmap_width(sensor)*16, al_get_bitmap_height(sensor)*16,
        0
    );
    
    // Position visualization
    int x = WorldToTile(Position.X);
    int y = WorldToTile(Position.Y);
    ShadeTile(x*16, y*16, al_map_rgb(128, 128, 255));
    
    // Interact Position
    int ix = x;
    int iy = y;
    switch (Direction) {
    case UP:
        iy = y-1;
        break;
    case DOWN:
        iy = y+1;
        break;
    case LEFT:
        ix = x-1;
        break;
    case RIGHT:
        ix = x+1;
        break;
    }
    ShadeTile(ix*16, iy*16, al_map_rgb(255, 255, 128));
    
    ShadeTile(Position.X-8, Position.Y-8, al_map_rgb(255, 128, 128));
}
#endif

void DrawMap(void) {
    ALLEGRO_BITMAP *mapImage = MapImage(CurrentMap);
    int centerX = DISPLAY_WIDTH/2-Position.X;
    int centerY = DISPLAY_HEIGHT/2-Position.Y;
    al_draw_bitmap(mapImage, centerX, centerY, 0);
    DrawDebugInformation();
}

#define WALK_SPEED 120
#define Passable(x, y) (TileInBounds(x, y) && !Tile(x, y).Flags)

void UpdateMap(void) {
    // Key press reading
    float dx = (KeyDown(KEY_RIGHT)-KeyDown(KEY_LEFT))*WALK_SPEED*LastFrameTimeElapsed;
    float dy = (KeyDown(KEY_DOWN)-KeyDown(KEY_UP))*WALK_SPEED*LastFrameTimeElapsed;
    
    // Set the player's direction if any motion is
    // REQUESTED (not if it's possible).
    // Prefer UP/DOWN on diagonal motion.
    if (dy > 0) {
        Direction = DOWN;
    } else if (dy < 0) {
        Direction = UP;
    } else if (dx > 0) {
        Direction = RIGHT;
    } else if (dx < 0) {
        Direction = LEFT;
    }

    // Collision checking
    int x = WorldToTile(Position.X);
    int y = WorldToTile(Position.Y);
    int xf = WorldToTile(Position.X+dx);
    int yf = WorldToTile(Position.Y+dy);
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
    Position.X += dx;
    Position.Y += dy;
    
    // Event checking
    Interact();
}
