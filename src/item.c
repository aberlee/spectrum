/**********************************************************//**
 * @file item.c
 * @brief Implements ITEM functions.
 * @author Rena Shinomiya
 * @date April 22, 2018
 **************************************************************/

#include "item.h"               // ITEM
#include "item.i"               // ITEM_DATA

/**********************************************************//**
 * @brief Gets ITEM data for the ITEM_ID.
 * @param id: Item ID to look up.
 * @return Pointer to the ITEM data.
 **************************************************************/
const ITEM *ItemByID(ITEM_ID id) {
    return &ITEM_DATA[id];
}

/**************************************************************/
