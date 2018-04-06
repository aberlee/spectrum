/**********************************************************//**
 * @file assets.c
 * @brief Includes and loads all the game assets.
 * @author Rena Shinomiya
 * @date March 22nd, 2018
 **************************************************************/

// C Standard Library
#include <string.h>         // strlen

// Allegro5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "location.h"       // MAP_ID, BACKGROUND_ID
#include "species.h"        // SPECIES_ID, AILMENT_ID
#include "menu.h"           // WINDOW_ID
#include "player.h"         // COSTUME_ID
#include "assets.h"         // FONT_ID

#include "debug.h"          // eprintf

typedef struct {
    char Filename[64];
    ALLEGRO_BITMAP *Image;
} IMAGE_ASSET;

typedef struct {
    char Filename[64];
    int Size;
    ALLEGRO_FONT *Font;
} FONT_ASSET;

#define IMAGE(filename) {"data/image/" filename, NULL}
#define FONT(filename, size) {"data/font/" filename, size, NULL}

static IMAGE_ASSET BackgroundAssets[N_BACKGROUND] = {
    [CAVE]              = IMAGE("background/cave.png"),
    [GRASS]             = IMAGE("background/grass.png"),
    [OCEAN]             = IMAGE("background/ocean.png"),
    [OXIDE]             = IMAGE("background/oxide.png"),
    [SAND]              = IMAGE("background/sand.png"),
    [CHARCOAL]          = IMAGE("background/watercolor.png"),
};

static IMAGE_ASSET SpeciesAssets[N_SPECIES] = {
    [ACISTAR]           = IMAGE("monster/acistar.png"),
    [ASTEROID]          = IMAGE("monster/asteroid.png"),
    [BASSMONSTER]       = IMAGE("monster/bassmonstr.png"),
    [BRONTO]            = IMAGE("monster/bronto.png"),
    [BUTCHERBIRD]       = IMAGE("monster/butchrbird.png"),
    [CATFISH]           = IMAGE("monster/catfish.png"),
    [COALSHARK]         = IMAGE("monster/coalshark.png"),
    [CUMULUS]           = IMAGE("monster/cumulus.png"),
    [DACTYLUS]          = IMAGE("monster/dactylus.png"),
    [FLOPJELLY]         = IMAGE("monster/flopjelly.png"),
    [FOGFANG]           = IMAGE("monster/fogfang.png"),
    [GARPIKE]           = IMAGE("monster/garpike.png"),
    [GASMOG]            = IMAGE("monster/gasmog.png"),
    [GIGACLAM]          = IMAGE("monster/gigaclam.png"),
    [GLACIALITH]        = IMAGE("monster/glacialith.png"),
    [GOLDDRAGON]        = IMAGE("monster/golddragon.png"),
    [HOCUS]             = IMAGE("monster/hocus.png"),
    [ICEBOULDER]        = IMAGE("monster/iceboulder.png"),
    [JAYRAPTOR]         = IMAGE("monster/jayraptor.png"),
    [KARDA]             = IMAGE("monster/karda.png"),
    [LAUNCHPAD]         = IMAGE("monster/launchpad.png"),
    [LEKTRON]           = IMAGE("monster/lektron.png"),
    [MEGATAR]           = IMAGE("monster/megatar.png"),
    [MINESPHERE]        = IMAGE("monster/minesphere.png"),
    [MOATMONSTER]       = IMAGE("monster/moatmonstr.png"),
    [NESSIE]            = IMAGE("monster/nessie.png"),
    [NITROBOMB]         = IMAGE("monster/nitrobomb.png"),
    [OSCILLATE]         = IMAGE("monster/oscillate.png"),
    [PHOENIX]           = IMAGE("monster/phoenix.png"),
    [PRGMERROR]         = IMAGE("monster/prgmerror.png"),
    [PUZZLE]            = IMAGE("monster/puzzle.png"),
    [RAIKEGON]          = IMAGE("monster/raikegon.png"),
    [REPLICA]           = IMAGE("monster/replica.png"),
    [SPACESNAKE]        = IMAGE("monster/spacesnake.png"),
    [SSSNAKE]           = IMAGE("monster/sssnake.png"),
    [TARHEAP]           = IMAGE("monster/tarheap.png"),
    [TELEVIRUS]         = IMAGE("monster/televirus.png"),
    [TOOLFISH]          = IMAGE("monster/toolfish.png"),
    [TOTEM_POLE]         = IMAGE("monster/totempole.png"),
    [VACUUM]            = IMAGE("monster/vacuum.png"),
    [VOLTDRAGON]        = IMAGE("monster/voltdragon.png"),
    [WATERWING]         = IMAGE("monster/waterwing.png"),
};

