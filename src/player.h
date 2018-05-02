/**********************************************************//**
 * @file player.h
 * @brief Defines the player character's information.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>            // bool

#include "coordinate.h"         // COORDINATE
#include "item.h"               // ITEM_ID
#include "species.h"            // SPECTRA
#include "location.h"           // LOCATION_ID
#include "event.h"              // SWITCH

/**************************************************************/
/// @brief Number of Spectra that can be on the player's
/// team at one time.
#define TEAM_SIZE 10

/// @brief Number of items the player can carry at once.
#define INVENTORY_SIZE 24

/**********************************************************//**
 * @enum COSTUME_ID
 * @brief Defines constants for each costume the player can
 * wear (displays on the battle screen and menu).
 **************************************************************/
typedef enum {
    NORMAL          = 1,
    RED_DRESS,
    BLACK_DRESS,
    PAJAMAS,
    SUIT,
    SWIMSUIT,
} COSTUME_ID;

/// The number of unique costumes defined by COSTUME_ID.
#define N_COSTUME 7

/**********************************************************//**
 * @struct PLAYER
 * @brief Stores all the player's save file data.
 **************************************************************/
typedef struct {
    // Player stats
    COSTUME_ID Costume;
    int Money;
    int PlayTime;
    
    // Player collections
    ITEM_ID Inventory[INVENTORY_SIZE];
    SPECTRA Spectra[TEAM_SIZE];
    
    // Player map data
    LOCATION_ID Location;
    COORDINATE Position;
    DIRECTION Direction;
    
    // Game switch data
    int Switch[N_SWITCH];
} PLAYER;

/**************************************************************/
/// @brief Pointer to the current player's data.
extern PLAYER *const Player;

#define Switch(n) Player->Switch[n]

extern bool GetItem(ITEM_ID id);

/**************************************************************/
extern void NewGame(void);
extern bool LoadGame(void);
extern bool SaveGame(void);
extern void StartPlayTime(void);
extern int UnaccountedPlayTime(void);

/**************************************************************/
#endif // _PLAYER_H_