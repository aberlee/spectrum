
#include <allegro5/allegro.h>

#include "assets.h"
#include "display.h"

#include "location.i"
#include "shop.i"
#include "keyboard.h"

static MAP_ID CurrentMap;
static COORDINATE Position;

void Warp(MAP_ID id, int x, int y) {
    CurrentMap = id;
    Position.X = x;
    Position.Y = y;
}

COORDINATE CurrentTilePosition(void) {
    COORDINATE tile = {Position.X/16, Position.Y/16};
    return tile;
}

void DrawMap(void) {
    ALLEGRO_BITMAP *mapImage = MapImage(CurrentMap);
    int centerX = DISPLAY_WIDTH/2-Position.X;
    int centerY = DISPLAY_HEIGHT/2-Position.Y;
    al_draw_bitmap(mapImage, centerX, centerY, 0);
}

#define WALK_SPEED 16

void UpdateMap(void) {
    float horizontal = (KeyDown(KEY_RIGHT)-KeyDown(KEY_LEFT))*WALK_SPEED;
    float vertical = (KeyDown(KEY_DOWN)-KeyDown(KEY_UP))*WALK_SPEED;
    Position.X += horizontal;
    Position.Y += vertical;
}