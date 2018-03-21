/**********************************************************//**
 * @file technique.i
 * @brief Defines the TECHNIQUE_DATA constant array.
 * @author Rena Shinomiya
 * @date March 19th, 2018
 **************************************************************/

#ifndef _TECHNIQUE_I_
#define _TECHNIQUE_I_

#include "technique.h"      // TECHNIQUE_ID, TECHNIQUE
#include "effect.h"         // TARGET_ID, EFFECT_ID

/**********************************************************//**
 * @var TECHNIQUE_DATA
 * @brief Defines a constant array for every technique's data.
 **************************************************************/
const TECHNIQUE TECHNIQUE_DATA[N_TECHNIQUES] = {
    
    [BITE]={
        .Name="Bite",
        .Type=BASIC,
        .Power=30,
        .Cost=8,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The user attacks with its sharp teeth.",
    },
    
    [COLLIDE]={
        .Name="Collide",
        .Type=BASIC,
        .Power=60,
        .Cost=12,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A reckless impact that damages the target.",
    },
    
    [SLICE]={
        .Name="Slice",
        .Type=BASIC,
        .Power=70,
        .Cost=18,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A sharp blade is swung at the target, dealing damage.",
    },
    
    [RAMPAGE]={
        .Name="Rampage",
        .Type=BASIC,
        .Power=70,
        .Cost=20,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The user rampages and tackles the target, causing damage.",
    },
    
    [ENGULF]={
        .Name="Engulf",
        .Type=BASIC,
        .Power=75,
        .Cost=22,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The user absorbs the target into its amorphous body.",
    },
    
    [MANGLE]={
        .Name="Mangle",
        .Type=BASIC,
        .Power=100,
        .Cost=36,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A vicious ripping and tearing attack.",
    },
    
    [BOMBARD]={
        .Name="Bombard",
        .Type=BASIC,
        .Power=40,
        .Cost=24,
        .Target=ALL_ENEMIES,
        .Effect=0,
        .Argument=0,
        .Description="Objects are lobbed at each enemy, causing damage.",
    },
    
    [TRI_BEAM]={
        .Name="Tri Beam",
        .Type=BASIC,
        .Power=90,
        .Cost=48,
        .Target=ALL_ENEMIES,
        .Effect=0,
        .Argument=0,
        .Description="Three powerful beams of light are fired, hitting each enemy.",
    },
    
    [FLATTEN]={
        .Name="Flatten",
        .Type=BASIC,
        .Power=25,
        .Cost=6,
        .Target=ENEMY,
        .Effect=AFFLICT_BURY,
        .Argument=30,
        .Description="The user uses its bulk to squish the target. The target might be buried in the ground.",
    },
    
    [CRUSH]={
        .Name="Crush",
        .Type=BASIC,
        .Power=80,
        .Cost=26,
        .Target=ENEMY,
        .Effect=AFFLICT_BURY,
        .Argument=30,
        .Description="The user uses its bulk to squish the target. The target might be buried in the ground.",
    },
    
    [RECOVER]={
        .Name="Recover",
        .Type=BASIC,
        .Power=0,
        .Cost=30,
        .Target=YOURSELF,
        .Effect=HEAL_PERCENT,
        .Argument=50,
        .Description="The user regenerates its injured body parts, restoring half its HP.",
    },
    
    [PURGE]={
        .Name="Purge",
        .Type=BASIC,
        .Power=0,
        .Cost=6,
        .Target=YOURSELF,
        .Effect=CURE_ANY,
        .Argument=0,
        .Description="Toxins are extracted and eliminated. The user is cured from any status ailment.",
    },
    
    [DISPEL]={
        .Name="Dispel",
        .Type=BASIC,
        .Power=0,
        .Cost=60,
        .Target=ANYONE,
        .Effect=RESET_STATS,
        .Argument=0,
        .Description="Additional effects and boosts are removed from the target.",
    },
    
    [SHARPEN]={
        .Name="Sharpen",
        .Type=BASIC,
        .Power=0,
        .Cost=8,
        .Target=YOURSELF,
        .Effect=BOOST_ATTACK,
        .Argument=1,
        .Description="The user sharpens its claws or blades, increasing its attack.",
    },
    
    [SNIPER]={
        .Name="Sniper",
        .Type=BASIC,
        .Power=0,
        .Cost=30,
        .Target=YOURSELF,
        .Effect=BOOST_ATTACK,
        .Argument=2,
        .Description="The user takes aim at the target, increasing its attack a lot.",
    },
    
    [PREVENT]={
        .Name="Prevent",
        .Type=BASIC,
        .Power=0,
        .Cost=10,
        .Target=YOURSELF,
        .Effect=BOOST_DEFEND,
        .Argument=1,
        .Description="The user hunkers down, increasing its defense.",
    },
    
    [DISARM]={
        .Name="Disarm",
        .Type=BASIC,
        .Power=20,
        .Cost=8,
        .Target=ENEMY,
        .Effect=BOOST_ATTACK,
        .Argument=-1,
        .Description="The user disables the target's weapons. The target's attack is decreased.",
    },
    
    [LURK]={
        .Name="Lurk",
        .Type=BASIC,
        .Power=0,
        .Cost=8,
        .Target=ENEMY,
        .Effect=BOOST_DEFEND,
        .Argument=-1,
        .Description="The user stalks the target, decreasing the target's defense.",
    },
    
    [PROVOKE]={
        .Name="Provoke",
        .Type=BASIC,
        .Power=0,
        .Cost=24,
        .Target=ENEMY,
        .Effect=BOOST_DEFEND,
        .Argument=-2,
        .Description="The user bullies the target, decreasing its defense a lot.",
    },
    
    [EXPLOSION]={
        .Name="Explosion",
        .Type=BASIC,
        .Power=255,
        .Cost=0,
        .Target=EVERYONE_ELSE,
        .Effect=KILL_USER,
        .Argument=0,
        .Description="The user explodes, causing massive damage to everyone. The user dies after attacking.",
    },
    
    [GUST]={
        .Name="Gust",
        .Type=WIND,
        .Power=40,
        .Cost=8,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A gust of wind knocks the target down, dealing damage.",
    },
    
    [CYCLONE]={
        .Name="Cyclone",
        .Type=WIND,
        .Power=80,
        .Cost=24,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A large gust of wind blows the target away, causing damage.",
    },
    
    [HURRICANE]={
        .Name="Hurricane",
        .Type=WIND,
        .Power=150,
        .Cost=100,
        .Target=ALL_ENEMIES,
        .Effect=0,
        .Argument=0,
        .Description="A massive storm that hits all enemies.",
    },
    
    [HOT_GUST]={
        .Name="Hot Gust",
        .Type=FIRE,
        .Power=50,
        .Cost=14,
        .Target=ENEMY,
        .Effect=AFFLICT_AFLAME,
        .Argument=10,
        .Description="Hot air is blown at the target, possibly setting it on fire.",
    },
    
    [FIREWALL]={
        .Name="Firewall",
        .Type=FIRE,
        .Power=0,
        .Cost=12,
        .Target=YOURSELF,
        .Effect=BOOST_DEFEND,
        .Argument=2
        .Description="The user surrounds itself with fire, raising its defense a lot.",
    },
    
    [INFERNO]={
        .Name="Inferno",
        .Type=FIRE,
        .Power=120,
        .Cost=66,
        .Target=ENEMY,
        .Effect=AFFLICT_AFLAME,
        .Argument=30,
        .Description="A gargantuan beam of fire engulfs the target, possibly setting it on fire.",
    },
    
    [SWIM_ATTACK]={
        .Name="Swim Attack",
        .Type=WATER,
        .Power=40,
        .Cost=8,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The user tackles the target by swimming at it. This attack can also be used on land.",
    },
    
    [SURGE]={
        .Name="Surge",
        .Type=WATER,
        .Power=40,
        .Cost=20,
        .Target=ALL_ENEMIES,
        .Effect=0,
        .Argument=0,
        .Description="Water washes over the enemies, dealing damage.",
    },
    
    [RIPTIDE]={
        .Name="Riptide",
        .Type=WATER,
        .Power=70,
        .Cost=24,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The target is swept out to sea, causing damage.",
    },
    
    [MAELSTROM]={
        .Name="Maelstrom",
        .Type=WATER,
        .Power=100,
        .Cost=48,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A large watery vortex traps the target, causing damage.",
    },
    
    [TSUNAMI]={
        .Name="Tsunami",
        .Type=WATER,
        .Power=150,
        .Cost=100,
        .Target=ALL_ENEMIES,
        .Effect=0,
        .Argument=0,
        .Description="A massive tidal wave washes over the enemies, dealing massive damage.",
    },
    
    [POISON_CLAW]={
        .Name="Poison Claw",
        .Type=DARK,
        .Power=70,
        .Cost=22,
        .Target=ENEMY,
        .Effect=AFFLICT_POISON,
        .Argument=10,
        .Description="The target is slashed with the user's toxic claws. This may poison the target.",
    },
    
    [INKY_BANE]={
        .Name="Inky Bane",
        .Type=DARK,
        .Power=90,
        .Cost=30,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A blast of sickening ink hits the target in the face.",
    },
    
    [GUNK_ATTACK]={
        .Name="Gunk Attack",
        .Type=DARK,
        .Power=50,
        .Cost=18,
        .Target=ENEMY,
        .Effect=AFFLICT_POISON,
        .Argument=30,
        .Description="Nasty matter is ejected onto the target. This might poison it.",
    },
    
    [POISON_GRIP]={
        .Name="Poison Grip",
        .Type=DARK,
        .Power=80,
        .Cost=38,
        .Target=ENEMY,
        .Effect=AFFLICT_POISON,
        .Argument=50,
        .Description="The target is grabbed and squeezed by the user's toxic body. This usually poisons the target.",
    },
    
    [SMOG]={
        .Name="Smog",
        .Type=WIND,
        .Power=0,
        .Cost=10,
        .Target=ENEMY,
        .Effect=AFFLICT_POISON,
        .Argument=100,
        .Description="Nasty gases surround the target, poisoning it.",
    },
    
    [ZAP]={
        .Name="Zap",
        .Type=LIGHT,
        .Power=40,
        .Cost=8,
        .Target=ENEMY,
        .Effect=AFFLICT_SHOCK,
        .Argument=10,
        .Description="A small bolt of electricity that might shock the target.",
    },
    
    [LIGHTNING]={
        .Name="Lightning",
        .Type=LIGHT,
        .Power=80,
        .Cost=26,
        .Target=ENEMY,
        .Effect=AFFLICT_SHOCK,
        .Argument=10,
        .Description="A massive bolt of electricity strikes the target. This might shock the target.",
    },
    
    [THUNDER]={
        .Name="Thunder",
        .Type=LIGHT,
        .Power=120,
        .Cost=66,
        .Target=ENEMY,
        .Effect=AFFLICT_SHOCK,
        .Argument=30,
        .Description="A massive bolt of electricity strikes the target. This might shock the target.",
    },
    
    [CHARGE]={
        .Name="Charge",
        .Type=LIGHT,
        .Power=0,
        .Cost=16,
        .Target=YOURSELF,
        .Effect=BOOST_ATTACK,
        .Argument=2,
        .Description="The user collects electric charge, raising both its attack a lot.",
    },
    
    [COLD_GUST]={
        .Name="Cold Gust",
        .Type=ICE,
        .Power=50,
        .Cost=14,
        .Target=ENEMY,
        .Effect=AFFLICT_SHOCK,
        .Argument=10,
        .Description="A blast of cold air that might shock the target. This might shock the target.",
    },
    
    [AVALANCHE]={
        .Name="Avalanche",
        .Type=ICE,
        .Power=80,
        .Cost=32,
        .Target=ENEMY,
        .Effect=AFFLICT_BURY,
        .Argument=30,
        .Description="Icy boulders fall from above, possibly burying the target in the ground.",
    },
    
    [MIND_RAY]={
        .Name="Mind Ray",
        .Type=BASIC,
        .Power=65,
        .Cost=20,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A beam of energy strikes the target.",
    },
    
    [BRAINWAVE]={
        .Name="Brainwave",
        .Type=BASIC,
        .Power=120,
        .Cost=44,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The user beats the target up using only its brain.",
    },
    
    [HYPNOSIS]={
        .Name="Hypnosis",
        .Type=BASIC,
        .Power=0,
        .Cost=10,
        .Target=ENEMY,
        .Effect=AFFLICT_ASLEEP,
        .Argument=100,
        .Description="The target is induced to fall asleep.",
    },
    
    [MIND_SCREW]={
        .Name="Mind Screw",
        .Type=DARK,
        .Power=40,
        .Cost=12,
        .Target=ENEMY,
        .Effect=BOOST_ATTACK,
        .Argument=-1,
        .Description="The target is confused and disoriented. It's attack is decreased.",
    },
    
    [OBSCURE]={
        .Name="Obscure",
        .Type=DARK,
        .Power=20,
        .Cost=6,
        .Target=ENEMY,
        .Effect=BOOST_LUCK,
        .Argument=-1,
        .Description="The user disrupts te target's vision, decreasing its luck.",
    },
    
    [FRACTURE]={
        .Name="Fracture",
        .Type=EARTH,
        .Power=65,
        .Cost=18,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="The target is struck with force enough to crack boulders.",
    },
    
    [EARTHQUAKE]={
        .Name="Earthquake",
        .Type=EARTH,
        .Power=180,
        .Cost=48,
        .Target=EVERYONE_ELSE,
        .Effect=0,
        .Argument=0,
        .Description="A massive earthquake occurs.",
    },
    
    [STRYATION]={
        .Name="Stryation",
        .Type=EARTH,
        .Power=120,
        .Cost=72,
        .Target=ENEMY,
        .Effect=AFFLICT_BURY,
        .Argument=100,
        .Description="The target is crushed by the forces of geology, burying it in the ground in the process.",
    },
    
    [CORE_BEAM]={
        .Name="Core Beam",
        .Type=METAL,
        .Power=75,
        .Cost=22,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="A beam of energy is fired at the target.",
    },
    
    [IRON_ATTACK]={
        .Name="Iron Attack",
        .Type=METAL,
        .Power=40,
        .Cost=8,
        .Target=ENEMY,
        .Effect=0,
        .Argument=0,
        .Description="Hunks of iron metal are thrown at the target.",
    },
    
    [IRON_HEAVE]={
        .Name="Iron Heave",
        .Type=METAL,
        .Power=90,
        .Cost=40,
        .Target=ENEMY,
        .Effect=AFFLICT_BURY,
        .Argument=30,
        .Description="Large amounts of metal is thrown at the target, crushing it. The target might become buried in the ground.",
    },
    
    [SOLIDIFY]={
        .Name="Solidify",
        .Type=EARTH,
        .Power=0,
        .Cost=24,
        .Target=YOURSELF,
        .Effect=BOOST_DEFEND,
        .Argument=2,
        .Description="The user sturdies itself, increasing its defense a lot.",
    },
    
    [CAPTURE]={
        .Name="Capture",
        .Type=BASIC,
        .Power=0,
        .Cost=8,
        .Target=ENEMY,
        .Effect=CAPTURE_SPECTRA,
        .Argument=0,
        .Description="Attempts to capture the target spectra. Success depends on remaining HP and the species.",
    },
    
    [AILMENT]={
        .Name="Ailment",
        .Type=BASIC,
        .Power=0,
        .Cost=24,
        .Target=ENEMY,
        .Effect=AFFLICT_ANY,
        .Argument=100,
        .Description="Toxins are emitted at an enemy. The target is afflicted with a random status condition.",
    },
    
    [FIRE_BREATH]={
        .Name="Fire Breath",
        .Type=FIRE,
        .Power=70,
        .Cost=18,
        .Target=ENEMY,
        .Effect=AFFLICT_AFLAME,
        .Argument=10,
        .Description="The user shoots fire from its mouth and nostrils. The target might be set on fire.",
    },
    
    [RECYCLE]={
        .Name="Recycle",
        .Type=METAL,
        .Power=0,
        .Cost=20,
        .Target=YOURSELF,
        .Effect=HEAL_PERCENT,
        .Argument=25,
        .Description="By practicing sustainable resource use, the user heals some HP.",
    },
};

/**************************************************************/
#endif
