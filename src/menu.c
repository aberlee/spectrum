#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "assets.h"
#include "species.h"
#include "technique.h"

static inline void DrawText(const char *text, int x, int y) {
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(0, 0, 0),
        x,
        y-3,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

static inline void DrawTitle(const char *text, int x, int y) {
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(163, 163, 163),
        x,
        y-3,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

static inline void DrawTextBox(const char *text, int x, int y, int width) {
    al_draw_multiline_text(
        Font(FONT_WINDOW),
        al_map_rgb(20, 20, 20),
        x,
        y-3,
        width,
        10,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

static inline void DrawSelector(int x, int y, int width, int height) {
    al_draw_filled_rectangle(x, y, x+width, y+height, al_map_rgba(0, 0, 0, 60));
}

void DrawChoice(const OPTIONS *choice) {
    al_draw_bitmap(WindowImage(MENU_CHOICE), 0, 0, 0);
    DrawText(choice->Option[0], 4, 4);
    DrawText(choice->Option[1], 4, 17);
    DrawSelector(2, 2+13*choice->Index, 34, 12);
}

void DrawAlert(const char *text) {
    al_draw_bitmap(WindowImage(ALERT), 0, 0, 0);
    DrawTextBox(text, 4, 17, 120);
}

void DrawWarning(const char *text) {
    al_draw_bitmap(WindowImage(WARNING), 0, 0, 0);
    DrawTextBox(text, 4, 17, 120);
}

void DrawOption(const OPTIONS *options) {
    al_draw_bitmap(WindowImage(MENU_OPTION), 0, 0, 0);
    DrawText(options->Option[0], 4, 4);
    DrawText(options->Option[1], 4, 17);
    DrawText(options->Option[2], 4, 30);
    DrawSelector(2, 2+13*options->Index, 96, 12);
}

void DrawColumn(const OPTIONS *options) {
    al_draw_bitmap(WindowImage(MENU_COLUMN), 0, 0, 0);
    
    // Left column
    DrawText(options->Option[0], 4, 4);
    DrawText(options->Option[2], 4, 17);
    DrawText(options->Option[4], 4, 30);
    DrawText(options->Option[6], 4, 43);
    DrawText(options->Option[8], 4, 56);
    DrawText(options->Option[10], 4, 69);
    
    // Right column
    DrawText(options->Option[1], 101, 4);
    DrawText(options->Option[3], 101, 17);
    DrawText(options->Option[5], 101, 30);
    DrawText(options->Option[7], 101, 43);
    DrawText(options->Option[9], 101, 56);
    DrawText(options->Option[11], 101, 69);
    
    DrawSelector(2, 2+13*options->Index, 138, 12);
}

void DrawBar(float percent, int x, int y) {
    int x0 = x + (int)(percent*81);
    al_draw_filled_rectangle(x, y, x0, y+8, al_color_hsv(120*percent, 0.5f, 0.8f));
}

void DrawSpectraDisplay(const SPECTRA *spectra) {
    al_draw_bitmap(WindowImage(SPECTRA_DISPLAY), 0, 0, 0);
    
    // Information
    const SPECIES *species = SpeciesOfSpectra(spectra);
    char buffer[33];
    
    // Spectra title bar
    DrawTitle(species->Name, 4, 4);
    snprintf(buffer, 32, "Lv.%d", spectra->Level);
    DrawTitle(buffer, 162, 4);
    
    // Icons
    al_draw_bitmap(TypeImage(species->Type[0]), 160, 18, 0);
    if (species->Type[1]) {
        al_draw_bitmap(TypeImage(species->Type[1]), 200, 18, 0);
    }
    if (spectra->Ailment) {
        al_draw_bitmap(AilmentImage(spectra->Ailment), 193, 31, 0);
    }
    
    // Bars
    DrawBar((float)spectra->Health/spectra->MaxHealth, 22, 21);
    DrawBar((float)spectra->Power/spectra->MaxPower, 22, 32);
    
    // Stats
    snprintf(buffer, 32, "%d/%d", spectra->Health, spectra->MaxHealth);
    DrawText(buffer, 22, 21);
    snprintf(buffer, 32, "%d/%d", spectra->Power, spectra->MaxPower);
    DrawText(buffer, 22, 32);
    snprintf(buffer, 32, "%d", spectra->Attack);
    DrawText(buffer, 54, 45);
    snprintf(buffer, 32, "%d", spectra->Defend);
    DrawText(buffer, 54, 58);
    snprintf(buffer, 32, "%d", spectra->Evade);
    DrawText(buffer, 54, 71);
    snprintf(buffer, 32, "%d", spectra->Luck);
    DrawText(buffer, 54, 84);
    
    // Moveset
    int i = 0;
    for (i = 0; i < spectra->MovesetSize; i++) {
        const TECHNIQUE *technique = TechniqueByID(spectra->Moveset[i]);
        DrawText(technique->Name, 7, 101+13*i);
    }
    while (i < MOVESET_SIZE) {
        DrawTitle("None", 7, 101+13*i);
        i++;
    }
    
    // Experience
    DrawText("???", 185, 179);
    snprintf(buffer, 32, "%d", spectra->Experience);
    DrawText(buffer, 185, 192);
    
    // Sprite pane
    ALLEGRO_BITMAP *sprite;
    if (spectra->Species == AMY) {
        sprite = CostumeImage(1);
    } else {
        sprite = SpeciesImage(spectra->Species);
    }
    int width = al_get_bitmap_width(sprite);
    int height = al_get_bitmap_height(sprite);
    int xOffset = (127-width)/2;
    int yOffset = (123-height)/2;
    al_draw_bitmap(sprite, 111+xOffset, 48+yOffset, ALLEGRO_FLIP_HORIZONTAL);
}

void DrawHudUser(const SPECTRA *spectra) {
    al_draw_bitmap(WindowImage(HUD_USER), 0, 0, 0);
    
    const SPECIES *species = SpeciesOfSpectra(spectra);
    char buffer[33];
    
    DrawText(species->Name, 4, 4);
    snprintf(buffer, 32, "%d", spectra->Level);
    DrawText(buffer, 26, 15);
    DrawBar((float)spectra->Health/spectra->MaxHealth, 116, 4);
    DrawBar((float)spectra->Power/spectra->MaxPower, 116, 15);
    
    if (spectra->Ailment) {
        al_draw_bitmap(AilmentImage(spectra->Ailment), 54, 13, 0);
    }
    
    snprintf(buffer, 32, "%d/%d", spectra->Health, spectra->MaxHealth);
    DrawText(buffer, 116, 4);
    snprintf(buffer, 32, "%d/%d", spectra->Power, spectra->MaxPower);
    DrawText(buffer, 116, 15);
}

void DrawHudEnemy(const SPECTRA *spectra) {
    al_draw_bitmap(WindowImage(HUD_ENEMY), 0, 0, 0);
    
    const SPECIES *species = SpeciesOfSpectra(spectra);
    char buffer[33];
    
    DrawText(species->Name, 105, 4);
    snprintf(buffer, 32, "%d", spectra->Level);
    DrawText(buffer, 127, 15);
    DrawBar((float)spectra->Health/spectra->MaxHealth, 19, 4);
    DrawBar((float)spectra->Power/spectra->MaxPower, 19, 15);
    
    if (spectra->Ailment) {
        al_draw_bitmap(AilmentImage(spectra->Ailment), 155, 13, 0);
    }
}
    