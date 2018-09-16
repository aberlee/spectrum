/**********************************************************//**
 * @file effect.c
 * @brief Implements technique and items effects.
 * @author Rena Shinomiya
 * @date September 08, 2018
 **************************************************************/

#include <stdbool.h>        // bool

#include "random.h"         // randint
#include "effect.h"         // EFFECT_ID
#include "battler.h"        // BATTLER
#include "output.h"         // Output
#include "debug.h"          // eprintf, assert

/**********************************************************//**
 * @enum STAT_ID
 * @brief Keys for each unique battler stat, as would be used
 * in an array indexed from 0.
 **************************************************************/
typedef enum {
    ATTACK,
    DEFEND,
    EVADE,
    LUCK,
} STAT_ID;

/**********************************************************//**
 * @brief Determines if the effect hits the target.
 * @param id: Effect to check.
 * @param argument: Argument to the effect.
 * @return True if the effect hits the target.
 **************************************************************/
bool ShouldEffectActivate(EFFECT_ID id, int argument) {
    switch (id) {
    // Efefcts where argument represents chance to occur.
    case AFFLICT_BURY:
    case AFFLICT_AFLAME:
    case AFFLICT_POISON:
    case AFFLICT_SHOCK:
    case AFFLICT_ANY:
        return randint(0, 99) < argument;
    
    default:
        return true;
    }
}

/**********************************************************//**
 * @brief Tries to afflict a status ailment on the battler.
 * @param battler: Target of the ailment.
 * @param ailment: Ailment to afflict.
 * @return True if the ailment was applied successfully.
 **************************************************************/
static bool Afflict(BATTLER *battler, AILMENT_ID ailment) {
    // Already afflicted?
    if (battler->Spectra->Ailment) {
        return false;
    }
    
    // Blocked by type?
    bool typeBlocks = false;
    for (int i=0; i<2; i++) {
        TYPE_ID type = BattlerSpecies(battler)->Type[i];
        switch (type) {
        case FIRE:
        case WATER:
            typeBlocks |= ailment==AFLAME;
            break;

        case METAL:
            typeBlocks |= ailment==POISONED;
            break;
        
        default:
            break;
        }
    }
    if (typeBlocks) {
        return false;
    }
    
    // Apply effect
    battler->Spectra->Ailment = ailment;
    switch (ailment) {
    case POISONED:
        OutputF("%s was poisoned!", BattlerName(battler));
        break;
    case SHOCKED:
        OutputF("%s was shocked!", BattlerName(battler));
        break;
    case BURIED:
        OutputF("%s was buried in the ground!", BattlerName(battler));
        break;
    case ASLEEP:
        OutputF("%s fell asleep!", BattlerName(battler));
        break;
    case AFLAME:
        OutputF("%s was set on fire!", BattlerName(battler));
        break;
    default:
        break;
    }
    return true;
}

/**********************************************************//**
 * @brief Cures status ailments for the battler.
 * @param battler: Battler to cure.
 * @param id: Ailment to remove.
 * @return True if the ailment was removed successfully.
 **************************************************************/
static bool CureInBattle(BATTLER *battler, AILMENT_ID id) {
    if (battler->Spectra->Ailment == id) {
        battler->Spectra->Ailment = 0;
        switch (id) {
        case POISONED:
            OutputF("%s is no longer poisoned!", BattlerName(battler));
            break;
        case SHOCKED:
            OutputF("%s is no longer shocked!", BattlerName(battler));
            break;
        case BURIED:
            OutputF("%s was unburied!", BattlerName(battler));
            break;
        case ASLEEP:
            OutputF("%s woke up!", BattlerName(battler));
            break;
        case AFLAME:
            OutputF("%s was extinguished!", BattlerName(battler));
            break;
        default:
            break;
        }
        return true;
    }
    return false;
}

/**********************************************************//**
 * @brief Restores an amount of health to a battler.
 * @param battler: Battler to heal.
 * @param amount: Maximum amount of health to heal.
 * @return True if the battler was healed.
 **************************************************************/
static bool HealInBattle(BATTLER *battler, int amount) {
    int delta = Heal(battler->Spectra, amount);
    if (delta) {
        OutputF("%s healed by %d!", BattlerName(battler), delta);
        return true;
    }
    return false;
}

/**********************************************************//**
 * @brief Gets the boost value for a stat.
 * @param battler: Battler to check.
 * @param stat: Stat to  check boost for.
 * @return Boost value.
 **************************************************************/
static int BattlerBoost(const BATTLER *battler, STAT_ID stat) {
    switch (stat) {
    case ATTACK:
        return battler->AttackBoost;
    case DEFEND:
        return battler->DefendBoost;
    case EVADE:
        return battler->EvadeBoost;
    case LUCK:
        return battler->LuckBoost;
    default:
        return 0;
    }
}

/**********************************************************//**
 * @brief Increase or decrease the boost level for a battler.
 * @param battler: Battler to boost.
 * @param stat: Stat to boost.
 * @param boost: Amount toboost by.
 * @return True if any boost changed.
 **************************************************************/
