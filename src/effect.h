/**********************************************************//**
 * @file effect.h
 * @brief Lists all the effects that take place during battle
 * or through the menu.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _EFFECT_H_
#define _EFFECT_H_

/**********************************************************//**
 * @enum TARGET_TYPE
 * @brief Defines a constant for each different target a
 * technique or item can be used on in battle (0 is invalid).
 **************************************************************/
typedef enum {
    YOURSELF=1,         ///< Hits the user of the technique.
    ENEMY,              ///< Hits one enemy.
    ALLY,               ///< Hits one teammate, or the user.
    ALL_ENEMIES,        ///< Hits each enemy.
    EVERYONE_ELSE,      ///< Hits everyone but the user.
} TARGET_TYPE;

/**********************************************************//**
 * @enum EFFECT_ID
 * @brief Defines a constant for each different effect that
 * can be caused by a technique or item. 0 is not defined, and
 * denotes that there is no effect to be performed.
 **************************************************************/
typedef enum {
    // Ailment afflict
    AFFLICT_BURY=1,     ///< Buries the target in the ground. Expects a chance percentage.
    AFFLICT_AFLAME,     ///< Sets the target on fire. Expects a chance percentage.
    AFFLICT_POISON,     ///< Poisons the target. Expects a chance percentage.
    AFFLICT_SHOCK,      ///< Shocks the target. Expects a chance percentage.
    AFFLICT_SLEEP,      ///< Puts the target to sleep. Expects a chance percentage.
    AFFLICT_ANY,        ///< Inflicts a random status condition on the target.
    
    // Ailment cure
    CURE_BURY,          ///< Cures the "Buried" condition.
    CURE_AFLAME,        ///< Cures the "Aflame" condition.
    CURE_POISON,        ///< Cures the "Poison" condition.
    CURE_SHOCK,         ///< Cures the "Shock" condition.
    CURE_SLEEP,         ///< Cures the "Sleep" condition.
    CURE_ANY,           ///< Cures any status ailment.
    
    // Heal health
    HEAL_CONSTANT,      ///< Heals a fixed amount. Expects the amount.
    HEAL_PERCENT,       ///< Heals by percentage of Max. HP. Expects the percentage.    
    
    // Stat boosts
    RESET_STATS,        ///< Removes all stat boosts.
    BOOST_ATTACK,       ///< Raises the "Attack" stat. Expects a level.
    BOOST_DEFEND,       ///< Raises the "Defend" stat. Expects a level.
    BOOST_EVADE,        ///< Raises the "Evade" stat. Expects a level.
    BOOST_LUCK,         ///< Raises the "Luck" stat. Expects a level.
    
    // Special
    CAPTURE_SPECTRA,    ///< Attempts to capture an enemy in battle.
    DIE,                ///< The user dies after attacking.
} EFFECT_ID;

/**************************************************************/
#endif
