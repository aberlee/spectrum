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
    SERUM           =  1,
    SHOVEL          =  3,
    WET_TOWEL       =  4,
    REFRESHER       =  6,

    // Foods
    CHOCOLATE       =  7,
    YOGURT          = 26,
    SANDWICH        = 10,
    BURGER          = 11,
    STEAK           = 12,
    SALAD           = 13,
    SPAGHETTI       = 14,
    JERKEY          = 17,
    PIE             = 16,
    FULL_MEAL       = 18,

    // Drinks
    WATER_BOTTLE    = 29,
    JUICE           =  9,
    SODA            = 27,
    COFFEE          =  8,
    TEA             = 15,
    POTION          = 19,

    // Herbs
    STRONG_HERB     = 31,
    TOUGH_HERB      = 32,
    QUICK_HERB      = 33,
    LUCK_HERB       = 25,

    // Key items
    FISHING_ROD     = 39,
    
    // Clothes
    ITEM_BLACK_DRESS= 40,
    ITEM_RED_DRESS  = 41,
    ITEM_CLOTHES    = 42,
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
    IMPORTANT       = 0x0008,
} ITEM_FLAGS;

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
    const char *Description;    ///< Text description of the item.
} ITEM;

/**************************************************************/
extern const ITEM *ItemByID(ITEM_ID id);

/**************************************************************/
#endif // _ITEM_H_
