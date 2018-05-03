/**********************************************************//**
 * @file menu.c
 * @brief Implements menu and main menu processing.
 * @author Rena Shinomiya
 * @date April 24, 2018
 **************************************************************/

#include <stdio.h>              // snprintf
#include <math.h>               // sin

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "game.h"               // KEY
#include "assets.h"             // Font, WindowImage
#include "species.h"            // SpeciesByID
#include "technique.h"          // TechniqueByID
#include "item.h"               // ItemByID
#include "player.h"             // Player
#include "output.h"             // GetOutput
#include "wait.h"               // WAIT
#include "debug.h"              // eprintf

/**********************************************************//**
 * @brief Draws standard text on the screen.
 * @param text: String to draw.
 * @param x: X position to draw at.
 * @param y: Y position to draw at.
 **************************************************************/
static inline void DrawText(const char *text, int x, int y) {
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(0, 0, 0),
        x,
        y-3,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

/**********************************************************//**
 * @brief Calls DrawText with printf-style formatting.
 * @param x: X position to draw at.
 * @param y : Y position to draw at.
 * @param format: printf-style format string.
 * @param ...: printf-style arguments.
 **************************************************************/
#define DrawTextF(x, y, format, ...) {\
    char buf[256];\
    snprintf(buf, 255, format, __VA_ARGS__);\
    DrawText(buf, x, y);\
}

/**********************************************************//**
 * @brief Draws a number on the screen. This is right-aligned.
 * @param x: X position to draw at.
 * @param y: Y position to draw at.
 * @param number: Number to draw.
 **************************************************************/
static inline void DrawNumber(int x, int y, int number) {
    const int BUF_SIZE = 32;
    char buf[BUF_SIZE+1];
    snprintf(buf, BUF_SIZE, "%d", number);
    al_draw_text(
        Font(FONT_WINDOW),
        number? al_map_rgb(0, 0, 0): al_map_rgb(128, 128, 128),
        x,
        y-3,
        ALLEGRO_ALIGN_RIGHT|ALLEGRO_ALIGN_INTEGER,
        buf);
}

/**********************************************************//**
 * @brief Like DrawText, but uses window title styling.
 * @param text: String to draw.
 * @param x: X position to draw at.
 * @param y: Y position to draw at.
 **************************************************************/
static inline void DrawTitle(const char *text, int x, int y) {
    al_draw_text(
        Font(FONT_WINDOW),
        al_map_rgb(226, 226, 226),
        x,
        y-3,
        ALLEGRO_ALIGN_LEFT|ALLEGRO_ALIGN_INTEGER,
        text);
}

/**********************************************************//**
 * @brief Calls DrawTitle with printf-style formatting.
 * @param x: X position to draw at.
 * @param y : Y position to draw at.
 * @param format: printf-style format string.
 * @param ...: printf-style arguments.
 **************************************************************/
#define DrawTitleF(x, y, format, ...) {\
    char buf[256];\
    snprintf(buf, 255, format, __VA_ARGS__);\
    DrawTitle(buf, x, y);\
}

/**********************************************************//**
 * @brief Draws a text box using DrawText styling.
 * @param text: String to draw.
 * @param x: X position to draw at.
 * @param y : Y position to draw at.
 * @param width: Width in pixels of the text box.
 **************************************************************/
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

/**********************************************************//**
 * @brief Like DrawTextBox, but uses printf-style formatting.
 * @param x: X position to draw at.
 * @param y : Y position to draw at.
 * @param width: Width in pixels of the text box.
 * @param format: printf-style format string.
 * @param ...: printf-style arguments.
 **************************************************************/
#define DrawTextBoxF(x, y, width, format, ...) {\
    char buf[256];\
    snprintf(buf, 255, format, __VA_ARGS__);\
    DrawTextBox(buf, x, y, width);\
}

/**********************************************************//**
 * @brief Draw a greyed-out box denoting an item has been
 * selected by the user.
 * @param x: Upper-left X position.
 * @param y: Upper-left Y position.
 * @param width: Width of the selector box.
 * @param height: Height of the seelctor box.
 **************************************************************/
static inline void DrawSelector(int x, int y, int width, int height) {
    al_draw_filled_rectangle(x, y, x+width, y+height, al_map_rgba(0, 0, 0, 60));
}

