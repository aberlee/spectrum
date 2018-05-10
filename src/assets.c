/**********************************************************//**
 * @file assets.c
 * @brief Includes and loads all the game assets.
 * @author Rena Shinomiya
 * @date March 22nd, 2018
 **************************************************************/

#include <stddef.h>             // NULL
#include <stdbool.h>            // bool

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "assets.h"             // FONT_ID
#include "location.h"           // MAP_ID, BACKGROUND_ID
#include "player.h"             // COSTUME_ID
#include "species.h"            // SPECIES_ID, AILMENT_ID
#include "menu.h"               // WINDOW_ID

#include "debug.h"              // eprintf

/**********************************************************//**
 * @struct IMAGE_ASSET
 * @brief Stores an image's filename and the image data if
 * it's loaded into memory.
 **************************************************************/
typedef struct {
    const char *Filename;
    ALLEGRO_BITMAP *Image;
} IMAGE_ASSET;

/**********************************************************//**
 * @struct FONT_ASSET
 * @brief Stores a font's file and parameters, and the actual
 * font data if it's been loaded into memory.
 **************************************************************/
typedef struct {
    const char *Filename;
    int Size;
    ALLEGRO_FONT *Font;
} FONT_ASSET;

/**************************************************************/
/// @brief The path to the game's assets directory relative to
/// the executable.
#define DATA "data/"

/// @brief Initializes an IMAGE_ASSET with a standard path.
#define IMAGE(filename) {DATA "image/" filename, NULL}

/// @brief Initializes a FONT_ASSET with a standard path.
#define FONT(filename, size) {DATA "font/" filename, size, NULL}

/**********************************************************//**
 * @brief Indexes BACKGROUND_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET BackgroundAssets[] = {
    [CAVE]              = IMAGE("background/cave.png"),
    [GRASS]             = IMAGE("background/grass.png"),
    [OCEAN]             = IMAGE("background/ocean.png"),
    [OXIDE]             = IMAGE("background/oxide.png"),
    [SAND]              = IMAGE("background/sand.png"),
    [CHARCOAL]          = IMAGE("background/watercolor.png"),
};

/**********************************************************//**
 * @brief Indexes SPECIES_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET SpeciesAssets[] = {
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
    [TOTEM_POLE]        = IMAGE("monster/totempole.png"),
    [VACUUM]            = IMAGE("monster/vacuum.png"),
    [VOLTDRAGON]        = IMAGE("monster/voltdragon.png"),
    [WATERWING]         = IMAGE("monster/waterwing.png"),
};

/**********************************************************//**
 * @brief Indexes COSTUME_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET CostumeAssets[] = {
    [NORMAL]            = IMAGE("costume/normal.png"),
    [BLACK_DRESS]       = IMAGE("costume/black_dress.png"),
    [RED_DRESS]         = IMAGE("costume/red_dress.png"),
    [SUIT]              = IMAGE("costume/suit.png"),
    [SWIMSUIT]          = IMAGE("costume/swimsuit.png"),
    [PAJAMAS]           = IMAGE("costume/pajamas.png"),
};

/**********************************************************//**
 * @brief Indexes WINDOW_ID members to IMAGE_ASSET data.
 **************************************************************/
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
    [OUTPUT]            = IMAGE("window/output.png"),
    [POPUP_WINDOW]      = IMAGE("window/popup.png"),
    [POPUP_BAR]         = IMAGE("window/popup_bar.png"),
    [POPUP_DETAIL]      = IMAGE("window/popup_detail.png"),
};

/**********************************************************//**
 * @brief Indexes AILMENT_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET AilmentAssets[] = {
    [POISONED]          = IMAGE("window/ailment/poison.png"),
    [SHOCKED]           = IMAGE("window/ailment/shock.png"),
    [BURIED]            = IMAGE("window/ailment/buried.png"),
    [ASLEEP]            = IMAGE("window/ailment/asleep.png"),
    [AFLAME]            = IMAGE("window/ailment/aflame.png"),
};

/**********************************************************//**
 * @brief Indexes TYPE_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET TypeAssets[] = {
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

/**********************************************************//**
 * @brief Indexes MAP_ID members to IMAGE_ASSET data for the
 * overworld maps.
 **************************************************************/
