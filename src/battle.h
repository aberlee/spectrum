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

/**************************************************************/
/// @brief Number of battlers on one TEAM.
#define TEAM_SIZE 3

/// @brief Number of battlers in the whole battle.
#define BATTLE_SIZE (TEAM_SIZE+TEAM_SIZE)

/**********************************************************//**
 * @typedef BATTLER_ID
 * @brief Defines a type for unique BATTLER_IDs.
 **************************************************************/
typedef int BATTLER_ID;

/**********************************************************//**
 * @brief Gets the BATTLER_ID for someone on the user team.
 * @param id: Index into the user's team.
 * @return BATTLER_ID
 **************************************************************/
static inline BATTLER_ID UserID(int id) {
    return id;
}

/**********************************************************//**
 * @brief Gets the BATTLER_ID for someone on the enemy team.
 * @param id: Index into the enemy's team.
 * @return BATTLER_ID
 **************************************************************/
static inline BATTLER_ID EnemyID(int id) {
    return id+TEAM_SIZE;
}

/**********************************************************//**
 * @struct BATTLER
 * @brief Defines battle-level information for a spectra
 * participating in the battle.
 **************************************************************/
typedef struct {
    BATTLER_ID ID;              ///< Unique identifier of the battler.
    SPECTRA *Spectra;           ///< Pointer to the spectra, or NULL.

    // Boosts
    int AttackBoost;            ///< Change to apply to attack stat.
    int DefendBoost;            ///< Change to apply to defend stat.
    int EvadeBoost;             ///< Change to apply to evade stat.
    int LuckBoost;              ///< Change to apply to luck stat.
} BATTLER;

#define BoostScale(boost) (1+boost/6.0)

static inline int Attack(const BATTLER *battler) {
    return battler->Spectra->Attack*BoostScale(battler->AttackBoost);
}

static inline int Defend(const BATTLER *battler) {
    return battler->Spectra->Defend*BoostScale(battler->DefendBoost);
}

static inline int Evade(const BATTLER *battler) {
    return battler->Spectra->Evade*BoostScale(battler->EvadeBoost);
}

static inline int Luck(const BATTLER *battler) {
    return battler->Spectra->Luck*BoostScale(battler->LuckBoost);
}

#undef BoostScale // Do not export!

typedef enum {
    ENCOUNTER_OVERWORLD         = 1,
    ENCOUNTER_FISHING           = 2,
} ENCOUNTER_TYPE;

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

typedef struct {
    BATTLER Member[BATTLE_SIZE];
} TEAM;

typedef struct {
    BATTLER_ID User;
    TECHNIQUE_ID Technique;
    TARGET_TYPE TargetType;
    BATTLER_ID Target;
} TURN;

/**************************************************************/
extern void InitializeRandomEncounter(int count, ENCOUNTER_TYPE type);
extern void InitializeBossEncounter(const BOSS *bosses);
extern void DrawBattle(void);
extern void UpdateBattle(void);

/**************************************************************/
#endif // _BATTLE_H_
