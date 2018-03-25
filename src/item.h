/**********************************************************//**
 * @file item.h
 * @brief Defines a constant for each kind of item.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _ITEM_H_
#define _ITEM_H_

#include "effect.h"     // EFFECT_ID

/**********************************************************//**
 * @enum ITEM_ID
 * @brief Defines a constant for each unique item in the game.
 * 0 is invalid and represents null.
 **************************************************************/
typedef enum {
    // Cure-based items
    SERUM=1,
    SHOVEL,
    WET_TOWEL,
    REFRESHER,
    
    // Foods and drinks
    CHOCOLATE,
    COFFEE,
    JUICE,
    SANDWICH,
    BURGER,
    STEAK,
    SALAD,
    SPAGHETTI,
    TEA,
    PIE,
    JERKEY,
    FULL_MEAL,
    POTION,
    YOGURT,
    SODA,
    WATER_BOTTLE,
    
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
    MENU_USE        = 0x0002,
    BATTLE_USE      = 0x0004,
} ITEM_FLAGS;

/**********************************************************//**
 * @struct ITEM
 * @brief Defines constant data for each item in the game.
 **************************************************************/
typedef struct {
    char Name[14];
    int Price;
    EFFECT_ID Effect;
    int Argument;
    ITEM_FLAGS Flags;
    char Description[128];
} ITEM;

/**************************************************************/
#endif // _ITEM_H_
