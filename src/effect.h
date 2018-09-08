/**********************************************************//**
 * @file effect.h
 * @brief Lists all the effects that take place during battle
 * or through the menu.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <stdbool.h>        // bool

#include "technique.h"      // EFFECT_ID
#include "battler.h"        // BATTLER

/**************************************************************/
extern bool ShouldEffectActivate(EFFECT_ID id, int argument);
extern bool ApplyEffectInBattle(EFFECT_ID id, BATTLER *user, BATTLER *target, int argument);

/**************************************************************/
#endif // _EFFECT_H_
