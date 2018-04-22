#include <stdbool.h>            // bool
#include <stdio.h>              // FILE

#include <allegro5/allegro.h>

#include "assets.h"             // SAVE
#include "player.h" // PLAYER
#include "species.h"
#include "location.h"

static PLAYER PlayerData;
PLAYER *const Player = &PlayerData;

static double StartTime;

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
    CreateSpectra(&Player->Spectra[0], AMY, 1);
    
    // Reset locations
    Warp(YOUR_HOUSE, 2, 3);
}

void StartPlayTime(void) {
    StartTime = al_get_time();
}

int UnaccountedPlayTime(void) {
    return (int)(al_get_time() - StartTime);
}

bool LoadGame(void) {
    FILE *saveFile = fopen(SAVE, "r");
    if (saveFile) {
        int nRead = fread(&PlayerData, sizeof(PLAYER), 1, saveFile);
        fclose(saveFile);
        return nRead == sizeof(PLAYER);
    }
    return false;
}

bool SaveGame(void) {
    FILE *saveFile = fopen(SAVE, "r");
    if (saveFile) {
        int nWrite = fwrite(&PlayerData, sizeof(PLAYER), 1, saveFile);
        fclose(saveFile);
        return nWrite == sizeof(PLAYER);
    }
    return false;
}
