
#include <allegro5/allegro.h>

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
    
    // Start time counting
    StartTime = al_get_time();
}

int UnaccountedPlayTime(void) {
    return (int)(al_get_time() - StartTime);
}