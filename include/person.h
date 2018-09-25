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
    AMY_NORMAL,
    AMY_RED_DRESS,
    AMY_BLACK_DRESS,
    
    // NPC Skins
    NPC_DOCTOR,
    NPC_NURSE,
} PERSON_ID;

/// @brief The number of different overworld person costumes.
#define N_PERSON 5

/**************************************************************/
extern void DrawPerson(PERSON_ID id, DIRECTION direction);
extern void DrawPlayer(int frame);

/**************************************************************/
#endif // _PERSON_H_