static IMAGE_ASSET MapAssets[] = {
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
    [MAP_GRANITE_WAREHOUSE]     = IMAGE("map/granite_city/warehouse.png"),
    [MAP_GRANITE_TOWER_1F]      = IMAGE("map/granite_city/tower_1st_floor.png"),
    [MAP_GRANITE_TOWER_2F]      = IMAGE("map/granite_city/tower_2nd_floor.png"),
    [MAP_GRANITE_TOWER_3F]      = IMAGE("map/granite_city/tower_3rd_floor.png"),
    [MAP_GRANITE_TOWER_4F]      = IMAGE("map/granite_city/tower_4th_floor.png"),
    [MAP_GRANITE_TOWER_5F]      = IMAGE("map/granite_city/tower_5th_floor.png"),
    [MAP_LAVATORY]              = IMAGE("map/granite_city/tower_bathroom.png"),
};

/**********************************************************//**
 * @brief Indexes MAP_ID members to IMAGE_ASSET data for the
 * overworld map sensor data.
 **************************************************************/
static IMAGE_ASSET SensorAssets[] = {
    [MAP_OVERWORLD]             = IMAGE("sensor/kaido.png"),
    [MAP_BOULDER_CAVE]          = IMAGE("sensor/boulder_cave.png"),
    [MAP_FALLS_CAVE_1F]         = IMAGE("sensor/falls_cave_1st_floor.png"),
    [MAP_FALLS_CAVE_B1F]        = IMAGE("sensor/falls_cave_basement.png"),
    [MAP_GRANITE_CAVE_1F]       = IMAGE("sensor/granite_cave_1st_floor.png"),
    [MAP_GRANITE_CAVE_B1F]      = IMAGE("sensor/granite_cave_basement.png"),
    [MAP_NEW_LAND_CAVE]         = IMAGE("sensor/new_land_cave.png"),
    [MAP_OXIDE_CRATER]          = IMAGE("sensor/oxide_crater.png"),
    [MAP_SAPLING_YOUR_HOUSE]    = IMAGE("sensor/sapling_town/amy_house.png"),
    [MAP_SAPLING_AIRPORT]       = IMAGE("sensor/sapling_town/airport.png"),
    [MAP_SAPLING_HOSPITAL]      = IMAGE("sensor/sapling_town/hospital.png"),
    [MAP_SAPLING_CITY_HALL]     = IMAGE("sensor/sapling_town/city_hall.png"),
    [MAP_SAPLING_GREENHOUSE]    = IMAGE("sensor/sapling_town/greenhouse.png"),
    [MAP_SAPLING_LABORATORY]    = IMAGE("sensor/sapling_town/laboratory.png"),
    [MAP_ROYAL_HOSPITAL]        = IMAGE("sensor/port_royal/hospital.png"),
    [MAP_ROYAL_WAREHOUSE]       = IMAGE("sensor/port_royal/warehouse.png"),
    [MAP_ROYAL_PORT]            = IMAGE("sensor/port_royal/port.png"),
    [MAP_SOLAR_AIRPORT]         = IMAGE("sensor/solar_city/airport.png"),
    [MAP_SOLAR_HOSPITAL]        = IMAGE("sensor/solar_city/hospital.png"),
    [MAP_SOLAR_EAST_CORP]       = IMAGE("sensor/solar_city/corporation_east.png"),
    [MAP_SOLAR_WEST_CORP]       = IMAGE("sensor/solar_city/corporation_west.png"),
    [MAP_SOLAR_INSTITUTE_1F]    = IMAGE("sensor/solar_city/institute_1st_floor.png"),
    [MAP_SOLAR_INSTITUTE_2F]    = IMAGE("sensor/solar_city/institute_2nd_floor.png"),
    [MAP_SOLAR_INSTITUTE_3F]    = IMAGE("sensor/solar_city/institute_3rd_floor.png"),
    [MAP_GENERATOR_ROOM]        = IMAGE("sensor/solar_city/institute_generator_room.png"),
    [MAP_REST_STOP]             = IMAGE("sensor/andora_falls/rest_stop.png"),
    [MAP_ANDORA_HOSPITAL]       = IMAGE("sensor/andora_falls/hospital.png"),
    [MAP_ANDORA_PORT]           = IMAGE("sensor/andora_falls/port.png"),
    [MAP_GRANITE_AIRPORT]       = IMAGE("sensor/granite_city/airport.png"),
    [MAP_GRANITE_AIR_EAST]      = IMAGE("sensor/granite_city/air_tower_east.png"),
    [MAP_GRANITE_AIR_WEST]      = IMAGE("sensor/granite_city/air_tower_west.png"),
    [MAP_GRANITE_CORP]          = IMAGE("sensor/granite_city/corporation.png"),
    [MAP_GRANITE_DEPARTMENT]    = IMAGE("sensor/granite_city/department_store.png"),
    [MAP_GAME_DESIGNER_ROOM]    = IMAGE("sensor/granite_city/game_designer_room.png"),
    [MAP_GRANITE_HOSPITAL]      = IMAGE("sensor/granite_city/hospital.png"),
    [MAP_GRANITE_LIBRARY]       = IMAGE("sensor/granite_city/library.png"),
    [MAP_GRANITE_STORE_1]       = IMAGE("sensor/granite_city/store_1_through_5.png"),
    [MAP_GRANITE_STORE_2]       = IMAGE("sensor/granite_city/store_1_through_5.png"),
    [MAP_GRANITE_STORE_3]       = IMAGE("sensor/granite_city/store_1_through_5.png"),
    [MAP_GRANITE_STORE_4]       = IMAGE("sensor/granite_city/store_1_through_5.png"),
    [MAP_GRANITE_STORE_5]       = IMAGE("sensor/granite_city/store_1_through_5.png"),
    [MAP_GRANITE_STORE_6]       = IMAGE("sensor/granite_city/store_6.png"),
    [MAP_GRANITE_WAREHOUSE]     = IMAGE("sensor/granite_city/warehouse.png"),
    [MAP_GRANITE_TOWER_1F]      = IMAGE("sensor/granite_city/tower_1st_floor.png"),
    [MAP_GRANITE_TOWER_2F]      = IMAGE("sensor/granite_city/tower_2nd_floor.png"),
    [MAP_GRANITE_TOWER_3F]      = IMAGE("sensor/granite_city/tower_3rd_floor.png"),
    [MAP_GRANITE_TOWER_4F]      = IMAGE("sensor/granite_city/tower_4th_floor.png"),
    [MAP_GRANITE_TOWER_5F]      = IMAGE("sensor/granite_city/tower_5th_floor.png"),
    [MAP_LAVATORY]              = IMAGE("sensor/granite_city/tower_bathroom.png"),
};