static IMAGE_ASSET CostumeAssets[N_COSTUME] = {
    [NORMAL]            = IMAGE("costume/normal.png"),
    [BLACK_DRESS]       = IMAGE("costume/black_dress.png"),
    [RED_DRESS]         = IMAGE("costume/red_dress.png"),
    [SUIT]              = IMAGE("costume/suit.png"),
    [SWIMSUIT]          = IMAGE("costume/swimsuit.png"),
    [PAJAMAS]           = IMAGE("costume/pajamas.png"),
};

static IMAGE_ASSET WindowAssets[N_WINDOW] = {
    [ALERT]             = IMAGE("window/alert.png"),
    [WARNING]           = IMAGE("window/warning.png"),
    
    [MENU_CHOICE]       = IMAGE("window/choice.png"),
    [MENU_OPTION]       = IMAGE("window/options.png"),
    [MENU_COLUMN]       = IMAGE("window/shop.png"),
    
    [HUD_ENEMY]         = IMAGE("window/hud_enemy.png"),
    [HUD_USER]          = IMAGE("window/hud_user.png"),
    
    [PLAYER_DISPLAY]    = IMAGE("window/player.png"),
    [SPECTRA_DISPLAY]   = IMAGE("window/monster.png"),
    [TECHNIQUE_DISPLAY] = IMAGE("window/technique.png"),
    [ITEM_DISPLAY]      = IMAGE("window/item.png"),
    
    [SPECTRA_LIST]      = IMAGE("window/monsters.png"),
    [ITEM_LIST]         = IMAGE("window/items.png"),
    
    [OUTPUT_BATTLE]     = IMAGE("window/output_battle.png"),
    [OUTPUT_MENU]       = IMAGE("window/output_menu.png"),
    [OUTPUT_MAP]        = IMAGE("window/output_overworld.png"),
};

static IMAGE_ASSET AilmentAssets[N_AILMENT] = {
    [POISONED]          = IMAGE("window/ailment/poison.png"),
    [SHOCKED]           = IMAGE("window/ailment/shock.png"),
    [BURIED]            = IMAGE("window/ailment/buried.png"),
    [ASLEEP]            = IMAGE("window/ailment/asleep.png"),
    [AFLAME]            = IMAGE("window/ailment/aflame.png"),
};

static IMAGE_ASSET TypeAssets[N_TYPE] = {
    [BASIC]             = IMAGE("window/type/basic.png"),
    [FIRE]              = IMAGE("window/type/fire.png"),
    [WATER]             = IMAGE("window/type/water.png"),
    [ICE]               = IMAGE("window/type/ice.png"),
    [WIND]              = IMAGE("window/type/wind.png"),
    [EARTH]             = IMAGE("window/type/earth.png"),
    [METAL]             = IMAGE("window/type/metal.png"),
    [LIGHT]             = IMAGE("window/type/light.png"),
    [DARK]              = IMAGE("window/type/dark.png"),
};

