/**********************************************************//**
 * @file coordinate.h
 * @brief Defines a type for 2D integer coordinates.
 * @author Rena Shinomiya
 * @date March 25th, 2018
 **************************************************************/

#ifndef _COORDINATE_H_
#define _COORDINATE_H_

/**********************************************************//**
 * @struct COORDINATE
 * @brief Defines a 2-D integer coordinate pair.
 **************************************************************/
typedef struct {
    int X;                      ///< X position.
    int Y;                      ///< Y position.
} COORDINATE;

/**********************************************************//**
 * @enum DIRECTION
 * @brief Constants for each of the four cardinal directions.
 **************************************************************/
typedef enum {
    DOWN            = 0,
    LEFT            = 1,
    UP              = 2,
    RIGHT           = 3,
} DIRECTION;

static inline DIRECTION OppositeDirection(DIRECTION start) {
    return (start+2)%4;
}

/**************************************************************/
#endif // _COORDINATE_H_