/**********************************************************//**
 * @brief Indexes PERSON_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET PersonAssets[N_PERSON] = {
    [AMY_NORMAL]        = IMAGE("person/costume/normal.png"),
    [AMY_RED_DRESS]     = IMAGE("person/costume/red_dress.png"),
    [AMY_BLACK_DRESS]   = IMAGE("person/costume/black_dress.png"),
    // TODO
};
/**********************************************************//**
 * @brief Indexes MISC_ID members to IMAGE_ASSET data.
 **************************************************************/
static IMAGE_ASSET MiscAssets[] = {
    [GIFT_CLOSED]       = IMAGE("gift_closed.png"),
    [GIFT_OPEN]         = IMAGE("gift_open.png"),
    [HUD_OK]            = IMAGE("window/hud_ok.png"),
    [HUD_UP]            = IMAGE("window/hud_up.png"),
};

/**********************************************************//**
 * @brief Indexes FONT_ID members to FONT_ASSET data.
 **************************************************************/
static FONT_ASSET FontAssets[] = {
    [FONT_WINDOW]       = FONT("legacy/legacy.ttf", 10),
};

/**************************************************************/
/// @brief Indicates whether all the assets have been loaded
/// properly. Used as a return value.
static bool LoadSuccess = true;

/**********************************************************//**
 * @brief Loads an array of image assets onto the GPU.
 * @param assets: The array of asset data to load.
 * @param nAssets: Size of the array. 
 **************************************************************/
static void LoadImageAssets(IMAGE_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        // Only load if filename defined and image not loaded
        // already (pointer initialized to NULL).
        if (assets[i].Filename && assets[i].Filename[0] && !assets[i].Image) {
            assets[i].Image = al_load_bitmap(assets[i].Filename);
            if (!assets[i].Image) {
                eprintf("Failed to load image \"%s\"!\n", assets[i].Filename);
                LoadSuccess = false;
            }
        }
    }
}

/**********************************************************//**
 * @brief Removes an array of image assets from memory.
 * @param assets: The assets to get rid of.
 * @param nAssets: The number of assets.
 **************************************************************/
static void DestroyImageAssets(IMAGE_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        // Doesn't fail on NULL.
        al_destroy_bitmap(assets[i].Image);
    }
}

/**********************************************************//**
 * @brief Loads an array of font assets onto the GPU.
 * @param assets: Array of font assets.
 * @param nAssets: Size of the array.
 **************************************************************/
