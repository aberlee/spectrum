/**********************************************************//**
 * @file location.h
 * @brief Lists all the locations and associated data in the
 * game's overworld and battle system.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _LOCATION_H_
#define _LOCATION_H_

#include "species.h"        // SPECIES_ID
#include "item.h"           // ITEM_ID
#include "coordinate.h"     // COORDINATE
#include "npc.h"            // NPC

/**********************************************************//**
 * @enum BACKGROUND_ID
 * @brief Defines a constant for each screen background used
 * in the battle system (0 is invalid).
 **************************************************************/
typedef enum {
    CHARCOAL=1,     ///< Basic black-and-white background.
    GRASS,          ///< Green grass.
    OXIDE,          ///< Rusty red.
    SAND,           ///< A sandy beach.
    OCEAN,          ///< Blue ocean waves.
    CAVE,           ///< A dark cave.
} BACKGROUND_ID;

/// The number of unique backgrounds.
#define N_BACKGROUND (CAVE+1)

/**********************************************************//**
 * @enum LOCATION_ID
 * @brief Defines a constant for each location the player can
 * visit.
 **************************************************************/
typedef enum {
    // Outskirts
    TRIANGLE_LAKE=1,
    OXIDE_COAST,
    OXIDE_CRATER,
    SOLAR_CITY_OUTSKIRTS,
    NEW_LAND_CAVE_OUTSKIRTS,
    FALLS_AREA,
    GRANITE_OUTSKIRTS,
    BOULDER_PARK,
    
    // Cities and towns
    SAPLING_TOWN,
    PORT_ROYAL,
    SOLAR_CITY,
    ANDORA_FALLS,
    GRANITE_CITY_SUBURBS,
    GRANITE_CITY_DOWNTOWN,
    
    // Caves
    OXIDE_CAVE,
    NEW_LAND_CAVE,
    FALLS_CAVE_1F,
    FALLS_CAVE_B1F,
    GRANITE_CAVE_1F,
    GRANITE_CAVE_B1F,
    BOULDER_CAVE,
    
    // Sapling town buildings
    YOUR_HOUSE,
    SAPLING_AIRPORT,
    SAPLING_HOSPITAL,
    SAPLING_CITY_HALL,
    SAPLING_GREENHOUSE,
    SAPLING_LABORATORY,
    
    // Port Royal buildings
    ROYAL_HOSPITAL,
    ROYAL_WAREHOUSE,
    ROYAL_PORT,
    
    // Solar City buildings
    SOLAR_AIRPORT,
    SOLAR_HOSPITAL,
    SOLAR_EAST_CORPORATION,
    SOLAR_WEST_CORPORATION,
    SOLAR_INSTITUTE_1F,
    SOLAR_INSTITUTE_2F,
    SOLAR_INSTITUTE_3F,
    SOLAR_GENERATOR_ROOM,
    
    // Andora Falls buildings
    ANDORA_REST_STOP,
    ANDORA_HOSPITAL,
    ANDORA_PORT,
    
    // Granite City buildings
    GRANITE_AIRPORT,
    GRANITE_AIR_TOWER_EAST,
    GRANITE_AIR_TOWER_WEST,
    GRANITE_CORPORATION,
    GRANITE_DEPARTMENT_STORE,
    GAME_DESIGNER_ROOM,
    GRANITE_HOSPITAL,
    GRANITE_LIBRARY,
    GRANITE_STORE_1,
    GRANITE_STORE_2,
    GRANITE_STORE_3,
    GRANITE_STORE_4,
    GRANITE_STORE_5,
    GRANITE_STORE_6,
    GRANITE_TOWER_1F,
    GRANITE_TOWER_2F,
    GRANITE_TOWER_3F,
    GRANITE_TOWER_4F,
    GRANITE_TOWER_5F,
    GRANITE_TOWER_LAVATORY,
} LOCATION_ID;

#define N_LOCATION (GRANITE_TOWER_LAVATORY+1)

/**********************************************************//**
 * @enum MAP_ID
 * @brief Defines a constant for each map image.
 **************************************************************/
typedef enum {
    MAP_OVERWORLD=1,
    
    // Caves
    MAP_BOULDER_CAVE,
    MAP_FALLS_CAVE_1F,
    MAP_FALLS_CAVE_B1F,
    MAP_GRANITE_CAVE_1F,
    MAP_GRANITE_CAVE_B1F,
    MAP_NEW_LAND_CAVE,
    MAP_OXIDE_CRATER,
    
    // Sapling Town buildings
    MAP_SAPLING_YOUR_HOUSE,
    MAP_SAPLING_AIRPORT,
    MAP_SAPLING_HOSPITAL,
    MAP_SAPLING_CITY_HALL,
    MAP_SAPLING_GREENHOUSE,
    MAP_SAPLING_LABORATORY,
    
    // Port Royal buildings
    MAP_ROYAL_HOSPITAL,
    MAP_ROYAL_WAREHOUSE,
    MAP_ROYAL_PORT,
    
    // Solar City buildings
    MAP_SOLAR_AIRPORT,
    MAP_SOLAR_HOSPITAL,
    MAP_SOLAR_EAST_CORP,
    MAP_SOLAR_WEST_CORP,
    MAP_SOLAR_INSTITUTE_1F,
    MAP_SOLAR_INSTITUTE_2F,
    MAP_SOLAR_INSTITUTE_3F,
    MAP_GENERATOR_ROOM,
    
    // Andora Falls buildings
    MAP_REST_STOP,
    MAP_ANDORA_HOSPITAL,
    MAP_ANDORA_PORT,
    
    // Granite City buildings
    MAP_GRANITE_AIRPORT,
    MAP_GRANITE_AIR_EAST,
    MAP_GRANITE_AIR_WEST,
    MAP_GRANITE_CORP,
    MAP_GRANITE_DEPARTMENT,
    MAP_GAME_DESIGNER_ROOM,
    MAP_GRANITE_HOSPITAL,
    MAP_GRANITE_LIBRARY,
    MAP_GRANITE_STORE_1,
    MAP_GRANITE_STORE_2,
    MAP_GRANITE_STORE_3,
    MAP_GRANITE_STORE_4,
    MAP_GRANITE_STORE_5,
    MAP_GRANITE_STORE_6,
    MAP_GRANITE_TOWER_1F,
    MAP_GRANITE_TOWER_2F,
    MAP_GRANITE_TOWER_3F,
    MAP_GRANITE_TOWER_4F,
    MAP_GRANITE_TOWER_5F,
    MAP_LAVATORY,
} MAP_ID;

#define N_MAP (MAP_LAVATORY+1)

/**********************************************************//**
 * @enum ENCOUNTER
 * @brief Data about one type of species that can be
 * encountered.
 **************************************************************/
typedef struct {
    int Chance;
    SPECIES_ID Spectra;
    int LevelRange[2];
} ENCOUNTER;

typedef struct {
    COORDINATE Where;
    COORDINATE Destination;
    LOCATION_ID Location;
} WARP;

typedef struct {
    COORDINATE Where;
    const char *Text;
} SIGN;

typedef struct {
    SPECIES_ID Spectra[3];
    int Level;
} BOSS;

typedef struct {
    char Name[32];
    MAP_ID Map;
    COORDINATE Bounds[2];
    BACKGROUND_ID Background;
    ENCOUNTER Encounters[8];
    ENCOUNTER FishingEncounters[4];
} LOCATION;

#endif