/**********************************************************//**
 * @file person.c
 * @brief Implements graphics for non-player characters.
 * @author Rena Shinomiya
 * @date May 02, 2018
 **************************************************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "person.h"             // PERSON_ID, PERSON
#include "assets.h"             // PersonImage
#include "coordinate.h"         // COORDINATE, DIRECTION
#include "player.h"             // COSTUME_ID, Player

/**********************************************************//**
 * @var PlayerCostume
 * @brief Maps COSTUME ID's to PERSON_ID values so the player
 * choice of costume is reflected on the overworld.
 **************************************************************/
static const PERSON_ID PlayerCostume[] = {
    [NORMAL]        = AMY_NORMAL,
    [RED_DRESS]     = AMY_RED_DRESS,
    [BLACK_DRESS]   = AMY_BLACK_DRESS,
    [PAJAMAS]       = AMY_PAJAMAS,
    [SUIT]          = AMY_SUIT,
    [SWIMSUIT]      = AMY_SWIMSUIT,
};

/**********************************************************//**
 * @brief Draws a person's frame from a person sprite sheet.
 * @param id: Person's costume sprite sheet ID. The person's
 * feet are centered at the draw location.
 * @param direction: Direction the person faces.
 * @param frame: Frame of animation, if applicable.
 **************************************************************/
void DrawPerson(PERSON_ID id, DIRECTION direction, int frame) {
    ALLEGRO_BITMAP *person = PersonImage(id);
    int sheetX = frame%4*16;
    int sheetY = direction*26;
    
    // Draw small drop shadow
    al_draw_filled_ellipse(0, 0, 6, 3, al_map_rgba_f(0, 0, 0, 0.2));
    
    // Offset drawing to center feet
    al_draw_bitmap_region(person, sheetX, sheetY, 16, 26, -8, -24, 0);
}

/**********************************************************//**
 * @brief Draws the player on the screen based on the current
 * costume. The player's feet are centered at the draw location.
 * @param frame: Frame of motion, if applicable.
 **************************************************************/
void DrawPlayer(int frame) {
    DrawPerson(PlayerCostume[Player->Costume], Player->Direction, frame);
}

/**************************************************************/
