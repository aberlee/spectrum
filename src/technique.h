/**********************************************************//**
 * @file technique.h
 * @brief Lists all the techniques usable by monsters.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _TECHNIQUE_H_
#define _TECHNIQUE_H_

#include "type.h"               // TYPE_ID

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
 * @enum TARGET_TYPE
 * @brief Defines a constant for each different target a
 * technique or item can be used on in battle (0 is invalid).
 **************************************************************/
typedef enum {
    YOURSELF        = 0x0001,  ///< Hits the user of the technique.
    TARGET_ENEMY    = 0x0100,  ///< Hits one enemy.
    ALLY            = 0x0011,  ///< Hits one teammate, or the user.
    ANYONE          = 0x0111,  ///< Hits any one spectra.
    ALL_ENEMIES     = 0x1100,  ///< Hits each enemy.
    EVERYONE_ELSE   = 0x1110,  ///< Hits everyone but the user.
} TARGET_TYPE;

/**************************************************************/
/// @brief Does the targetting group include the user?
#define TARGET_USER 0x000F

/// @brief Does the targetting group include enemies?
#define TARGET_ENEMY 0x0F00

/// @brief Does the targetting group include allies?
#define TARGET_ALLY 0x00F0

/// @brief Does the targetting group target everyone as a whole?
#define TARGET_GROUP 0xF000

/**********************************************************//**
 * @enum EFFECT_ID
 * @brief Defines a constant for each different effect that
 * can be caused by a technique or item. 0 is not defined, and
 * denotes that there is no effect to be performed.
 **************************************************************/
typedef enum {
    // Ailment afflict
    AFFLICT_BURY    =  1,       ///< Buries the target in the ground. Expects a chance percentage.
    AFFLICT_AFLAME,             ///< Sets the target on fire. Expects a chance percentage.
    AFFLICT_POISON,             ///< Poisons the target. Expects a chance percentage.
    AFFLICT_SHOCK,              ///< Shocks the target. Expects a chance percentage.
    AFFLICT_SLEEP,              ///< Puts the target to sleep. Expects a chance percentage.
    AFFLICT_ANY,                ///< Inflicts a random status condition on the target.

    // Ailment cure
    CURE_BURY,                  ///< Cures the "Buried" condition.
    CURE_AFLAME,                ///< Cures the "Aflame" condition.
    CURE_POISON,                ///< Cures the "Poison" condition.
    CURE_SHOCK,                 ///< Cures the "Shock" condition.
    CURE_SLEEP,                 ///< Cures the "Sleep" condition.
    CURE_ANY,                   ///< Cures any status ailment.

    // Heal health
    HEAL_CONSTANT,              ///< Heals a fixed amount. Expects the amount.
    HEAL_PERCENT,               ///< Heals by percentage of Max. HP. Expects the percentage.

    // Stat boosts
    RESET_STATS,                ///< Removes all stat boosts.
    BOOST_ATTACK,               ///< Raises the "Attack" stat. Expects a level.
    BOOST_DEFEND,               ///< Raises the "Defend" stat. Expects a level.
    BOOST_EVADE,                ///< Raises the "Evade" stat. Expects a level.
    BOOST_LUCK,                 ///< Raises the "Luck" stat. Expects a level.

    // Special
    KILL_USER,                  ///< The user dies after attacking.
    EFFECT_CAPTURE,             ///< Attempts to capture an enemy in battle.
    EFFECT_DEFEND,              ///< User is defending.
    EFFECT_ITEM,                ///< User using item.
    EFFECT_ESCAPE,              ///< Trying to escape battle.
} EFFECT_ID;

/**********************************************************//**
 * @enum TECHNIQUE_FLAGS
 * @brief Implements additional flags on techniques.
 **************************************************************/
typedef enum {
    TECHNIQUE_EFFECT_ONCE   = 0x0001,   ///< Effect activates after all group is hit.
} TECHNIQUE_FLAGS;

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
    TECHNIQUE_FLAGS Flags;      ///< Additional flags on the technique.
    const char *Description;    /// Describes the technique on the menu.
} TECHNIQUE;

/**************************************************************/
extern const TECHNIQUE *TechniqueByID(TECHNIQUE_ID id);

/**************************************************************/
#endif // _TECHNIQUE_H_
