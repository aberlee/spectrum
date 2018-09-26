/**********************************************************//**
 * @file person.h
 * @brief Defines each different type of human character,
 * including graphics and behavior.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _PERSON_H_
#define _PERSON_H_

#include "coordinate.h"         // COORDINATE, DIRECTION

/**********************************************************//**
 * @enum PERSON_ID
 * @brief Defines a constant for each costume an npc can use.
 **************************************************************/
typedef enum {
    // Player costumes
    AMY_NORMAL          = 1,
    AMY_RED_DRESS,
    AMY_BLACK_DRESS,
    
    // NPC Skins
    NPC_DOCTOR,
    NPC_NURSE,
    
    // Generic femme-presenting NPCs
    NPC_FEMME_A,
    NPC_FEMME_B,
    NPC_FEMME_C,
    NPC_FEMME_D,
    NPC_FEMME_E,
    NPC_FEMME_F,
    
    // Generic masc-presenting NPCs
    NPC_MASC_A,
    NPC_MASC_B,
    NPC_MASC_C,
    NPC_MASC_D,
    NPC_MASC_E,
    NPC_MASC_F,
} PERSON_ID;

/// @brief The number of different overworld person costumes.
#define N_PERSON (NPC_MASC_F+1)

/**************************************************************/
extern void DrawPerson(PERSON_ID id, DIRECTION direction);
extern void DrawPlayer(int frame);

/**************************************************************/
#endif // _PERSON_H_
