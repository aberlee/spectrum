
#include <allegro5/allegro.h>

#include "person.h"             // PERSON_ID, PERSON
#include "assets.h"             // PersonImage
#include "coordinate.h"         // COORDINATE, DIRECTION
#include "player.h"             // COSTUME_ID, Player

static const PERSON_ID PlayerCostume[] = {
    [NORMAL]        = AMY_NORMAL,
    [RED_DRESS]     = AMY_RED_DRESS,
    [BLACK_DRESS]   = AMY_BLACK_DRESS,
    [PAJAMAS]       = AMY_PAJAMAS,
    [SUIT]          = AMY_SUIT,
    [SWIMSUIT]      = AMY_SWIMSUIT,
};

void DrawPersonAt(PERSON_ID id, DIRECTION direction, int frame, int x, int y) {
    ALLEGRO_BITMAP *person = PersonImage(id);
    int px = frame%4*16;
    int py = direction*26;
    // Offset drawing to center feet at x, y
    al_draw_bitmap_region(person, px, py, 16, 26, x-8, y-24, 0);
}
void DrawPerson(PERSON_ID id, DIRECTION direction, int frame) {
    DrawPersonAt(id, direction, frame, 0, 0);
}

void DrawPlayer(int frame) {
    DrawPerson(PlayerCostume[Player->Costume], Player->Direction, frame);
}
