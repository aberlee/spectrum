/**********************************************************//**
 * @file shop.h
 * @brief Header for shop information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _SHOP_H_
#define _SHOP_H_

#include <stdbool.h>        // bool
#include "item.h"           // ITEM, ITEM_ID

/**********************************************************//**
 * @enum SHOP_ID
 * @brief Identifiers for each kind of shop the player can
 * use.
 **************************************************************/
typedef enum {
    VENDING_MACHINE=1,
    HERB_SHOP,
    ITALIAN_RESTAURANT,
    HEALTHY_FOOD,
    FAST_FOOD,
    MEDICAL_STORE,
    CONVENIENCE_STORE,
} SHOP_ID;

/// Defines a size large enough for an array to hold each SHOP_ID.
#define N_SHOP (CONVENIENCE_STORE+1)

/**********************************************************//**
 * @struct SHOP
 * @brief Contains a shop's inventory.
 **************************************************************/
typedef struct {
    ITEM_ID Inventory[10];
} SHOP;

/**************************************************************/
#endif // _SHOP_H_
