/**********************************************************//**
 * @file shop.h
 * @brief Header for shop information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _SHOP_H_
#define _SHOP_H_

#include <stdbool.h>

#include "item.h"       // ITEM, ITEM_ID

typedef enum {
    VENDING_MACHINE=1,
    HERB_SHOP,
    ITALIAN_RESTAURANT,
    HEALTHY_FOOD,
    FAST_FOOD,
    MEDICAL_STORE,
    CONVENIENCE_STORE,
} SHOP_ID;

#define N_SHOP (CONVENIENCE_STORE+1)

typedef struct {
    ITEM_ID Inventory[10];
} SHOP;

/**************************************************************/
#endif // _SHOP_H_