/**********************************************************//**
 * @brief Draws a colored health bar.
 * @param percent: Percent of health (in 0...1)
 * @param x: Upper-left X position.
 * @param y: Upper-left Y position.
 **************************************************************/
static inline void DrawBar(float percent, int x, int y) {
    int x0 = x + (int)(percent*81);
    al_draw_filled_rectangle(x, y, x0, y+8, al_color_hsv(120*percent, 0.5f, 0.8f));
}

/**********************************************************//**
 * @brief Draws a small two-index menu on the screen, with
 * scrolling enabled.
 * @param choice: MENU choices of 4 or less characters each.
 **************************************************************/
void DrawChoice(const MENU *choice) {
    al_draw_bitmap(WindowImage(MENU_CHOICE), 0, 0, 0);
    for (int i=0; i<2; i++) {
        DrawText(choice->Option[choice->Control.Scroll+i], 4, 4+13*i);
    }
    DrawSelector(2, 2+13*choice->Control.Index, 34, 12);
}

/**********************************************************//**
 * @brief Draws an alert box on the screen.
 * @param text: Text to put in the alert.
 **************************************************************/
void DrawAlert(const char *text) {
    al_draw_bitmap(WindowImage(ALERT), 0, 0, 0);
    DrawTextBox(text, 4, 17, 120);
}

/**********************************************************//**
 * @brief Draws a warning box on the screen.
 * @param text: Text to put in the warning.
 **************************************************************/
void DrawWarning(const char *text) {
    al_draw_bitmap(WindowImage(WARNING), 0, 0, 0);
    DrawTextBox(text, 4, 17, 120);
}

/**********************************************************//**
 * @brief Draws a 6-index menu with scrolling on the screen.
 * @param options: MENU items to draw.
 **************************************************************/
void DrawOption(const MENU *options) {
    al_draw_bitmap(WindowImage(MENU_OPTION), 0, 0, 0);
    for (int i=0; i<6; i++) {
        DrawText(options->Option[options->Control.Scroll+i], 4, 4+13*i);
    }
    DrawSelector(2, 2+13*options->Control.Index, 96, 12);
}

/**********************************************************//**
 * @brief Draws a 2-column menu on the screen. The control of
 * the first item supercedes the control of the second.
 * @param first: Items to put in the first column.
 * @param second: Items to put in the second column.
 **************************************************************/
void DrawColumn(const MENU *first, const MENU *second) {
    al_draw_bitmap(WindowImage(MENU_COLUMN), 0, 0, 0);
    for (int i=0; i<6; i++) {
        DrawText(first->Option[first->Control.Scroll+i], 4, 4+13*i);
        DrawText(second->Option[first->Control.Scroll+i], 101, 4+13*i);
    }
    DrawSelector(2, 2+13*first->Control.Index, 138, 12);
}

/**********************************************************//**
 * @brief Harnesses user input to update the state and
 * position of a CONTROL. Nothing is done if the control
 * isn't idle.
 * @param control: CONTROL to update.
 **************************************************************/
void UpdateControl(CONTROL *control) {
    switch (control->State) {
    case CONTROL_IDLE:
        // Respond to scrolling up
        if (KeyJustDown(KEY_UP)) {
            ControlUp(control);
        // Jump up
        } else if (control->Jump && KeyJustDown(KEY_LEFT)) {
            for (int i=0; i<control->Jump; i++) {
                ControlUp(control);
            }
        // Respond to scrolling down
        } else if (KeyJustDown(KEY_DOWN)) {
            ControlDown(control);
        // Jump down
        } else if (control->Jump && KeyJustDown(KEY_RIGHT)) {
            for (int i=0; i<control->Jump; i++) {
                ControlDown(control);
            }
        // Respond to confirm key
        } else if (KeyJustDown(KEY_CONFIRM)) {
            control->State = CONTROL_CONFIRM;
        // Respond to cancel key
        } else if (KeyJustDown(KEY_DENY)) {
            control->State = CONTROL_CANCEL;
        }
        break;
    
    case CONTROL_CONFIRM:
    case CONTROL_CANCEL:
    default:
        // No updating after end
        break;
    }
}

/**********************************************************//**
 * @brief Draws a stat sheet for a SPECTRA.
 * @param spectra: Spectra to display stats for.
 **************************************************************/
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
    DrawNumber(102, 109, ExperienceTotal(spectra));
    DrawNumber(102, 122, (spectra->Level==LEVEL_MAX)? 0: spectra->Experience);
    
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