static IMAGE_ASSET MapAssets[N_MAP] = {
    [MAP_OVERWORLD]             = IMAGE("map/kaido.png"),
    
    [MAP_BOULDER_CAVE]          = IMAGE("map/boulder_cave.png"),
    [MAP_FALLS_CAVE_1F]         = IMAGE("map/falls_cave_1st_floor.png"),
    [MAP_FALLS_CAVE_B1F]        = IMAGE("map/falls_cave_basement.png"),
    [MAP_GRANITE_CAVE_1F]       = IMAGE("map/granite_cave_1st_floor.png"),
    [MAP_GRANITE_CAVE_B1F]      = IMAGE("map/granite_cave_basement.png"),
    [MAP_NEW_LAND_CAVE]         = IMAGE("map/new_land_cave.png"),
    [MAP_OXIDE_CRATER]          = IMAGE("map/oxide_crater.png"),
    
    [MAP_SAPLING_YOUR_HOUSE]    = IMAGE("map/sapling_town/amy_house.png"),
    [MAP_SAPLING_AIRPORT]       = IMAGE("map/sapling_town/airport.png"),
    [MAP_SAPLING_HOSPITAL]      = IMAGE("map/sapling_town/hospital.png"),
    [MAP_SAPLING_CITY_HALL]     = IMAGE("map/sapling_town/city_hall.png"),
    [MAP_SAPLING_GREENHOUSE]    = IMAGE("map/sapling_town/greenhouse.png"),
    [MAP_SAPLING_LABORATORY]    = IMAGE("map/sapling_town/laboratory.png"),
    
    [MAP_ROYAL_HOSPITAL]        = IMAGE("map/port_royal/hospital.png"),
    [MAP_ROYAL_WAREHOUSE]       = IMAGE("map/port_royal/warehouse.png"),
    [MAP_ROYAL_PORT]            = IMAGE("map/port_royal/port.png"),
    
    [MAP_SOLAR_AIRPORT]         = IMAGE("map/solar_city/airport.png"),
    [MAP_SOLAR_HOSPITAL]        = IMAGE("map/solar_city/hospital.png"),
    [MAP_SOLAR_EAST_CORP]       = IMAGE("map/solar_city/corporation_east.png"),
    [MAP_SOLAR_WEST_CORP]       = IMAGE("map/solar_city/corporation_west.png"),
    [MAP_SOLAR_INSTITUTE_1F]    = IMAGE("map/solar_city/institute_1st_floor.png"),
    [MAP_SOLAR_INSTITUTE_2F]    = IMAGE("map/solar_city/institute_2nd_floor.png"),
    [MAP_SOLAR_INSTITUTE_3F]    = IMAGE("map/solar_city/institute_3rd_floor.png"),
    [MAP_GENERATOR_ROOM]        = IMAGE("map/solar_city/institute_generator_room.png"),
    
    [MAP_REST_STOP]             = IMAGE("map/andora_falls/rest_stop.png"),
    [MAP_ANDORA_HOSPITAL]       = IMAGE("map/andora_falls/hospital.png"),
    [MAP_ANDORA_PORT]           = IMAGE("map/andora_falls/port.png"),
    
    [MAP_GRANITE_AIRPORT]       = IMAGE("map/granite_city/airport.png"),
    [MAP_GRANITE_AIR_EAST]      = IMAGE("map/granite_city/air_tower_east.png"),
    [MAP_GRANITE_AIR_WEST]      = IMAGE("map/granite_city/air_tower_west.png"),
    [MAP_GRANITE_CORP]          = IMAGE("map/granite_city/corporation.png"),
    [MAP_GRANITE_DEPARTMENT]    = IMAGE("map/granite_city/department_store.png"),
    [MAP_GAME_DESIGNER_ROOM]    = IMAGE("map/granite_city/game_designer_room.png"),
    [MAP_GRANITE_HOSPITAL]      = IMAGE("map/granite_city/hospital.png"),
    [MAP_GRANITE_LIBRARY]       = IMAGE("map/granite_city/library.png"),
    [MAP_GRANITE_STORE_1]       = IMAGE("map/granite_city/store_1.png"),
    [MAP_GRANITE_STORE_2]       = IMAGE("map/granite_city/store_2.png"),
    [MAP_GRANITE_STORE_3]       = IMAGE("map/granite_city/store_3.png"),
    [MAP_GRANITE_STORE_4]       = IMAGE("map/granite_city/store_4.png"),
    [MAP_GRANITE_STORE_5]       = IMAGE("map/granite_city/store_5.png"),
    [MAP_GRANITE_STORE_6]       = IMAGE("map/granite_city/store_6.png"),
    [MAP_GRANITE_TOWER_1F]      = IMAGE("map/granite_city/tower_1st_floor.png"),
    [MAP_GRANITE_TOWER_2F]      = IMAGE("map/granite_city/tower_2nd_floor.png"),
    [MAP_GRANITE_TOWER_3F]      = IMAGE("map/granite_city/tower_3rd_floor.png"),
    [MAP_GRANITE_TOWER_4F]      = IMAGE("map/granite_city/tower_4th_floor.png"),
    [MAP_GRANITE_TOWER_5F]      = IMAGE("map/granite_city/tower_5th_floor.png"),
    [MAP_LAVATORY]              = IMAGE("map/granite_city/tower_bathroom.png"),
};

