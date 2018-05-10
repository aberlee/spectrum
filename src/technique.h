/**********************************************************//**
 * @file technique.h
 * @brief Lists all the techniques usable by monsters.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _TECHNIQUE_H_
#define _TECHNIQUE_H_

#include "type.h"               // TYPE_ID
#include "effect.h"             // TARGET_ID, EFFECT_ID

/**********************************************************//**
 * @enum TECHNIQUE_ID
 * @brief Defines a constant for each different technique
 * that can be learned by a monster (0 is invalid).
 **************************************************************/
typedef enum {
    // Default techniques
    DEFAULT_ATTACK  = 81,
    DEFAULT_DEFEND  = 82,
    DEFAULT_ITEM    = 83,
    DEFAULT_ESCAPE  = 84,
    
    // Basic techniques
    BITE            =  1,
    COLLIDE         =  2,
    SLICE           =  3,
    RAMPAGE         =  4,
    ENGULF          =  5,
    MANGLE          =  6,
    BOMBARD         =  8,
    TRI_BEAM        =  9,
    FLATTEN         = 10,
    CRUSH           = 11,
    RECOVER         = 12,
    PURGE           = 13,
    DISPEL          = 14,
    SHARPEN         = 16,
    SNIPER          = 18,
    PREVENT         = 19,
    DISARM          = 21,
    LURK            = 22,
    PROVOKE         = 23,
    EXPLOSION       = 24,
    MIND_RAY        = 56,
    BRAINWAVE       = 57,
    HYPNOSIS        = 59,
    CAPTURE         = 72,
    AILMENT         = 73,

    // Wind techniques
    GUST            = 25,
    CYCLONE         = 26,
    HURRICANE       = 28,

    // Fire techniques
    HOT_GUST        = 31,
    FIRE_BREATH     = 79,
    INFERNO         = 36,
    FIREWALL        = 34,

    // Water techniques
    SWIM_ATTACK     = 37,
    RIPTIDE         = 39,
    MAELSTROM       = 40,
    SURGE           = 38,
    TSUNAMI         = 41,

    // Dark techniques
    GUNK_ATTACK     = 44,
    POISON_CLAW     = 42,
    POISON_GRIP     = 45,
    INKY_BANE       = 43,
    SMOG            = 46,
    OBSCURE         = 61,
    MIND_SCREW      = 60,

    // Light techniques
    ZAP             = 47,
    LIGHTNING       = 50,
    THUNDER         = 51,
    CHARGE          = 52,

    // Ice techniques
    COLD_GUST       = 53,
    AVALANCHE       = 55,

    // Earth techniques
    FRACTURE        = 62,
    EARTHQUAKE      = 63,
    STRYATION       = 64,
    SOLIDIFY        = 69,

    // Metal techniques
    CORE_BEAM       = 65,
    IRON_ATTACK     = 67,
    IRON_HEAVE      = 68,
    RECYCLE         = 80,
} TECHNIQUE_ID;

/// A size large enough to contain every TECHNIQUE_ID in an array.
#define N_TECHNIQUES 85

/**********************************************************//**
 * @struct TECHNIQUE
 * @brief Stores all constant data about one technique
 * specified with a TECHNIQUE_ID.
 **************************************************************/
typedef struct {
    char Name[13];              ///< The name of the technique.
    TYPE_ID Type;               ///< The type of the technique.
    int Power;                  ///< Technique power (0 is none).
    int Cost;                   ///< Cost of the technique (0 is free).
    TARGET_TYPE Target;         ///< Who the technique targets.
    EFFECT_ID Effect;           ///< Effect on the targets.
    int Argument;               ///< Argument to the effect.
    const char *Description;    /// Describes the technique on the menu.
} TECHNIQUE;

/**************************************************************/
extern const TECHNIQUE *TechniqueByID(TECHNIQUE_ID id);

/**************************************************************/
#endif // _TECHNIQUE_H_
