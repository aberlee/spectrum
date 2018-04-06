/**********************************************************//**
 * @file spectra.i
 * @brief Defines constant species data for every species.
 * @author Rena Shinomiya
 * @date March 6th, 2018
 **************************************************************/

#ifndef _SPECIES_I_
#define _SPECIES_I_

#include "type.h"           // TYPE_ID
#include "species.h"        // SPECIES_ID, EXPERIENCE_TYPE, SPECIES
#include "technique.h"      // TECHNIQUE_ID

/**********************************************************//**
 * @var SPECIES_DATA
 * @brief Defines all the SPECIES data.
 **************************************************************/
const SPECIES SPECIES_DATA[N_SPECIES] = {
    
    [COALSHARK]={
        .Name="Coalshark",
        .Type={WATER, FIRE},
        .Health=80,
        .Power=90,
        .Attack=130,
        .Defend=70,
        .Evade=91,
        .Luck=74,
        .Rate=AVERAGE,
        .Experience=80,
        .Money=25,
        .CatchRate=3,
        .MovesetSize=7,
        .Moveset={
            { 1, BITE},
            { 6, LURK},
            { 9, SWIM_ATTACK},
            {20, FIRE_BREATH},
            {28, MANGLE},
            {36, RIPTIDE},
            {44, SNIPER},
        },
    },

    [JAYRAPTOR]={
        .Name="Jayraptor",
        .Type={WIND, 0},
        .Health=82,
        .Power=90,
        .Attack=91,
        .Defend=60,
        .Evade=107,
        .Luck=80,
        .Rate=FAST,
        .Experience=35,
        .Money=4,
        .CatchRate=20,
        .MovesetSize=6,
        .Moveset={
            { 1, GUST},
            { 4, DISARM},
            {12, PROVOKE},
            {28, CYCLONE},
            {36, MANGLE},
            {52, HURRICANE},
        },
    },
    
    [MINESPHERE]={
        .Name="Minesphere",
        .Type={METAL, WATER},
        .Health=72,
        .Power=80,
        .Attack=143,
        .Defend=98,
        .Evade=40,
        .Luck=90,
        .Rate=AVERAGE,
        .Experience=100,
        .Money=180,
        .CatchRate=8,
        .MovesetSize=5,
        .Moveset={
            { 1, EXPLOSION},
            { 1, LURK},
            { 1, COLLIDE},
            {15, BOMBARD},
            {30, CORE_BEAM},
        },
    },
    
    [HOCUS]={
        .Name="Hocus",
        .Type={DARK, 0},
        .Health=70,
        .Power=107,
        .Attack=45,
        .Defend=60,
        .Evade=95,
        .Luck=103,
        .Rate=FAST,
        .Experience=40,
        .Money=8,
        .CatchRate=20,
        .MovesetSize=6,
        .Moveset={
            { 1, HYPNOSIS},
            { 6, MIND_SCREW},
            {12, OBSCURE},
            {22, MIND_RAY},
            {34, RECOVER},
            {44, BRAINWAVE},
        },
    },
    
    [NESSIE]={
        .Name="Nessie",
        .Type={WATER, 0},
        .Health=140,
        .Power=85,
        .Attack=70,
        .Defend=88,
        .Evade=54,
        .Luck=92,
        .Rate=SLOW,
        .Experience=155,
        .Money=150,
        .CatchRate=5,
        .MovesetSize=7,
        .Moveset={
            { 1, SWIM_ATTACK},
            { 4, LURK},
            {12, FLATTEN},
            {22, PURGE},
            {30, MAELSTROM},
            {40, RAMPAGE},
            {56, TSUNAMI},
        },
    },
    
    [OSCILLATE]={
        .Name="Oscillate",
        .Type={METAL, 0},
        .Health=70,
        .Power=90,
        .Attack=75,
        .Defend=80,
        .Evade=45,
        .Luck=50,
        .Rate=FAST,
        .Experience=20,
        .Money=2,
        .CatchRate=25,
        .MovesetSize=6,
        .Moveset={
            { 1, GUST},
            {10, COLD_GUST},
            {10, HOT_GUST},
            {22, CHARGE},
            {30, CYCLONE},
            {48, HURRICANE},
        },
    },
    
    [PUZZLE]={
        .Name="Puzzle",
        .Type={EARTH, 0},
        .Health=100,
        .Power=85,
        .Attack=30,
        .Defend=180,
        .Evade=35,
        .Luck=90,
        .Rate=AVERAGE,
        .Experience=120,
        .Money=33,
        .CatchRate=5,
        .MovesetSize=6,
        .Moveset={
            { 1, COLLIDE},
            { 6, PREVENT},
            { 9, FLATTEN},
            {18, FRACTURE},
            {30, EXPLOSION},
            {36, SOLIDIFY},
            {46, CRUSH},
        },
    },
    
    [LAUNCHPAD]={
        .Name="Launchpad",
        .Type={METAL, 0},
        .Health=85,
        .Power=75,
        .Attack=60,
        .Defend=110,
        .Evade=35,
        .Luck=75,
        .Rate=AVERAGE,
        .Experience=90,
        .Money=52,
        .CatchRate=10,
        .MovesetSize=5,
        .Moveset={
            { 1, ZAP},
            { 5, CHARGE},
            {22, LIGHTNING},
            {34, BOMBARD},
            {60, THUNDER},
        },
    },
    
    [GARPIKE]={
        .Name="Garpike",
        .Type={WATER, 0},
        .Health=80,
        .Power=70,
        .Attack=125,
        .Defend=60,
        .Evade=102,
        .Luck=70,
        .Rate=AVERAGE,
        .Experience=60,
        .Money=16,
        .CatchRate=20,
        .MovesetSize=7,
        .Moveset={
            { 1, BITE},
            { 9, SWIM_ATTACK},
            {14, SHARPEN},
            {22, PROVOKE},
            {30, RIPTIDE},
            {38, SNIPER},
            {48, MANGLE},
        },
    },
    
    [MOATMONSTER]={
        .Name="MoatMonster",
        .Type={WATER, DARK},
        .Health=104,
        .Power=95,
        .Attack=125,
        .Defend=80,
        .Evade=76,
        .Luck=79,
        .Rate=SLOW,
        .Experience=125,
        .Money=120,
        .CatchRate=3,
        .MovesetSize=7,
        .Moveset={
            { 1, BITE},
            { 6, LURK},
            {18, FIRE_BREATH},
            {28, POISON_CLAW},
            {38, MANGLE},
            {46, SNIPER},
            {54, POISON_GRIP},
        },
    },
    
    [GOLDDRAGON]={
        .Name="GoldDragon",
        .Type={METAL, 0},
        .Health=100,
        .Power=100,
        .Attack=106,
        .Defend=100,
        .Evade=93,
        .Luck=81,
        .Rate=SLOWEST,
        .Experience=150,
        .Money=220,
        .CatchRate=1,
        .MovesetSize=7,
        .Moveset={
            { 1, BITE},
            { 6, SHARPEN},
            {13, FIRE_BREATH},
            {28, SOLIDIFY},
            {36, CORE_BEAM},
            {48, MANGLE},
            {56, INFERNO},
        },
    },
    
    [REPLICA]={
        .Name="Replica",
        .Type={BASIC, 0},
        .Health=80,
        .Power=130,
        .Attack=50,
        .Defend=80,
        .Evade=60,
        .Luck=110,
        .Rate=AVERAGE,
        .Experience=70,
        .Money=80,
        .CatchRate=10,
        .MovesetSize=6,
        .Moveset={
            { 1, PURGE},
            { 6, RECOVER},
            {18, ENGULF},
            {26, DISPEL},
            {38, TRI_BEAM},
            {52, EXPLOSION},
        },
    },
    
    [NITROBOMB]={
        .Name="Nitrobomb",
        .Type={METAL, FIRE},
        .Health=75,
        .Power=90,
        .Attack=70,
        .Defend=85,
        .Evade=55,
        .Luck=75,
        .Rate=FAST,
        .Experience=45,
        .Money=36,
        .CatchRate=25,
        .MovesetSize=6,
        .Moveset={
            { 1, ZAP},
            { 8, FIREWALL},
            {15, EXPLOSION},
            {22, BOMBARD},
            {30, CHARGE},
            {40, CORE_BEAM},
        },
    },
    
    [FLOPJELLY]={
        .Name="Flopjelly",
        .Type={BASIC, 0},
        .Health=100,
        .Power=80,
        .Attack=40,
        .Defend=70,
        .Evade=30,
        .Luck=40,
        .Rate=FAST,
        .Experience=30,
        .Money=2,
        .CatchRate=30,
        .MovesetSize=6,
        .Moveset={
            { 1, BITE},
            { 5, PREVENT},
            {12, COLLIDE},
            {20, RECOVER},
            {26, PURGE},
            {32, ENGULF},
        },
    },
    
    [GASMOG]={
        .Name="Gasmog",
        .Type={WIND, 0},
        .Health=48,
        .Power=90,
        .Attack=90,
        .Defend=40,
        .Evade=105,
        .Luck=55,
        .Rate=FASTEST,
        .Experience=25,
        .Money=5,
        .CatchRate=30,
        .MovesetSize=4,
        .Moveset={
            { 1, SMOG},
            { 4, LURK},
            {10, OBSCURE},
            {25, ENGULF},
        },
    },
    
    [FOGFANG]={
        .Name="FogFang",
        .Type={WIND, 0},
        .Health=70,
        .Power=115,
        .Attack=89,
        .Defend=50,
        .Evade=96,
        .Luck=60,
        .Rate=AVERAGE,
        .Experience=70,
        .Money=7,
        .CatchRate=7,
        .MovesetSize=8,
        .Moveset={
            { 1, BITE},
            { 4, LURK},
            {12, GUST},
            {20, SMOG},
            {26, OBSCURE},
            {32, CYCLONE},
            {40, RECOVER},
            {52, HURRICANE},
        },
    },
    
    [GLACIALITH]={
        .Name="Glacialith",
        .Type={ICE, 0},
        .Health=230,
        .Power=68,
        .Attack=93,
        .Defend=140,
        .Evade=5,
        .Luck=20,
        .Rate=SLOWEST,
        .Experience=155,
        .Money=20,
        .CatchRate=3,
        .MovesetSize=8,
        .Moveset={
            { 1, FLATTEN},
            {12, SHARPEN},
            {20, COLD_GUST},
            {26, FRACTURE},
            {36, CRUSH},
            {44, SOLIDIFY},
            {52, AVALANCHE},
            {64, STRYATION},
        },
    },
    
    [BUTCHERBIRD]={
        .Name="ButcherBird",
        .Type={WIND, METAL},
        .Health=100,
        .Power=80,
        .Attack=121,
        .Defend=130,
        .Evade=71,
        .Luck=58,
        .Rate=SLOW,
        .Experience=110,
        .Money=44,
        .CatchRate=5,
        .MovesetSize=6,
        .Moveset={
            { 1, GUST},
            { 5, SHARPEN},
            {14, IRON_ATTACK},
            {28, SOLIDIFY},
            {36, CYCLONE},
            {48, IRON_HEAVE},
        },
    },
    
    [CUMULUS]={
        .Name="Cumulus",
        .Type={WIND, 0},
        .Health=150,
        .Power=100,
        .Attack=45,
        .Defend=60,
        .Evade=70,
        .Luck=55,
        .Rate=SLOW,
        .Experience=100,
        .Money=35,
        .CatchRate=7,
        .MovesetSize=6,
        .Moveset={
            { 1, GUST},
            {12, COLD_GUST},
            {12, HOT_GUST},
            {22, LIGHTNING},
            {35, CYCLONE},
            {44, THUNDER},
            {56, HURRICANE},
        },
    },
    
    [SSSNAKE]={
        .Name="Sssnake",
        .Type={DARK, 0},
        .Health=70,
        .Power=78,
        .Attack=75,
        .Defend=60,
        .Evade=90,
        .Luck=60,
        .Rate=FAST,
        .Experience=20,
        .Money=1,
        .CatchRate=30,
        .MovesetSize=3,
        .Moveset={
            { 1, BITE},
            { 4, LURK},
            {22, POISON_GRIP},
        },
    },
    
    [DACTYLUS]={
        .Name="Dactylus",
        .Type={WIND, 0},
        .Health=83,
        .Power=80,
        .Attack=94,
        .Defend=66,
        .Evade=102,
        .Luck=70,
        .Rate=AVERAGE,
        .Experience=70,
        .Money=39,
        .CatchRate=15,
        .MovesetSize=6,
        .Moveset={
            { 1, SLICE},
            { 8, LURK},
            {12, GUST},
            {24, PROVOKE},
            {34, CYCLONE},
            {50, FIRE_BREATH},
        },
    },
    
    [MEGATAR]={
        .Name="Megatar",
        .Type={EARTH, 0},
        .Health=110,
        .Power=95,
        .Attack=90,
        .Defend=92,
        .Evade=45,
        .Luck=35,
        .Rate=SLOW,
        .Experience=95,
        .Money=65,
        .CatchRate=10,
        .MovesetSize=8,
        .Moveset={
            { 1, COLLIDE},
            { 8, SMOG},
            {16, FLATTEN},
            {24, SOLIDIFY},
            {32, CRUSH},
            {40, EXPLOSION},
            {50, INFERNO},
            {60, EARTHQUAKE},
        },
    },
    
    [ACISTAR]={
        .Name="Acistar",
        .Type={WATER, 0},
        .Health=92,
        .Power=108,
        .Attack=65,
        .Defend=90,
        .Evade=35,
        .Luck=65,
        .Rate=AVERAGE,
        .Experience=60,
        .Money=12,
        .CatchRate=7,
        .MovesetSize=6,
        .Moveset={
            { 1, COLLIDE},
            {12, SURGE},
            {20, RECOVER},
            {30, CORE_BEAM},
            {42, INKY_BANE},
            {56, INFERNO},
        },
    },
    
    [TARHEAP]={
        .Name="Tarheap",
        .Type={EARTH, 0},
        .Health=127,
        .Power=86,
        .Attack=85,
        .Defend=92,
        .Evade=45,
        .Luck=35,
        .Rate=SLOW,
        .Experience=95,
        .Money=65,
        .CatchRate=10,
        .MovesetSize=7,
        .Moveset={
            { 1, FLATTEN},
            {10, SMOG},
            {24, ENGULF},
            {30, GUNK_ATTACK},
            {40, SOLIDIFY},
            {48, CRUSH},
            {56, EARTHQUAKE},
        },
    },
    
    [GIGACLAM]={
        .Name="Gigaclam",
        .Type={WATER, 0},
        .Health=80,
        .Power=80,
        .Attack=45,
        .Defend=160,
        .Evade=45,
        .Luck=40,
        .Rate=AVERAGE,
        .Experience=60,
        .Money=10,
        .CatchRate=15,
        .MovesetSize=6,
        .Moveset={
            { 1, BITE},
            { 8, PREVENT},
            {16, SURGE},
            {24, SOLIDIFY},
            {34, CRUSH},
            {46, MAELSTROM},
        },
    },
    
    [TOTEM_POLE]={
        .Name="Totem Pole",
        .Type={EARTH, 0},
        .Health=108,
        .Power=110,
        .Attack=78,
        .Defend=132,
        .Evade=22,
        .Luck=80,
        .Rate=SLOWEST,
        .Experience=100,
        .Money=68,
        .CatchRate=3,
        .MovesetSize=6,
        .Moveset={
            { 1, FLATTEN},
            {10, COLLIDE},
            {20, SHARPEN},
            {28, BOMBARD},
            {35, CRUSH},
            {48, TRI_BEAM},
            {56, EARTHQUAKE},
        },
    },
    
    [PRGMERROR]={
        .Name="PrgmERROR",
        .Type={BASIC, 0},
        .Health=64,
        .Power=105,
        .Attack=128,
        .Defend=40,
        .Evade=103,
        .Luck=76,
        .Rate=AVERAGE,
        .Experience=160,
        .Money=190,
        .CatchRate=1,
        .MovesetSize=5,
        .Moveset={
            { 1, ZAP},
            { 1, EXPLOSION},
            { 8, FIREWALL},
            {18, CHARGE},
            {25, OBSCURE},
            {36, DISPEL},
            {45, LIGHTNING},
        },
    },
    
    [PHOENIX]={
        .Name="Phoenix",
        .Type={WIND, FIRE},
        .Health=88,
        .Power=85,
        .Attack=82,
        .Defend=60,
        .Evade=90,
        .Luck=82,
        .Rate=AVERAGE,
        .Experience=65,
        .Money=19,
        .CatchRate=15,
        .MovesetSize=7,
        .Moveset={
            { 1, GUST},
            {12, HOT_GUST},
            {24, RECOVER},
            {30, CYCLONE},
            {38, MANGLE},
            {46, INFERNO},
            {60, HURRICANE},
        },
    },
    
    [RAIKEGON]={
        .Name="Raikegon",
        .Type={LIGHT, WIND},
        .Health=90,
        .Power=110,
        .Attack=90,
        .Defend=65,
        .Evade=104,
        .Luck=90,
        .Rate=SLOW,
        .Experience=110,
        .Money=85,
        .CatchRate=1,
        .MovesetSize=7,
        .Moveset={
            { 1, ZAP},
            { 8, GUST},
            {12, CHARGE},
            {19, FIRE_BREATH},
            {24, PROVOKE},
            {29, LIGHTNING},
            {34, CYCLONE},
            {52, THUNDER},
        },
    },
    
    [BRONTO]={
        .Name="Bronto",
        .Type={BASIC, 0},
        .Health=180,
        .Power=70,
        .Attack=113,
        .Defend=87,
        .Evade=35,
        .Luck=50,
        .Rate=SLOW,
        .Experience=100,
        .Money=90,
        .CatchRate=5,
        .MovesetSize=5,
        .Moveset={
            { 1, FLATTEN},
            {10, PREVENT},
            {18, RAMPAGE},
            {29, CRUSH},
            {40, EARTHQUAKE},
        },
    },
    
    [TOOLFISH]={
        .Name="Toolfish",
        .Type={WATER, METAL},
        .Health=78,
        .Power=70,
        .Attack=90,
        .Defend=90,
        .Evade=80,
        .Luck=72,
        .Rate=AVERAGE,
        .Experience=50,
        .Money=25,
        .CatchRate=15,
        .MovesetSize=6,
        .Moveset={
            { 1, SWIM_ATTACK},
            { 1, IRON_ATTACK},
            { 8, SHARPEN},
            {24, SLICE},
            {32, RECYCLE},
            {40, RIPTIDE},
        },
    },
    
    [WATERWING]={
        .Name="WaterWing",
        .Type={WATER, 0},
        .Health=75,
        .Power=110,
        .Attack=50,
        .Defend=60,
        .Evade=89,
        .Luck=84,
        .Rate=AVERAGE,
        .Experience=50,
        .Money=12,
        .CatchRate=10,
        .MovesetSize=6,
        .Moveset={
            { 1, GUST},
            {10, PURGE},
            {18, SURGE},
            {24, CYCLONE},
            {32, RECOVER},
            {40, MAELSTROM},
        },
    },
    
    [VARAN]={
        .Name="Varan",
        .Type={BASIC, 0},
        .Health=255,
        .Power=255,
        .Attack=170,
        .Defend=170,
        .Evade=99,
        .Luck=140,
        .Rate=SLOWEST,
        .Experience=255,
        .Money=255,
        .CatchRate=0,
        .MovesetSize=6,
        .Moveset={
            { 1, SMOG},
            {20, MANGLE},
            {38, MIND_SCREW},
            {52, INKY_BANE},
            {65, INFERNO},
            {79, THUNDER},
        },
    },
    
    [SPACESNAKE]={
        .Name="SpaceSnake",
        .Type={DARK, 0},
        .Health=82,
        .Power=100,
        .Attack=95,
        .Defend=70,
        .Evade=90,
        .Luck=65,
        .Rate=AVERAGE,
        .Experience=90,
        .Money=63,
        .CatchRate=10,
        .MovesetSize=6,
        .Moveset={
            { 1, BITE},
            { 8, LURK},
            {18, FIRE_BREATH},
            {28, PURGE},
            {44, MANGLE},
            {52, POISON_GRIP},
        },
    },
    
    [VACUUM]={
        .Name="Vacuum",
        .Type={METAL, WIND},
        .Health=72,
        .Power=80,
        .Attack=55,
        .Defend=85,
        .Evade=60,
        .Luck=30,
        .Rate=FAST,
        .Experience=30,
        .Money=13,
        .CatchRate=25,
        .MovesetSize=5,
        .Moveset={
            { 1, GUST},
            { 6, COLLIDE},
            {14, IRON_ATTACK},
            {24, CYCLONE},
            {50, HURRICANE},
        },
    },
    
    [AMY]={
        .Name="Amy",
        .Type={BASIC, 0},
        .Health=80,
        .Power=80,
        .Attack=81,
        .Defend=59,
        .Evade=100,
        .Luck=150,
        .Rate=FASTEST,
        .Experience=0,
        .Money=0,
        .CatchRate=0,
        .MovesetSize=1,
        .Moveset={
            { 1, CAPTURE},
        },
    },
    
    [ASTEROID]={
        .Name="Asteroid",
        .Type={EARTH, ICE},
        .Health=80,
        .Power=83,
        .Attack=88,
        .Defend=90,
        .Evade=97,
        .Luck=45,
        .Rate=AVERAGE,
        .Experience=70,
        .Money=52,
        .CatchRate=7,
        .MovesetSize=5,
        .Moveset={
            { 1, COLLIDE},
            { 8, FLATTEN},
            {18, BOMBARD},
            {30, EXPLOSION},
            {45, EARTHQUAKE},
        },
    },
    
    [BASSMONSTER]={
        .Name="BassMonster",
        .Type={WATER, 0},
        .Health=93,
        .Power=95,
        .Attack=65,
        .Defend=80,
        .Evade=65,
        .Luck=97,
        .Rate=AVERAGE,
        .Experience=40,
        .Money=12,
        .CatchRate=20,
        .MovesetSize=6,
        .Moveset={
            { 1, SWIM_ATTACK},
            {12, PURGE},
            {24, SURGE},
            {32, RECOVER},
            {40, HYPNOSIS},
            {50, MAELSTROM},
        },
    },
    
    [CATFISH]={
        .Name="Catfish",
        .Type={WATER, 0},
        .Health=60,
        .Power=75,
        .Attack=45,
        .Defend=59,
        .Evade=106,
        .Luck=55,
        .Rate=FASTEST,
        .Experience=25,
        .Money=6,
        .CatchRate=25,
        .MovesetSize=5,
        .Moveset={
            { 1, SWIM_ATTACK},
            { 8, DISARM},
            {15, LURK},
            {26, SURGE},
            {40, MAELSTROM},
        },
    },
    
    [ICEBOULDER]={
        .Name="IceBoulder",
        .Type={ICE, 0},
        .Health=80,
        .Power=97,
        .Attack=45,
        .Defend=135,
        .Evade=30,
        .Luck=50,
        .Rate=SLOW,
        .Experience=85,
        .Money=19,
        .CatchRate=5,
        .MovesetSize=6,
        .Moveset={
            { 1, FLATTEN},
            {10, COLD_GUST},
            {22, PREVENT},
            {30, AVALANCHE},
            {40, EXPLOSION},
            {50, SOLIDIFY},
        },
    },
    
    [LEKTRON]={
        .Name="Lektron",
        .Type={LIGHT, 0},
        .Health=70,
        .Power=112,
        .Attack=45,
        .Defend=60,
        .Evade=150,
        .Luck=55,
        .Rate=FAST,
        .Experience=50,
        .Money=35,
        .CatchRate=7,
        .MovesetSize=5,
        .Moveset={
            { 1, ZAP},
            { 8, CHARGE},
            {20, LIGHTNING},
            {32, AILMENT},
            {50, THUNDER},
        },
    },
    
    [TELEVIRUS]={
        .Name="Televirus",
        .Type={DARK, 0},
        .Health=60,
        .Power=100,
        .Attack=40,
        .Defend=50,
        .Evade=140,
        .Luck=90,
        .Rate=FASTEST,
        .Experience=40,
        .Money=10,
        .CatchRate=7,
        .MovesetSize=5,
        .Moveset={
            { 1, OBSCURE},
            {10, MIND_SCREW},
            {20, HYPNOSIS},
            {28, MIND_RAY},
            {36, AILMENT},
        },
    },
    
    [VOLTDRAGON]={
        .Name="VoltDragon",
        .Type={METAL, LIGHT},
        .Health=98,
        .Power=80,
        .Attack=100,
        .Defend=112,
        .Evade=60,
        .Luck=85,
        .Rate=SLOW,
        .Experience=90,
        .Money=56,
        .CatchRate=7,
        .MovesetSize=8,
        .Moveset={
            { 1, BITE},
            { 8, ZAP},
            {15, DISARM},
            {24, LIGHTNING},
            {30, CHARGE},
            {40, MANGLE},
            {48, PROVOKE},
            {56, THUNDER},
        },
    },
    
    [KARDA]={
        .Name="Karda",
        .Type={DARK, WIND},
        .Health=50,
        .Power=110,
        .Attack=80,
        .Defend=40,
        .Evade=120,
        .Luck=121,
        .Rate=FASTEST,
        .Experience=50,
        .Money=12,
        .CatchRate=10,
        .MovesetSize=5,
        .Moveset={
            { 1, GUST},
            { 8, OBSCURE},
            {15, PROVOKE},
            {20, POISON_CLAW},
            {28, DISARM},
        },
    },
};

/**************************************************************/
const SPECIES *SpeciesByID(SPECIES_ID id) {
    return &SPECIES_DATA[id];
}

const SPECIES *SpeciesOfSpectra(const SPECTRA *spectra) {
    return &SPECIES_DATA[spectra->Species];
}

const SPECIES *SpeciesOfBattler(const BATTLER *battler) {
    return &SPECIES_DATA[battler->Spectra->Species];
}

/**************************************************************/
#endif // _SPECIES_I_
