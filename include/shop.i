/**********************************************************//**
 * @file shop.i
 * @brief Database for shop information.
 * @author Rena Shinomiya
 * @date April 5th, 2018
 **************************************************************/

#ifndef _SHOP_I_
#define _SHOP_I_

#include "shop.h"       // SHOP, SHOP_ID
#include "item.h"       // ITEM, ITEM_ID

const SHOP SHOP_DATA[N_SHOP] = {
    
    [VENDING_MACHINE]={
        .Inventory={
            WATER_BOTTLE,
            SODA,
            TEA,
            JUICE,
            COFFEE,
        },
    },
    
    [HERB_SHOP]={
        .Inventory={
            STRONG_HERB,
            TOUGH_HERB,
            QUICK_HERB,
            LUCK_HERB,
            SERUM,
            POTION,
            REFRESHER,
        },
    },
    
    [ITALIAN_RESTAURANT]={
        .Inventory={
            FULL_MEAL,
            SPAGHETTI,
            SALAD,
            WATER_BOTTLE,
        },
    },
    
    [HEALTHY_FOOD]={
        .Inventory={
            FULL_MEAL,
            SANDWICH,
            SALAD,
            YOGURT,
            WATER_BOTTLE,
            TEA,
        },
    },
    
    [FAST_FOOD]={
        .Inventory={
            BURGER,
            STEAK,
            SANDWICH,
            PIE,
            SODA,
            JUICE,
        },
    },
    
    [MEDICAL_STORE]={
        .Inventory={
            POTION,
            REFRESHER,
            SERUM,
            WATER_BOTTLE,
            
        },
    },
    
    [CONVENIENCE_STORE]={
        .Inventory={
            SHOVEL,
            FISHING_ROD,
            SANDWICH,
            JERKEY,
            CHOCOLATE,
            YOGURT,
        },
    },
};

/**************************************************************/
#endif // _SHOP_I_
