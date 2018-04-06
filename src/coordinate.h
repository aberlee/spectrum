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
    int X;
    int Y;
} COORDINATE;

#define EXPAND(coordinate) coordinate.X, coordinate.Y

/**********************************************************//**
 * @enum DIRECTION_TYPE
 * @brief Constants for each of the four cardinal directions.
 **************************************************************/
typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT,
} DIRECTION;

/**************************************************************/
#endif // _COORDINATE_H_