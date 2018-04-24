/**********************************************************//**
 * @file item.h
 * @brief Defines a constant for each kind of item.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _ITEM_H_
#define _ITEM_H_

#include "effect.h"             // EFFECT_ID

/**********************************************************//**
 * @enum ITEM_ID
 * @brief Defines a constant for each unique item in the game.
 * 0 is invalid and represents null.
 **************************************************************/
typedef enum {
    // Cure-based items
    SERUM           = 1,
    SHOVEL,
    WET_TOWEL,
    REFRESHER,

    // Foods
    CHOCOLATE,
    YOGURT,
    SANDWICH,
    BURGER,
    STEAK,
    SALAD,
    SPAGHETTI,
    JERKEY,
    PIE,
    FULL_MEAL,

    // Drinks
    WATER_BOTTLE,
    JUICE,
    SODA,
    COFFEE,
    TEA,
    POTION,

    // Herbs
    STRONG_HERB,
    TOUGH_HERB,
    QUICK_HERB,
    LUCK_HERB,

    // Key items
    FISHING_ROD,
} ITEM_ID;

/**********************************************************//**
 * @enum ITEM_FLAGS
 * @brief Boolean properties that can be combined using bitwise
 * OR to form a bitset. Used in the ITEM data.
 **************************************************************/
typedef enum {
    REUSABLE        = 0x0001,
    MENU_ONLY       = 0x0002,
    BATTLE_ONLY     = 0x0004,
    ANYWHERE        = 0x0006,
    IMPORTANT       = 0x0009,
    SPECIAL         = 0x0010,
} ITEM_FLAGS;

typedef enum {
    FISHING         = 1,
} SPECIAL_ID;

//typedef enum {
//
//} BATTLE_SPECIAL_HOOK;

/**********************************************************//**
 * @struct ITEM
 * @brief Defines constant data for each item in the game.
 **************************************************************/
typedef struct {
    char Name[13];              ///< Name of the item.
    int Price;                  ///< Cost of the item.
    EFFECT_ID Effect;           ///< Battle effect.
    int Argument;               ///< Argument to the effect.
    ITEM_FLAGS Flags;           ///< Bitset of ITEM_FLAGS.
    SPECIAL_ID SpecialEffect;   ///< Special effect ID flag.
    const char *Description;    ///< Text description of the item.
} ITEM;

/**************************************************************/
extern const ITEM *ItemByID(ITEM_ID id);

/**************************************************************/
#endif // _ITEM_H_