/**********************************************************//**
 * @brief Draw a HUD for a SPECTRA from the user's perspective.
 * @param spectra: SPECTRA to show.
 **************************************************************/
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

/**********************************************************//**
 * @brief Draw a HUD for a SPECTRA from the enemy's perspective.
 * @param spectra: SPECTRA to show.
 **************************************************************/
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

/**********************************************************//**
 * @brief Draw information for a TECHNIQUE.
 * @param id: ID of a TECHNIQUE.
 **************************************************************/
void DrawTechniqueDisplay(TECHNIQUE_ID id) {
    al_draw_bitmap(WindowImage(TECHNIQUE_DISPLAY), 0, 0, 0);
    const TECHNIQUE *technique = TechniqueByID(id);
    
    DrawTitle(technique->Name, 4, 4);
    DrawNumber(105, 17, technique->Power);
    DrawNumber(171, 17, technique->Cost);
    DrawTextBox(technique->Description, 4, 30, 165);
    al_draw_bitmap(TypeImage(technique->Type), 2, 15, 0);
}

/**********************************************************//**
 * @brief Draw information for an ITEM.
 * @param id: ID of an ITEM.
 **************************************************************/
void DrawItemDisplay(ITEM_ID id) {
    al_draw_bitmap(WindowImage(ITEM_DISPLAY), 0, 0, 0);
    const ITEM *item = ItemByID(id);
    
    DrawTitle(item->Name, 4, 4);
    DrawTextF(39, 17, "$%d.00", item->Price);
    DrawTextBox(item->Description, 4, 30, 165);
}

/**********************************************************//**
 * @brief Draw the animated wait icon for output.
 * @param x: X position of the icon.
 * @param y: Y position of the icon.
 **************************************************************/
static inline void DrawWaitingIcon(int x, int y) {
    if (sin(al_get_time()*8)>0) {
        DrawSelector(x, y, 5, 8);
    }
}

/**********************************************************//**
 * @brief Draw the current output.
 **************************************************************/
void DrawOutput(void) {
    al_draw_bitmap(WindowImage(OUTPUT), 4, 328, 0);
    DrawTextBox(GetOutput(), 8, 332, 464);
    if (OutputWaiting()) {
        DrawWaitingIcon(468, 345);
    }
}

/**********************************************************//**
 * @brief Draws a popup bar on the screen.
 * @param text: Text to put in the popup.
 **************************************************************/
void DrawPopupBar(const char *text) {
    al_draw_bitmap(WindowImage(POPUP_BAR), 0, 0, 0);
    DrawText(text, 4, 4);
}

/**********************************************************//**
 * @brief Draw the player's information on the screen.
 **************************************************************/
void DrawPlayerDisplay(void) {
    al_draw_bitmap(WindowImage(PLAYER_DISPLAY), 0, 0, 0);
    DrawText("Amy", 45, 4);
    DrawTextBox(Location(Player->Location)->Name, 45, 17, 94);
    
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

/**********************************************************//**
 * @brief Set the upper-left corner of the subsequent menus
 * to be drawn. Use DrawAt(0, 0) to return to normal.
 * @param x: X position to draw at.
 * @param y: Y position to draw at.
 **************************************************************/
void DrawAt(int x, int y) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_translate_transform(&trans, x, y);
    al_use_transform(&trans);
}

/**************************************************************/
/// @brief Controls the spectra menu.
static CONTROL SpectraControl = {
    .Jump               = 3,
    .State              = CONTROL_IDLE,
};

/// @brief Controls the items menu.
static CONTROL ItemControl = {
    .Jump               = 8,
    .State              = CONTROL_IDLE,
};

/**********************************************************//**
 * @brief Gets the Y offset into the party screen.
 * @param index: Index of spectra in the party.
 * @return The Y position to offset to.
 **************************************************************/
static inline int PartyY(int index) {
    if (index < 3) {
        return 17+13*index;
    } else {
        return 72+13*(index-3);
    }
}

/**********************************************************//**
 * @brief Draws the party menu on the screen.
 **************************************************************/