static FONT_ASSET FontAssets[N_FONT] = {
    [FONT_WINDOW]       = FONT("legacy/legacy.ttf", 10),
};

static bool LoadSuccess = true;

static void LoadImageAssets(IMAGE_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        if (assets[i].Filename && strlen(assets[i].Filename) > 0) {
            assets[i].Image = al_load_bitmap(assets[i].Filename);
            if (!assets[i].Image) {
                eprintf("Failed to load image \"%s\"!\n", assets[i].Filename);
                LoadSuccess = false;
            }
        }
    }
}

static void DestroyImageAssets(IMAGE_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        al_destroy_bitmap(assets[i].Image);
    }
}

static void LoadFontAssets(FONT_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        if (assets[i].Filename && strlen(assets[i].Filename) > 0) {
            assets[i].Font = al_load_ttf_font(assets[i].Filename, assets[i].Size, ALLEGRO_TTF_MONOCHROME);
            if (!assets[i].Font) {
                eprintf("Failed to load font \"%s\"!\n", assets[i].Filename);
                LoadSuccess = false;
            }
        }
    }
}

static void DestroyFontAssets(FONT_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        al_destroy_font(assets[i].Font);
    }
}

bool LoadAssets(void) {
    LoadImageAssets(BackgroundAssets, N_BACKGROUND);
    LoadImageAssets(SpeciesAssets, N_SPECIES);
    LoadImageAssets(CostumeAssets, N_COSTUME);
    LoadImageAssets(WindowAssets, N_WINDOW);
    LoadImageAssets(AilmentAssets, N_AILMENT);
    LoadImageAssets(TypeAssets, N_TYPE);
    LoadImageAssets(MapAssets, N_MAP);
    LoadFontAssets(FontAssets, N_FONT);
    return LoadSuccess;
}

void DestroyAssets(void) {
    DestroyImageAssets(BackgroundAssets, N_BACKGROUND);
    DestroyImageAssets(SpeciesAssets, N_SPECIES);
    DestroyImageAssets(CostumeAssets, N_COSTUME);
    DestroyImageAssets(WindowAssets, N_WINDOW);
    DestroyImageAssets(AilmentAssets, N_AILMENT);
    DestroyImageAssets(TypeAssets, N_TYPE);
    DestroyImageAssets(MapAssets, N_MAP);
    DestroyFontAssets(FontAssets, N_FONT);
}

ALLEGRO_BITMAP *BackgroundImage(BACKGROUND_ID id) {
    return BackgroundAssets[id].Image;
}

ALLEGRO_BITMAP *SpeciesImage(SPECIES_ID id) {
    return SpeciesAssets[id].Image;
}

ALLEGRO_BITMAP *CostumeImage(COSTUME_ID id) {
    return CostumeAssets[id].Image;
}

ALLEGRO_BITMAP *WindowImage(WINDOW_ID id) {
    return WindowAssets[id].Image;
}

ALLEGRO_BITMAP *AilmentImage(AILMENT_ID id) {
    return AilmentAssets[id].Image;
}

ALLEGRO_BITMAP *TypeImage(TYPE_ID id) {
    return TypeAssets[id].Image;
}

ALLEGRO_BITMAP *MapImage(MAP_ID id) {
    return MapAssets[id].Image;
}

ALLEGRO_FONT *Font(FONT_ID id) {
    return FontAssets[id].Font;
}