/**********************************************************//**
 * @file type.h
 * @brief Defines universal types that are used across the
 * species and techniques.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _TYPE_H_
#define _TYPE_H_

/**********************************************************//**
 * @enum TYPE_ID
 * @brief Defines a constant for each type used.
 **************************************************************/
typedef enum {
    BASIC           = 1,
    FIRE            = 2,
    WATER           = 3,
    ICE             = 4,
    WIND            = 5,
    EARTH           = 6,
    METAL           = 7,
    LIGHT           = 8,
    DARK            = 9,
} TYPE_ID;

/// The number of unique TYPE_ID members.
#define N_TYPE 10

/**************************************************************/
extern float TypeMatchup(TYPE_ID attacking, TYPE_ID defending);

/**************************************************************/
#endif // _TYPE_H_
