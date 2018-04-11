/**********************************************************//**
 * @file event.i
 * @brief Database for map events.
 * @author Rena Shinomiya
 * @date April 9th, 2018
 **************************************************************/

#ifndef _EVENT_I_
#define _EVENT_I_

#include "event.h"          // EVENT, EVENT_TYPE
#include "location.h"       // MAP_ID
#include "shop.h"           // SHOP_ID
#include "species.h"        // SPECIES_ID

#define WARP(id, x, y) {EVENT_WARP, {.Warp={{x, y}, id}}}
#define TEXT(text) {EVENT_TEXT, {.Text=text}}
#define SHOP(id) {EVENT_SHOP, {.Shop=id}}
#define BOSS(spectra, level) {EVENT_BOSS, {.Boss={spectra, level}}}
#define REDIRECT(event) {EVENT_REDIRECT, {.Redirect=event}}
#define UNDEFINED {0}
#define HOUSE TEXT("You can't go in other people's houses!")

/**********************************************************//**
 * @var EVENT_DATA
 * @brief Stores all events that occur on maps.
 **************************************************************/
static const EVENT *(EVENT_DATA[]) = {
    // Overworld
    [MAP_OVERWORLD] = (EVENT[]){
        // Sapling Town and Triangle Lake
        [  1] = WARP(SAPLING_AIRPORT, 1, 6),
        [  2] = WARP(YOUR_HOUSE, 5, 6),
        [  3] = HOUSE,
        [  4] = WARP(SAPLING_HOSPITAL, 4, 6),
        [  5] = HOUSE,
        [  6] = WARP(SAPLING_GREENHOUSE, 10, 8),
        [  7] = HOUSE,
        [  8] = HOUSE,
        [  9] = WARP(SAPLING_CITY_HALL, 2, 8),
        [ 10] = WARP(SAPLING_CITY_HALL, 11, 8),
        [ 13] = TEXT(""),
        [ 20] = TEXT(""),
        [ 21] = TEXT(""),
        [ 22] = WARP(SAPLING_LABORATORY, 4, 7),
        
        // Port Royal
        [ 11] = WARP(ROYAL_HOSPITAL, 4, 6),
        [ 12] = TEXT(""),
        [ 14] = WARP(ROYAL_WAREHOUSE, 1, 7),
        [ 15] = WARP(ROYAL_WAREHOUSE, 9, 7),
        [ 16] = HOUSE,
        [ 17] = HOUSE,
        [ 18] = HOUSE,
        [ 19] = TEXT(""),
        [ 25] = HOUSE,
        [ 26] = HOUSE,
        
        // Oxide Crater
        [ 23] = WARP(OXIDE_CAVE, 18, 17),
        [ 24] = WARP(OXIDE_CAVE, 30, 20),
        
        // Royal Port
        [ 40] = TEXT(""),
        [ 41] = TEXT(""),
        [ 42] = TEXT(""),
        [ 43] = WARP(ROYAL_PORT, 1, 6),
        [ 44] = WARP(ROYAL_PORT, 8, 6),
        
        // Solar City
        [ 27] = HOUSE,
        [ 28] = HOUSE,
        [ 29] = HOUSE,
        [ 30] = HOUSE,
        [ 31] = WARP(SOLAR_HOSPITAL, 4, 6),
        [ 32] = WARP(SOLAR_WEST_CORPORATION, 3, 9),
        [ 33] = HOUSE,
        [ 34] = HOUSE,
        [ 35] = HOUSE,
        [ 36] = WARP(SOLAR_EAST_CORPORATION, 6, 9),
        [ 37] = WARP(SOLAR_AIRPORT, 2, 8),
        [ 38] = REDIRECT(37),
        [ 39] = REDIRECT(37),
        
        // Solar Institute
        [ 46] = WARP(SOLAR_INSTITUTE_1F, 5, 9),
        [ 47] = REDIRECT(46),
        [ 49] = REDIRECT(46),
        [ 50] = REDIRECT(46),
        [ 51] = REDIRECT(46),
        [ 52] = REDIRECT(46),
        [ 53] = TEXT(""),
        [ 54] = WARP(SOLAR_GENERATOR_ROOM, 3, 6),
        [ 55] = TEXT(""),
        
        // Falls Area
        [ 59] = WARP(FALLS_CAVE_1F, 5, 15),
        [ 60] = WARP(FALLS_CAVE_1F, 24, 17),
        [ 64] = WARP(ANDORA_REST_STOP, 2, 6),
        [ 65] = TEXT(""),
        [ 66] = WARP(NEW_LAND_CAVE, 54, 12),
        [ 67] = WARP(NEW_LAND_CAVE, 41, 20),
        [ 68] = WARP(NEW_LAND_CAVE, 8, 16),
        
        // Andora Falls
        [ 48] = TEXT(""),
        [ 56] = WARP(ANDORA_PORT, 8, 4),
        [ 57] = WARP(ANDORA_PORT, 1, 6),
        [ 58] = WARP(ANDORA_HOSPITAL, 5, 6),
        [ 61] = HOUSE,
        [ 62] = HOUSE,
        [ 63] = HOUSE,
        [ 69] = HOUSE,
        
        // Granite Cave
        [ 70] = WARP(GRANITE_CAVE_1F, 20, 3),
        [ 71] = WARP(GRANITE_CAVE_1F, 35, 13),
        [ 72] = WARP(GRANITE_CAVE_1F, 14, 17),
        
        
        // Granite City Downtown
        [ 73] = WARP(GRANITE_TOWER_1F, 9, 12),
        [ 74] = REDIRECT(73),
        [ 75] = REDIRECT(73),
        [ 76] = REDIRECT(73),
        [ 77] = REDIRECT(73),
        [ 78] = REDIRECT(73),
        [ 79] = REDIRECT(73),
        [ 80] = REDIRECT(73),
        [ 81] = REDIRECT(73),
        [ 82] = WARP(GRANITE_STORE_1, 4, 6),
        [ 83] = WARP(GRANITE_STORE_2, 4, 6),
        [ 84] = WARP(GRANITE_STORE_3, 4, 6),
        [ 86] = WARP(GRANITE_DEPARTMENT_STORE, 6, 12),
        [ 87] = REDIRECT(86),
        [ 88] = REDIRECT(86),
        [ 89] = WARP(GRANITE_WAREHOUSE, 1, 7),
        [ 90] = WARP(GRANITE_WAREHOUSE, 7, 7),
        [ 91] = WARP(GRANITE_WAREHOUSE, 12, 7),
        [ 93] = WARP(GRANITE_STORE_4, 4, 6),
        [ 94] = WARP(GRANITE_STORE_5, 4, 6),
        [ 95] = WARP(GRANITE_STORE_6, 4, 6),
        [ 96] = HOUSE,
        [ 97] = TEXT(""),
        [ 99] = WARP(GRANITE_CORPORATION, 1, 4),
        [105] = WARP(GRANITE_CORPORATION, 7, 9),
        [100] = WARP(GRANITE_HOSPITAL, 13, 6),
        [102] = WARP(GRANITE_HOSPITAL, 1, 11),
        [103] = WARP(GRANITE_HOSPITAL, 8, 11),
        [104] = WARP(GRANITE_LIBRARY, 5, 8),
        [116] = WARP(GRANITE_AIRPORT, 1, 8),
        [117] = REDIRECT(116),
        [118] = WARP(GRANITE_AIRPORT, 10, 8),
        [119] = REDIRECT(118),
        [120] = WARP(GRANITE_AIR_TOWER_WEST, 2, 5),
        [121] = WARP(GRANITE_AIR_TOWER_EAST, 2, 5),
        
        // Granite City Suburbs
        [101] = HOUSE,
        [106] = HOUSE,
        [107] = HOUSE,
        [108] = TEXT(""),
        [109] = HOUSE,
        [110] = HOUSE,
        [111] = HOUSE,
        [112] = HOUSE,
        [113] = HOUSE,
        [114] = HOUSE,
        [115] = HOUSE,
        [122] = HOUSE,
        [123] = HOUSE,
        [124] = HOUSE,
        [125] = HOUSE,
        [126] = HOUSE,
        [127] = HOUSE,
        [128] = HOUSE,
        
        // Boulder Park
        [ 85] = TEXT(""),
        [ 92] = WARP(BOULDER_CAVE, 20, 5),
        [ 98] = WARP(BOULDER_CAVE, 13, 17),
    },
    
    // Boulder Cave
    [MAP_BOULDER_CAVE] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 59, 220),         // Boulder Park exit
        [  2] = WARP(OVERWORLD, 52, 232),         // Granite City exit
    },
    
    // Falls Cave
    [MAP_FALLS_CAVE_1F] = (EVENT[]){
        [  1] = WARP(FALLS_CAVE_B1F, 12, 9),      // Basement left
        [  2] = WARP(FALLS_CAVE_B1F, 22, 11),     // Basement right
        [  3] = TEXT(""),                         // Sign left
        [  4] = TEXT(""),                         // Sign right
        [  5] = WARP(OVERWORLD, 30, 155),         // Andora Falls exit
        [  6] = WARP(OVERWORLD, 49, 157),         // Falls Area exit
    },
    [MAP_FALLS_CAVE_B1F] = (EVENT[]){
        [  1] = WARP(FALLS_CAVE_1F, 11, 10),      // 1F left
        [  2] = WARP(FALLS_CAVE_1F, 21, 12),      // 1F right
    },
    
    // Granite Cave
    [MAP_GRANITE_CAVE_1F] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 28, 187),         // Andora Falls
        [  2] = WARP(GRANITE_CAVE_B1F, 22, 39),   // Basement
        [  3] = TEXT(""),                         // Basement sign
        [  4] = TEXT(""),                         // Boulder Park exit sign
        [  5] = TEXT(""),                         // Center sign
        [  6] = WARP(OVERWORLD, 43, 197),         // Boulder Park exit
        [  7] = WARP(OVERWORLD, 22, 200),         // Granite City exit
    },
    [MAP_GRANITE_CAVE_B1F] = (EVENT[]){
        [  1] = TEXT(""),                         // Sign
        [  2] = WARP(GRANITE_CAVE_1F, 20, 10),    // Exit
    },
    
    // New Land Cave
    [MAP_NEW_LAND_CAVE] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = WARP(OVERWORLD, 127, 164),        // Solar City
        [  5] = TEXT(""),
        [  6] = WARP(OVERWORLD, 82, 168),         // Falls Area middle
        [  7] = WARP(OVERWORLD, 114, 172),        // Falls Area right
    },
    
    // Oxide Cave
    [MAP_OXIDE_CRATER] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = WARP(OVERWORLD, 61, 65),          // Crater exit
        [  3] = WARP(OVERWORLD, 73, 67),          // Enterance
        [  4] = TEXT(""),
    },

    // Sapling Town buildings
    [MAP_SAPLING_YOUR_HOUSE] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = WARP(OVERWORLD, 136, 12),
    },
    [MAP_SAPLING_AIRPORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 110, 8),
    },
    [MAP_SAPLING_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = WARP(OVERWORLD, 84, 15),
    },
    [MAP_SAPLING_CITY_HALL] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = WARP(OVERWORLD, 92, 23),
        [  4] = WARP(OVERWORLD, 96, 23),
    },
    [MAP_SAPLING_GREENHOUSE] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = WARP(OVERWORLD, 104, 16),
    },
    [MAP_SAPLING_LABORATORY] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 112, 62),
    },

    // Port Royal buildings
    [MAP_ROYAL_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = WARP(OVERWORLD, 27, 30),
    },
    [MAP_ROYAL_WAREHOUSE] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 22, 36),
        [  2] = WARP(OVERWORLD, 25, 36),
    },
    [MAP_ROYAL_PORT] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = WARP(OVERWORLD, 65, 117),
        [  5] = WARP(OVERWORLD, 69, 117),
    },

    // Solar City buildings
    [MAP_SOLAR_AIRPORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 117, 104),
    },
    [MAP_SOLAR_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = WARP(OVERWORLD, 111, 86),
    },
    [MAP_SOLAR_EAST_CORP] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 131, 91),
    },
    [MAP_SOLAR_WEST_CORP] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 125, 86),
    },
    
    // Solar Institute
    [MAP_SOLAR_INSTITUTE_1F] = (EVENT[]){
        [  1] = WARP(SOLAR_INSTITUTE_3F, 13, 3),
        [  2] = WARP(OVERWORLD, 104, 128),
    },
    [MAP_SOLAR_INSTITUTE_2F] = (EVENT[]){
        [  1] = WARP(SOLAR_INSTITUTE_3F, 13, 3),
        [  2] = WARP(SOLAR_INSTITUTE_1F, 13, 3),
    },
    [MAP_SOLAR_INSTITUTE_3F] = (EVENT[]){
        [  1] = WARP(SOLAR_INSTITUTE_2F, 13, 3),
    },
    [MAP_GENERATOR_ROOM] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 112, 133),
    },

    // Andora Falls buildings
    [MAP_REST_STOP] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 90, 161),
    },
    [MAP_ANDORA_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = WARP(OVERWORLD, 20, 150),
    },
    [MAP_ANDORA_PORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 13, 148),
        [  2] = WARP(OVERWORLD, 9, 150),
    },

    // Granite City buildings
    [MAP_GRANITE_AIRPORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 97, 254),
        [  2] = WARP(OVERWORLD, 109, 254),
    },
    [MAP_GRANITE_AIR_EAST] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 121, 255),
    },
    [MAP_GRANITE_AIR_WEST] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 117, 255),
    },
    [MAP_GRANITE_CORP] = (EVENT[]){
        [  1] = WARP(GAME_DESIGNER_ROOM, 4, 3),
        [  2] = WARP(OVERWORLD, 129, 234),
        [  3] = WARP(OVERWORLD, 132, 240),
    },
    [MAP_GRANITE_DEPARTMENT] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = WARP(OVERWORLD, 117, 213),
    },
    [MAP_GAME_DESIGNER_ROOM] = (EVENT[]){
        [  1] = WARP(GRANITE_CORPORATION, 4, 3),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
    },
    [MAP_GRANITE_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = WARP(OVERWORLD, 94, 238),
        [  5] = WARP(OVERWORLD, 88, 240),
        [  6] = WARP(OVERWORLD, 90, 240),
    },
    [MAP_GRANITE_LIBRARY] = (EVENT[]){
        [  1] = TEXT(""),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = TEXT(""),
        [  5] = TEXT(""),
        [  6] = TEXT(""),
        [  7] = TEXT(""),
        [  8] = TEXT(""),
        [  9] = TEXT(""),
        [ 10] = WARP(OVERWORLD, 109, 240),
    },
    [MAP_GRANITE_STORE_1] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 89, 209),
    },
    [MAP_GRANITE_STORE_2] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 95, 209),
    },
    [MAP_GRANITE_STORE_3] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 102, 209),
    },
    [MAP_GRANITE_STORE_4] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 88, 225),
    },
    [MAP_GRANITE_STORE_5] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 94, 225),
    },
    [MAP_GRANITE_STORE_6] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 99, 225),
    },
    [MAP_GRANITE_WAREHOUSE] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 87, 218),
        [  2] = WARP(OVERWORLD, 94, 218),
        [  3] = WARP(OVERWORLD, 99, 218),
    },
    
    // Granite Tower
    [MAP_GRANITE_TOWER_1F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_2F, 16, 3),
        [  2] = WARP(OVERWORLD, 99, 201), // TODO this is weird.
    },
    [MAP_GRANITE_TOWER_2F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_3F, 1, 3),
        [  2] = WARP(GRANITE_TOWER_LAVATORY, 5, 7),
        [  3] = WARP(GRANITE_TOWER_1F, 16, 3),
        [  4] = TEXT(""),
        [  5] = TEXT(""),
    },
    [MAP_GRANITE_TOWER_3F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_2F, 1, 3),
        [  2] = TEXT(""),
        [  3] = TEXT(""),
        [  4] = TEXT(""),
        [  5] = TEXT(""),
        [  6] = TEXT(""),
        [  7] = WARP(GRANITE_TOWER_4F, 16, 3),
    },
    [MAP_GRANITE_TOWER_4F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_5F, 16, 3),
        [  2] = WARP(GRANITE_TOWER_3F, 16, 3),
    },
    [MAP_GRANITE_TOWER_5F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_4F, 1, 3),
        [  2] = TEXT(""),
    },
    [MAP_LAVATORY] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_2F, 9, 3),
    },
};

/**************************************************************/
#undef WARP
#undef TEXT
#undef SHOP
#undef BOSS
#undef REDIRECT
#undef HOUSE
#undef UNDEFINED

#endif // _EVENT_I_
