/**********************************************************//**
 * @file battler.c
 * @brief Implements battler properties and graphics.
 * @author Rena Shinomiya
 * @date May 26, 2018
 **************************************************************/

#include "battler.h"        // BATTLER

/**********************************************************//**
 * @brief Gets the multiplication factor for the battler's
 * base stat, given a certain boost level.
 * @param boost: Levels of boosting to apply.
 * @return Factor to multiply a stat by.
 **************************************************************/
static inline float BoostScale(int boost) {
    float scale = (float)boost/BOOST_MAX;
    if (boost>0) {
        return 1.0 + 3.0*scale*scale;
    } else if (boost<0) {
        return 1.0 - 0.75*scale*scale;
    } else {
        return 1.0;
    }
}

/**********************************************************//**
 * @brief Get the battler's boosted attack stat.
 * @param battler: The battler to read.
 * @return Boosted attack stat.
 **************************************************************/
int BattlerAttack(const BATTLER *battler) {
    return battler->Spectra->Attack*BoostScale(battler->AttackBoost);
}

/**********************************************************//**
 * @brief Get the battler's boosted defend stat.
 * @param battler: The battler to read.
 * @return Boosted defend stat.
 **************************************************************/
int BattlerDefend(const BATTLER *battler) {
    return battler->Spectra->Defend*BoostScale(battler->DefendBoost);
}

/**********************************************************//**
 * @brief Get the battler's boosted evade stat.
 * @param battler: The battler to read.
 * @return Boosted evade stat.
 **************************************************************/
int BattlerEvade(const BATTLER *battler) {
    return battler->Spectra->Evade*BoostScale(battler->EvadeBoost);
}

/**********************************************************//**
 * @brief Get the battler's boosted luck stat.
 * @param battler: The battler to read.
 * @return Boosted luck stat.
 **************************************************************/
int BattlerLuck(const BATTLER *battler) {
    return battler->Spectra->Luck*BoostScale(battler->LuckBoost);
}

/**************************************************************/
