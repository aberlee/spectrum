/**********************************************************//**
 * @file wait.h
 * @brief Defines WAIT.
 * @author Rena Shinomiya
 * @date April 22, 2018
 **************************************************************/

#ifndef _WAIT_H_
#define _WAIT_H_

#include <stdbool.h>            // bool
#include "game.h"               // KEY

/**********************************************************//**
 * @enum WAIT_STATE
 * @brief State flags for a WAIT process.
 **************************************************************/
typedef enum {
    WAIT_BEFORE,
    WAIT_DURING,
    WAIT_AFTER,
} WAIT_STATE;

/**********************************************************//**
 * @struct WAIT
 * @brief Control information used when waiting for a key.
 **************************************************************/
typedef struct {
    KEY Key;
    WAIT_STATE State;
} WAIT;

/**********************************************************//**
 * @brief Static initializer for a WAIT object.
 * @param key: The key to wait for.
 **************************************************************/
#define WAIT_INITIALIZER(key) {key, WAIT_BEFORE}

/**************************************************************/
extern void ResetWait(WAIT *wait);
extern void UpdateWait(WAIT *wait);
extern bool IsWaiting(const WAIT *wait);

/**************************************************************/
#endif // _WAIT_H_
