/**********************************************************//**
 * @file species.h
 * @brief Defines structures that contain monster data.
 * @author Rena Shinomiya
 * @date March 6th, 2018
 **************************************************************/

#ifndef _SPECIES_H_
#define _SPECIES_H_

#include "coordinate.h"         // COORDINATE
#include "technique.h"          // TECHNIQUE_ID

/**********************************************************//**
 * @enum SPECIES_ID
 * @brief Defines a unique identifier for each spectra in the
 * system. These are indexed from 1, with 0 representing a
 * null value.
 **************************************************************/
typedef enum {
    COALSHARK       =  1,
    JAYRAPTOR       =  2,
    MINESPHERE      =  3,
    HOCUS           =  4,
    NESSIE          =  5,
    OSCILLATE       =  6,
    PUZZLE          =  7,
    LAUNCHPAD       =  8,
    GARPIKE         =  9,
    MOATMONSTER     = 10,
    GOLDDRAGON      = 11,
    REPLICA         = 12,
    NITROBOMB       = 13,
    FLOPJELLY       = 14,
    GASMOG          = 15,
    FOGFANG         = 16,
    GLACIALITH      = 20,
    BUTCHERBIRD     = 21,
    CUMULUS         = 22,
    SSSNAKE         = 23,
    DACTYLUS        = 24,
    MEGATAR         = 25,
    ACISTAR         = 26,
    TARHEAP         = 27,
    GIGACLAM        = 28,
    TOTEM_POLE      = 29,
    PRGMERROR       = 30,
    PHOENIX         = 31,
    RAIKEGON        = 32,
    BRONTO          = 33,
    TOOLFISH        = 34,
    WATERWING       = 35,
    VARAN           = 36,
    SPACESNAKE      = 37,
    VACUUM          = 38,
    AMY             = 40,
    ASTEROID        = 41,
    BASSMONSTER     = 42,
    CATFISH         = 43,
    ICEBOULDER      = 44,
    LEKTRON         = 45,
    TELEVIRUS       = 46,
    VOLTDRAGON      = 47,
    KARDA           = 52,
} SPECIES_ID;

/// The maximum number of SPECIES_ID entries.
#define N_SPECIES 53

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
    const struct {
        int Level;              ///< Level to learn the technique.
        TECHNIQUE_ID Technique; ///< The technique learned.
    } Moveset[MOVESET_SIZE];    ///< The spectra's full moveset.
    
    // Image properties
    COORDINATE Offset;
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

/**************************************************************/
extern const SPECIES *SpeciesByID(SPECIES_ID id);
extern const SPECIES *SpeciesOfSpectra(const SPECTRA *spectra);
extern void UpdateActiveStats(SPECTRA *spectra);
extern void Recover(SPECTRA *spectra);
extern int ExperienceNeeded(const SPECTRA *spectra);
extern int ExperienceTotal(const SPECTRA *spectra);
extern void CreateSpectra(SPECTRA *spectra, SPECIES_ID species, int level);

/**************************************************************/
#endif // _SPECIES_H_
