/**********************************************************//**
 * @file species.c
 * @brief Implements all spectra-related functions for menus,
 * battle, and technique usage.
 **************************************************************/

#include <math.h>               // ceil

#include "species.h"            // SPECIES_ID, SPECIES
#include "technique.h"          // TECHNIQUE_ID

#include "species.i"            // SPECIES_DATA

/**********************************************************//**
 * @brief Gets the SPECIES data from its ID.
 * @param id: The scalar identity of the species.
 * @return Pointer to species data.
 **************************************************************/
const SPECIES *SpeciesByID(SPECIES_ID id) {
    return &SPECIES_DATA[id];
}

/**********************************************************//**
 * @brief Recomputes the SPECTRA stats.
 * @param spectra: Spectra to update.
 **************************************************************/
void UpdateActiveStats(SPECTRA *spectra) {
    const SPECIES *species = SpeciesOfSpectra(spectra);
    
    // Recalculate stats
    float scale = 0.03f*(spectra->Level+5);
    spectra->MaxHealth = (int)ceil(species->Health*scale);
    spectra->MaxPower = (int)ceil(species->Power*scale);
    spectra->Attack = (int)ceil(species->Attack*scale);
    spectra->Defend = (int)ceil(species->Defend*scale);
    spectra->Evade = (int)ceil(species->Evade*scale);
    spectra->Luck = (int)ceil(species->Luck*scale);
    
    // Recalculate moveset
    int i;
    for (i = 0; species->Moveset[i].Technique && species->Moveset[i].Level <= spectra->Level; i++) {
        spectra->Moveset[i] = species->Moveset[i].Technique;
    }
    spectra->MovesetSize = i;
    spectra->Moveset[i] = 0;
}

/**********************************************************//**
 * @brief Restores the spectra to max health and power.
 * @param spectra: Spectra to change.
 **************************************************************/
void Recover(SPECTRA *spectra) {
    spectra->Health = spectra->MaxHealth;
    spectra->Power = spectra->MaxPower;
    spectra->Ailment = 0;
}

/**********************************************************//**
 * @brief Experience rate table indexed by EXPERIENCE_TYPE.
 **************************************************************/
static const int EXPERIENCE_RATE_DATA[] = {
    [FASTEST]           =  60,
    [FAST]              =  80,
    [AVERAGE]           = 100,
    [SLOW]              = 120,
    [SLOWEST]           = 140,
};

static int ExperienceNeededAtLevel(const SPECTRA *spectra, int level) {
    int levelCubed = level*level*level;
    const SPECIES *species = SpeciesOfSpectra(spectra);
    int rate = EXPERIENCE_RATE_DATA[species->Rate];
    return 1 + rate*levelCubed/1000;
}

/**********************************************************//**
 * @brief Compute the experience needed to level up.
 * @param spectra: Spectra to compute for.
 * @return The total experience needed.
 **************************************************************/
int ExperienceNeeded(const SPECTRA *spectra) {
    return ExperienceNeededAtLevel(spectra, spectra->Level);
}

/**********************************************************//**
 * @brief Determines the total amount of experience the
 * given spectra has earned at any level.
 * @param spectra: Spectra to check.
 * @return Total experience.
 **************************************************************/
int ExperienceTotal(const SPECTRA *spectra) {
    int total = 0;
    for (int l=1; l<spectra->Level; l++) {
        total += ExperienceNeededAtLevel(spectra, l);
    }
    return total + ExperienceNeeded(spectra) - spectra->Experience;
}

/**********************************************************//**
 * @brief Creates a new SPECTRA.
 * @param spectra: Spectra to create.
 * @param species: The species of the spectra.
 * @param level: The level of the spectra.
 **************************************************************/
void CreateSpectra(SPECTRA *spectra, SPECIES_ID species, int level) {
    spectra->Species = species;
    spectra->Level = level;
    spectra->Experience = ExperienceNeeded(spectra);
    UpdateActiveStats(spectra);
    Recover(spectra);
}

/**************************************************************/
