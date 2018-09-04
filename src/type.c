/**********************************************************//**
 * @file type.c
 * @brief Defines the type matchup table.
 * @author Rena Shinomiya
 * @date September 3rd, 2018
 **************************************************************/

#include "type.h"

/**************************************************************/
#define ONCE 1.0        ///< Normal effectiveness.
#define TWICE 2.0       ///< Double efectiveness.
#define HALF 0.5        ///< Half effectiveness.

/**********************************************************//**
 * @var TypeMatchupTable
 * @brief Defines each type matchup, with [ATTACK][DEFEND]
 * pairings.
 **************************************************************/
static const float TypeMatchupTable[N_TYPE][N_TYPE] = {
    // Type =  Basic  Fire   Water  Ice    Wind   Earth  Metal  Light  Dark
    [BASIC] = {ONCE,  ONCE,  ONCE,  HALF,  ONCE,  ONCE,  HALF,  ONCE,  ONCE},
    [FIRE]  = {ONCE,  HALF,  HALF,  TWICE, ONCE,  TWICE, TWICE, ONCE,  TWICE},
    [WATER] = {ONCE,  TWICE, HALF,  HALF,  ONCE,  ONCE,  TWICE, ONCE,  ONCE},
    [ICE]   = {ONCE,  HALF,  ONCE,  ONCE,  ONCE,  TWICE, HALF,  ONCE,  ONCE},
    [WIND]  = {ONCE,  ONCE,  ONCE,  ONCE,  ONCE,  TWICE, HALF,  ONCE,  ONCE},
    [EARTH] = {ONCE,  TWICE, ONCE,  ONCE,  TWICE, ONCE,  ONCE,  ONCE,  ONCE},
    [METAL] = {ONCE,  ONCE,  HALF,  TWICE, ONCE,  ONCE,  HALF,  ONCE,  ONCE},
    [LIGHT] = {ONCE,  ONCE,  TWICE, ONCE,  ONCE,  HALF,  ONCE,  HALF,  TWICE},
    [DARK]  = {ONCE,  ONCE,  ONCE,  ONCE,  ONCE,  ONCE,  ONCE,  TWICE, HALF},
};

/**********************************************************//**
 * @brief Gets the type matchup factor between two types.
 * @param attacking: The attacking type.
 * @param defending: The defending type.
 * @return Multiplication factor of the matchup.
 **************************************************************/
float TypeMatchup(TYPE_ID attacking, TYPE_ID defending) {
    return TypeMatchupTable[attacking][defending];
}

/**************************************************************/
