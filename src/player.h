/**********************************************************//**
 * @file player.h
 * @brief Defines the player character's information.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _PLAYER_H_
#define _PLAYER_H_

/**********************************************************//**
 * @enum COSTUME_ID
 * @brief Defines constants for each costume the player can
 * wear (displays on the battle screen and menu).
 **************************************************************/
typedef enum {
    NORMAL=1,
    BLACK_DRESS,
    RED_DRESS,
    SUIT,
    SWIMSUIT,
    PAJAMAS,
} COSTUME_ID;

/// The number of unique costumes defined by COSTUME_ID.
#define N_COSTUME (PAJAMAS+1)

/**********************************************************//**
 * @struct PLAYER
 * @brief Stores all the player's save file data.
 **************************************************************/
typedef struct {
    COSTUME_ID Costume;
    int InventorySize;
    ITEM_ID Inventory[20];
    int SpectraCaught;
    SPECTRA Spectra[100];
    SPECTRA *Team[3];
    int Money;
    int PlayTime;
    LOCATION_ID Location;
    COORDINATE Where;
} PLAYER;

/**************************************************************/
#endif // _PLAYER_H_