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
 * @brief Defines a constant for each type used, starting
 * at 1 (0 is invalid).
 **************************************************************/
typedef enum {
    BASIC           = 1,
    FIRE,
    WATER,
    ICE,
    WIND,
    EARTH,
    METAL,
    LIGHT,
    DARK,
} TYPE_ID;

/// The number of unique TYPE_ID members.
#define N_TYPE (DARK+1)

/**************************************************************/
#endif // _TYPE_H_
