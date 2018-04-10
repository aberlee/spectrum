
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "assets.h"
#include "game.h"
#include "debug.h"

#include "location.i"
#include "shop.i"

static MAP_ID CurrentMap = 0;
static SENSOR CurrentSensor = {0, 0, NULL};
static COORDINATE Position = {0, 0};

#define Tile(x, y) CurrentSensor.Sensor[y*CurrentSensor.Width+x]
#define Coordinate(x, y) Tile(x/16, y/16)

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
            bool argumentOn = false;
            if (r==239 && g==239 && b==239) {
                flags = 0;
            } else if (r==132 && g==183 && b==244) {
                flags = TILE_SOLID;
            } else if (r==24 && g==119 && b==235) {
                flags = TILE_WATER;
            } else if (r==0 && g==0 && b==0) {
                flags = TILE_WARP;
                argumentOn = true;
            } else if (r==128 && g==128 && b==128) {
                flags = TILE_SIGN;
                argumentOn = true;
            } else {
                eprintf("Invalid color in sensor %d at %d,%d\n", id, x, y);
            }
            
            // Load the tile
            Tile(x, y).Flags = flags;
            Tile(x, y).Argument = 0;
            if (argumentOn) {
                Tile(x, y).Argument = argument++;
            }
        }
    }
    
    // Done
    al_unlock_bitmap(sensorImage);
}

void Warp(MAP_ID id, int x, int y) {
    // Set the current map and position
    CurrentMap = id;
    Position.X = x;
    Position.Y = y;
    
    // Load the sensor at the map
    UseSensor(id);
}

void DrawMap(void) {
    ALLEGRO_BITMAP *mapImage = MapImage(CurrentMap);
    int centerX = DISPLAY_WIDTH/2-Position.X;
    int centerY = DISPLAY_HEIGHT/2-Position.Y;
    al_draw_bitmap(mapImage, centerX, centerY, 0);
    al_draw_filled_circle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, 16, al_map_rgb(255, 128, 128));
}

#define WALK_SPEED 80

#define FlagSet(flags, what) (flags&what)
#define Passable(x, y) (!Tile(x, y).Flags)

void UpdateMap(void) {
    float dx = (KeyDown(KEY_RIGHT)-KeyDown(KEY_LEFT))*WALK_SPEED*LastFrameTimeElapsed;
    float dy = (KeyDown(KEY_DOWN)-KeyDown(KEY_UP))*WALK_SPEED*LastFrameTimeElapsed;

    // Collision checking
    int x = Position.X/16;
    int y = Position.Y/16;
    int xf = (Position.X+dx)/16;
    int yf = (Position.Y+dy)/16;
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
    
    Position.X += dx;
    Position.Y += dy;
}