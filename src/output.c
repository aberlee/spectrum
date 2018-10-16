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
#define TYPING_SPEED 32

#define FAST_FORWARD_WAIT 0.5f

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
    int length = strlen(text);
    strncpy(Log[Tail], text, (length<MESSAGE_SIZE)? length: MESSAGE_SIZE);
    Log[Tail][length] = '\0';
    Tail = (Tail+1)%LOG_SIZE;
    if (Head == Tail) {
        eprintf("Output queue overflow.");
    }
}

void OutputSplitByCR(const char *text) {
    char page[MESSAGE_SIZE+1];
    char *p = page;
    for (const char *t=text; *t; t++) {
        if (*t == '\r') {
            *p = '\0';
            Output(page);
            p = page;
        } else {
            *p = *t;
            if (p-page < (ptrdiff_t)MESSAGE_SIZE) {
                p++;
            } else {
                eprintf("Buffer overflow.");
            }
        }
    }
    if (p != page) {
        *p = '\0';
        Output(page);
    }
}

/**********************************************************//**
 * @brief Types more characters of the output based on the
 * duration of the previous frame.
 **************************************************************/
void UpdateOutput(void) {
    static float Progress = 0.0f;
    static float FastForwardTime = 0.0f;
    int max = strlen(Log[Head]);
    if (Head == Tail) {
        return;
    }
    
    if (KeyUp(KEY_CONFIRM)) {
        FastForwardTime = 0.0f;
    }
    
    WaitingForUser = true;
    if (CurrentCharacter == max) {
        // Done typing - wait for user to press CONFIRM
        bool again = FastForwardTime && al_get_time()>FastForwardTime;
        if (KeyJustUp(KEY_CONFIRM) || again) {
            WaitingForUser = false;
            Head = (Head+1) % LOG_SIZE;
            Progress = 0.0;
            CurrentCharacter = 0;
            if (again) {
                CurrentCharacter = max;
                FastForwardTime = al_get_time() + FAST_FORWARD_WAIT;
            }
        }
    } else {
        // Update the progress of the typing
        Progress += TYPING_SPEED*LastFrameTime();
        if (Progress > max) {
            CurrentCharacter = max;
        } else if (KeyJustDown(KEY_CONFIRM)) {
            CurrentCharacter = max;
            FastForwardTime = al_get_time() + FAST_FORWARD_WAIT;
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
    return Head == Tail && !WaitingForUser;
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
