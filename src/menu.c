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
#include "player.h" // Player
#include "item.h"

#include "debug.h"

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
    for (int i=0; i<2; i++) {
        DrawText(choice->Option[choice->Scroll+i], 4, 4+13*i);
    }
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
    for (int i=0; i<6; i++) {
        DrawText(options->Option[options->Scroll+i], 4, 4+13*i);
    }
    DrawSelector(2, 2+13*options->Index, 96, 12);
}

void DrawColumn(const OPTIONS *first, const OPTIONS *second) {
    al_draw_bitmap(WindowImage(MENU_COLUMN), 0, 0, 0);
    for (int i=0; i<6; i++) {
        DrawText(first->Option[first->Scroll+i], 4, 4+13*i);
        DrawText(second->Option[first->Scroll+i], 101, 4+13*i);
    }
    DrawSelector(2, 2+13*first->Index, 138, 12);
}

void UpdateMenu(OPTIONS *options) {
    switch (options->State) {
    case MENU_IDLE:
        // Respond to scrolling up
        if (KeyDown(KEY_UP)) {
            options->State = MENU_MOVE;
            if (options->Index > 0) {
                options->Index--;
            } else if (options->Scroll > 0) {
                options->Scroll--;
            }
        // Jump up
        } else if (options->Jump && KeyDown(KEY_LEFT)) {
            options->State = MENU_MOVE;
            options->Index -= options->Jump;
            if (options->Index < 0) {
                options->Scroll += options->Index;
                options->Index = 0;
                if (options->Scroll < 0) {
                    options->Scroll = 0;
                }
            }
        // Respond to scrolling down
        } else if (KeyDown(KEY_DOWN)) {
            options->State = MENU_MOVE;
            if (options->Index < options->IndexMax) {
                options->Index++;
            } else if (options->Scroll < options->ScrollMax) {
                options->Scroll++;
            }
        // Jump down
        } else if (options->Jump && KeyDown(KEY_RIGHT)) {
            options->State = MENU_MOVE;
            options->Index += options->Jump;
            if (options->Index > options->IndexMax) {
                options->Scroll += (options->Index-options->IndexMax);
                options->Index = options->IndexMax;
                if (options->Scroll > options->ScrollMax) {
                    options->Scroll = options->ScrollMax;
                }
            }
        // Respond to confirm key
        } else if (KeyDown(KEY_CONFIRM)) {
            options->State = MENU_CONFIRM;
        // Respond to cancel key
        } else if (KeyDown(KEY_DENY)) {
            options->State = MENU_CANCEL;
        }
        break;
    
    case MENU_MOVE:
        // Make the menu idle if no scrolling
        if (!KeyDown(KEY_UP) && !KeyDown(KEY_DOWN)) {
            options->State = MENU_IDLE;
        }
        break;
    
    case MENU_CONFIRM:
        break;

    case MENU_CANCEL:
    default:
        // Can't use the menu after confirming or cancelling.
        eprintf("Use of menu after termination.\n");
        break;
    }
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
        sprite = CostumeImage(Player->Costume);
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

static inline void DrawWaitingIcon(int x, int y) {
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

void DrawOutputMap(void) {
    al_draw_bitmap(WindowImage(OUTPUT_MAP), 147, 328, 0);
    DrawTextBox(GetOutput(), 151, 332, 322);
    if (OutputWaiting()) {
        DrawWaitingIcon(468, 345);
    }
}

void DrawPlayerDisplay(void) {
    al_draw_bitmap(WindowImage(PLAYER_DISPLAY), 0, 0, 0);
    DrawText("Amy", 45, 4);
    DrawText(Location(Player->Location)->Name, 45, 17);
    //DrawText("Unused", 45, 30);
    
    // Spectra count
    int spectraCount = 0;
    for (int i = 0; i < TEAM_SIZE; i++) {
        if (Player->Spectra[i].Species) {
            spectraCount++;
        }
    }
    DrawNumber(141, 46, spectraCount);
    
    // Item count
    int itemCount = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (Player->Inventory[i]) {
            itemCount++;
        }
    }
    DrawNumber(141, 59, itemCount);
    
    // Time formatting
    int time = Player->PlayTime + UnaccountedPlayTime();
    char timeString[10];
    const char *format = time%2? "%d:%02d": "%d %02d";
    snprintf(timeString, 9, format, time/3600, time/60%60);
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(0, 0, 0),
        141,
        72-3,
        ALLEGRO_ALIGN_RIGHT|ALLEGRO_ALIGN_INTEGER,
        timeString);
    
    // Money
    DrawNumber(141, 85, Player->Money);
    
    // Sprite pane
    ALLEGRO_BITMAP *sprite = CostumeImage(Player->Costume);
    int width = al_get_bitmap_width(sprite);
    int height = al_get_bitmap_height(sprite);
    int xOffset = (32-width)/2;
    int yOffset = (85-height)/2;
    al_draw_bitmap(sprite, 6+xOffset, 6+yOffset, 0);
}

void DrawAt(int x, int y) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_translate_transform(&trans, x, y);
    al_use_transform(&trans);
}

static OPTIONS SpectraControl = {
    .State              = MENU_IDLE,
};

static OPTIONS ItemControl = {
    .Jump               = 8,
    .State              = MENU_IDLE,
};

