/**********************************************************//**
 * @file assets.h
 * @brief Includes and loads all the game assets.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _ASSETS_H_
#define _ASSETS_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "location.h"       // MAP_ID, BACKGROUND_ID
#include "species.h"        // SPECIES_ID, AILMENT_ID
#include "menu.h"           // WINDOW_ID
#include "player.h"         // COSTUME_ID

typedef enum {
    FONT_WINDOW=1,
} FONT_ID;

#define N_FONT (FONT_WINDOW+1)

extern bool LoadAssets(void);
extern void DestroyAssets(void);

extern ALLEGRO_BITMAP *BackgroundImage(BACKGROUND_ID id);
extern ALLEGRO_BITMAP *SpeciesImage(SPECIES_ID id);
extern ALLEGRO_BITMAP *CostumeImage(COSTUME_ID id);
extern ALLEGRO_BITMAP *WindowImage(WINDOW_ID id);
extern ALLEGRO_BITMAP *AilmentImage(AILMENT_ID id);
extern ALLEGRO_BITMAP *TypeImage(TYPE_ID id);
extern ALLEGRO_BITMAP *MapImage(MAP_ID id);

extern ALLEGRO_FONT *Font(FONT_ID id);

#endif