/**********************************************************//**
 * @file species.h
 * @brief Defines structures that contain monster data.
 * @author Rena Shinomiya
 * @date March 6th, 2018
 **************************************************************/

#ifndef _SPECIES_H_
#define _SPECIES_H_

#include "technique.h"          // TECHNIQUE_ID

/**********************************************************//**
 * @enum SPECIES_ID
 * @brief Defines a unique identifier for each spectra in the
 * system. These are indexed from 1, with 0 representing a
 * null value.
 **************************************************************/
typedef enum {
    COALSHARK       = 1,
    JAYRAPTOR,
    MINESPHERE,
    HOCUS,
    NESSIE,
    OSCILLATE,
    PUZZLE,
    LAUNCHPAD,
    GARPIKE,
    MOATMONSTER,
    GOLDDRAGON,
    REPLICA,
    NITROBOMB,
    FLOPJELLY,
    GASMOG,
    FOGFANG,
    GLACIALITH,
    BUTCHERBIRD,
    CUMULUS,
    SSSNAKE,
    DACTYLUS,
    MEGATAR,
    ACISTAR,
    TARHEAP,
    GIGACLAM,
    TOTEM_POLE,
    PRGMERROR,
    PHOENIX,
    RAIKEGON,
    BRONTO,
    TOOLFISH,
    WATERWING,
    VARAN,
    SPACESNAKE,
    VACUUM,
    AMY,
    ASTEROID,
    BASSMONSTER,
    CATFISH,
    ICEBOULDER,
    LEKTRON,
    TELEVIRUS,
    VOLTDRAGON,
    KARDA,
} SPECIES_ID;

/// The maximum number of SPECIES_ID entries.
#define N_SPECIES (KARDA+1)

/// The maximum moveset size for each species.
#define MOVESET_SIZE 8

/**********************************************************//**
 * @enum EXPERIENCE_TYPE
 * @brief Defines all the base statistics for a species of
 * spectra. These should be kept constant.
 **************************************************************/
typedef enum {
    FASTEST         = 1,
    FAST,
    AVERAGE,
    SLOW,
    SLOWEST,
} EXPERIENCE_TYPE;

/**********************************************************//**
 * @struct SPECIES
 * @brief Defines all the base statistics for a species of
 * spectra. These should be kept constant.
 **************************************************************/
typedef struct {
    char Name[13];              ///< The official species name.

    // Base statistics
    TYPE_ID Type[2];            ///< The spectra's intrinsic type.
    int Health;                 ///< Base maximum health stat.
    int Power;                  ///< Base maximum power stat.
    int Attack;                 ///< Base attack stat.
    int Defend;                 ///< Base defend stat.
    int Luck;                   ///< Base luck stat.
    int Evade;                  ///< Base evasion stat.

    // Battle statistics
    EXPERIENCE_TYPE Rate;       ///< Growth rate of the spectra.
    int CatchRate;              ///< Chance to catch the spectra (0-100).
    int Experience;             ///< Experience earned by defeating the spectra.
    int Money;                  ///< Money earned by defeating the spectra.

    // Technique moveset
    int MovesetSize;            ///< The size of the moveset member list.
    const struct {
        int Level;              ///< Level to learn the technique.
        TECHNIQUE_ID Technique; ///< The technique learned.
    } Moveset[MOVESET_SIZE];    ///< The spectra's full moveset.
} SPECIES;

/**********************************************************//**
 * @enum AILMENT_ID
 * @brief Defines unique identifiers for each ailment that can
 * be inflicted during battle.
 **************************************************************/
typedef enum {
    POISONED        = 1,
    SHOCKED,
    BURIED,
    ASLEEP,
    AFLAME,
} AILMENT_ID;

/// The number of unique AILMENT_ID elements.
#define N_AILMENT (AFLAME+1)

/**********************************************************//**
 * @struct SPECTRA
 * @brief Defines menu and battle independent information
 * for each spectra the player has in their party.
 **************************************************************/
typedef struct {
    SPECIES_ID Species;         ///< The species of the spectra.

    // Active stats
    int MaxHealth;              ///< Active max health.
    int MaxPower;               ///< Active max power.
    int Attack;                 ///< Active attack.
    int Defend;                 ///< Active defend.
    int Evade;                  ///< Active evade.
    int Luck;                   ///< Active luck.

    /// The spectra's active moveset.
    TECHNIQUE_ID Moveset[MOVESET_SIZE];
    int MovesetSize;            ///< The number of techniques learned.

    // Working stats
    int Health;                 ///< Current health of the spectra.
    int Power;                  ///< Current power of the spectra.
    AILMENT_ID Ailment;         ///< The ailment that afflicts the spectra.
    int Level;                  ///< Current level of the spectra.
    int Experience;             ///< Current experience of the spectra.
} SPECTRA;

/// The highest SPECTRA level.
#define LEVEL_MAX 100

/**********************************************************//**
 * @struct BATTLER
 * @brief Defines battle-level information for a spectra
 * participating in the battle.
 **************************************************************/
typedef struct {
    SPECTRA *Spectra;           ///< Pointer to the spectra, or NULL.

    // Boosts
    int AttackBoost;            ///< Change to apply to attack stat.
    int DefendBoost;            ///< Change to apply to defend stat.
    int EvadeBoost;             ///< Change to apply to evade stat.
    int LuckBoost;              ///< Change to apply to luck stat.
} BATTLER;

// SPECIES getters
extern const SPECIES *SpeciesByID(SPECIES_ID id);
extern const SPECIES *SpeciesOfSpectra(const SPECTRA *spectra);
extern const SPECIES *SpeciesOfBattler(const BATTLER *battler);

/**************************************************************/
#endif // _SPECIES_H_
