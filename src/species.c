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
 * @brief Gets the SPECIES data from a SPECTRA.
 * @param spectra: Pointer to a SPECTRA.
 * @return Pointer to species data.
 **************************************************************/
const SPECIES *SpeciesOfSpectra(const SPECTRA *spectra) {
    return &SPECIES_DATA[spectra->Species];
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

/**********************************************************//**
 * @brief Compute the experience needed to level up.
 * @param spectra: Spectra to compute for.
 * @return The total experience needed.
 **************************************************************/
int ExperienceNeeded(const SPECTRA *spectra) {
    int levelSquared = spectra->Level*spectra->Level;
    const SPECIES *species = SpeciesOfSpectra(spectra);
    int rate = EXPERIENCE_RATE_DATA[species->Rate];
    return 1 + rate*levelSquared/100;
}

int ExperienceTotal(const SPECTRA *spectra) {
    int levelCubed = spectra->Level*spectra->Level*spectra->Level;
    const SPECIES *species = SpeciesOfSpectra(spectra);
    int rate = EXPERIENCE_RATE_DATA[species->Rate];
    int currentLevel = ExperienceNeeded(spectra)-spectra->Experience;
    return spectra->Level-1 + rate*levelCubed/300 + currentLevel;
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

// Dummy for output script
// #define Output(...) (void)0

void GainExperience(SPECTRA *spectra, int experience) {
    if (spectra->Level < LEVEL_MAX) {
        // Health gain initialization
        int health = spectra->MaxHealth;
        int power = spectra->MaxPower;
        
        // Manage level-up
        spectra->Experience -= experience;
        int gained = 0;
        while (spectra->Experience <= 0 && spectra->Level < LEVEL_MAX) {
            gained++;
            spectra->Level++;
            spectra->Experience += ExperienceNeeded(spectra);
        }
        
        // Gained any levels?
        if (gained > 0) {
            if (spectra->Level == LEVEL_MAX) {
                spectra->Experience = 0;
            }
            
            // Output level gains.
            /* const SPECIES *species = SpeciesOfSpectra(spectra);
            if (gained == 1) {
                Output("%s'S LEVEL WENT UP!\n", species->Name);
            } else if (gained > 1) {
                Output("%s'S LEVEL WENT UP BY %d!\n", species->Name, gained);
            } */
            
            // Health gain
            UpdateActiveStats(spectra);
            spectra->Health += (spectra->MaxHealth - health);
            spectra->Power += (spectra->MaxPower - power);
        }
    }
}