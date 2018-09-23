/**********************************************************//**
 * @file random.h
 * @brief Basic pseudorandom number generator functions to
 * go on top of the stdlib rand() and RAND_MAX.
 * @author Rena Shinomiya
 * @version 1.0
 * @date January 2017
 **************************************************************/

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <stdlib.h>             // rand, RAND_MAX

/**********************************************************//**
 * @brief Generate a random integer from a to b, inclusive.
 * @param a: The lower bound.
 * @param b: The upper bound.
 * @return A random integer.
 **************************************************************/
static inline int randint(int a, int b) {
    return a + (rand() % (b-a+1));
}

/**********************************************************//**
 * @brief Generate a random double from a to b, inclusive.
 * @param a: The lower bound.
 * @param b: The upper bound.
 * @return A random double.
 **************************************************************/
static inline double uniform(double a, double b) {
    return a + (b-a)*((double)rand()/RAND_MAX);
}

/**************************************************************/
#endif // _RANDOM_H_
