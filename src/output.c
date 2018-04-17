/**********************************************************//**
 * @file output.c
 * @brief Controls the game's output system.
 * @author Rena Shinomiya
 * @date April 16, 2018
 **************************************************************/

#include <string.h>             // strncpy, strlen
#include <stdio.h>              // snprintf

#include "output.h"             // MESSAGE_SIZE
#include "game.h"               // KEY
#include "debug.h"              // eprintf

/**************************************************************/
/// @brief Number of characters typed per second.
#define TYPING_SPEED 16

/**************************************************************/
/// @brief Queue for output messages.
static char Log[LOG_SIZE][MESSAGE_SIZE+1] = {{'\0'}};

/// @brief Pointer to current output message.
static int Head = 0;

/// @brief Pointer to one past the end of the queue.
static int Tail = 0;

/// @brief The character position that's been typed to within
/// the message at the Head of the Log.
static int CurrentCharacter = 0;

/// @brief True if the system is waiting for the user.
static bool WaitingForUser = false;

/**********************************************************//**
 * @brief Enqueues a new output message.
 * @param text: The message.
 **************************************************************/
void Output(const char *text) {
    strncpy(Log[Tail], text, MESSAGE_SIZE);
    Tail = (Tail+1)%LOG_SIZE;
    if (Head == Tail) {
        eprintf("Output queue overflow.");
    }
}

/**********************************************************//**
 * @brief Types more characters of the output based on the
 * duration of the previous frame.
 **************************************************************/
void UpdateOutput(void) {
    static bool isDown = false;
    static float Progress = 0.0;
    int max = strlen(Log[Head]);
    if (Head == Tail) {
        return;
    }
    
    // User intracts by pressing the space bar once -
    // can't speed through everything on one press though!
    bool activateKey = false;
    if (!isDown && KeyDown(KEY_CONFIRM)) {
        activateKey = true;
        isDown = true;
    } else if (isDown && !KeyDown(KEY_CONFIRM)) {
        isDown = false;
    }
    
    // Done typing - wait for user to press CONFIRM
    WaitingForUser = false;
    if (CurrentCharacter == max) {
        if (activateKey) {
            Head = (Head+1) % LOG_SIZE;
            // Reset the progress for the next output
            Progress = 0.0;
            CurrentCharacter = 0;
        } else {
            WaitingForUser = true;
        }
    } else {
        // Update the progress of the typing
        Progress += TYPING_SPEED*LastFrameTimeElapsed;
        if (Progress > max || activateKey) {
            CurrentCharacter = max;
        } else {
            CurrentCharacter = (int)Progress;
        }
    }
}

/**********************************************************//**
 * @brief Gets the amount of the current output that's been
 * typed.
 * @return Pointer to the output string.
 **************************************************************/
const char *GetOutput(void) {
    static char display[MESSAGE_SIZE+1];
    if (Head == Tail) {
        return NULL;
    }
    strncpy(display, Log[Head], CurrentCharacter);
    display[CurrentCharacter] = '\0';
    return display;
}

/**********************************************************//**
 * @brief Determines if there is more output or not.
 * @return True if there's no more output.
 **************************************************************/
bool OutputDone(void) {
    return Head == Tail;
}

/**********************************************************//**
 * @brief Determines if the output system is waiting for the
 * user to press the CONFIRM key.
 * @return True if the output system is waiting.
 **************************************************************/
bool OutputWaiting(void) {
    return WaitingForUser;
}

/**************************************************************/
