/**********************************************************//**
 * @file event.c
 * @brief Contains functions pertaining to events.
 * @author Rena Shinomiya
 * @date April 15, 2018
 **************************************************************/

#include "event.h"              // EVENT
#include "event.i"              // EVENT_DATA

/**********************************************************//**
 * @brief Gets the events for the given map.
 * @param id: The map identity to get events on.
 * @return Pointer to the constant event data.
 **************************************************************/
const EVENT* Events(MAP_ID id) {
    return EVENT_DATA[id];
}

/**************************************************************/
