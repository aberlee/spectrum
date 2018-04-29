/**********************************************************//**
 * @file event.h
 * @brief Header file for EVENT data on the overworld.
 * @author Rena Shinomiya
 * @date April 9th, 2018
 **************************************************************/

#ifndef _EVENT_H_
#define _EVENT_H_

#include "coordinate.h"         // COORDINATE
#include "location.h"           // LOCATION_ID
#include "shop.h"               // SHOP_ID
#include "species.h"            // SPECIES_ID

/**********************************************************//**
 * @struct WARP
 * @brief Data describing a warp between maps.
 **************************************************************/
typedef struct {
    COORDINATE Destination;     ///< Destination tile coordinate.
    LOCATION_ID Location;       ///< Destination LOCATION.
    DIRECTION Direction;        ///< Destination point direction.
} WARP;

/**********************************************************//**
 * @struct BOSS
 * @brief Data about a structured BOSS encounter.
 **************************************************************/
typedef struct {
    SPECIES_ID Spectra[3];      ///< Boss spectra that appear.
    int Level;                  ///< Level of the bosses.
} BOSS;

/**********************************************************//**
 * @enum EVENT_TYPE
 * @brief Type identifiers that describe which member of the
 * EVENT Union field should be accessed.
 **************************************************************/
typedef enum {
    EVENT_WARP      = 1,
    EVENT_TEXT,
    EVENT_SHOP,
    EVENT_BOSS,
    EVENT_REDIRECT,
} EVENT_TYPE;

/**********************************************************//**
 * @struct EVENT
 * @brief Describes an event that happens on a location.
 **************************************************************/
typedef struct {
    EVENT_TYPE Type;            ///< Which field of the Union is available.
    union {
        WARP Warp;              ///< EVENT_WARP field.
        const char *Text;       ///< EVENT_TEXT field.
        SHOP_ID Shop;           ///< EVENT_SHOP field.
        BOSS Boss;              ///< EVENT_BOSS field.
        int Redirect;           ///< EVENT_REDIRECT
    } Union;                    ///< EVENT data stored as a union.
} EVENT;

/**************************************************************/
extern const EVENT* Events(MAP_ID id);

/**************************************************************/
#endif // _EVENT_H_