void DrawParty(void) {
    al_draw_bitmap(WindowImage(SPECTRA_LIST), 0, 0, 0);
    for (int i=0; i<TEAM_SIZE && Player->Spectra[i].Species; i++) {
        // Contend with vertical divider
        int y;
        if (i < 3) {
            y = 17+13*i;
        } else {
            y = 72+13*(i-3);
        }
        
        // Display spectra information
        const SPECTRA *spectra = &Player->Spectra[i];
        DrawText(SpeciesOfSpectra(spectra)->Name, 4, y);
        DrawTextF(109, y, "%d", spectra->Level);
        DrawBar((float)spectra->Health/spectra->MaxHealth, 133, y);
        DrawBar((float)spectra->Power/spectra->MaxPower, 219, y);
        DrawTextF(133, y, "%d/%d", spectra->Health, spectra->MaxHealth);
        DrawTextF(219, y, "%d/%d", spectra->Power, spectra->MaxPower);
    }
    DrawSelector(2, 15+SpectraControl.Index*13, 104, 12);
}

void DrawItems(void) {
    al_draw_bitmap(WindowImage(ITEM_LIST), 0, 0, 0);
    int x, y;
    for (int i=0; i<INVENTORY_SIZE && Player->Inventory[i]; i++) {
        // Contend with columns
        x = 4 + 105*(i%3);
        y = 17 + 13*(i/3);
        
        // Get item information
        //const ITEM *item = Item(Player->Inventory[i]);
        //DrawText(item->Name, x, y);
    }
    
    // Draw selector
    x = 2 + 105*(ItemControl.Index%3);
    y = 15 + 13*(ItemControl.Index/3);
    DrawSelector(x, y, 104, 12);
}

typedef enum {
    MENU_PARTY,
    MENU_ITEMS,
    MENU_PLAYER,
    MENU_SAVE,
    MENU_EXIT,
} MAIN_MENU_OPTION;

static OPTIONS MainMenu = {
    .Option = {
        [MENU_PARTY]    = "Party",
        [MENU_ITEMS]    = "Items",
        [MENU_PLAYER]   = "Player",
        [MENU_SAVE]     = "Save",
        [MENU_EXIT]     = "Exit",
    },
    .IndexMax           = 4,
    .State              = MENU_IDLE,
};

void DrawMainMenu(void) {
    // Errors
    if (MainMenu.State == MENU_CANCEL) {
        eprintf("Tried to draw main menu in cancel state.\n");
    }
    
    // Draw main menu
    DrawAt(326, 92);
    DrawOption(&MainMenu);
    
    // Draw sub-menus
    if (MainMenu.State == MENU_CONFIRM) {
        switch (MainMenu.Index) {
        case MENU_PARTY:
            DrawAt(18, 92);
            DrawParty();
            break;

        case MENU_ITEMS:
            DrawAt(4, 92);
            DrawItems();
            break;

        case MENU_PLAYER:
            DrawAt(179, 92);
            DrawPlayerDisplay();
            break;

        case MENU_SAVE:
            // Draw save screen
            break;
        default:
            break;
        }
    }
}

void ResetWait(WAIT *wait) {
    wait->State = WAIT_BEFORE;
}

void UpdateWait(WAIT *wait) {
    switch (wait->State) {
    case WAIT_BEFORE:
        if (KeyDown(wait->Key)) {
            wait->State = WAIT_DURING;
        }
        break;
    
    case WAIT_DURING:
        if (!KeyDown(wait->Key)) {
            wait->State = WAIT_AFTER;
        }
        break;
    default:
        break;
    }
}

bool IsWaiting(WAIT *wait) {
    return wait->State != WAIT_AFTER;
}

static WAIT Overlay = {KEY_DENY, WAIT_BEFORE};

void UpdateMainMenu(void) {
    MENU_STATE before = MainMenu.State;
    UpdateMenu(&MainMenu);
    if (MainMenu.State == MENU_CONFIRM) {
        // Initialize new menu if just confirmed an option
        if (before != MainMenu.State) {
            switch (MainMenu.Index) {
            case MENU_PARTY:
                // Update party controls with bounds
                for (int i=0; i<TEAM_SIZE && Player->Spectra[i].Species; i++) {
                    SpectraControl.IndexMax = i;
                }
                SpectraControl.State = MENU_IDLE;
                break;
            case MENU_ITEMS:
                // Update item controls with bounds
                for (int i=0; i<TEAM_SIZE && Player->Spectra[i].Species; i++) {
                    ItemControl.IndexMax = i;
                }
                ItemControl.State = MENU_IDLE;
                break;

            case MENU_PLAYER:
                ResetWait(&Overlay);
                break;
            case MENU_SAVE:
                // Update save menu
                break;
            default:
                break;
            }
        // Update existing menu
        } else if (!KeyDown(KEY_CONFIRM)) {
            switch (MainMenu.Index) {
            case MENU_PARTY:
                UpdateMenu(&SpectraControl);
                eprintf("Spectra state %d\n", SpectraControl.State);
                if (SpectraControl.State == MENU_CANCEL && !KeyDown(KEY_DENY)) {
                    MainMenu.State = MENU_IDLE;
                }
                break;
            case MENU_ITEMS:
                UpdateMenu(&ItemControl);
                if (ItemControl.State == MENU_CANCEL && !KeyDown(KEY_DENY)) {
                    MainMenu.State = MENU_IDLE;
                }
                break;

            case MENU_PLAYER:
                UpdateWait(&Overlay);
                if (!IsWaiting(&Overlay)) {
                    MainMenu.State = MENU_IDLE;
                }
                break;

            case MENU_SAVE:
                // Update save menu
                break;
            default:
                break;
            }
        }
    }
}
