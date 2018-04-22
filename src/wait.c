/**********************************************************//**
 * @file wait.c
 * @brief Implements a number of WAIT functions.
 * @author Rena Shinomiya
 * @date April 22, 2018
 **************************************************************/

#include <stdbool.h>            // bool

#include "game.h"               // KEY
#include "wait.h"               // WAIT

/**********************************************************//**
 * @brief Reset the WAIT state so it can begin waiting again.
 * @param wait: The WAIT data to reset.
 **************************************************************/
void ResetWait(WAIT *wait) {
    wait->State = WAIT_BEFORE;
}

/**********************************************************//**
 * @brief Reads the keyboard state to update the waiting
 * status. 
 * @param wait: The WAIT data to update.
 **************************************************************/
void UpdateWait(WAIT *wait) {
    switch (wait->State) {
    case WAIT_BEFORE:
        if (KeyDown(wait->Key)) {
            wait->State = WAIT_DURING;
        }
        break;
    
    case WAIT_DURING:
        if (!KeyDown(wait->Key)) {
            wait->State = WAIT_AFTER;
        }
        break;
    default:
        break;
    }
}

/**********************************************************//**
 * @brief Check if the WAIT is currently waiting for the key.
 * @param wait: WAIT to check.
 * @return True if it's waiting.
 **************************************************************/
bool IsWaiting(const WAIT *wait) {
    return wait->State != WAIT_AFTER;
}

/**************************************************************/