void DrawParty(void) {
    al_draw_bitmap(WindowImage(SPECTRA_LIST), 0, 0, 0);
    for (int i=0; i<TEAM_SIZE && Player->Spectra[i].Species; i++) {
        // Contend with vertical divider
        int y = PartyY(i);
        
        // Display spectra information
        const SPECTRA *spectra = &Player->Spectra[i];
        DrawText(SpeciesOfSpectra(spectra)->Name, 4, y);
        DrawNumber(130, y, spectra->Level);
        DrawBar((float)spectra->Health/spectra->MaxHealth, 133, y);
        DrawBar((float)spectra->Power/spectra->MaxPower, 219, y);
        DrawTextF(133, y, "%d/%d", spectra->Health, spectra->MaxHealth);
        DrawTextF(219, y, "%d/%d", spectra->Power, spectra->MaxPower);
    }
    DrawSelector(2, PartyY(SpectraControl.Index)-2, 104, 12);
}

/**********************************************************//**
 * @brief Draws the items menu on the screen.
 **************************************************************/
void DrawItems(void) {
    al_draw_bitmap(WindowImage(ITEM_LIST), 0, 0, 0);
    int x, y;
    for (int i=0; i<INVENTORY_SIZE && Player->Inventory[i]; i++) {
        // Contend with columns
        x = 4 + 105*(i/8);
        y = 17 + 13*(i%8);
        
        // Get item information
        const ITEM *item = ItemByID(Player->Inventory[i]);
        DrawText(item->Name, x, y);
    }
    
    // Draw selector
    x = 2 + 105*(ItemControl.Index/8);
    y = 15 + 13*(ItemControl.Index%8);
    DrawSelector(x, y, 104, 12);
}

/**********************************************************//**
 * @enum MAIN_MENU_OPTION
 * @brief Enumerates options in the main menu.
 **************************************************************/
typedef enum {
    MENU_PARTY,
    MENU_ITEMS,
    MENU_PLAYER,
    MENU_INFO,
    MENU_SAVE,
    MENU_EXIT,
} MAIN_MENU_OPTION;

/**************************************************************/
/// @brief Defines the main menu and its control structure.
static MENU MainMenu = {
    .Option = {
        [MENU_PARTY]    = "Party",
        [MENU_ITEMS]    = "Items",
        [MENU_PLAYER]   = "Player",
        [MENU_INFO]     = "Options",
        [MENU_SAVE]     = "Save",
        [MENU_EXIT]     = "Exit",
    },
    .Control = {
        .IndexMax       = 5,
    },
};

/// @brief Wait data for when the main menu opens an overlay.
static WAIT Overlay = WAIT_INITIALIZER(KEY_DENY);

/**********************************************************//**
 * @enum SAVE_PHASE
 * @brief Informs the save system of how the save is progressing.
 **************************************************************/
typedef enum {
    SAVE_BEFORE,
    SAVE_DURING,
    SAVE_AFTER,
} SAVE_PHASE;

/// @brief Current save system phase.
static SAVE_PHASE SavePhase;

/// @brief Whether the last save succeeded or failed.
static bool SaveStatus = false;

/**********************************************************//**
 * @brief Sets up the main menu when it's opened.
 **************************************************************/
void InitializeMainMenu(void) {
    for (int i=0; i<TEAM_SIZE && Player->Spectra[i].Species; i++) {
        SpectraControl.IndexMax = i;
    }
    for (int i=0; i<INVENTORY_SIZE && Player->Inventory[i]; i++) {
        ItemControl.IndexMax = i;
    }
    ResetControl(&MainMenu.Control);
}

/**********************************************************//**
 * @brief Draws the main menu and any of its descendants
 * on the screen.
 **************************************************************/
void DrawMainMenu(void) {
    // Draw main menu
    DrawAt(326, 92);
    DrawOption(&MainMenu);
    
    // Draw sub-menus
    if (MainMenu.Control.State == CONTROL_CONFIRM) {
        switch (MenuItem(&MainMenu)) {
        case MENU_PARTY:
            DrawAt(18, 92);
            DrawParty();
            if (SpectraControl.State == CONTROL_CONFIRM) {
                DrawAt(26, 100);
                DrawSpectraDisplay(&Player->Spectra[ControlItem(&SpectraControl)]);
            }
            break;

        case MENU_ITEMS:
            DrawAt(4, 92);
            DrawItems();
            DrawAt(4, 216);
            DrawItemDisplay(Player->Inventory[ControlItem(&ItemControl)]);
            if (ItemControl.State == CONTROL_CONFIRM) {
                DrawAt(12, 100);
                DrawParty();
            }
            break;

        case MENU_PLAYER:
            DrawAt(179, 92);
            DrawPlayerDisplay();
            break;

        case MENU_SAVE:
            DrawAt(194, 92);
            switch (SavePhase) {
            case SAVE_BEFORE:
            case SAVE_DURING:
                DrawAlert("Now saving...");
                break;
            case SAVE_AFTER:
                DrawAlert(SaveStatus? "Now saving...\nComplete!": "Now saving...\nFailed.");
                break;
            }
            break;
        }
    }
}

