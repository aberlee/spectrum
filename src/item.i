/**********************************************************//**
 * @file item.i
 * @brief 
 * @author Rena Shinomiya
 * @date April 22, 2018
 **************************************************************/

#ifndef _ITEM_I_
#define _ITEM_I_

#include "effect.h"             // EFFECT
#include "item.h"               // ITEM, ITEM_FLAGS

/**********************************************************//**
 * @var ITEM_DATA
 * @brief Holds read-only constant ITEM data indexed by the
 * ITEM_ID.
 **************************************************************/
static const ITEM ITEM_DATA[] = {

    ////////////////////////////////////////////////////////
    // Medical
    [SERUM] = {
        .Name           = "Serum",
        .Price          = 10,
        .Effect         = CURE_POISON,
        .Flags          = ANYWHERE,
        .Description    = "A concoction that cures poison.",
    },

    [SHOVEL] = {
        .Name           = "Shovel",
        .Price          = 39,
        .Effect         = CURE_BURY,
        .Flags          = REUSABLE|ANYWHERE,
        .Description    = "A big metal shovel. In battle, use it to unbury the target.",
    },

    [WET_TOWEL] = {
        .Name           = "Wet Towel",
        .Price          = 2,
        .Effect         = CURE_AFLAME,
        .Flags          = ANYWHERE,
        .Description    = "A wet towel that puts out fires.",
    },

    [REFRESHER] = {
        .Name           = "Refresher",
        .Price          = 12,
        .Effect         = CURE_ANY,
        .Flags          = ANYWHERE,
        .Description    = "A medecine that cures the target's status ailments.",
    },

    ////////////////////////////////////////////////////////
    // Foods
    [CHOCOLATE] = {
        .Name           = "Chocolate",
        .Price          = 1,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 6,
        .Flags          = ANYWHERE,
        .Description    = "A block of milk chocolate that restores around 6 Hp.",
    },

    [YOGURT] = {
        .Name           = "Yogurt",
        .Price          = 1,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 24,
        .Flags          = ANYWHERE,
        .Description    = "Well-aged yoghurt that restores around 24 Hp.",
    },

    [SANDWICH] = {
        .Name           = "Sandwich",
        .Price          = 6,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 60,
        .Flags          = ANYWHERE,
        .Description    = "An assortment of sandwiches that restores around 60 Hp.",
    },

    [BURGER] = {
        .Name           = "Burger",
        .Price          = 6,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 60,
        .Flags          = ANYWHERE,
        .Description    = "A juicy burger that restores around 60 Hp.",
    },

    [STEAK] = {
        .Name           = "Steak",
        .Price          = 12,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 150,
        .Flags          = ANYWHERE,
        .Description    = "Gourmet steak that restores around 150 Hp.",
    },

    [SALAD] = {
        .Name           = "Salad",
        .Price          = 4,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 100,
        .Flags          = ANYWHERE,
        .Description    = "A healthy vegetable salad that restores around 100 Hp.",
    },

    [SPAGHETTI] = {
        .Name           = "Spaghetti",
        .Price          = 8,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 80,
        .Flags          = ANYWHERE,
        .Description    = "Nutritious noodles in tomato sauce that restores around 80 Hp.",
    },

    [JERKEY] = {
        .Name           = "Jerkey",
        .Price          = 4,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 30,
        .Flags          = ANYWHERE,
        .Description    = "Dried meat that restores around 30 Hp.",
    },

    [PIE] = {
        .Name           = "Pie",
        .Price          = 12,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 120,
        .Flags          = ANYWHERE,
        .Description    = "A delicious baked pie that restores around 120 Hp.",
    },

    [FULL_MEAL] = {
        .Name           = "Full Meal",
        .Price          = 16,
        .Effect         = HEAL_PERCENT,
        .Argument       = 100,
        .Flags          = ANYWHERE,
        .Description    = "A nutritious, complete meal that restores all Hp.",
    },

    ////////////////////////////////////////////////////////
    // Drink
    [WATER_BOTTLE] = {
        .Name           = "Water",
        .Price          = 1,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 60,
        .Flags          = ANYWHERE,
        .Description    = "Pure spring water that restores around 60 Hp.",
    },

    [JUICE] = {
        .Name           = "Juice",
        .Price          = 1,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 60,
        .Flags          = ANYWHERE,
        .Description    = "Nutritious fruit juice that restores around 60 Hp.",
    },

    [SODA] = {
        .Name           = "Soda",
        .Price          = 1,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 30,
        .Flags          = ANYWHERE,
        .Description    = "A sugary carbonated beverage that restores around 30 Hp.",
    },

    [COFFEE] = {
        .Name           = "Coffee",
        .Price          = 3,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 30,
        .Flags          = ANYWHERE,
        .Description    = "Black, bitter coffee that restores around 30 Hp.",
    },

    [TEA] = {
        .Name           = "Tea",
        .Price          = 3,
        .Effect         = HEAL_CONSTANT,
        .Argument       = 80,
        .Flags          = ANYWHERE,
        .Description    = "Healthy, piping hot green tea that restores around 80 Hp.",
    },

    [POTION] = {
        .Name           = "Potion",
        .Price          = 20,
        .Effect         = HEAL_PERCENT,
        .Argument       = 100,
        .Flags          = ANYWHERE,
        .Description    = "A scientific potion that restores all Hp.",
    },

    ////////////////////////////////////////////////////////
    // Herbs
    [STRONG_HERB] = {
        .Name           = "Strong Herb",
        .Price          = 24,
        .Effect         = BOOST_ATTACK,
        .Argument       = 1,
        .Flags          = BATTLE_ONLY,
        .Description    = "A reddish herb that exudes power. It raises attack in battle.",
    },

    [TOUGH_HERB] = {
        .Name           = "Tough Herb",
        .Price          = 22,
        .Effect         = BOOST_DEFEND,
        .Argument       = 1,
        .Flags          = BATTLE_ONLY,
        .Description    = "A blueish, rigid herb. It raises defense in battle.",
    },

    [QUICK_HERB] = {
        .Name           = "Quick Herb",
        .Price          = 22,
        .Effect         = BOOST_EVADE,
        .Argument       = 1,
        .Flags          = BATTLE_ONLY,
        .Description    = "A yellow herb that clears the mind. It raises evasion in battle.",
    },

    [LUCK_HERB] = {
        .Name           = "Luck Herb",
        .Price          = 45,
        .Effect         = BOOST_LUCK,
        .Argument       = 1,
        .Flags          = BATTLE_ONLY,
        .Description    = "A golden herb of legend. It raises luck in battle.",
    },

    ////////////////////////////////////////////////////////
    // Key items
    [FISHING_ROD] = {
        .Name           = "Fishing Rod",
        .Price          = 48,
        .Effect         = 0,
        .Flags          = REUSABLE|IMPORTANT|MENU_ONLY,
        .Description    = "A fishing rod for finding spectra.",
    },
};

/**************************************************************/
#endif // _ITEM_I_
