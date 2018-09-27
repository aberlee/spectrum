/**********************************************************//**
 * @file shop.h
 * @brief Header for shop information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _SHOP_H_
#define _SHOP_H_

#include <stdbool.h>            // bool

#include "item.h"               // ITEM, ITEM_ID
#include "menu.h"               // MENU_MAX_OPTION

/**********************************************************//**
 * @enum SHOP_ID
 * @brief Identifiers for each kind of shop the player can
 * use.
 **************************************************************/
typedef enum {
    VENDING_MACHINE     = 1,
    HERB_SHOP,
    ITALIAN_RESTAURANT,
    HEALTHY_FOOD,
    FAST_FOOD,
    MEDICAL_STORE,
    CONVENIENCE_STORE,
} SHOP_ID;

/// Defines a size large enough for an array to hold each SHOP_ID.
#define N_SHOP 8

/// The maximum number of items a shop can sell.
#define SHOP_SIZE MENU_MAX_OPTION

/**********************************************************//**
 * @struct SHOP
 * @brief Contains a shop's inventory.
 **************************************************************/
typedef struct {
    ITEM_ID Inventory[SHOP_SIZE];       ///< Items the shop sells.
} SHOP;

/**************************************************************/
extern const SHOP *ShopByID(SHOP_ID id);
extern void InitializeShop(SHOP_ID id);
extern void DrawShop(void);
extern void UpdateShop(void);
extern bool ShopDone(void);

/**************************************************************/
#endif // _SHOP_H_
