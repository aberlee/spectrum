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

/**********************************************************//**
 * @enum COSTUME_ID
 * @brief Defines constants for each costume the player can
 * wear (displays on the battle screen and menu).
 **************************************************************/
typedef enum {
    NORMAL          = 1,
    BLACK_DRESS,
    RED_DRESS,
    SUIT,
    SWIMSUIT,
    PAJAMAS,
} COSTUME_ID;

/// The number of unique costumes defined by COSTUME_ID.
#define N_COSTUME (PAJAMAS+1)

#define TEAM_SIZE 10
#define INVENTORY_SIZE 24

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
} PLAYER;

extern PLAYER *const Player;

extern void NewGame(void);
extern bool LoadGame(void);
extern bool SaveGame(void);

extern void StartPlayTime();
extern int UnaccountedPlayTime(void);

/**************************************************************/
#endif // _PLAYER_H_