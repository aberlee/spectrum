/**********************************************************//**
 * @file location.h
 * @brief Lists all the locations and associated data in the
 * game's overworld and battle system.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _LOCATION_H_
#define _LOCATION_H_

#include "coordinate.h"         // COORDINATE, DIRECTION
#include "species.h"            // SPECIES_ID
#include "person.h"             // PERSON_ID

/**********************************************************//**
 * @enum LOCATION_ID
 * @brief Defines a constant for each LOCATION.
 **************************************************************/
typedef enum {
    // Special
    OVERWORLD                   = 255,
    
    // Sapling Town
    TRIANGLE_LAKE               = 1,
    SAPLING_TOWN,
    YOUR_HOUSE,
    SAPLING_AIRPORT,
    SAPLING_HOSPITAL,
    SAPLING_CITY_HALL,
    SAPLING_GREENHOUSE,
    SAPLING_LABORATORY,
    SAPLING_OUTSKIRTS,

    // Port Royal buildings
    PORT_ROYAL,
    ROYAL_HOSPITAL,
    ROYAL_WAREHOUSE,
    ROYAL_PORT,

    // Oxide Crater
    OXIDE_COAST,
    OXIDE_CRATER,
    OXIDE_CAVE,

    // Solar City
    SOLAR_CITY,
    SOLAR_AIRPORT,
    SOLAR_HOSPITAL,
    SOLAR_EAST_CORPORATION,
    SOLAR_WEST_CORPORATION,
    SOLAR_CITY_OUTSKIRTS,
    SOLAR_PORT,

    // Solar Institute
    SOLAR_INSTITUTE_1F,
    SOLAR_INSTITUTE_2F,
    SOLAR_INSTITUTE_3F,
    SOLAR_GENERATOR_ROOM,

    // New Land Cave
    NEW_LAND_CAVE_OUTSKIRTS,
    NEW_LAND_CAVE,

    // Falls Cave
    FALLS_AREA,
    ANDORA_REST_STOP,
    FALLS_CAVE_1F,
    FALLS_CAVE_B1F,

    // Andora Falls
    ANDORA_FALLS,
    ANDORA_HOSPITAL,
    ANDORA_PORT,

    // Granite Cave
    GRANITE_CAVE_1F,
    GRANITE_CAVE_B1F,

    // Granite City
    GRANITE_OUTSKIRTS,
    GRANITE_CITY_SUBURBS,
    GRANITE_CITY_DOWNTOWN,
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
    GRANITE_WAREHOUSE,

    // Granite Tower
    GRANITE_TOWER_1F,
    GRANITE_TOWER_2F,
    GRANITE_TOWER_3F,
    GRANITE_TOWER_4F,
    GRANITE_TOWER_5F,
    GRANITE_TOWER_LAVATORY,

    // Boulder Park
    BOULDER_CAVE,
    BOULDER_PARK,
} LOCATION_ID;

/// The number of unique LOCATION_ID members.
#define N_LOCATION (BOULDER_PARK+1)

/**********************************************************//**
 * @enum MAP_ID
 * @brief Defines a constant for each map image.
 **************************************************************/
typedef enum {
    // Overworld
    MAP_OVERWORLD               = 1,

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
    MAP_GRANITE_WAREHOUSE,
    MAP_GRANITE_TOWER_1F,
    MAP_GRANITE_TOWER_2F,
    MAP_GRANITE_TOWER_3F,
    MAP_GRANITE_TOWER_4F,
    MAP_GRANITE_TOWER_5F,
    MAP_LAVATORY,
} MAP_ID;

/// The number of unique MAP_ID members.
#define N_MAP (MAP_LAVATORY+1)

/**********************************************************//**
 * @enum BACKGROUND_ID
 * @brief Defines a constant for each screen background used
 * in the battle system (0 is invalid).
 **************************************************************/
typedef enum {
    CHARCOAL        = 1,
    GRASS,
    OXIDE,
    SAND,
    OCEAN,
    CAVE,
} BACKGROUND_ID;

/// The number of unique backgrounds.
#define N_BACKGROUND (CAVE+1)

/**********************************************************//**
 * @struct ENCOUNTER
 * @brief Data about one type of species that can be
 * encountered.
 **************************************************************/
typedef struct {
    int Chance;                 ///< Chance of the encounter.
    SPECIES_ID Spectra;         ///< SPECIES that can be encountered.
    int LevelRange[2];          ///< Level range of encounter {min, max}
} ENCOUNTER;

/**********************************************************//**
 * @enum TILE_FLAGS
 * @brief Flags that describe a tile (16*16 cell) on a map.
 * These can be combined with a bitwise-OR.
 **************************************************************/
typedef enum {
    TILE_SOLID      = 0x0001,   ///< Tile can't be walked through.
    TILE_WATER      = 0x0002,   ///< Tile can be fished in.
    TILE_EVENT      = 0x0004,   ///< Tile contains an indexed event.
} TILE_FLAGS;

/**********************************************************//**
 * @struct TILE
 * @brief Contains information about one 16*16 region on a
 * map. This is used by a SENSOR to control motion.
 **************************************************************/
typedef struct {
    TILE_FLAGS Flags;           ///< Describes the tile properties.
    int Argument;               ///< Indexed TILE_EVENT ID.
} TILE;

/**********************************************************//**
 * @struct SENSOR
 * @brief Contains all the tile information for a map.
 **************************************************************/
typedef struct {
    int Height;                 ///< Height in tiles of the map.
    int Width;                  ///< Width in tiles of the map.
    TILE *Sensor;               ///< All tile information.
} SENSOR;

/**********************************************************//**
 * @struct LOCATION
 * @brief Stores all information for a LOCATION.
 **************************************************************/
typedef struct {
    const char *Name;           ///< Name of the location.
    MAP_ID Map;                 ///< Map to display for the location.
    COORDINATE Bounds[2];       ///< Bounding box for the location.
    BACKGROUND_ID Background;   ///< Battle background used.
    ENCOUNTER *Encounters;      ///< Overworld SPECIES encounters.
    ENCOUNTER *Fishing;         ///< Fishing SPECIES encounters.
} LOCATION;

/**************************************************************/
extern const LOCATION *Location(LOCATION_ID id);
extern void InitializeLocation(void);
extern void Warp(LOCATION_ID id, int x, int y, DIRECTION direction);
extern void DrawMap(void);
extern void UpdateMap(void);

/**************************************************************/
#endif // _LOCATION_H_
