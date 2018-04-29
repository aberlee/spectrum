/**********************************************************//**
 * @file location.h
 * @brief Defines LOCATION_DATA.
 * @author Rena Shinomiya
 * @date March 26th, 2018
 **************************************************************/

#ifndef _LOCATION_I_
#define _LOCATION_I_

#include "location.h"           // LOCATION, LOCATION_ID

/**********************************************************//**
 * @var LOCATION_DATA
 * @brief Defines constant LOCATION data for each LOCATION_ID.
 **************************************************************/
const LOCATION LOCATION_DATA[] = {

    ////////////////////////////////////////////////////////
    // Sapling Town and Triangle Lake area
    [TRIANGLE_LAKE] = {
        .Name           = "Triangle Lake",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{1312, 560}, {2320, 1184}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {50, JAYRAPTOR,     { 4,  8}},
            {25, FLOPJELLY,     { 3,  6}},
            {19, SSSNAKE,       { 3,  6}},
            { 5, HOCUS,         { 5,  5}},
            { 1, JAYRAPTOR,     {12, 12}},
        },
        .Fishing        = (ENCOUNTER[]){
            {90, CATFISH,       { 4,  8}},
            { 9, CATFISH,       { 8, 15}},
            { 1, NESSIE,        {10, 10}}
        },
    },

    [SAPLING_TOWN] = {
        .Name           = "Sapling Town",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{1072, 0}, {2320, 560}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {50, JAYRAPTOR,     { 4,  6}},
            {25, SSSNAKE,       { 2,  5}},
            {25, FLOPJELLY,     { 2,  5}},
        },
        .Fishing        = (ENCOUNTER[]){
            {90, CATFISH,       { 4,  8}},
            { 9, CATFISH,       { 8, 15}},
            { 1, NESSIE,        {10, 10}},
        },
    },

    [YOUR_HOUSE] = {
        .Name           = "Amy's House",
        .Map            = MAP_SAPLING_YOUR_HOUSE,
        .Bounds         = {{0, 0}, {192, 112}},
    },

    [SAPLING_AIRPORT] = {
        .Name           = "Sapling Town",
        .Map            = MAP_SAPLING_AIRPORT,
        .Bounds         = {{0, 0}, {112, 112}},
    },

    [SAPLING_HOSPITAL] = {
        .Name           = "Sapling Town",
        .Map            = MAP_SAPLING_HOSPITAL,
        .Bounds         = {{0, 0}, {192, 112}},
    },

    [SAPLING_CITY_HALL] = {
        .Name           = "Sapling Town",
        .Map            = MAP_SAPLING_CITY_HALL,
        .Bounds         = {{0, 0}, {224, 144}},
    },

    [SAPLING_GREENHOUSE] = {
        .Name           = "Sapling Town",
        .Map            = MAP_SAPLING_GREENHOUSE,
        .Bounds         = {{0, 0}, {208, 144}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {99, SSSNAKE,       { 4,  6}},
            { 1, SPACESNAKE,    { 8,  8}},
        },
        .Fishing        = NULL,
    },

    [SAPLING_LABORATORY] = {
        .Name           = "Triangle Lake",
        .Map            = MAP_SAPLING_LABORATORY,
        .Bounds         = {{0, 0}, {128, 128}},
    },
    
    [SAPLING_OUTSKIRTS] = {
        .Name           = "Sapling Outskirts",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{816, 560}, {1312, 912}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {50, JAYRAPTOR,     { 6,  8}},
            {25, FLOPJELLY,     { 5,  6}},
            {20, SSSNAKE,       { 5,  6}},
            { 5, HOCUS,         { 6,  6}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Port Royal area
    [PORT_ROYAL] = {
        .Name           = "Port Royal",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{0, 0}, {896, 1296}},
        .Background     = SAND,
        .Encounters     = (ENCOUNTER[]){
            {50, JAYRAPTOR,     { 7,  9}},
            {49, FLOPJELLY,     { 5,  8}},
            { 1, GASMOG,        { 5,  8}},
        },
        .Fishing        = (ENCOUNTER[]){
            {60, CATFISH,       { 5,  8}},
            {30, GARPIKE,       { 7,  9}},
            { 9, GARPIKE,       {10, 16}},
            { 1, MINESPHERE,    {11, 11}},
        },
    },

    [ROYAL_HOSPITAL] = {
        .Name           = "Port Royal",
        .Map            = MAP_ROYAL_HOSPITAL,
        .Bounds         = {{0, 0}, {192, 112}},
    },

    [ROYAL_WAREHOUSE] = {
        .Name           = "Port Royal",
        .Map            = MAP_ROYAL_WAREHOUSE,
        .Bounds         = {{0, 0}, {176, 128}},
    },

    [ROYAL_PORT] = {
        .Name           = "Port Royal",
        .Map            = MAP_ROYAL_PORT,
        .Bounds         = {{0, 0}, {160, 112}},
    },

    ////////////////////////////////////////////////////////
    // Oxide Crater area
    [OXIDE_COAST] = {
        .Name           = "Oxide Crater Outskirts",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{0, 1296}, {1392, 1808}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {40, JAYRAPTOR,     { 9, 12}},
            {30, FLOPJELLY,     { 8, 12}},
            {20, NITROBOMB,     { 8, 12}},
        },
        .Fishing        = (ENCOUNTER[]){
            {50, CATFISH,       { 8, 12}},
            {40, GARPIKE,       { 9, 15}},
            { 9, COALSHARK,     {12, 16}},
            { 1, MINESPHERE,    {17, 17}},
        },
    },

    [OXIDE_CRATER] = {
        .Name           = "Oxide Crater",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{816, 912}, {1312, 1296}},
        .Background     = OXIDE,
        .Encounters     = (ENCOUNTER[]){
            {40, JAYRAPTOR,     { 9, 12}},
            {30, ASTEROID,      {10, 14}},
            {20, NITROBOMB,     { 8, 12}},
            { 9, BUTCHERBIRD,   {15, 15}},
            { 1, PUZZLE,        {12, 12}},
        },
        .Fishing        = NULL,
    },

    [OXIDE_CAVE] = {
        .Name           = "Oxide Crater",
        .Map            = MAP_OXIDE_CRATER,
        .Bounds         = {{0, 0}, {768, 640}},
        .Background     = OXIDE,
        .Encounters     = (ENCOUNTER[]){
            {40, ASTEROID,      {10, 18}},
            {30, NITROBOMB,     { 8, 12}},
            {20, BUTCHERBIRD,   {11, 16}},
            { 9, MEGATAR,       {16, 18}},
            { 1, PUZZLE,        {16, 16}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Solar City area
    [SOLAR_CITY] = {
        .Name           = "Solar City",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{1392, 1184}, {2320, 1872}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {40, OSCILLATE,     { 8, 12}},
            {40, GASMOG,        { 8, 13}},
            {19, LEKTRON,       { 5, 12}},
            { 1, CUMULUS,       {12, 12}},
        },
        .Fishing        = (ENCOUNTER[]){
            {50, CATFISH,       { 8, 12}},
            {49, GARPIKE,       { 9, 15}},
            { 1, MINESPHERE,    {17, 17}},
        },
    },

    [SOLAR_AIRPORT] = {
        .Name           = "Solar City",
        .Map            = MAP_SOLAR_AIRPORT,
        .Bounds         = {{0, 0}, {192, 144}},
    },

    [SOLAR_HOSPITAL] = {
        .Name           = "Solar City",
        .Map            = MAP_SOLAR_HOSPITAL,
        .Bounds         = {{0, 0}, {192, 112}},
    },

    [SOLAR_EAST_CORPORATION] = {
        .Name           = "Solar City",
        .Map            = MAP_SOLAR_EAST_CORP,
        .Bounds         = {{0, 0}, {192, 160}},
    },

    [SOLAR_WEST_CORPORATION] = {
        .Name           = "Solar City",
        .Map            = MAP_SOLAR_WEST_CORP,
        .Bounds         = {{0, 0}, {192, 160}},
    },

    [SOLAR_CITY_OUTSKIRTS] = {
        .Name           = "Solar City Outskirts",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{1392, 1872}, {2320, 2304}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {40, OSCILLATE,     { 9, 15}},
            {30, REPLICA,       {12, 20}},
            {20, LEKTRON,       { 8, 16}},
            { 5, CUMULUS,       {10, 18}},
            { 4, VOLTDRAGON,    {15, 21}},
            { 1, LAUNCHPAD,     {21, 22}},
        },
        .Fishing        = (ENCOUNTER[]){
            {50, CATFISH,       {10, 18}},
            {30, GARPIKE,       {12, 18}},
            {20, TOOLFISH,      {14, 16}},
        },
    },
    
    [SOLAR_PORT] = {
        .Name           = "Solar City Port",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{620, 1808}, {1392, 2304}},
        .Background     = WATER,
        .Encounters     = (ENCOUNTER[]){
            {40, OSCILLATE,     { 9, 15}},
            {30, REPLICA,       {12, 20}},
            {20, LEKTRON,       { 8, 16}},
            { 5, CUMULUS,       {10, 18}},
            { 4, VOLTDRAGON,    {15, 21}},
            { 1, LAUNCHPAD,     {21, 22}},
        },
        .Fishing        = (ENCOUNTER[]){
            {30, CATFISH,       {10, 18}},
            {20, GARPIKE,       {12, 18}},
            {20, BASSMONSTER,   {12, 13}},
            {10, TOOLFISH,      {14, 16}},
            { 5, MINESPHERE,    {20, 20}},
            { 4, COALSHARK,     {18, 18}},
            { 1, CATFISH,       {30, 30}},
        },
    },

    ////////////////////////////////////////////////////////
    // Solar Institute
    [SOLAR_INSTITUTE_1F] = {
        .Name           = "Solar Institute",
        .Map            = MAP_SOLAR_INSTITUTE_1F,
        .Bounds         = {{0, 0}, {224, 160}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {90, LEKTRON,       {12, 20}},
            {10, VOLTDRAGON,    {16, 20}},
        },
        .Fishing        = NULL,
    },

    [SOLAR_INSTITUTE_2F] = {
        .Name           = "Solar Institute",
        .Map            = MAP_SOLAR_INSTITUTE_2F,
        .Bounds         = {{0, 0}, {224, 160}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {70, LEKTRON,       {16, 20}},
            {30, VOLTDRAGON,    {16, 20}},
        },
        .Fishing        = NULL,
    },

    [SOLAR_INSTITUTE_3F] = {
        .Name           = "Solar Institute",
        .Map            = MAP_SOLAR_INSTITUTE_3F,
        .Bounds         = {{0, 0}, {224, 160}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {50, LEKTRON,       {18, 22}},
            {50, VOLTDRAGON,    {18, 22}},
        },
        .Fishing        = NULL,
    },

    [SOLAR_GENERATOR_ROOM] = {
        .Name           = "Solar Institute",
        .Map            = MAP_GENERATOR_ROOM,
        .Bounds         = {{0, 0}, {112, 128}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {95, LEKTRON,       {26, 34}},
            { 5, PRGMERROR,     {20, 20}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // New Land Cave
    [NEW_LAND_CAVE_OUTSKIRTS] = {
        .Name           = "New Land Cave",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{1680, 2304}, {2320, 2880}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {50, JAYRAPTOR,     {10, 18}},
            {30, LEKTRON,       { 8, 16}},
            {20, CUMULUS,       {10, 18}},
        },
        .Fishing        = (ENCOUNTER[]){
            {50, CATFISH,       {10, 18}},
            {30, GARPIKE,       {12, 18}},
            {20, TOOLFISH,      {14, 16}},
        },
    },

    [NEW_LAND_CAVE] = {
        .Name           = "New Land Cave",
        .Map            = MAP_NEW_LAND_CAVE,
        .Bounds         = {{0, 0}, {1168, 528}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {40, KARDA,         {10, 16}},
            {30, LEKTRON,       { 8, 16}},
            {20, BUTCHERBIRD,   {12, 20}},
            { 5, PUZZLE,        {16, 19}},
            { 4, ICEBOULDER,    {18, 20}},
            { 1, BUTCHERBIRD,   {28, 28}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Andora Falls area
    [FALLS_AREA] = {
        .Name           = "Falls Path",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{620, 2304}, {1680, 2880}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {39, JAYRAPTOR,     {17, 24}},
            {30, PHOENIX,       {18, 22}},
            {20, TELEVIRUS,     {15, 22}},
            { 9, CUMULUS,       {18, 24}},
            { 1, TOTEM_POLE,    {27, 27}},
            { 1, JAYRAPTOR,     {30, 30}},
        },
        .Fishing        = (ENCOUNTER[]){
            {50, CATFISH,       {18, 24}},
            {30, BASSMONSTER,   {20, 25}},
            {19, ACISTAR,       {22, 26}},
            { 1, ACISTAR,       {27, 32}},
        },
    },

    [ANDORA_REST_STOP] = {
        .Name           = "Falls Path",
        .Map            = MAP_REST_STOP,
        .Bounds         = {{0, 0}, {80, 112}},
    },

    [FALLS_CAVE_1F] = {
        .Name           = "Falls Cave",
        .Map            = MAP_FALLS_CAVE_1F,
        .Bounds         = {{0, 0}, {512, 336}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {30, GIGACLAM,      {16, 22}},
            {30, WATERWING,     {18, 22}},
            {30, BASSMONSTER,   {18, 22}},
            { 9, ACISTAR,       {20, 24}},
            { 1, ICEBOULDER,    {22, 22}},
        },
        .Fishing        = NULL,
    },

    [FALLS_CAVE_B1F] = {
        .Name           = "Falls Cave",
        .Map            = MAP_FALLS_CAVE_B1F,
        .Bounds         = {{0, 0}, {544, 368}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {30, GIGACLAM,      {20, 26}},
            {20, WATERWING,     {20, 26}},
            {20, BASSMONSTER,   {20, 26}},
            {10, ACISTAR,       {22, 28}},
            {10, MOATMONSTER,   {26, 28}},
            { 5, ICEBOULDER,    {23, 26}},
            { 4, MOATMONSTER,   {30, 34}},
            { 1, GIGACLAM,      {36, 36}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Andora Falls
    [ANDORA_FALLS] = {
        .Name           = "Andora Falls",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{0, 1808}, {620, 3104}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {40, JAYRAPTOR,     {19, 27}},
            {40, WATERWING,     {20, 24}},
            {20, CUMULUS,       {18, 24}},
        },
        .Fishing        = (ENCOUNTER[]){
            {30, CATFISH,       {18, 24}},
            {30, WATERWING,     {20, 24}},
            {20, BASSMONSTER,   {20, 25}},
            {19, ACISTAR,       {22, 26}},
            { 1, NESSIE,        {32, 32}},
        },
    },

    [ANDORA_HOSPITAL] = {
        .Name           = "Andora Falls",
        .Map            = MAP_ANDORA_HOSPITAL,
        .Bounds         = {{0, 0}, {192, 112}},
    },

    [ANDORA_PORT] = {
        .Name           = "Andora Falls",
        .Map            = MAP_ANDORA_PORT,
        .Bounds         = {{0, 0}, {160, 112}},
    },

    ////////////////////////////////////////////////////////
    // Granite Cave
    [GRANITE_CAVE_1F] = {
        .Name           = "Granite Cave",
        .Map            = MAP_GRANITE_CAVE_1F,
        .Bounds         = {{0, 0}, {704, 528}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {40, KARDA,         {24, 28}},
            {30, NITROBOMB,     {24, 28}},
            {20, DACTYLUS,      {24, 28}},
            { 5, TARHEAP,       {26, 32}},
            { 5, MEGATAR,       {28, 32}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_CAVE_B1F] = {
        .Name           = "Granite Cave",
        .Map            = MAP_GRANITE_CAVE_B1F,
        .Bounds         = {{0, 0}, {688, 688}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {30, NITROBOMB,     {28, 32}},
            {20, KARDA,         {26, 34}},
            {20, DACTYLUS,      {28, 34}},
            {10, TARHEAP,       {26, 32}},
            {10, MEGATAR,       {28, 32}},
            { 5, PUZZLE,        {28, 32}},
            { 4, BUTCHERBIRD,   {30, 36}},
            { 1, GOLDDRAGON,    {40, 40}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Granite City (overworld)
    [GRANITE_OUTSKIRTS] = {
        .Name           = "Granite Outskirts",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{0, 3104}, {620, 3648}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {50, JAYRAPTOR,     {24, 29}},
            {30, FLOPJELLY,     {22, 28}},
            {20, TELEVIRUS,     {22, 28}},
        },
        .Fishing        = (ENCOUNTER[]){
            {50, CATFISH,       {22, 28}},
            {30, BASSMONSTER,   {22, 30}},
            {19, ACISTAR,       {27, 32}},
            { 1, CATFISH,       {29, 38}},
        },
    },

    [GRANITE_CITY_SUBURBS] = {
        .Name           = "Granite City",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{0, 3648}, {1168, 4320}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {40, JAYRAPTOR,     {22, 30}},
            {30, GASMOG,        {22, 28}},
            {20, VACUUM,        {24, 28}},
            {10, FOGFANG,       {26, 32}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_CITY_DOWNTOWN] = {
        .Name           = "Granite City",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{1168, 2880}, {2320, 4320}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {40, GASMOG,        {28, 38}},
            {30, FOGFANG,       {28, 36}},
            {20, NITROBOMB,     {26, 32}},
            { 9, VACUUM,        {30, 36}},
            { 1, SPACESNAKE,    {28, 36}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Granite City (indoors)
    [GRANITE_AIRPORT] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_AIRPORT,
        .Bounds         = {{0, 0}, {192, 144}},
    },

    [GRANITE_AIR_TOWER_EAST] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_AIR_EAST,
        .Bounds         = {{0, 0}, {80, 96}},
    },

    [GRANITE_AIR_TOWER_WEST] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_AIR_WEST,
        .Bounds         = {{0, 0}, {80, 96}},
    },

    [GRANITE_CORPORATION] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_CORP,
        .Bounds         = {{0, 0}, {208, 160}},
    },

    [GRANITE_DEPARTMENT_STORE] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_DEPARTMENT,
        .Bounds         = {{0, 0}, {192, 208}},
    },

    [GAME_DESIGNER_ROOM] = {
        .Name           = "Granite City",
        .Map            = MAP_GAME_DESIGNER_ROOM,
        .Bounds         = {{0, 0}, {144, 128}},
    },

    [GRANITE_HOSPITAL] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_HOSPITAL,
        .Bounds         = {{0, 0}, {240, 192}},
    },

    [GRANITE_LIBRARY] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_LIBRARY,
        .Bounds         = {{0, 0}, {208, 144}},
    },

    [GRANITE_STORE_1] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_STORE_1,
        .Bounds         = {{0, 0}, {128, 112}},
    },

    [GRANITE_STORE_2] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_STORE_2,
        .Bounds         = {{0, 0}, {128, 112}},
    },

    [GRANITE_STORE_3] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_STORE_3,
        .Bounds         = {{0, 0}, {128, 112}},
    },

    [GRANITE_STORE_4] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_STORE_4,
        .Bounds         = {{0, 0}, {128, 112}},
    },

    [GRANITE_STORE_5] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_STORE_5,
        .Bounds         = {{0, 0}, {128, 112}},
    },

    [GRANITE_STORE_6] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_STORE_6,
        .Bounds         = {{0, 0}, {128, 112}},
    },
    
    [GRANITE_WAREHOUSE] = {
        .Name           = "Granite City",
        .Map            = MAP_GRANITE_WAREHOUSE,
        .Bounds         = {{0, 0}, {224, 128}},
    },

    ////////////////////////////////////////////////////////
    // Granite Tower
    [GRANITE_TOWER_1F] = {
        .Name           = "Granite Tower",
        .Map            = MAP_GRANITE_TOWER_1F,
        .Bounds         = {{0, 0}, {288, 208}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {70, NITROBOMB,     {65, 70}},
            {20, LEKTRON,       {48, 54}},
            {10, GASMOG,        {48, 54}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_TOWER_2F] = {
        .Name           = "Granite Tower",
        .Map            = MAP_GRANITE_TOWER_2F,
        .Bounds         = {{0, 0}, {288, 208}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {70, NITROBOMB,     {45, 50}},
            {20, LEKTRON,       {48, 54}},
            {10, ASTEROID,      {58, 64}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_TOWER_3F] = {
        .Name           = "Granite Tower",
        .Map            = MAP_GRANITE_TOWER_3F,
        .Bounds         = {{0, 0}, {288, 208}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {70, NITROBOMB,     {50, 55}},
            {30, ASTEROID,      {58, 64}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_TOWER_4F] = {
        .Name           = "Granite Tower",
        .Map            = MAP_GRANITE_TOWER_4F,
        .Bounds         = {{0, 0}, {288, 208}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {50, NITROBOMB,     {55, 60}},
            {30, ASTEROID,      {58, 64}},
            {20, SPACESNAKE,    {58, 64}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_TOWER_5F] = {
        .Name           = "Granite Tower",
        .Map            = MAP_GRANITE_TOWER_5F,
        .Bounds         = {{0, 0}, {288, 208}},
        .Background     = CHARCOAL,
        .Encounters     = (ENCOUNTER[]){
            {50, ASTEROID,      {60, 65}},
            {50, SPACESNAKE,    {58, 64}},
        },
        .Fishing        = NULL,
    },

    [GRANITE_TOWER_LAVATORY] = {
        .Name           = "Granite Tower",
        .Map            = MAP_LAVATORY,
        .Bounds         = {{0, 0}, {144, 128}},
        .Background     = WATER,
        .Encounters     = (ENCOUNTER[]){
            {40, WATERWING,     {48, 54}},
            {30, FLOPJELLY,     {48, 54}},
            {20, TARHEAP,       {48, 54}},
            { 9, MOATMONSTER,   {48, 54}},
            { 1, PRGMERROR,     {70, 70}},
        },
        .Fishing        = NULL,
    },

    ////////////////////////////////////////////////////////
    // Boulder Park
    [BOULDER_CAVE] = {
        .Name           = "Boulder Cave",
        .Map            = MAP_BOULDER_CAVE,
        .Bounds         = {{0, 0}, {624, 416}},
        .Background     = CAVE,
        .Encounters     = (ENCOUNTER[]){
            {30, DACTYLUS,      {34, 36}},
            {30, ASTEROID,      {32, 36}},
            {20, SPACESNAKE,    {30, 34}},
            {10, PUZZLE,        {30, 36}},
            {10, PHOENIX,       {32, 36}},
        },
        .Fishing        = NULL,
    },

    [BOULDER_PARK] = {
        .Name           = "Boulder Park",
        .Map            = MAP_OVERWORLD,
        .Bounds         = {{620, 2880}, {1168, 3648}},
        .Background     = GRASS,
        .Encounters     = (ENCOUNTER[]){
            {30, JAYRAPTOR,     {28, 36}},
            {29, BRONTO,        {44, 54}},
            {19, MOATMONSTER,   {44, 54}},
            {10, PUZZLE,        {34, 40}},
            { 9, TOTEM_POLE,    {44, 54}},
            { 1, GLACIALITH,    {54, 64}},
            { 1, BRONTO,        {54, 64}},
            { 1, MOATMONSTER,   {54, 64}},
        },
        .Fishing        = (ENCOUNTER[]){
            {30, GARPIKE,       {44, 54}},
            {30, BASSMONSTER,   {28, 36}},
            {19, COALSHARK,     {44, 54}},
            {10, GIGACLAM,      {34, 40}},
            { 9, MOATMONSTER,   {44, 54}},
            { 1, CATFISH,       {70, 80}},
            { 1, COALSHARK,     {54, 64}},
        },
    },
};

/**************************************************************/
#endif // _LOCATION_I_
