/**********************************************************//**
 * @file type.h
 * @brief Defines universal types that are used across the
 * species and techniques.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _TYPE_H_
#define _TYPE_H_

/**********************************************************//**
 * @enum TYPE_ID
 * @brief Defines a constant for each type used, starting
 * at 1 (0 is invalid).
 **************************************************************/
typedef enum {
    BASIC=1,    ///< Basic type - unclassifiable or typeless.
    FIRE,       ///< Fire type - heat-related.
    WATER,      ///< Water type - marine life and liquids.
    ICE,        ///< Ice type - anything cold.
    WIND,       ///< Wind type - weather and birds.
    EARTH,      ///< Earth type - geology or dirt related.
    METAL,      ///< Metal-type - mineral or machine related.
    LIGHT,      ///< Light-type - based on electricity and energy.
    DARK,       ///< Dark-type - based on night, poison, and fear.
} TYPE_ID;

#define N_TYPE (DARK+1)

/**************************************************************/
#endif // _TYPE_H_
