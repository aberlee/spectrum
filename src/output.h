/**********************************************************//**
 * @file output.h
 * @brief Header file for the game output system.
 * @author Rena Shinomiya
 * @date April 16, 2018
 **************************************************************/

#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdio.h>              // snprintf
#include <stdbool.h>            // bool

/**************************************************************/
/// @brief The size of an output message in characters.
#define MESSAGE_SIZE 255

/// @brief The number of output messages that can be queued.
#define LOG_SIZE 32

/**********************************************************//**
 * @brief Enqueues a new output message, with formatting.
 * @param format: printf-style format string for the output.
 * @param ...: Variadic arguments to format.
 **************************************************************/
#define OutputF(format, ...) {\
    char buf[MESSAGE_SIZE+1];\
    snprintf(buf, MESSAGE_SIZE, format, __VA_ARGS__);\
    Output(buf);\
}

/**************************************************************/
extern void Output(const char *text);
extern void UpdateOutput(void);
extern const char *GetOutput(void);
extern bool OutputDone(void);
extern bool OutputWaiting(void);

/**************************************************************/
#endif // _OUTPUT_H_
