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
    // Business-people
    BUSINESSWOMAN_BRUNETTE      = 1,
    BUSINESSWOMAN_BLONDE,
    BUSINESSMAN_BALD,
    BUSINESSMAN_GRAY,
    BUSINESSMAN_BLOND,
    BUSINESSMAN_BLACK,
    
    // Everyday adults
    WOMAN_SHORT_BLONDE,
    WOMAN_SHORT_BRUNETTE,
    WOMAN_BRUNETTE,
    MAN_BALD,
    MAN_GRAY,
    MAN_BRUNET,
    GRANDMOTHER,
    
    // Everyday young adults
    GIRL_BRUNETTE,
    GIRL_BLONDE,
    BOY_BRUNET,
    
    // Vocation
    FARMER,
    MEDIC,
    SCIENTIST,
    
    // Player costumes
    AMY_NORMAL,
    AMY_RED_DRESS,
    AMY_BLACK_DRESS,
    AMY_PAJAMAS,
    AMY_SUIT,
    AMY_SWIMSUIT,
} PERSON_ID;

#define N_PERSON (AMY_SWIMSUIT+1)

extern void DrawPersonAt(PERSON_ID id, DIRECTION direction, int frame, int x, int y);
extern void DrawPerson(PERSON_ID id, DIRECTION direction, int frame);
extern void DrawPlayer(int frame);

/**************************************************************/
#endif // _PERSON_H_
