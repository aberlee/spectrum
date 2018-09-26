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
#include "person.h"             // PERSON_ID
#include "battle.h"             // ENEMY

/**********************************************************//**
 * @enum SWITCH
 * @brief Stores slots for permanent game switches and
 * variables stored in the player's save file.
 **************************************************************/
typedef enum {
    SWITCH_AMY_HOUSE_1          = 1,
} SWITCH;

#define N_SWITCH 256

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
 * @struct PRESENT
 * @brief Stores one present on the map. Can only be acquired
 * one time, and then it goes away.
 **************************************************************/
typedef struct {
    ITEM_ID Item;
    SWITCH Switch;
} PRESENT;

typedef enum {
    PERSON_SPEECH,
    PERSON_HOSPITAL,
    PERSON_SHOP,
} PERSON_TYPE;

/**********************************************************//**
 * @struct PERSON
 * @brief Defines data for each non-player character.
 **************************************************************/
typedef struct {
    PERSON_ID Person;
    DIRECTION Direction;
    PERSON_TYPE Type;
    const char *Speech;
    SHOP_ID Shop;
} PERSON;

/**********************************************************//**
 * @enum EVENT_TYPE
 * @brief Type identifiers that describe which member of the
 * EVENT Union field should be accessed.
 **************************************************************/
typedef enum {
    EVENT_WARP      = 1,
    EVENT_TEXT,
    EVENT_PERSON,
    EVENT_PRESENT,
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
        PERSON Person;          ///< EVENT_PERSON field.
        PRESENT Present;        ///< EVENT_PRESENT field.
        BOSS Boss;              ///< EVENT_BOSS field.
        int Redirect;           ///< EVENT_REDIRECT
    } Union;                    ///< EVENT data stored as a union.
} EVENT;

/**************************************************************/
extern const EVENT* Events(MAP_ID id);

/**************************************************************/
#endif // _EVENT_H_
