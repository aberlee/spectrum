/**********************************************************//**
 * @file shop.c
 * @brief Implements the shop system logic and rendering.
 * @author Rena Shinomiya
 * @date September 27, 2018
 **************************************************************/

#include <stdbool.h>            // bool
#include <stdio.h>              // snprintf

#include "shop.h"               // SHOP_ID
#include "item.h"               // ITEM_ID
#include "menu.h"               // MENU
#include "player.h"             // GetItem
#include "output.h"             // Output

#include "shop.i"               // SHOP_DATA

/**************************************************************/
/// ID of the current shop being used.
static SHOP_ID CurrentShopID = 0;

static MENU ShopItems;
static MENU ShopPrices;

static MENU BuyMenu = {
    .Option = {
        "Buy",
        "Exit",
    },
    .Control = {
        .IndexMax       = 1,
    },
};

/**********************************************************//**
 * @brief Gets the SHOP data for a given SHOP_ID.
 * @param id: Identity of the shop.
 * @return Pointer to SHOP data.
 **************************************************************/
const SHOP *ShopByID(SHOP_ID id) {
    return &SHOP_DATA[id];
}

/**********************************************************//**
 * @brief Gets the item currently selected on the menu.
 * @return ITEM_ID
 **************************************************************/
static inline ITEM_ID SelectedItemID(void) {
    const SHOP *shop = ShopByID(CurrentShopID);
    return shop->Inventory[MenuItem(&ShopItems)];
}

/**********************************************************//**
 * @brief Initialize the shop system.
 * @param id: The shop to use.
 **************************************************************/
void InitializeShop(SHOP_ID id) {
    static char PriceString[SHOP_SIZE][10];
    
    // Populate the shop menus
    CurrentShopID = id;
    const SHOP *shop = ShopByID(id);
    int i = 0;
    for (i=0; i<SHOP_SIZE && shop->Inventory[i]; i++) {
        const ITEM *item = ItemByID(shop->Inventory[i]);
        ShopItems.Option[i] = item->Name;
        snprintf(PriceString[i], 10, "$%d", item->Price);
        ShopPrices.Option[i] = PriceString[i];
    }
    ShopItems.Option[i] = NULL;
    ShopPrices.Option[i] = NULL;
    
    // Initialize shop menus
    InitializeMenuScroll(&ShopItems, MENU_COLUMN);
    InitializeMenuScroll(&ShopPrices, MENU_COLUMN);
    ResetMenu(&ShopItems);
    ResetMenu(&ShopPrices);
}

/**********************************************************//**
 * @brief Draws the shop system menus.
 **************************************************************/
void DrawShop(void) {
    DrawAt(10, 10);
    DrawColumn(&ShopItems, &ShopPrices);
    if (MenuConfirmed(&ShopItems)) {
        DrawAt(18, 18);
        DrawChoice(&BuyMenu);
    }
    
    DrawAt(10, 125);
    DrawItemDisplay(SelectedItemID());
}

/**********************************************************//**
 * @brief Updates the shop buy/exit menu.
 **************************************************************/
static void UpdateShopBuy(void) {
    switch (MenuState(&BuyMenu)) {
    case CONTROL_CONFIRM:
        if (MenuItem(&BuyMenu) == 0) {
            GetItem(SelectedItemID());
            const ITEM *item = ItemByID(SelectedItemID());
            OutputF("Bought %s!", item->Name);
        }
        ResetMenuToIdle(&ShopItems);
        break;

    case CONTROL_CANCEL:
        ResetMenuToIdle(&ShopItems);
        break;
    
    case CONTROL_IDLE:
        UpdateMenu(&BuyMenu);
        break;
    }
}

/**********************************************************//**
 * @brief Updates shop processing.
 **************************************************************/
void UpdateShop(void) {
    if (InventoryFull()) {
        Output("Your inventory is full!");
        CurrentShopID = 0;
        return;
    }
    
    switch (MenuState(&ShopItems)) {
    case CONTROL_CONFIRM:
        UpdateShopBuy();
        break;

    case CONTROL_CANCEL:
        // Leave the shop system
        CurrentShopID = 0;
        break;
    
    case CONTROL_IDLE:
        UpdateMenu(&ShopItems);
        if (MenuConfirmed(&ShopItems)) {
            ResetMenu(&BuyMenu);
        }
        break;
    }
}

/**********************************************************//**
 * @brief Determine if the shop system is complete.
 * @return True if the shop system is done.
 **************************************************************/
bool ShopDone(void) {
    return CurrentShopID == 0;
}

/**************************************************************/
