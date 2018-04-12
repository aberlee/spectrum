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
    // Basic techniques
    BITE            = 1,
    COLLIDE,
    SLICE,
    RAMPAGE,
    ENGULF,
    MANGLE,
    BOMBARD,
    TRI_BEAM,
    FLATTEN,
    CRUSH,
    RECOVER,
    PURGE,
    DISPEL,
    SHARPEN,
    SNIPER,
    PREVENT,
    DISARM,
    LURK,
    PROVOKE,
    EXPLOSION,
    MIND_RAY,
    BRAINWAVE,
    HYPNOSIS,
    CAPTURE,
    AILMENT,

    // Wind techniques
    GUST,
    CYCLONE,
    HURRICANE,

    // Fire techniques
    HOT_GUST,
    FIRE_BREATH,
    INFERNO,
    FIREWALL,

    // Water techniques
    SWIM_ATTACK,
    RIPTIDE,
    MAELSTROM,
    SURGE,
    TSUNAMI,

    // Dark techniques
    GUNK_ATTACK,
    POISON_CLAW,
    POISON_GRIP,
    INKY_BANE,
    SMOG,
    OBSCURE,
    MIND_SCREW,

    // Light techniques
    ZAP,
    LIGHTNING,
    THUNDER,
    CHARGE,

    // Ice techniques
    COLD_GUST,
    AVALANCHE,

    // Earth techniques
    FRACTURE,
    EARTHQUAKE,
    STRYATION,
    SOLIDIFY,

    // Metal techniques
    CORE_BEAM,
    IRON_ATTACK,
    IRON_HEAVE,
    RECYCLE,
} TECHNIQUE_ID;

/// A size large enough to contain every TECHNIQUE_ID in an array.
#define N_TECHNIQUES (RECYCLE+1)

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
