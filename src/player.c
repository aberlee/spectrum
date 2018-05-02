/**********************************************************//**
 * @file player.c
 * @brief Implements player data management functions.
 * @author Rena Shinomiya
 * @date April 24, 2018
 **************************************************************/

#include <stdbool.h>            // bool
#include <stdio.h>              // FILE

#include <allegro5/allegro.h>

#include "player.h"             // PLAYER
#include "assets.h"             // SAVE_FILE
#include "species.h"            // CreateSpectra
#include "location.h"           // Warp

/**************************************************************/
/// @brief Defines the path to the save file.
#define SAVE_FILE "spectrum.save"

/**************************************************************/
/// @brief The player's data (referenced via Player pointer
/// from any external code).
static PLAYER PlayerData;

// External pointer for PlayerData, declared in player.h
PLAYER *const Player = &PlayerData;

/// @brief Time in seconds when the game was started or last
/// saved. This is used to keep a running total of the player's
/// play time without writing save files to disk (that happens
/// when Save is chosen from the main menu).
static double StartTime;

bool GetItem(ITEM_ID id) {
    for (int i=0; i<INVENTORY_SIZE; i++) {
        if (!PlayerData.Inventory[i]) {
            PlayerData.Inventory[i] = id;
            return true;
        }
    }
    return false;
}

/**********************************************************//**
 * @brief Configures a new game, and removes old player data.
 * Be sure the user is OK with deleting data before calling
 * this function!
 **************************************************************/
void NewGame(void) {
    // Reset all the stats
    Player->Costume = NORMAL;
    Player->Money = 0;
    Player->PlayTime = 0;
    
    // Reset collections to just yourself
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        Player->Inventory[i] = 0;
    }
    for (int i = 0; i < TEAM_SIZE; i++) {
        Player->Spectra[i].Species = 0;
    }
    CreateSpectra(&Player->Spectra[0], AMY, 5);
    
    // Reset locations
    Warp(YOUR_HOUSE, 2, 3, DOWN);
}

/**********************************************************//**
 * @brief Starts the play time counter when the game begins.
 **************************************************************/
void StartPlayTime(void) {
    StartTime = al_get_time();
}

/**********************************************************//**
 * @brief Get the current session's play time - time since
 * the game started, or time since the game was last saved.
 * @return The current session's play time.
 **************************************************************/
int UnaccountedPlayTime(void) {
    return (int)(al_get_time() - StartTime);
}

/**********************************************************//**
 * @brief Loads a save file from disk. Currently data are
 * stored in SAVE_FILE and no other location is possible.
 * @return True if the load succeeded.
 **************************************************************/
bool LoadGame(void) {
    FILE *saveFile = fopen(SAVE_FILE, "r");
    if (saveFile) {
        int nRead = fread(&PlayerData, sizeof(PLAYER), 1, saveFile);
        fclose(saveFile);
        //if (nRead == 1) {
            InitializeLocation();
            return true;
        //}
    }
    return false;
}

/**********************************************************//**
 * @brief Saves the player's data to the SAVE_FILE. Currently
 * this is the only possible location to save to.
 * @return True if the save succeeded.
 **************************************************************/
bool SaveGame(void) {
    FILE *saveFile = fopen(SAVE_FILE, "w");
    if (saveFile) {
        int nWrite = fwrite(&PlayerData, sizeof(PLAYER), 1, saveFile);
        fclose(saveFile);
        return nWrite == 1;
    }
    return false;
}

/**************************************************************/
