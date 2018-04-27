
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

static void DrawPersonFrame(PERSON_ID id, DIRECTION direction, int frame) {
    ALLEGRO_BITMAP *person = PersonImage(id);
    int x = frame%4*16;
    int y = direction*26;
    // Offset drawing to center feet at x, y
    al_draw_bitmap_region(person, x, y, 16, 26, -8, -24, 0);
}

void DrawPlayer(int frame) {
    DrawPersonFrame(PlayerCostume[Player->Costume], Player->Direction, frame);
}
