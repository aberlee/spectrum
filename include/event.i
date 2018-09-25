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

#define WARP(id, x, y, d) {EVENT_WARP, {.Warp={{x, y}, id, d}}}
#define TEXT(text) {EVENT_TEXT, {.Text=text}}
#define SHOP(id) {EVENT_SHOP, {.Shop=id}}
#define BOSS(spectra, level) {EVENT_BOSS, {.Boss={spectra, level}}}
#define REDIRECT(event) {EVENT_REDIRECT, {.Redirect=event}}
#define UNDEFINED {0}
#define HOUSE TEXT("You can't go in other people's houses!")
#define PRESENT(item, switch) {EVENT_PRESENT, {.Present={item, switch}}}
#define PERSON(person, direction, speech) {EVENT_PERSON, {.Person={person, direction, speech}}}

/**********************************************************//**
 * @var EVENT_DATA
 * @brief Stores all events that occur on maps.
 **************************************************************/
static const EVENT *(EVENT_DATA[]) = {
    // Overworld
    [MAP_OVERWORLD] = (EVENT[]){
        // Sapling Town and Triangle Lake
        [  1] = WARP(SAPLING_AIRPORT, 1, 6, UP),
        [  2] = WARP(YOUR_HOUSE, 5, 6, UP),
        [  3] = HOUSE,
        [  4] = WARP(SAPLING_HOSPITAL, 4, 6, UP),
        [  5] = HOUSE,
        [  6] = WARP(SAPLING_GREENHOUSE, 10, 8, UP),
        [  7] = HOUSE,
        [  8] = HOUSE,
        [  9] = WARP(SAPLING_CITY_HALL, 2, 8, UP),
        [ 10] = WARP(SAPLING_CITY_HALL, 11, 8, UP),
        [ 13] = TEXT("Sapling Town"),
        [ 21] = TEXT(
            "East - Triangle Lake\n"
            "West - Port Royal"
        ),
        [ 22] = WARP(SAPLING_LABORATORY, 4, 7, UP),
        
        // Port Royal
        [ 11] = WARP(ROYAL_HOSPITAL, 4, 6, UP),
        [ 12] = TEXT("Port Royal Hospital"),
        [ 14] = WARP(ROYAL_WAREHOUSE, 1, 7, UP),
        [ 15] = WARP(ROYAL_WAREHOUSE, 9, 7, UP),
        [ 16] = HOUSE,
        [ 17] = HOUSE,
        [ 18] = HOUSE,
        [ 19] = TEXT("Closed until further notice!"),
        [ 25] = HOUSE,
        [ 26] = HOUSE,
        [ 20] = TEXT("Port Royal"),
        
        // Oxide Crater
        [ 23] = WARP(OXIDE_CAVE, 18, 17, UP),
        [ 24] = WARP(OXIDE_CAVE, 30, 20, UP),
        
        // Solar City Port
        [ 40] = TEXT(
            "WARNING:\n"
            "Minesphere are abundant in these waters!\r"
            "This spectra is known to explode.\n"
            "Use caution when fishing!"
        ),
        [ 41] = TEXT(
            "WARNING:\n"
            "Dock is slippery when wet.\r"
            "Solar City Port does not accept responsibility\n"
            "for injury or accident."
        ),
        [ 42] = TEXT("Solar City Port"),
        [ 43] = WARP(ROYAL_PORT, 1, 6, UP),
        [ 44] = WARP(ROYAL_PORT, 8, 6, UP),
        
        // Solar City
        [ 27] = HOUSE,
        [ 28] = HOUSE,
        [ 29] = HOUSE,
        [ 30] = HOUSE,
        [ 31] = WARP(SOLAR_HOSPITAL, 4, 6, UP),
        [ 32] = WARP(SOLAR_WEST_CORPORATION, 3, 9, UP),
        [ 33] = HOUSE,
        [ 34] = HOUSE,
        [ 35] = HOUSE,
        [ 36] = WARP(SOLAR_EAST_CORPORATION, 6, 9, UP),
        [ 37] = WARP(SOLAR_AIRPORT, 2, 8, UP),
        [ 38] = REDIRECT(37),
        [ 39] = REDIRECT(37),
        
        // Solar Institute
        [ 45] = WARP(SOLAR_INSTITUTE_1F, 5, 9, UP),
        [ 46] = REDIRECT(45),
        [ 48] = REDIRECT(45),
        [ 49] = REDIRECT(45),
        [ 50] = REDIRECT(45),
        [ 51] = REDIRECT(45),
        [ 52] = TEXT(
            "Solar Institute\n"
            "Researching and providing solar energy."
        ),
        [ 53] = WARP(SOLAR_GENERATOR_ROOM, 3, 6, UP),
        [ 54] = TEXT(
            "BEWARE - The generator is dangerous\n"
            "when active!"
        ),
        
        // Falls Area
        [ 58] = WARP(FALLS_CAVE_1F, 5, 15, UP),
        [ 59] = WARP(FALLS_CAVE_1F, 24, 17, UP),
        [ 63] = WARP(ANDORA_REST_STOP, 2, 6, UP),
        [ 64] = TEXT("Rest Stop"),
        [ 65] = WARP(NEW_LAND_CAVE, 54, 12, UP),
        [ 67] = WARP(NEW_LAND_CAVE, 41, 20, UP),
        [ 66] = WARP(NEW_LAND_CAVE, 8, 16, UP),
        
        // Andora Falls
        [ 47] = TEXT("Andora Falls Port"),
        [ 55] = WARP(ANDORA_PORT, 8, 4, UP),
        [ 56] = WARP(ANDORA_PORT, 1, 6, UP),
        [ 57] = WARP(ANDORA_HOSPITAL, 4, 6, UP),
        [ 60] = HOUSE,
        [ 61] = HOUSE,
        [ 62] = HOUSE,
        [ 68] = HOUSE,
        
        // Granite Cave
        [ 69] = WARP(GRANITE_CAVE_1F, 20, 3, UP),
        [ 70] = WARP(GRANITE_CAVE_1F, 35, 13, UP),
        [ 71] = WARP(GRANITE_CAVE_1F, 14, 17, UP),
        
        
        // Granite City Downtown
        [ 72] = WARP(GRANITE_TOWER_1F, 9, 12, UP),
        [ 73] = REDIRECT(72),
        [ 74] = REDIRECT(72),
        [ 75] = REDIRECT(72),
        [ 76] = REDIRECT(72),
        [ 77] = REDIRECT(72),
        [ 78] = REDIRECT(72),
        [ 79] = REDIRECT(72),
        [ 80] = WARP(GRANITE_STORE_1, 4, 6, UP),
        [ 81] = WARP(GRANITE_STORE_2, 4, 6, UP),
        [ 82] = WARP(GRANITE_STORE_3, 4, 6, UP),
        [ 84] = WARP(GRANITE_DEPARTMENT_STORE, 6, 12, UP),
        [ 85] = REDIRECT(84),
        [ 86] = REDIRECT(84),
        [ 87] = WARP(GRANITE_WAREHOUSE, 1, 7, UP),
        [ 88] = WARP(GRANITE_WAREHOUSE, 7, 7, UP),
        [ 89] = WARP(GRANITE_WAREHOUSE, 12, 7, UP),
        [ 91] = WARP(GRANITE_STORE_4, 4, 6, UP),
        [ 92] = WARP(GRANITE_STORE_5, 4, 6, UP),
        [ 93] = WARP(GRANITE_STORE_6, 4, 6, UP),
        [ 94] = HOUSE,
        [ 95] = TEXT("Granite City"),
        
        [ 97] = WARP(GRANITE_CORPORATION, 1, 4, UP),
        [103] = WARP(GRANITE_CORPORATION, 7, 9, UP),
        [ 98] = WARP(GRANITE_HOSPITAL, 13, 6, UP),
        [100] = WARP(GRANITE_HOSPITAL, 1, 11, UP),
        [101] = WARP(GRANITE_HOSPITAL, 8, 11, UP),
        [102] = WARP(GRANITE_LIBRARY, 5, 8, UP),
        [114] = WARP(GRANITE_AIRPORT, 1, 8, UP),
        [115] = REDIRECT(114),
        [116] = WARP(GRANITE_AIRPORT, 10, 8, UP),
        [117] = REDIRECT(116),
        [118] = WARP(GRANITE_AIR_TOWER_WEST, 2, 5, UP),
        [119] = WARP(GRANITE_AIR_TOWER_EAST, 2, 5, UP),
        
        // Granite City Suburbs
        [ 99] = HOUSE,
        [104] = HOUSE,
        [105] = HOUSE,
        [106] = TEXT(
            "NO FISHING\n"
            "- Posted by the Granite City Council"
        ),
        [107] = HOUSE,
        [108] = HOUSE,
        [109] = HOUSE,
        [110] = HOUSE,
        [111] = HOUSE,
        [112] = HOUSE,
        [113] = HOUSE,
        [120] = HOUSE,
        [121] = HOUSE,
        [122] = HOUSE,
        [123] = HOUSE,
        [124] = HOUSE,
        [125] = HOUSE,
        [126] = HOUSE,
        
        // Boulder Park
        [ 83] = TEXT(
            "Boulder National Park\n"
            "Kaido's protected nature preserve."
        ),
        [ 90] = WARP(BOULDER_CAVE, 20, 5, UP),
        [ 96] = WARP(BOULDER_CAVE, 13, 17, UP),
    },
    
    // Boulder Cave
    [MAP_BOULDER_CAVE] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 59, 220, DOWN),
        [  2] = WARP(OVERWORLD, 52, 232, DOWN),
    },
    
    // Falls Cave
    [MAP_FALLS_CAVE_1F] = (EVENT[]){
        [  1] = WARP(FALLS_CAVE_B1F, 12, 9, UP),
        [  2] = WARP(FALLS_CAVE_B1F, 22, 11, UP),
        [  3] = TEXT("Welcome to the town of Andora Falls"),
        [  4] = TEXT(
            "Andora Falls Cave\n"
            "Also known as The Chasm"
        ),
        [  5] = WARP(OVERWORLD, 30, 155, DOWN),
        [  6] = WARP(OVERWORLD, 49, 157, DOWN),
    },
    [MAP_FALLS_CAVE_B1F] = (EVENT[]){
        [  1] = WARP(FALLS_CAVE_1F, 11, 10, DOWN),
        [  2] = WARP(FALLS_CAVE_1F, 21, 12, DOWN),
    },
    
    // Granite Cave
    [MAP_GRANITE_CAVE_1F] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 28, 187, DOWN),
        [  2] = WARP(GRANITE_CAVE_B1F, 22, 39, UP),
        [  3] = TEXT(
            "WARNING:\n"
            "Strong monsters inhabit that part\r"
            "of the cave ahead!\r"
            "There's a note attached...\r"
            "\"WARNING - Strong monsters are in\n"
            "this part of the cave now, too.\r"
            "\"Proceed with caution.\""
        ),
        [  4] = TEXT("This exit leads to Boulder Park."),
        [  5] = TEXT(
            "Granite Cave\n"
            "Formerly known as Granite Quarry\r"
            "North - Andora Falls\n"
            "South - Granite City\r"
            "East - Boulder Park"
        ),
        [  6] = WARP(OVERWORLD, 43, 197, DOWN),
        [  7] = WARP(OVERWORLD, 22, 200, DOWN),
    },
    [MAP_GRANITE_CAVE_B1F] = (EVENT[]){
        [  1] = TEXT("Watch out while climbing rocks."),
        [  2] = WARP(GRANITE_CAVE_1F, 20, 10, DOWN),
    },
    
    // New Land Cave
    [MAP_NEW_LAND_CAVE] = (EVENT[]){
        [  1] = TEXT(
            "It's a faded sign...\r"
            "\"Memorial for those killed during the\n"
            "excavation of this cave.\""
        ),
        [  2] = TEXT(
            "New Land Cave\n"
            "Dug by the Solar City Mining Company\r"
            "There's some vandalism...\r"
            "\"DOWN WITH COAL\" is scrawled..."
        ),
        [  3] = TEXT(
            "WARNING:\n"
            "Due to the prevalence of rockslides,\r"
            "exercise caution when climbing in the\n"
            "cave!"
        ),
        [  4] = WARP(OVERWORLD, 127, 164, DOWN),
        [  5] = TEXT(
            "Advertisement:\n"
            "Need a refreshment? Visit the Rest Stop\r"
            "on your way to Andora Falls!"
        ),
        [  6] = WARP(OVERWORLD, 82, 168, DOWN),
        [  7] = WARP(OVERWORLD, 114, 172, DOWN),
    },
    
    // Oxide Cave
    [MAP_OXIDE_CRATER] = (EVENT[]){
        [  1] = TEXT(
            "Oxide Crater National Park\n"
            "Formed by the impact of a meteor\r"
            "on the Earth's crust more than 20,000\n"
            "yeard ago."
        ),
        [  2] = WARP(OVERWORLD, 61, 65, DOWN),
        [  3] = WARP(OVERWORLD, 73, 67, DOWN),
        [  4] = TEXT("Upcoming cave leads to dead end."),
    },

    // Sapling Town buildings
    [MAP_SAPLING_YOUR_HOUSE] = (EVENT[]){
        [  1] = TEXT(
            "There's a book titled \"About Spectra:\"\n"
            "Part 1\r"
            "\"Spectra are the various species of wildlife\n"
            "found in the world. Humans and spectra have\r"
            "\"always lived in peace, often working together.\""
        ),
        [  2] = TEXT(
            "There's a book titled \"About Spectra:\n"
            "Part 2\r"
            "\"Many spectra are biological in nature, while\n"
            "others are magical or mechanical.\r"
            "\"Regardless of form, spectra exhibit intelligence\n"
            "and emotion.\""
        ),
        [  3] = TEXT(
            "There's a book titled \"About Spectra:\n"
            "Part 3\r"
            "\"Many people work together with spectra to achieve\n"
            "progress where neither one could alone.\r"
            "\"Some spectra are better at fighting; others excel\n"
            "at support or defense.\""
        ),
        [  4] = PRESENT(POTION, SWITCH_AMY_HOUSE_1),
        [  5] = WARP(OVERWORLD, 136, 12, DOWN),
    },
    [MAP_SAPLING_AIRPORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 110, 8, DOWN),
    },
    [MAP_SAPLING_HOSPITAL] = (EVENT[]){
        [  1] = TEXT("OUT OF ORDER"),
        [  2] = PERSON(NPC_DOCTOR, DOWN, "Hello!"),
        [  3] = WARP(OVERWORLD, 84, 15, DOWN),
    },
    [MAP_SAPLING_CITY_HALL] = (EVENT[]){
        [  1] = TEXT(
            "There's a book titled \"A History\n"
            "of Sapling Town\"\r"
            "\"Sapling Town was founded by the first\n"
            "settlers of Kaido nearly a hundred years ago.\""
        ),
        [  2] = TEXT(
            "There's a book titled \"A History\n"
            "of Triangle Lake\"\r"
            "\"Triangle Lake, named for its distinct shape,\n"
            "provided the a source of food and power to the\r"
            "\"earliest residents of Sapling Town.\""
        ),
        [  3] = WARP(OVERWORLD, 92, 23, DOWN),
        [  4] = WARP(OVERWORLD, 96, 23, DOWN),
    },
    [MAP_SAPLING_GREENHOUSE] = (EVENT[]){
        [  1] = TEXT("Sapling Greenhouse"),
        [  2] = WARP(OVERWORLD, 104, 16, DOWN),
    },
    [MAP_SAPLING_LABORATORY] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 112, 62, DOWN),
    },

    // Port Royal buildings
    [MAP_ROYAL_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(
            "There's a book titled \"A History\n"
            "of Port Royal\"\r"
            "\"Port Royal was founded around the same\n"
            "time as Sapling Town, and served as Kaido's\r"
            "main port until Solar City Port opened.\r"
            "Now, Port Royal has become far less busy\r"
            "as many workers move away."
        ),
        [  2] = TEXT(
            "There's a book titled \"Port Royal\n"
            "Hospital - Visitor's Guide\"\r"
            "\"Port Royal Hospital is a member of the Kaido\n"
            "Hospital Network, providing all people and spectra\r"
            "\"with free, quality health care.\r"
            "\"This follows the wishes of the founder, Itoi.\""
        ),
        [  3] = TEXT(
            "There's a book titled \"Biography of\n"
            "Keiichi Itoi\"\r"
            "\"Mr. Keiichi Itoi was a sailor and doctor\n"
            "who helped build Port Royal. In his free time\r"
            "he collected rocks near the Oxide Crater."
        ),
        [  4] = WARP(OVERWORLD, 27, 30, DOWN),
    },
    [MAP_ROYAL_WAREHOUSE] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 22, 36, DOWN),
        [  2] = WARP(OVERWORLD, 25, 36, DOWN),
    },
    [MAP_ROYAL_PORT] = (EVENT[]){
        [  1] = TEXT(
            "There is a pamphlet about Solar City Port.\r"
            "\"Solar City Port works together with the Solar\n"
            "Institute, and runs entirely on solar power."
        ),
        [  2] = TEXT(
            "There's a book titled \"Local Fishing at\n"
            "Solar City\"\r"
            "Garpike and Coalshark are extremely aggressive,\n"
            "so be prepared to fight.\r"
            "If you hook a Minesphere, run away!\r"
            "Sometimes, Catfish have been seen that grow extremely\n"
            "strong. These are very rare."
        ),
        [  3] = TEXT(
            "There's a book titled \"Tourism at the\n"
            "Oxide Crater\"\r"
            "The Oxide Crater is a national park, and is home to\n"
            "a number of spectra rumored to be from space."
        ),
        [  4] = WARP(OVERWORLD, 65, 117, DOWN),
        [  5] = WARP(OVERWORLD, 69, 117, DOWN),
    },

    // Solar City buildings
    [MAP_SOLAR_AIRPORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 117, 104, DOWN),
    },
    [MAP_SOLAR_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(
            "There's a book titled \"A History\n"
            "of Solar City\"\r"
            "Solar City was once a mining village which\n"
            "prospered through the export of rich minerals\r"
            "from the Oxide Crater.\r"
            "Today, Solar City leads the protection of Kaido's\n"
            "environment, and is its largest producer of energy."
        ),
        [  2] = WARP(OVERWORLD, 111, 86, DOWN),
    },
    [MAP_SOLAR_EAST_CORP] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 131, 91, DOWN),
    },
    [MAP_SOLAR_WEST_CORP] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 125, 86, DOWN),
    },
    
    // Solar Institute
    [MAP_SOLAR_INSTITUTE_1F] = (EVENT[]){
        [  1] = WARP(SOLAR_INSTITUTE_3F, 13, 3, DOWN),
        [  2] = WARP(OVERWORLD, 104, 128, DOWN),
    },
    [MAP_SOLAR_INSTITUTE_2F] = (EVENT[]){
        [  1] = WARP(SOLAR_INSTITUTE_3F, 13, 3, DOWN),
        [  2] = WARP(SOLAR_INSTITUTE_1F, 12, 3, DOWN),
    },
    [MAP_SOLAR_INSTITUTE_3F] = (EVENT[]){
        [  1] = WARP(SOLAR_INSTITUTE_2F, 12, 3, DOWN),
    },
    [MAP_GENERATOR_ROOM] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 112, 133, DOWN),
    },

    // Andora Falls buildings
    [MAP_REST_STOP] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 90, 161, DOWN),
    },
    [MAP_ANDORA_HOSPITAL] = (EVENT[]){
        [  1] = TEXT("Hey! Occupied!"),
        [  2] = WARP(OVERWORLD, 20, 150, DOWN),
    },
    [MAP_ANDORA_PORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 13, 148, DOWN),
        [  2] = WARP(OVERWORLD, 9, 150, DOWN),
    },

    // Granite City buildings
    [MAP_GRANITE_AIRPORT] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 97, 254, DOWN),
        [  2] = WARP(OVERWORLD, 109, 254, DOWN),
    },
    [MAP_GRANITE_AIR_EAST] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 121, 255, DOWN),
    },
    [MAP_GRANITE_AIR_WEST] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 117, 255, DOWN),
    },
    [MAP_GRANITE_CORP] = (EVENT[]){
        [  1] = WARP(GAME_DESIGNER_ROOM, 4, 3, DOWN),
        [  2] = WARP(OVERWORLD, 129, 234, DOWN),
        [  3] = WARP(OVERWORLD, 132, 240, DOWN),
    },
    [MAP_GRANITE_DEPARTMENT] = (EVENT[]){
        [  1] = TEXT("WARNING:\nToilet is overflowing."),
        [  2] = WARP(OVERWORLD, 117, 213, DOWN),
    },
    [MAP_GAME_DESIGNER_ROOM] = (EVENT[]){
        [  1] = WARP(GRANITE_CORPORATION, 4, 3, DOWN),
        [  2] = TEXT(
            "There's a book titled \"Basic Rules\n"
            "of Game Design\"\r"
            "Rule 1 - Eat well.\n"
            "Rule 2 - Sleep well.\r"
            "Rule 3 - Avoid overwork.\n"
            "Rule 4 - Motivate yourself!"
        ),
        [  3] = TEXT(
            "There's a book titled \"Allegro 5\n"
            "Reference Manual\"\r"
            "It looks too complicated to read..."
        ),
    },
    [MAP_GRANITE_HOSPITAL] = (EVENT[]){
        [  1] = TEXT(
            "There's a book titled \"About Granite\n"
            "Hospital\"\r"
            "\"Granite Hospital is the largest hospital\n"
            "in Kaido, and is the leader of the Kaido\r"
            "\"Hospital Network.\""
        ),
        [  2] = TEXT(
            "There's a book titled \"A History\n"
            "of Granite City\"\r"
            "\"Granite City is the capital of Kaido,\n"
            "and is by far the largest city."
        ),
        [  3] = TEXT(
            "There's a book titled \"A History\n"
            "of Boulder Park\"\r"
            "\"Wildlife in Boulder Park is protected, and\n"
            "many rare species roam free.\r"
            "\"Boulder Park can be accessed through Granite\n"
            "Cave, or Boulder Cave.\""
        ),
        [  4] = WARP(OVERWORLD, 94, 238, DOWN),
        [  5] = WARP(OVERWORLD, 88, 240, DOWN),
        [  6] = WARP(OVERWORLD, 90, 240, DOWN),
    },
    [MAP_GRANITE_LIBRARY] = (EVENT[]){
        [  1] = TEXT(
            "There's a book titled \"A History\n"
            "of Kaido\"\r"
            "\"Kaido began as a hub of industry. The\n"
            "environment became extremely polluted,\r"
            "\"and many national parks were built in\n"
            "an effort to save them.\r"
            "\"Today, Kaido has shut down its mines\n"
            "and produces solar energy.\""
        ),
        [  2] = TEXT(
            "There's a cute story about two girls\n"
            "falling in love."
        ),
        [  3] = TEXT(
            "There's a book titled \"The Legend\n"
            "of Varan\"\r"
            "\"A long time ago, there was a great king\n"
            "named Varan.\r"
            "\"Varan ruled over the spectra peacefully,\n"
            "but this changed when humans came to Kaido.\r"
            "\"Varan became enraged at how humans treated\n"
            "the environment. They burned and dug everything\r"
            "\"out of the earth. So Varan called the spectra\n"
            "to fight against the humans.\r"
            "\"Cities were devastated and many were killed.\n"
            "Once the fighting stopped, Varan would still\r"
            "\"not be quelled. As the king, he used his\n"
            "influence to force fighting to continue.\r"
            "\"His power was so great, very few could resist.\r"
            "But as the chaos grew, some people and spectra\n"
            "fought back...\"\r"
            "The story ends here..."
        ),
        [  4] = TEXT(
            "There's a book titled \"The Origin of\n"
            "the Oxide Crater\"\r"
            "\"The Oxide Crater seems to have been created\n"
            "by a large meteorite - but, traces of manmade\r"
            "\"materials have been excavated. Perhaps there\n"
            "was once a city there?\""
        ),
        [  5] = TEXT(
            "There's a book titled \"Kaido Before\n"
            "Humans\"\r"
            "\"Before the first settlers arrived, Kaido\n"
            "was populated entirely by spectra.\r"
            "\"Some traces of structures have been found\n"
            "that indicate that spectra had their own cities\r"
            "\"and possibly, their own hierarchy.\""
        ),
        [  6] = TEXT(
            "There's an academic thesis titled\n"
            "\"Humans and Spectra in Industry\"\r"
            "\"In early Kaido, humans forced spectra to do\n"
            "labor for them. This frequently involved mining\r"
            "\"and woodcutting, which contributed to the\n"
            "devastation of the environment.\""
        ),
        [  7] = TEXT(
            "There's a book titled \"On the Rights\n"
            "of Spectra\"\r"
            "\"Spectra should never be forced to do things\n"
            "they don't want to.\r"
            "\"Humans and spectra should always work together\n"
            "as equals.\""
        ),
        [  8] = TEXT(
            "There's a book titled \"The Legend\n"
            "of the Cataclysm\"\r"
            "\"A long time ago, many people succumbed to\n"
            "greed and power, destroying everything they\r"
            "\"didn't value or care for.\r"
            "\"Eventually, the forces of nature came down\n"
            "on all humans, not just the greedy.\r"
            "\"It is said that if humanity ever becomes so\n"
            "destructive, disaster will occur again.\""
        ),
        [  9] = TEXT(
            "There's a book titled \"The Legend\n"
            "of the Rebellion\"\r"
            "\"A long time ago, humans and spectra joined\n"
            "together to fight an evil king bringing disaster.\r"
            "\"The humans and spectra worked together to stop\n"
            "the evil, although many perished.\""
        ),
        [ 10] = WARP(OVERWORLD, 109, 240, DOWN),
    },
    [MAP_GRANITE_STORE_1] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 89, 209, DOWN),
    },
    [MAP_GRANITE_STORE_2] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 95, 209, DOWN),
    },
    [MAP_GRANITE_STORE_3] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 102, 209, DOWN),
    },
    [MAP_GRANITE_STORE_4] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 88, 225, DOWN),
    },
    [MAP_GRANITE_STORE_5] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 94, 225, DOWN),
    },
    [MAP_GRANITE_STORE_6] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 99, 225, DOWN),
    },
    [MAP_GRANITE_WAREHOUSE] = (EVENT[]){
        [  1] = WARP(OVERWORLD, 87, 218, DOWN),
        [  2] = WARP(OVERWORLD, 94, 218, DOWN),
        [  3] = WARP(OVERWORLD, 99, 218, DOWN),
    },
    
    // Granite Tower
    [MAP_GRANITE_TOWER_1F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_2F, 16, 3, DOWN),
        [  2] = WARP(OVERWORLD, 99, 201, DOWN),
    },
    [MAP_GRANITE_TOWER_2F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_3F, 1, 3, DOWN),
        [  2] = WARP(GRANITE_TOWER_LAVATORY, 5, 7, UP),
        [  3] = WARP(GRANITE_TOWER_1F, 16, 3, DOWN),
        [  4] = TEXT(
            "CONTRACT:\n"
            "The Granite Quarry will be repoened in\r"
            "the next two years, yielding a wealth of\n"
            "riches and gold."
        ),
        [  5] = TEXT(
            "CONTRACT:\n"
            "The national park status of the Oxide Crater\r"
            "will be repealed within five years,\n"
            "allowing the extraction of minerals."
        ),
    },
    [MAP_GRANITE_TOWER_3F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_2F, 1, 3, DOWN),
        [  2] = TEXT(
            "RESEARCH DOCUMENT:\n"
            "Granite Tower Corporation will be researching\r"
            "the Solar Institute for possible means of\n"
            "competition. The Solar Institute must be\r"
            "driven out so the public will support mining."
        ),
        [  3] = TEXT(
            "RESEARCH DOCUMENT:\n"
            "Determine which species of spectra are most\r"
            "suited to working in mines. Ensure they are\n"
            "responsive to human trainers."
        ),
        [  4] = TEXT(
            "RESEARCH DOCUMENT:\n"
            "Investigate whether lowering wages of Granite\r"
            "Tower employees will increase productivity."
        ),
        [  5] = TEXT(
            "RESEARCH DOCUMENT:\n"
            "Investigate which Granite City mayoral\r"
            "candidates will support Granite Tower in\n"
            "exchange for endorsement and financial support."
        ),
        [  6] = TEXT(
            "RESEARCH DOCUMENT:\n"
            "Challenge the Kaido Hospital Network's\r"
            "sanctions against mining-related injury."
        ),
        [  7] = WARP(GRANITE_TOWER_4F, 16, 3, DOWN),
    },
    [MAP_GRANITE_TOWER_4F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_5F, 16, 3, DOWN),
        [  2] = WARP(GRANITE_TOWER_3F, 16, 3, DOWN),
    },
    [MAP_GRANITE_TOWER_5F] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_4F, 1, 3, DOWN),
        [  2] = TEXT(
            "TOP SECRET:\n"
            "Granite Tower must secure political and\r"
            "financial power in order to prolong and\n"
            "reignite conflict between humans and spectra.\r"
            "The profit in funding both sides of the battle\n"
            "cannot be overlooked."
        ),
    },
    [MAP_LAVATORY] = (EVENT[]){
        [  1] = WARP(GRANITE_TOWER_2F, 9, 3, DOWN),
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
