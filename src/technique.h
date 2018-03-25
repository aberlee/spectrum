/**********************************************************//**
 * @file technique.h
 * @brief Lists all the techniques usable by monsters.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _TECHNIQUE_H_
#define _TECHNIQUE_H_

#include "type.h"           // TYPE_ID
#include "effect.h"         // TARGET_ID, EFFECT_ID

/**********************************************************//**
 * @enum TECHNIQUE_ID
 * @brief Defines a constant for each different technique
 * that can be learned by a monster (0 is invalid).
 **************************************************************/
typedef enum {
    // Basic techniques
    BITE=1,
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
    
    // Wind techniques
    GUST,
    CYCLONE,
    HURRICANE,
    
    // Fire techniques
    HOT_GUST,
    FIREWALL,
    INFERNO,
    
    // Water techniques
    SWIM_ATTACK,
    SURGE,
    RIPTIDE,
    MAELSTROM,
    TSUNAMI,
    
    // Dark techniques
    POISON_CLAW,
    INKY_BANE,
    GUNK_ATTACK,
    POISON_GRIP,
    SMOG,
    
    // Light techniques
    ZAP,
    LIGHTNING,
    THUNDER,
    CHARGE,
    
    // Ice techniques
    COLD_GUST,
    AVALANCHE,
    
    // Mind techniques
    MIND_RAY,
    BRAINWAVE,
    HYPNOSIS,
    MIND_SCREW,
    OBSCURE,
    
    // Earth techniques
    FRACTURE,
    EARTHQUAKE,
    STRYATION,
    
    // Metal techniques
    CORE_BEAM,
    IRON_ATTACK,
    IRON_HEAVE,
    SOLIDIFY,
    
    // Extra techniques
    CAPTURE,
    AILMENT,
    FIRE_BREATH,
    RECYCLE,
} TECHNIQUE_ID;

/// The number of unique techniques.
#define N_TECHNIQUES (RECYCLE+1)

/**********************************************************//**
 * @struct TECHNIQUE
 * @brief Stores all constant data about one technique
 * specified with a TECHNIQUE_ID.
 **************************************************************/
typedef struct {
    char Name[13];          ///< The name of the technique.
    TYPE_ID Type;           ///< The type of the technique.
    int Power;              ///< Technique power (0 is none).
    int Cost;               ///< Cost of the technique (0 is free).
    TARGET_TYPE Target;     ///< Who the technique targets.
    EFFECT_ID Effect;       ///< Effect on the targets.
    int Argument;           ///< Argument to the effect.
    
    /// Describes the technique on the menu.
    const char Description[128];
} TECHNIQUE;

/// Defines all the techniques in the game.
extern const TECHNIQUE *TECHNIQUE_DATA;

/**************************************************************/
#endif // _TECHNIQUE_H_
