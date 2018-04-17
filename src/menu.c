#include <stdio.h>
#include "math.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "game.h"
#include "assets.h"
#include "species.h"
#include "technique.h"
#include "output.h" // GetOutput

static inline void DrawText(const char *text, int x, int y) {
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(0, 0, 0),
        x,
        y-3,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

#define DrawTextF(x, y, format, ...) {\
    char buf[256];\
    snprintf(buf, 255, format, __VA_ARGS__);\
    DrawText(buf, x, y);\
}

static inline void DrawNumber(int x, int y, int number) {
    char buf[33];
    snprintf(buf, 32, "%d", number);
    al_draw_text(
        Font(FONT_WINDOW),
        number? al_map_rgb(0, 0, 0): al_map_rgb(128, 128, 128),
        x,
        y-3,
        ALLEGRO_ALIGN_RIGHT|ALLEGRO_ALIGN_INTEGER,
        buf);
}

static inline void DrawTitle(const char *text, int x, int y) {
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(226, 226, 226),
        x,
        y-3,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

#define DrawTitleF(x, y, format, ...) {\
    char buf[256];\
    snprintf(buf, 255, format, __VA_ARGS__);\
    DrawTitle(buf, x, y);\
}

static inline void DrawTextBox(const char *text, int x, int y, int width) {
    al_draw_multiline_text(
        Font(FONT_WINDOW),
        al_map_rgb(20, 20, 20),
        x,
        y-3,
        width,
        13,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

#define DrawTextBoxF(x, y, width, format, ...) {\
    char buf[256];\
    snprintf(buf, 255, format, __VA_ARGS__);\
    DrawTextBox(buf, x, y, width);\
}

static inline void DrawSelector(int x, int y, int width, int height) {
    al_draw_filled_rectangle(x, y, x+width, y+height, al_map_rgba(0, 0, 0, 60));
}

static inline void DrawBar(float percent, int x, int y) {
    int x0 = x + (int)(percent*81);
    al_draw_filled_rectangle(x, y, x0, y+8, al_color_hsv(120*percent, 0.5f, 0.8f));
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

void DrawSpectraDisplay(const SPECTRA *spectra) {
    al_draw_bitmap(WindowImage(SPECTRA_DISPLAY), 0, 0, 0);
    const SPECIES *species = SpeciesOfSpectra(spectra);
    
    // Spectra title bar
    DrawTitle(species->Name, 4, 4);
    DrawTitleF(198, 4, "Lv.%d", spectra->Level);
    
    // Icons
    al_draw_bitmap(TypeImage(species->Type[0]), 4, 15, 0);
    if (species->Type[1]) {
        al_draw_bitmap(TypeImage(species->Type[1]), 44, 15, 0);
    }
    if (spectra->Ailment) {
        al_draw_bitmap(AilmentImage(spectra->Ailment), 109, 19, 0);
    }
    
    // Bars
    DrawBar((float)spectra->Health/spectra->MaxHealth, 19, 30);
    DrawBar((float)spectra->Power/spectra->MaxPower, 19, 41);
    
    // Stats
    DrawTextF(20, 30, "%d/%d", spectra->Health, spectra->MaxHealth);
    DrawTextF(20, 41, "%d/%d", spectra->Power, spectra->MaxPower);
    DrawNumber(102, 54, spectra->Attack);
    DrawNumber(102, 67, spectra->Defend);
    DrawNumber(102, 80, spectra->Evade);
    DrawNumber(102, 93, spectra->Luck);
    
    // Moveset
    int i = 0;
    for (i = 0; i < spectra->MovesetSize; i++) {
        const TECHNIQUE *technique = TechniqueByID(spectra->Moveset[i]);
        DrawText(technique->Name, 4, 150+13*i);
        al_draw_bitmap(TypeImage(technique->Type), 103, 148+13*i, 0);
        DrawNumber(190, 150+13*i, technique->Power);
        DrawNumber(240, 150+13*i, technique->Cost);
        
    }
    
    // Experience
    DrawNumber(102, 109, 0);
    DrawNumber(102, 122, spectra->Experience);
    
    // Sprite pane
    ALLEGRO_BITMAP *sprite;
    if (spectra->Species == AMY) {
        sprite = CostumeImage(1);
    } else {
        sprite = SpeciesImage(spectra->Species);
    }
    int width = al_get_bitmap_width(sprite);
    int height = al_get_bitmap_height(sprite);
    int xOffset = (125-width)/2;
    int yOffset = (123-height)/2;
    al_draw_bitmap(sprite, 109+xOffset, 19+yOffset, ALLEGRO_FLIP_HORIZONTAL);
}

void DrawHudUser(const SPECTRA *spectra) {
    al_draw_bitmap(WindowImage(HUD_USER), 0, 0, 0);
    const SPECIES *species = SpeciesOfSpectra(spectra);
    
    DrawText(species->Name, 4, 4);
    DrawTextF(26, 15, "%d", spectra->Level);
    DrawBar((float)spectra->Health/spectra->MaxHealth, 116, 4);
    DrawBar((float)spectra->Power/spectra->MaxPower, 116, 15);
    
    if (spectra->Ailment) {
        al_draw_bitmap(AilmentImage(spectra->Ailment), 54, 13, 0);
    }
    
    DrawTextF(116, 4, "%d/%d", spectra->Health, spectra->MaxHealth);
    DrawTextF(116, 15, "%d/%d", spectra->Power, spectra->MaxPower);
}

void DrawHudEnemy(const SPECTRA *spectra) {
    al_draw_bitmap(WindowImage(HUD_ENEMY), 0, 0, 0);
    const SPECIES *species = SpeciesOfSpectra(spectra);
    
    DrawText(species->Name, 105, 4);
    DrawTextF(127, 15, "%d", spectra->Level);
    DrawBar((float)spectra->Health/spectra->MaxHealth, 19, 4);
    DrawBar((float)spectra->Power/spectra->MaxPower, 19, 15);
    
    if (spectra->Ailment) {
        al_draw_bitmap(AilmentImage(spectra->Ailment), 155, 13, 0);
    }
}

void DrawTechniqueDisplay(TECHNIQUE_ID id) {
    al_draw_bitmap(WindowImage(TECHNIQUE_DISPLAY), 0, 0, 0);
    const TECHNIQUE *technique = TechniqueByID(id);
    
    DrawTitle(technique->Name, 4, 4);
    DrawNumber(105, 17, technique->Power);
    DrawNumber(171, 17, technique->Cost);
    DrawTextBox(technique->Description, 4, 30, 165);
    al_draw_bitmap(TypeImage(technique->Type), 2, 15, 0);
}

static void DrawWaitingIcon(int x, int y) {
    if (sin(TotalTimeElapsed*8)>0) {
        DrawSelector(x, y, 5, 8);
    }
}

void DrawOutputBattle(void) {
    al_draw_bitmap(WindowImage(OUTPUT_BATTLE), 117, 328, 0);
    DrawTextBox(GetOutput(), 121, 332, 352);
    if (OutputWaiting()) {
        DrawWaitingIcon(468, 345);
    }
}

void DrawOutputMenu(void) {
    al_draw_bitmap(WindowImage(OUTPUT_MENU), 0, 305, 0);
    DrawTextBox(GetOutput(), 4, 309, 472);
    if (OutputWaiting()) {
        DrawWaitingIcon(471, 348);
    }
}

void DrawOutputMap() {
    al_draw_bitmap(WindowImage(OUTPUT_MAP), 147, 328, 0);
    DrawTextBox(GetOutput(), 151, 332, 322);
    if (OutputWaiting()) {
        DrawWaitingIcon(468, 345);
    }
}