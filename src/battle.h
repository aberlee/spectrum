/**********************************************************//**
 * @file battle.h
 * @brief Defines battle-related structures.
 * @author Rena Shinomiya
 * @date May 06, 2018
 **************************************************************/

#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "species.h"            // SPECTRA
#include "technique.h"          // TECHNIQUE_ID
#include "battler.h"            // BATTLER

/**************************************************************/
/// @brief Number of battlers on one TEAM.
#define TEAM_SIZE 3

/**********************************************************//**
 * @enum ENCOUNTER_TYPE
 * @brief Describes the type of encounter table to read from.
 **************************************************************/
typedef enum {
    ENCOUNTER_OVERWORLD         = 1,
    ENCOUNTER_FISHING           = 2,
    ENCOUNTER_BOSS              = 3,
} ENCOUNTER_TYPE;
/**********************************************************//**
 * @struct ENEMY
 * @brief Defines enemy data that should be used to generate
 * a random encounter or boss battle.
 **************************************************************/
typedef struct {
    SPECIES_ID Species;
    int Level;
} ENEMY;

/**********************************************************//**
 * @struct BOSS
 * @brief Data about a structured BOSS encounter.
 **************************************************************/
typedef struct {
    ENEMY Boss[TEAM_SIZE];
} BOSS;

/**********************************************************//**
 * @struct TEAM
 * @brief Contains the battlers on a given team, user or enemy.
 **************************************************************/
typedef struct {
    BATTLER Member[TEAM_SIZE];
} TEAM;

/**************************************************************/
extern void InitializeRandomEncounter(int count, ENCOUNTER_TYPE type);
extern void InitializeBossEncounter(const BOSS *bosses);
extern void DrawBattle(void);
extern void UpdateBattle(void);

/**************************************************************/
#endif // _BATTLE_H_
