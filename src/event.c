/**********************************************************//**
 * @file event.c
 * @brief Contains functions pertaining to events.
 * @author Rena Shinomiya
 * @date April 15, 2018
 **************************************************************/

#include "event.h"              // EVENT
#include "event.i"              // EVENT_DATA

const EVENT* Events(MAP_ID id) {
    return EVENT_DATA[id];
}