static void LoadFontAssets(FONT_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        // Only load if filename defined and font not loaded
        // already (pointer initialized to NULL).
        if (assets[i].Filename && assets[i].Filename[0] && !assets[i].Font) {
            assets[i].Font = al_load_ttf_font(assets[i].Filename, assets[i].Size, ALLEGRO_TTF_MONOCHROME);
            if (!assets[i].Font) {
                eprintf("Failed to load font \"%s\"!\n", assets[i].Filename);
                LoadSuccess = false;
            }
        }
    }
}

/**********************************************************//**
 * @brief Removes an array of font assets from memory.
 * @param assets: Array of font assets to get rid of.
 * @param nAssets: Size of the array
 **************************************************************/
static void DestroyFontAssets(FONT_ASSET *assets, int nAssets) {
    for (int i = 0; i < nAssets; i++) {
        // Doesn't fail on NULL
        al_destroy_font(assets[i].Font);
    }
}

/**********************************************************//**
 * @brief Loads all assets from the entire game based on the
 * static arrays defined in assets.c.
 * @return Whether the loading succeeded.
 **************************************************************/
bool LoadAssets(void) {
    LoadImageAssets(BackgroundAssets, N_BACKGROUND);
    LoadImageAssets(SpeciesAssets, N_SPECIES);
    LoadImageAssets(CostumeAssets, N_COSTUME);
    LoadImageAssets(WindowAssets, N_WINDOW);
    LoadImageAssets(AilmentAssets, N_AILMENT);
    LoadImageAssets(TypeAssets, N_TYPE);
    LoadImageAssets(MapAssets, N_MAP);
    LoadImageAssets(SensorAssets, N_MAP);
    LoadImageAssets(PersonAssets, N_PERSON);
    LoadImageAssets(MiscAssets, N_MISC);
    LoadFontAssets(FontAssets, N_FONT);
    return LoadSuccess;
}

/**********************************************************//**
 * @brief Removes all game assets from memory.
 **************************************************************/
void DestroyAssets(void) {
    DestroyImageAssets(BackgroundAssets, N_BACKGROUND);
    DestroyImageAssets(SpeciesAssets, N_SPECIES);
    DestroyImageAssets(CostumeAssets, N_COSTUME);
    DestroyImageAssets(WindowAssets, N_WINDOW);
    DestroyImageAssets(AilmentAssets, N_AILMENT);
    DestroyImageAssets(TypeAssets, N_TYPE);
    DestroyImageAssets(MapAssets, N_MAP);
    DestroyImageAssets(SensorAssets, N_MAP);
    DestroyImageAssets(PersonAssets, N_PERSON);
    DestroyImageAssets(MiscAssets, N_MISC);
    DestroyFontAssets(FontAssets, N_FONT);
}

/**********************************************************//**
 * @brief Gets a background image asset.
 * @param id: The identity of the background image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *BackgroundImage(BACKGROUND_ID id) {
    return BackgroundAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a species image asset.
 * @param id: The identity of the species image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *SpeciesImage(SPECIES_ID id) {
    return SpeciesAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a costume image asset.
 * @param id: The identity of the costume image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *CostumeImage(COSTUME_ID id) {
    return CostumeAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a window image asset.
 * @param id: The identity of the window image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *WindowImage(WINDOW_ID id) {
    return WindowAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a ailment image asset.
 * @param id: The identity of the ailment image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *AilmentImage(AILMENT_ID id) {
    return AilmentAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a type image asset.
 * @param id: The identity of the type image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *TypeImage(TYPE_ID id) {
    return TypeAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a map image asset.
 * @param id: The identity of the map image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *MapImage(MAP_ID id) {
    return MapAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a sensor image asset.
 * @param id: The identity of the sensor image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *SensorImage(MAP_ID id) {
    return SensorAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a person image asset.
 * @param id: The identity of the person image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *PersonImage(PERSON_ID id) {
    return PersonAssets[id].Image;
}
/**********************************************************//**
 * @brief Gets a misc image asset.
 * @param id: The identity of the misc image.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_BITMAP *MiscImage(MISC_ID id) {
    return MiscAssets[id].Image;
}

/**********************************************************//**
 * @brief Gets a font asset.
 * @param id: The identity of the font asset.
 * @return Pointer to the ALLEGRO_BITMAP data.
 **************************************************************/
ALLEGRO_FONT *Font(FONT_ID id) {
    return FontAssets[id].Font;
}
