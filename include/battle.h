/**********************************************************//**
 * @file battle.h
 * @brief Defines battle-related structures.
 * @author Rena Shinomiya
 * @date May 06, 2018
 **************************************************************/

#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "species.h"            // SPECTRA
#include "item.h"               // ITEM_ID
#include "technique.h"          // TECHNIQUE_ID
#include "battler.h"            // BATTLER

/**************************************************************/
/// @brief Number of battlers on one TEAM.
#define TEAM_SIZE 3

/// @brief Number of battlers in the whole battle.
#define BATTLE_SIZE (TEAM_SIZE+TEAM_SIZE)

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

/**********************************************************//**
 * @enum TURN_STATE
 * @brief Represents the state of each active turn.
 **************************************************************/
typedef enum {
    TURN_INACTIVE,      ///< The turn should not be processed.
    TURN_PENDING,       ///< The turn needs to be processed.
    TURN_ACTIVE,        ///< Turn processing is ongoing.
    TURN_RESULT,        ///< The turn was processed, but needs to be finished.
    TURN_DONE,          ///< Everything about the turn is done.
} TURN_STATE;

/**********************************************************//**
 * @struct TURN
 * @brief Represents all information required to perform
 * one battler's turn.
 **************************************************************/
typedef struct {
    TURN_STATE      State;      ///< Current state of this turn.
    int             User;       ///< User ID for this turn.
    TECHNIQUE_ID    Technique;  ///< Technique used by the user.
    ITEM_ID         Item;       ///< Item used if Technique=DEFAULT_ITEM.
    int             Target;     ///< Chosen target of the technique, if applicable.
} TURN;

/**************************************************************/
extern void InitializeRandomEncounter(int count, ENCOUNTER_TYPE type);
extern void InitializeBossEncounter(const BOSS *bosses);
extern void DrawBattle(void);
extern void UpdateBattle(void);
extern bool EscapeBattle(void);

/**************************************************************/
extern BATTLER *BattlerByID(int id);
extern TURN *TurnByID(int id);
extern int GetTargets(int *targets, int user, TARGET_TYPE type);

/**********************************************************//**
 * @brief Shortcut to get the name of a battler for output.
 * @param id: ID of the battler.
 * @return The battler's name.
 **************************************************************/
static inline const char *BattlerNameByID(int id) {
    return BattlerName(BattlerByID(id));
}

/**************************************************************/
#endif // _BATTLE_H_