static bool BoostStat(BATTLER *battler, STAT_ID stat, int boost) {
    // Determine if the boost does anything
    int start = BattlerBoost(battler, stat);
    int final = start + boost;
    if (final > BOOST_MAX) {
        final = BOOST_MAX;
    } else if (final < -BOOST_MAX) {
        final = -BOOST_MAX;
    }
    if (start == final) {
        return false;
    }
    
    // Text formatting
    const char *change = (final>start)? "rose": "fell";
    const char *magnitude = (abs(final-start)>1)? "a lot": "a little";
    switch (stat) {
    case ATTACK:
        battler->AttackBoost = final;
        OutputF("%s's attack %s %s!", BattlerName(battler), change, magnitude);
        break;
    
    case DEFEND:
        battler->DefendBoost = final;
        OutputF("%s's defend %s %s!", BattlerName(battler), change, magnitude);
        break;
    
    case EVADE:
        battler->EvadeBoost = final;
        OutputF("%s's evade %s %s!", BattlerName(battler), change, magnitude);
        break;
    
    case LUCK:
        battler->DefendBoost = final;
        OutputF("%s's luck %s %s!", BattlerName(battler), change, magnitude);
        break;
    }
    return true;
}

/**********************************************************//**
 * @brief Reset all the battler's boosts to 0.
 * @param battler: Battler to reset.
 **************************************************************/
static void ResetStats(BATTLER *battler) {
    battler->AttackBoost = 0;
    battler->DefendBoost = 0;
    battler->EvadeBoost = 0;
    battler->LuckBoost = 0;
    OutputF("%s's stats went back to normal.", BattlerName(battler));
}

/**********************************************************//**
 * @brief Destroys the battler immediately.
 * @param battler: Battler to kill.
 **************************************************************/
static void Kill(BATTLER *battler) {
    battler->Spectra->Health = 0;
    OutputF("%s died!", BattlerName(battler));
}

/**********************************************************//**
 * @brief Applies a flag to the battler.
 * @param battler: Battler to change.
 * @param flag: Flag to apply.
 **************************************************************/
static void SetFlag(BATTLER *battler, BATTLER_FLAGS flag) {
    battler->Flags |= flag;
    switch (flag) {
    case BATTLER_DEFEND:
        OutputF("%s is defending.", BattlerName(battler));
        break;
    default:
        break;
    }
}

/**********************************************************//**
 * @brief Applies any effect to the battler.
 * @param id: Effect to apply.
 * @param user: Battler using the effect.
 * @param target: Battler targetted by the effect.
 * @param argument: Argument to the effect.
 * @return True if the effect changed something.
 **************************************************************/
bool ApplyEffectInBattle(EFFECT_ID id, BATTLER *user, BATTLER *target, int argument) {
    switch (id) {
    // Afflicting ailments
    case AFFLICT_BURY:
        return Afflict(target, BURIED);
    case AFFLICT_AFLAME:
        return Afflict(target, AFLAME);
    case AFFLICT_POISON:
        return Afflict(target, POISONED);
    case AFFLICT_SHOCK:
        return Afflict(target, SHOCKED);
    case AFFLICT_SLEEP:
        return Afflict(target, ASLEEP);
    case AFFLICT_ANY:
        return Afflict(target, randint(1, 5));
    
    // Curing ailments
    case CURE_BURY:
        return CureInBattle(target, BURIED);
    case CURE_AFLAME:
        return CureInBattle(target, AFLAME);
    case CURE_POISON:
        return CureInBattle(target, POISONED);
    case CURE_SHOCK:
        return CureInBattle(target, SHOCKED);
    case CURE_SLEEP:
        return CureInBattle(target, ASLEEP);
    case CURE_ANY:
        return CureInBattle(target, target->Spectra->Ailment);
    
    // Healing
    case HEAL_CONSTANT:
        return HealInBattle(target, argument);
    case HEAL_PERCENT:
        return HealInBattle(target, target->Spectra->MaxHealth*argument/100);
    
    // Stat boosting
    case RESET_STATS:
        ResetStats(target);
        return true;
    case BOOST_ATTACK:
        return BoostStat(target, ATTACK, argument);
    case BOOST_DEFEND:
        return BoostStat(target, DEFEND, argument);
    case BOOST_EVADE:
        return BoostStat(target, EVADE, argument);
    case BOOST_LUCK:
        return BoostStat(target, LUCK, argument);
    
    // Special effects
    case KILL_USER:
        Kill(user);
        return true;
    case EFFECT_DEFEND:
        SetFlag(target, BATTLER_DEFEND);
        return true;
    
    // Effects that are implemented elsewhere
    case EFFECT_SPECIAL:
        eprintf("Attempt to use a special effect in battle: %d\n", id);
        assert(false);
        break;

    default:
        return false;
    }
}

bool ApplyEffectInMenu(EFFECT_ID id, SPECTRA *target, int argument) {
    return false;
}

/**************************************************************/
