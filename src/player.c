/**********************************************************//**
 * @file player.c
 * @brief Implements player data management functions.
 * @author Rena Shinomiya
 * @date April 24, 2018
 **************************************************************/

#include <stdbool.h>            // bool
#include <stdio.h>              // FILE
#include <stdlib.h>             // memcpy

#include <allegro5/allegro.h>

#include "player.h"             // PLAYER
#include "assets.h"             // SAVE_FILE
#include "species.h"            // CreateSpectra
#include "location.h"           // Warp

/**************************************************************/
/// @brief Defines the path to the save file.
#define SAVE_FILE "spectrum.save"

/// @brief Define the path to the backup save file.
#define BACKUP_SAVE_FILE "backup.save"

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

/**********************************************************//**
 * @brief Give the player a new item.
 * @param id: Item to obtain.
 * @return Whether the item can be carried.
 **************************************************************/
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
 * @brief Determine if the player posesses an item.
 * @param id: The item tp check.
 * @return True if the player has the item.
 **************************************************************/
bool HasItem(ITEM_ID id) {
    for (int i=0; i<INVENTORY_SIZE && PlayerData.Inventory[i]; i++) {
        ITEM_ID current = PlayerData.Inventory[i];
        if (current == id) {
            return true;
        } else if (!current) {
            return false;
        }
    }
    return false;
}

/**********************************************************//**
 * @brief The player loses the item provided.
 * @param id: Item to use.
 **************************************************************/
void DropItem(ITEM_ID id) {
    for (int i=0; i<INVENTORY_SIZE && PlayerData.Inventory[i]; i++) {
        ITEM_ID current = PlayerData.Inventory[i];
        if (current == id) {
            while (i<INVENTORY_SIZE) {
                PlayerData.Inventory[i] = PlayerData.Inventory[i+1];
                i++;
            }
            PlayerData.Inventory[INVENTORY_SIZE-1] = 0;
        }
    }
}

/**********************************************************//**
 * @brief Attempts to add a spectra to the player's party.
 * @param spectra: Spectra to capture.
 * @return True if the spectra was added.
 **************************************************************/
bool GetSpectra(const SPECTRA *spectra) {
    // Try to find a slot for the spectra
    for (int i=0; i<PARTY_SIZE; i++) {
        if (!PlayerData.Spectra[i].Species) {
            memcpy(&PlayerData.Spectra[i], spectra, sizeof(SPECTRA));
            return true;
        }
    }
    return false;
}

void ReleaseSpectra(int index) {
    // Can't release the player character
    if (PlayerData.Spectra[index].Species == AMY) {
        return;
    }
    
    // Rotate spectra list down
    for (int i=index; i<PARTY_SIZE; i++) {
        PlayerData.Spectra[i] = PlayerData.Spectra[i+1];
    }
    PlayerData.Spectra[PARTY_SIZE-1].Species = 0;
}

void RecoverParty(void) {
    for (int i=0; i<PARTY_SIZE; i++) {
        SPECTRA *spectra = &PlayerData.Spectra[i];
        if (spectra->Species) {
            Recover(spectra);
        }
    }
}

/**********************************************************//**
 * @brief Configures a new game, and removes old player data.
 * Be sure the user is OK with deleting data before calling
 * this function!
 **************************************************************/
void NewGame(void) {
    // Reset all the stats
    Player->Costume = BLACK_DRESS;
    Player->Money = 0;
    Player->PlayTime = 0;
    
    // Reset collections to just yourself
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        Player->Inventory[i] = 0;
    }
    for (int i = 0; i < PARTY_SIZE; i++) {
        Player->Spectra[i].Species = 0;
    }
    CreateSpectra(&Player->Spectra[0], AMY, 5);
    
    // Reset locations
    Warp(YOUR_HOUSE, 2, 3, DOWN);
    SetMode(MODE_MAP);
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
    FILE *saveFile = fopen(SAVE_FILE, "rb");
    if (saveFile) {
        int nRead = fread(&PlayerData, sizeof(PLAYER), 1, saveFile);
        fclose(saveFile);
        if (nRead == 1) {
            InitializeLocation();
            SetMode(MODE_MAP);
#ifdef DEBUG
            FILE *copyFile = fopen(BACKUP_SAVE_FILE, "wb");
            fwrite(&PlayerData, sizeof(PLAYER), 1, copyFile);
            fflush(copyFile);
            fclose(copyFile);
#endif // DEBUG
            return true;
        }
    }
    return false;
}

/**********************************************************//**
 * @brief Saves the player's data to the SAVE_FILE. Currently
 * this is the only possible location to save to.
 * @return True if the save succeeded.
 **************************************************************/
bool SaveGame(void) {
    // Update time stamping
    Player->PlayTime += UnaccountedPlayTime();
    StartPlayTime();
    FILE *saveFile = fopen(SAVE_FILE, "wb");
    if (saveFile) {
        int nWrite = fwrite(&PlayerData, sizeof(PLAYER), 1, saveFile);
        fflush(saveFile);
        fclose(saveFile);
        return nWrite == 1;
    }
    return false;
}

/**************************************************************/
