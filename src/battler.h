/**********************************************************//**
 * @file battler.h
 * @brief Defines spectra who are in a battle.
 * @author Rena Shinomiya
 * @date May 26, 2018
 **************************************************************/

#ifndef _BATTLER_H_
#define _BATTLER_H_

#include <stddef.h>             // NULL
#include <stdbool.h>            // bool

#include "species.h"            // SPECTRA

/**************************************************************/
/// @brief Defines the maximum number of levels a stat can be
/// increased or lowered.
#define BOOST_MAX 6

/**********************************************************//**
 * @enum BATTLER_FLAGS
 * @brief Defines flag that apply to each battler.
 **************************************************************/
typedef enum {
    BATTLER_DEFEND      = 0x0001,
} BATTLER_FLAGS;

/**********************************************************//**
 * @struct BATTLER
 * @brief Defines battle-level information for a spectra
 * participating in the battle.
 **************************************************************/
typedef struct {
    SPECTRA *Spectra;           ///< Pointer to the spectra, or NULL.
    BATTLER_FLAGS Flags;        ///< Battler information.
    int AttackBoost;            ///< Change to apply to attack stat.
    int DefendBoost;            ///< Change to apply to defend stat.
    int EvadeBoost;             ///< Change to apply to evade stat.
    int LuckBoost;              ///< Change to apply to luck stat.
} BATTLER;

/**********************************************************//**
 * @brief Determines if the BATTLER struct represents an
 * actual valid battler.
 * @param battler: The BATTLER to check.
 * @return True if the BATTLER is a valid battler.
 **************************************************************/
static inline bool BattlerIsActive(const BATTLER *battler) {
    return battler->Spectra != NULL;
}

/**********************************************************//**
 * @brief Gets the species of the BATTLER.
 * @param battler: The BATTLER to read.
 * @return The BATTLER's species.
 **************************************************************/
static inline const SPECIES *BattlerSpecies(const BATTLER *battler) {
    return SpeciesByID(battler->Spectra->Species);
}

/**********************************************************//**
 * @brief Gets the name of the BATTLER.
 * @param battler: The BATTLER to read.
 * @return The BATTLER's name.
 **************************************************************/
static inline const char *BattlerName(const BATTLER *battler) {
    return BattlerSpecies(battler)->Name;
}

/**************************************************************/
extern void InitializeBattler(BATTLER *battler, SPECTRA *spectra);

/**********************************************************//**
 * @brief Initializes a BATTLER struct to an inactive state,
 * representing that there is no battler.
 * @param battler: BATTLER struct to inactivate.
 **************************************************************/
static inline void InitializeBattlerAsInactive(BATTLER *battler) {
    InitializeBattler(battler, NULL);
}

/**********************************************************//**
 * @brief Determines if the battler is alive.
 * @param battler: The battler to check.
 * @return True if the battler is alive.
 **************************************************************/
static inline bool IsAlive(const BATTLER *battler) {
    return battler->Spectra->Health > 0;
}

/**********************************************************//**
 * @brief Get the current health of a battler.
 * @param battler: Battler to read.
 * @return The battler's current health.
 **************************************************************/
static inline int BattlerHealth(const BATTLER *battler) {
    return battler->Spectra->Health;
}

/**********************************************************//**
 * @brief Get the current power of a battler.
 * @param battler: Battler to read.
 * @return The battler's current power.
 **************************************************************/
static inline int BattlerPower(const BATTLER *battler) {
    return battler->Spectra->Power;
}

/**********************************************************//**
 * @brief Get the maximum health of a battler.
 * @param battler: Battler to read.
 * @return The battler's maximum health.
 **************************************************************/
static inline int BattlerMaxHealth(const BATTLER *battler) {
    return battler->Spectra->MaxHealth;
}

/**********************************************************//**
 * @brief Get the maximum power of a battler.
 * @param battler: Battler to read.
 * @return The battler's maximum power.
 **************************************************************/
static inline int BattlerMaxPower(const BATTLER *battler) {
    return battler->Spectra->MaxPower;
}

/**************************************************************/
extern int BattlerAttack(const BATTLER *battler);
extern int BattlerDefend(const BATTLER *battler);
extern int BattlerEvade(const BATTLER *battler);
extern int BattlerLuck(const BATTLER *battler);

/**************************************************************/
#endif // _BATTLER_H_