/**********************************************************//**
 * @brief Updates the main menu and any of its descendants.
 **************************************************************/
void UpdateMainMenu(void) {
    switch (MainMenu.Control.State) {
    // Cases where a submenu is currently open:
    // Party, Items, Player, Info, and Save screens.
    case CONTROL_CONFIRM:
        switch (MenuItem(&MainMenu)) {
        case MENU_PARTY:
            switch (SpectraControl.State) {
            case CONTROL_CONFIRM:
                if (KeyJustUp(KEY_DENY)) {
                    SpectraControl.State = CONTROL_IDLE;
                }
                break;
            case CONTROL_CANCEL:
                MainMenu.Control.State = CONTROL_IDLE;
                break;
            case CONTROL_IDLE:
                UpdateControl(&SpectraControl);
                break;
            }
            break;
        
        case MENU_ITEMS:
            switch (ItemControl.State) {
            case CONTROL_CONFIRM:
                // Select a spectra to use the item on
                switch (SpectraControl.State) {
                case CONTROL_CONFIRM:
                    // Use the item
                    break;
                case CONTROL_CANCEL:
                    ItemControl.State = CONTROL_IDLE;
                    break;
                case CONTROL_IDLE:
                    UpdateControl(&SpectraControl);
                    break;
                }
                break;
            case CONTROL_CANCEL:
                MainMenu.Control.State = CONTROL_IDLE;
                break;
            case CONTROL_IDLE:
                UpdateControl(&ItemControl);
                // Initialize spectra choice submenu
                if (ItemControl.State == CONTROL_CONFIRM) {
                    // Ensure the item can be used from the menu
                    const ITEM *item = ItemByID(Player->Inventory[ControlItem(&ItemControl)]);
                    if (item->Flags & MENU_ONLY) {
                        ResetControl(&SpectraControl);
                    } else {
                        ItemControl.State = CONTROL_IDLE;
                    }
                }
                break;
            }
            break;
        
        case MENU_PLAYER:
        case MENU_INFO:
            UpdateWait(&Overlay);
            if (!IsWaiting(&Overlay)) {
                MainMenu.Control.State = CONTROL_IDLE;
            }
            break;
        
        case MENU_SAVE:
            switch (SavePhase) {
            case SAVE_BEFORE:
                SavePhase = SAVE_DURING;
                break;
            case SAVE_DURING:
                SaveStatus = SaveGame();
                SavePhase = SAVE_AFTER;
                break;
            case SAVE_AFTER:
                UpdateWait(&Overlay);
                if (!IsWaiting(&Overlay)) {
                    MainMenu.Control.State = CONTROL_IDLE;
                }
                break;
            }
            break;
        }
        break;
     
     // The player can use the main menu currently.
     case CONTROL_IDLE:
        // Update menu and initialize new submenu if needed
        UpdateMenu(&MainMenu);
        if (MainMenu.Control.State == CONTROL_CONFIRM) {
            switch (MenuItem(&MainMenu)) {
            case MENU_PARTY:
                ResetControl(&SpectraControl);
                break;
            case MENU_ITEMS:
                ResetControl(&ItemControl);
                break;
            case MENU_PLAYER:
            case MENU_INFO:
                ResetWait(&Overlay);
                break;
            case MENU_SAVE:
                ResetWait(&Overlay);
                SavePhase = SAVE_BEFORE;
                break;
            case MENU_EXIT:
                // Immediately cancel out of menu
                MainMenu.Control.State = CONTROL_CANCEL;
                break;
            }
        }
        break;
    
    // The menu has been cancelled.
    case CONTROL_CANCEL:
        break;
    }
}

/**********************************************************//**
 * @brief Check if the main menu was cancelled and processing
 * should return to whatever invoked the menu.
 * @return True if the main menu is closed.
 **************************************************************/
bool MainMenuClosed(void) {
    return MainMenu.Control.State == CONTROL_CANCEL;
}

/**************************************************************/
