/**********************************************************//**
 * @file battle.c
 * @brief Implements battle system.
 * @author Rena Shinomiya
 * @date May 06, 2018
 **************************************************************/

#include <stdio.h>              // snprintf
#include <math.h>               // fmod

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "random.h"             // randint
#include "assets.h"             // MiscImage
#include "species.h"            // SPECTRA
#include "technique.h"          // TECHNIQUE
#include "menu.h"               // MENU
#include "player.h"             // Player
#include "battler.h"            // BATTLER
#include "battle.h"             // TEAM
#include "battle_menu.h"        // BattleMenu
#include "type.h"               // TYPE
#include "output.h"             // Output

/**********************************************************//**
 * @enum BATTLE_STATE
 * @brief Controls how the battle is finished.
 **************************************************************/
typedef enum {
    BATTLE_STATE_INTRO,         ///< The battle is being introduced.
    BATTLE_STATE_ACTIVE,        ///< Battle is ongoing.
    BATTLE_STATE_WIN,           ///< The user won.
    BATTLE_STATE_LOSE,          ///< The user lost.
    BATTLE_STATE_ESCAPE,        ///< The user escaped.
    BATTLE_STATE_NO_ESCAPE,     ///< The user failed to escape.
    BATTLE_STATE_EXIT,          ///< The battle is just ending.
} BATTLE_STATE;

/**************************************************************/
/// @brief Battlers in the player's team.
static TEAM PlayerTeam;

/// @brief Battlers in the enemy team.
static TEAM EnemyTeam;

/// @brief Pointer to a battler that was successfully captured
/// during this turn, or NULL.
static BATTLER *Captured = NULL;

/// @brief The state of the current battle.
static BATTLE_STATE BattleState;

/// @brief Buffer for actions the battlers will take on this turn.
static TURN Turns[BATTLE_SIZE];

/// @brief Turn being executed currently.
static TURN *CurrentTurn = NULL;

/**********************************************************//**
 * @brief Gets the team the BATTLER belongs to.
 * @param id: BATTLER's unique ID.
 * @return Pointer to the BATTLER's team.
 **************************************************************/
static inline bool BattlerIsAlly(int id) {
    return (id<TEAM_SIZE);
}

/**********************************************************//**
 * @brief Gets the BATTLER data from its ID.
 * @param id: Unique ID of the BATTLER.
 * @return Pointer to the BATTLER's data.
 **************************************************************/
BATTLER *BattlerByID(int id) {
    if (BattlerIsAlly(id)) {
        return &PlayerTeam.Member[id];
    } else {
        return &EnemyTeam.Member[id-TEAM_SIZE];
    }
}

/**********************************************************//**
 * @brief Gets the TURN data for a battler.
 * @param id: Unique ID if the turn.
 * @return Pointer to the TURN data.
 **************************************************************/
TURN *TurnByID(int id) {
    return &Turns[id];
}

/**********************************************************//**
 * @brief Sets up the player's team.
 **************************************************************/
static void InitializePlayerTeam(void) {
    for (int id=0; id<TEAM_SIZE; id++) {
        // Collect data that we'll use to set up the battler.
        SPECTRA *spectra = &Player->Spectra[id];
        BATTLER *battler = &PlayerTeam.Member[id];
        
        // Initialize battler data
        if (spectra->Species) {
            InitializeBattler(battler, spectra);
        } else {
            InitializeBattlerAsInactive(battler);
        }
    }
}

/**********************************************************//**
 * @brief Set up the enemy's team.
 * @param enemies: Basic ENEMY data to set up.
 **************************************************************/
static void InitializeEnemyTeam(const ENEMY *enemies) {
    // Enemy team members will point into this static space when
    // they are initialized.
    static SPECTRA EnemySpectra[TEAM_SIZE];
    for (int id=0; id<TEAM_SIZE; id++) {
        BATTLER *enemy = &EnemyTeam.Member[id];
        if (enemies[id].Species) {
            CreateSpectra(&EnemySpectra[id], enemies[id].Species, enemies[id].Level);
            InitializeBattler(enemy, &EnemySpectra[id]);
        } else {
            EnemySpectra[id].Species = 0;
            InitializeBattlerAsInactive(enemy);
        }
    }
}

/**********************************************************//**
 * @brief Generates a random ENEMY based on the encounters
 * defined for the map.
 * @param enemy: ENEMY to generate.
 * @param encounters: ENCOUNTER data from the map.
 **************************************************************/
static void RandomEnemy(ENEMY *enemy, const ENCOUNTER *encounters) {
    int chance = randint(1, 100);
    const ENCOUNTER *encounter = NULL;
    for (int i=0, total=0; total<chance; total+=encounters[i++].Chance) {
        encounter = &encounters[i];
    }
    if (encounter) {
        enemy->Species = encounter->Spectra;
        enemy->Level = randint(encounter->LevelRange[0], encounter->LevelRange[1]);
    } else {
        // Summoned enemy for location where no enemies are defined.
        // Make do with this easter egg.
        enemy->Species = PRGMERROR;
        enemy->Level = 80;
    }
}

/**********************************************************//**
 * @brief Generates the output for the beginning of battle.
 * @param type: The type of encounter happening.
 **************************************************************/
static void IntroduceBattle(ENCOUNTER_TYPE type) {
    BattleState = BATTLE_STATE_INTRO;
    int count = 0;
    const BATTLER *leader = NULL;
    for (int i=0; i<TEAM_SIZE; i++) {
        const BATTLER *battler = &EnemyTeam.Member[i];
        if (BattlerIsActive(battler)) {
            if (!leader) {
                leader = battler;
            }
            count++;
        }
    }
    
    // Determine if the sentence should be "A [name]" or "An [name]"
    const char *name = BattlerName(leader);
    const char *an = NULL;
    switch (name[0]) {
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
    case 'Y':
        an = "An";
        break;
    default:
        an = "A";
        break;
    }
    
    // Determine how many enemies
    const char *others = NULL;
    switch (count) {
    case 1:
        others = NULL;
        break;
    case 2:
        others = "and its cohort";
        break;
    default:
        others = "and its cohorts";
        break;
    }
    
    // Determine how the enemy are appearing
    const char *description = NULL;
    switch (type) {
    case ENCOUNTER_FISHING:
        description = (count>1)? "were hooked": "was hooked";
    default:
        description = (count>1)? "draw near": "draws near";
        break;
    }
    
    // Finalize sentence
    if (others) {
        OutputF("%s %s %s %s!", an, name, others, description);
    } else {
        OutputF("%s %s %s!", an, name, description);
    }
}

/**********************************************************//**
 * @brief Initializes setup for the current round of battle.
 **************************************************************/
static void InitializeRound(void) {
    CurrentTurn = NULL;
    BattleState = BATTLE_STATE_ACTIVE;
    for (int i=0; i<BATTLE_SIZE; i++) {
        Turns[i].State = TURN_INACTIVE;
    }
    InitializeBattleMenu();
}

/**********************************************************//**
 * @brief Set up a new random encounter with spectra on the
 * overworld (normal or fishing).
 * @param count: Number of enemies.
 * @param type: Type of random encounter.
 **************************************************************/
void InitializeRandomEncounter(int count, ENCOUNTER_TYPE type) {
    // Get the encounter table
    const ENCOUNTER *encounters = NULL;
    switch (type) {
    case ENCOUNTER_FISHING:
        encounters = Location(Player->Location)->Fishing;
        break;
    case ENCOUNTER_OVERWORLD:
        encounters = Location(Player->Location)->Encounters;
        break;
    default:
        break;
    }
    
    // Get random enemies
    ENEMY enemies[TEAM_SIZE];
    for (int i=0; i<TEAM_SIZE; i++) {
        if (i < count) {
            RandomEnemy(&enemies[i], encounters);
        } else {
            enemies[i].Species = 0;
        }
    }
    InitializeEnemyTeam(enemies);
    InitializePlayerTeam();
    InitializeRound();
    IntroduceBattle(type);
}

/**********************************************************//**
 * @brief Set up a non-random encounter with spectra.
 * @param bosses: Boss enemies to generate.
 **************************************************************/
void InitializeBossEncounter(const BOSS *bosses) {
    InitializeEnemyTeam(bosses->Boss);
    InitializePlayerTeam();
    InitializeRound();
    IntroduceBattle(ENCOUNTER_BOSS);
}

/**********************************************************//**
 * @brief Gets a list of target IDs for the given type.
 * @param targets: Location to store the list of targets.
 * @param user: User ID
 * @param type: Targetting type
 * @return Number of targets loaded.
 **************************************************************/
int GetTargets(int *targets, int user, TARGET_TYPE type) {
    int i = 0;
    for (int id=0; id<BATTLE_SIZE; id++) {
        const BATTLER *battler = BattlerByID(id);
        // Can't target gone or incapacitated battlers
        if (!BattlerIsAlive(battler)) {
            continue;
        } else if (type&TARGET_USER && id==user) {
            // Targetting yourself
            targets[i++] = id;
        } else if (type&TARGET_ALLY && BattlerIsAlly(id)==BattlerIsAlly(user)) {
            // Targetting any ally
            targets[i++] = id;
        } else if (type&TARGET_ENEMY && BattlerIsAlly(id)!=BattlerIsAlly(user)) {
            // Targetting any enemy
            targets[i++] = id;
        }
    }
    return i;
}

/**********************************************************//**
 * @brief Attempt to escape the battle.
 * @return True if the battle was escaped.
 **************************************************************/
bool EscapeBattle(void) {
    // Get the escape chance
    int ally = 0;
    int enemy = 0;
    for (int id=0; id<BATTLE_SIZE; id++) {
        const BATTLER *battler = BattlerByID(id);
        if (BattlerIsAlive(battler)) {
            if (id<TEAM_SIZE) {
                ally += BattlerEvade(battler)+BattlerLuck(battler);
            } else {
                enemy += BattlerEvade(battler);
            }
        }
    }
    assert(ally && enemy);
    float chance = (float)ally / (ally+enemy);
    
    // Maybe escape
    Output("The party tries to escape...");
    if (uniform(0.0, 1.0) < chance) {
        Output("And succeeds!");
        BattleState = BATTLE_STATE_ESCAPE;
        return true;
    } else {
        Output("And fails...");
        BattleState = BATTLE_STATE_NO_ESCAPE;
        return false;
    }
}

/**********************************************************//**
 * @brief Randomly generates turns for all the enemies in
 * the current battle.
 **************************************************************/
static void LoadEnemyTurns(void) {
    for (int id=TEAM_SIZE; id<BATTLE_SIZE; id++) {
        BATTLER *enemy = BattlerByID(id);
        TURN *turn = &Turns[id];
        if (BattlerIsAlive(enemy)) {
            turn->State = TURN_PENDING;
            turn->User = id;
            
            // Get a list of legal techniques to use here
            TECHNIQUE_ID usableTechniques[MOVESET_SIZE+2];
            usableTechniques[0] = DEFAULT_ATTACK;
            usableTechniques[1] = DEFAULT_DEFEND;
            int u = 2;
            for (int t=0; t<enemy->Spectra->MovesetSize; t++) {
                const TECHNIQUE *technique = TechniqueByID(enemy->Spectra->Moveset[t]);
                if (technique->Cost > enemy->Spectra->Power) {
                    continue;
                }
                usableTechniques[u++] = enemy->Spectra->Moveset[t];
            }
            turn->Technique = usableTechniques[randint(0,u-1)];
            
            // Get the primary target, if applicable
            TARGET_TYPE type = TechniqueByID(turn->Technique)->Target;
            if (type&TARGET_GROUP) {
                turn->Target = -1;
            } else {
                int targets[BATTLE_SIZE];
                int nTargets = GetTargets(targets, id, type);
                turn->Target = targets[randint(0,nTargets-1)];
            }
        } else {
            // Turn skipped
            turn->State = TURN_INACTIVE;
        }
    }
}

/**********************************************************//**
 * @brief Attempts to capture the target.
 * @param battler: Target to capture.
 * @return True if the target was captured.
 **************************************************************/
static bool ExecuteCapture(BATTLER *battler) {
    // Determine if capture succeeds
    int rate = BattlerSpecies(battler)->CatchRate;
    float percent = (float)BattlerHealth(battler)/BattlerMaxHealth(battler);
    
    // Increased rate if ailed
    switch (battler->Spectra->Ailment) {
    case ASLEEP:
    case BURIED:
        rate += 10;
        break;
    case SHOCKED:
        rate += 5;
        break;
    default:
        break;
    }
    
    // Test rate
    OutputSplitByCR("...\r......\r.........");
    int test = randint(0, 99);
    int threshold = rate + (100-rate)*(1-percent)*(1-percent);
    if (test < threshold) {
        if (GetSpectra(battler->Spectra)) {
            Output("The capture succeeded!");
            Captured = battler;
            return true;
        } else {
            Output("You can't capture any more!");
        }
    } else if (test-10 < threshold) {
        Output("It just got away...");
    } else if (test-20 < threshold) {
        Output("It managed to break free!");
    } else {
        Output("The capture failed!");
    }
    return false;
}

static float HitRate(int uid, int tid, const TECHNIQUE *technique) {
    if (uid==tid) {
        // Can't miss yourself
        return 1.0;
    } else if (BattlerIsAlly(uid)==BattlerIsAlly(tid) && technique->Power==0) {
        // Can't miss an ally with non-damaging attack
        return 1.0;
    } else {
        // Can never have worse than 50% chance to hit.
        const BATTLER *user = BattlerByID(uid);
        const BATTLER *target = BattlerByID(tid);
        float base = (float)BattlerLuck(user)/BattlerEvade(target);
        return (base < 0.5)? 0.5: base;
    }
}

static float CriticalHitRate(const BATTLER *user, const BATTLER *target) {
    float ratio = (float)BattlerLuck(user)/BattlerLuck(target);
    
    // Minimum critical hit rate:  0%
    // Average critical hit rate: 10%
    // Maximum critical hit rate: 20%
    float base = 0.1*ratio;
    return (base > 0.2)? 0.2: base;
}

/**********************************************************//**
 * @brief Performs one turn during battle.
 * @param turn: Turn to execute.
 **************************************************************/
static void ExecuteTurn(const TURN *turn) {
    // Get the targets of the technique
    int Targets[BATTLE_SIZE];
    TARGET_TYPE targetType = TechniqueByID(turn->Technique)->Target;
    int nTargets;
    if (targetType & TARGET_GROUP) {
        nTargets = GetTargets(Targets, turn->User, targetType);
    } else {
        Targets[0] = turn->Target;
        nTargets = 1;
    }
    
    // Maybe fail if there are status conditions
    BATTLER *user = BattlerByID(turn->User);
    const TECHNIQUE *technique = TechniqueByID(turn->Technique);
    switch (user->Spectra->Ailment) {
    case SHOCKED:
        if (uniform(0.0,1.0)<0.5) {
            OutputF("%s can't move...", BattlerName(user));
            return;
        }
        break;
    case BURIED:
        OutputF("%s is buried in the ground...", BattlerName(user));
        user->Spectra->Ailment = 0;
        return;
    case ASLEEP:
        OutputF("%s is fast asleep...", BattlerName(user));
        return;
    default:
        break;
    }
    
    // Spend P
    if (BattlerPower(user) >= technique->Cost) {
        user->Spectra->Power -= technique->Cost;
    } else {
        OutputF("%s is out of power!", BattlerName(user));
        return;
    }
    
    // Perform the turn
    bool allInvalid = true;
    for (int i=0; i<nTargets; i++) {
        BATTLER *target = BattlerByID(Targets[i]);
        if (!BattlerIsAlive(target)) {
            continue;
        }
        allInvalid = false;
        
        // Maybe miss the target
        if (uniform(0.0, 1.0) > HitRate(turn->User, Targets[i], technique)) {
            OutputF("%s avoided the attack!", BattlerName(target));
            continue;
        }
        
        // Inflict damage
        int damage = 0;
        if (technique->Power) {
            float ratio = (float)BattlerAttack(user)/BattlerDefend(target);
            float scale = (float)user->Spectra->Level/LEVEL_MAX;
            const TYPE_ID *targetType = SpeciesByID(target->Spectra->Species)->Type;
            float matchup = TypeMatchup(technique->Type, targetType[0]);
            if (targetType[1]) {
                matchup *= TypeMatchup(technique->Type, targetType[1]);
            }
            float power = (technique->Power>10)? (technique->Power-10)*scale+10: 10;
            damage = 1 + power*ratio*matchup;
            
            // Maybe critical hit?
            if (uniform(0.0, 1.0) < CriticalHitRate(user, target)) {
                damage *= 2;
                OutputF("A critical hit on %s!", BattlerName(target));
            }
            
            // Inflict damage
            target->Spectra->Health -= damage;
            if (target->Spectra->Health < 0) {
                target->Spectra->Health = 0;
            }
            
            // Messages
            if (damage) {
                OutputF("%s took %d damage!", BattlerName(target), damage);
                if (!BattlerIsAlive(target)) {
                    OutputF("%s passed out!", BattlerName(target));
                }
            } else {
                OutputF("%s didn't take any damage!", BattlerName(target));
            }
        }
        
        // Apply effect if applicable
        if (BattlerIsAlive(target) && !(technique->Flags&TECHNIQUE_EFFECT_ONCE)) {
            const ITEM *item;
            switch (turn->Technique) {
            case DEFAULT_ITEM:
                item = ItemByID(turn->Item);
                if (item->Flags&BATTLE_ONLY) {
                    if (ApplyEffectInBattle(item->Effect, user, target, item->Argument)) {
                        DropItem(turn->Item);
                    } else {
                        Output("There was no effect...");
                    }
                } else {
                    Output("That's not useful right now!");
                }
                
                break;
            
            case CAPTURE:
                ExecuteCapture(target);
                break;
            
            default:
                if (ShouldEffectActivate(technique->Effect, technique->Argument)) {
                    ApplyEffectInBattle(technique->Effect, user, target, technique->Argument);
                } else if (!technique->Power) {
                // Technique doesn't do damage, and effect missed.
                OutputF("%s avoided the attack!", BattlerName(target));
                }
                break;
            }
        }
    }
    
    // All invalid targets?
    if (allInvalid) {
        Output("There was no target...");
    }
    
    // Perform effect if it activates after all targets are hit
    if (technique->Flags&TECHNIQUE_EFFECT_ONCE) {
        if (ShouldEffectActivate(technique->Effect, technique->Argument)) {
            ApplyEffectInBattle(technique->Effect, user, NULL, technique->Argument);
        }
    }
}

/**********************************************************//**
 * @brief Check if the battle shound end.
 **************************************************************/
static void MaybeUpdateBattleState(void) {
    // No point calling this again
    if (BattleState != BATTLE_STATE_ACTIVE) {
        return;
    }
    
    // Clean up any dead battlers
    bool win = true;
    bool lose = true;
    for (int id=0; id<BATTLE_SIZE; id++) {
        BATTLER *battler = BattlerByID(id);
        if(BattlerIsAlive(battler)) {
            if (BattlerIsAlly(id)) {
                lose = false;
            } else {
                win = false;
            }
        }
    }
    
    // If it's a tie, you lose.
    if (lose) {
        BattleState = BATTLE_STATE_LOSE;
        Output("You lost!");
    } else if (win) {
        BattleState = BATTLE_STATE_WIN;
        Output("You won!");
    }
}

/**********************************************************//**
 * @brief Gets the turn priority for a battler.
 * @param battler: Battler to check.
 * @return Priority
 **************************************************************/
static inline int Priority(const BATTLER *battler) {
    int evade = BattlerEvade(battler);
    if (battler->Spectra->Ailment==SHOCKED) {
        return evade/2;
    } else {
        return evade;
    }
}

/**********************************************************//**
 * @brief Updates a step of the battle system during each
 * frame of rendering.
 **************************************************************/
static void UpdateBattleExecution(void) {
    // Update to the next turn; if needed.
    if (!CurrentTurn || CurrentTurn->State==TURN_DONE) {
        CurrentTurn = NULL;
        int maxPriority = 0;
        for (int id=0; id<BATTLE_SIZE; id++) {
            BATTLER *battler = BattlerByID(id);
            if (Turns[id].State == TURN_PENDING) {
                if (BattlerIsAlive(battler)) {
                    int priority = Priority(BattlerByID(id));
                    if (priority > maxPriority || !CurrentTurn) {
                        maxPriority = priority;
                        CurrentTurn = &Turns[id];
                    }
                } else {
                    // Inactivate turns for dead battlers
                    Turns[id].State = TURN_INACTIVE;
                }
            }
        }
        if (!CurrentTurn) {
            return;
        }
    }
    
    // Execute the current turn
    switch (CurrentTurn->State) {
    case TURN_PENDING:
        CurrentTurn->State = TURN_ACTIVE;
        if (CurrentTurn->Technique==DEFAULT_ITEM) {
            const ITEM *item = ItemByID(CurrentTurn->Item);
            OutputF("%s used %s!", BattlerNameByID(CurrentTurn->User), item->Name);
        } else {
            const TECHNIQUE *technique = TechniqueByID(CurrentTurn->Technique);
            OutputF("%s used %s!", BattlerNameByID(CurrentTurn->User), technique->Name);
        }
        break;
    
    case TURN_ACTIVE:
        // Await previous phase completion
        UpdateOutput();
        if (OutputDone()) {
            CurrentTurn->State = TURN_RESULT;
            ExecuteTurn(CurrentTurn);
        }
        break;
    
    case TURN_RESULT:
        // Await previous phase completion
        UpdateOutput();
        if (OutputDone()) {
            if (Captured) {
                Captured->Spectra = 0;
                Captured = NULL;
            }
            CurrentTurn->State = TURN_DONE;
            MaybeUpdateBattleState();
        }
        break;
    default:
        break;
    }
}

/**********************************************************//**
 * @brief Determines if the battle execution step has finished.
 * @return True if battle execution is done and we should go
 * to the next turn.
 **************************************************************/
static bool BattleExecutionDone(void) {
    for (int i=0; i<BATTLE_SIZE; i++) {
        switch (Turns[i].State) {
        case TURN_PENDING:
        case TURN_ACTIVE:
        case TURN_RESULT:
            return false;
        case TURN_INACTIVE:
        case TURN_DONE:
        default:
            break;
        }
    }
    return true;
}

/**********************************************************//**
 * @brief Apply effects to battlers at the end of each round.
 **************************************************************/
static void ApplyEndOfRoundEffects(void) {
    for (int id=0; id<BATTLE_SIZE; id++) {
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsAlive(battler)) {
            continue;
        }
        
        // Unset the battler's flags.
        battler->Flags = 0;
        
        // Apply status ailments
        int damage;
        switch (battler->Spectra->Ailment) {
        case ASLEEP:
            if (uniform(00.0,1.0)<0.5) {
                OutputF("%s woke up!", BattlerName(battler));
                battler->Spectra->Ailment =0;
            }
            break;
        
        case POISONED:
            damage = 1 + BattlerMaxHealth(battler)/8;
            battler->Spectra->Health -= damage;
            if (battler->Spectra->Health < 0) {
                battler->Spectra->Health = 0;
            }
            OutputF("%s took %d damage from poison!", BattlerName(battler), damage);
            if (!BattlerIsAlive(battler)) {
                OutputF("%s passed out!", BattlerName(battler));
            }
            break;  
            
        case AFLAME:
            damage = 1 + BattlerMaxHealth(battler)/8;
            battler->Spectra->Health -= damage;
            if (battler->Spectra->Health < 0) {
                battler->Spectra->Health = 0;
            }
            OutputF("%s took %d damage from fire!", BattlerName(battler), damage);
            if (!BattlerIsAlive(battler)) {
                OutputF("%s passed out!", BattlerName(battler));
            }
            break;
            
        default:
            break;
        }
        
        // Check win and lose conditions
        MaybeUpdateBattleState();
        if (BattleState!=BATTLE_STATE_ACTIVE) {
            return;
        }
    }
}

/**********************************************************//**
 * @brief Give experience to a spectra.
 * @param spectra: Spectra to gain experience.
 * @param experience: Amount of experience.
 **************************************************************/
static void GainExperience(SPECTRA *spectra, int experience) {
    if (spectra->Level < LEVEL_MAX && experience>0) {
        // Health gain initialization
        int health = spectra->MaxHealth;
        int power = spectra->MaxPower;
        
        // Manage level-up
        const SPECIES *species = SpeciesOfSpectra(spectra);
        OutputF("%s gained %d experience!", species->Name, experience);
        spectra->Experience -= experience;
        int gained = 0;
        while (spectra->Experience <= 0 && spectra->Level < LEVEL_MAX) {
            gained++;
            spectra->Level++;
            spectra->Experience += ExperienceNeeded(spectra);
        }
        
        // Gained any levels?
        if (gained > 0) {
            if (spectra->Level == LEVEL_MAX) {
                spectra->Experience = 0;
            }
            
            // Output level gains.
            if (gained == 1) {
                OutputF("%s's level went up!\n", species->Name);
            } else if (gained > 1) {
                OutputF("%s's level went up by %d!\n", species->Name, gained);
            }
            
            // Health gain
            UpdateActiveStats(spectra);
            spectra->Health += (spectra->MaxHealth - health);
            spectra->Power += (spectra->MaxPower - power);
        }
    }
}

/**********************************************************//**
 * @brief Apply effects that happen when the user wins.
 **************************************************************/
void ApplyWinEffects(void) {
    int experience = 0;
    int money = 0;
    for (int id=TEAM_SIZE; id<BATTLE_SIZE; id++) {
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsActive(battler)) {
            continue;
        }
        
        // Gain exp from this battler
        experience += BattlerSpecies(battler)->Experience*battler->Spectra->Level/5;
        money += BattlerSpecies(battler)->Money*battler->Spectra->Level/5;
    }
    
    // Each user gains experience
    for (int id=0; id<TEAM_SIZE; id++) {
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsAlive(battler)) {
            continue;
        }
        
        // This user's spectra gains the experience
        GainExperience(battler->Spectra, experience);
    }
    
    // You gain money
    if (money) {
        OutputF("Gained $%.2f", (double)money);
        Player->Money += money;
    }
}

/**********************************************************//**
 * @brief Updates one step of the battle system.
 **************************************************************/
void UpdateBattle(void) {
    switch (BattleState) {
    case BATTLE_STATE_INTRO:
        // Happens once at the start of each battle
        UpdateOutput();
        if (OutputDone()) {
            InitializeRound();
        }
        break;

    case BATTLE_STATE_ACTIVE:
        // Typical ongoing battle
        if (!BattleMenuDone()) {
            UpdateBattleMenu();
            // Get enemy turns
            if (BattleMenuDone()) {
                LoadEnemyTurns();
            }
        } else if (!BattleExecutionDone()) {
            UpdateBattleExecution();
            if (BattleExecutionDone()) {
                ApplyEndOfRoundEffects();
                if (OutputDone()) {
                    InitializeRound();
                }
            }
        } else if (!OutputDone()) {
            UpdateOutput();
            if (OutputDone()) {
                InitializeRound();
            }
        }
       break;
    
    case BATTLE_STATE_NO_ESCAPE:
        UpdateOutput();
        if (OutputDone()) {
            BattleState = BATTLE_STATE_ACTIVE;
        }
        break;
    
    // End conditions
    case BATTLE_STATE_WIN:
        UpdateOutput();
        if (OutputDone()) {
            ApplyWinEffects();
            BattleState = BATTLE_STATE_EXIT;
        }
        break;
    
    case BATTLE_STATE_LOSE:
        UpdateOutput();
        if (OutputDone()) {
            WarpToLastHospital();
            RecoverParty();
            BattleState = BATTLE_STATE_EXIT;
        }
        break;

    case BATTLE_STATE_ESCAPE:
        UpdateOutput();
        if (OutputDone()) {
            BattleState = BATTLE_STATE_EXIT;
        }
        break;
    
    case BATTLE_STATE_EXIT:
        UpdateOutput();
        if (OutputDone()) {
            SetMode(MODE_MAP);
        }
        break;
    }
}

/**********************************************************//**
 * @brief Draws each battler's sprite on the screen.
 **************************************************************/
static void DrawBattlers(void) {
    static const COORDINATE BattlerPosition[] = {
        // User's team
        [0] = { 60, 260},
        [1] = {110, 240},
        [2] = {160, 220},
        
        // Enemy's team
        [3] = {320, 220},
        [4] = {370, 240},
        [5] = {420, 260},
    };
    
    // Draw drop shadows
    int target = BattleMenuCurrentTargetID();
    for (int id=0; id<BATTLE_SIZE; id++) {
        const COORDINATE *center = &BattlerPosition[id];
        if (!BattlerIsActive(BattlerByID(id))) {
            continue;
        }
        
        // Different colors for seletion
        ALLEGRO_COLOR color = al_map_rgba(0, 0, 0, 60);
        if (!BattleMenuDone()) {
            if (id==BattleMenuCurrentUserID() && BattlerIsAlly(id)) {
                color = al_map_rgba(0, 127, 255, 200);
            } else if (id==target) {
                color = al_map_rgba(255, 20, 0, 200);
            }
        }
        al_draw_filled_ellipse(center->X, center->Y, 40, 10, color);
    }
    
    // Draw existing spectra in layer order
    static const int Order[] = {3, 2, 4, 1, 5, 0};
    for (int i=0; i<6; i++) {
        int id = Order[i];
        BATTLER *battler = BattlerByID(id);
        if (!BattlerIsAlive(battler)) {
            continue;
        }
        SPECIES_ID speciesID = BattlerByID(id)->Spectra->Species;
        const SPECIES *species = SpeciesByID(speciesID);
        const COORDINATE *center = &BattlerPosition[id];
        const COORDINATE *offset = &species->Offset;
        ALLEGRO_BITMAP *image = NULL;
        if (speciesID == AMY) {
            image = CostumeImage(Player->Costume);
        } else {
            image = SpeciesImage(speciesID);
        }
        
        // Flip enemies
        if (id<TEAM_SIZE) {
            al_draw_bitmap(image, center->X-offset->X, center->Y-offset->Y, 0);
        } else {
            int flipped = al_get_bitmap_width(image) - offset->X;
            al_draw_bitmap(image, center->X-flipped, center->Y-offset->Y, ALLEGRO_FLIP_HORIZONTAL);
        }
    }
}

/**********************************************************//**
 * @brief Draws all the battle HUDs on the screen.
 **************************************************************/
static void DrawHUDs(void) {
    // User HUDs
    for (int id=0, y=4; id<TEAM_SIZE; id++) {
        if (!BattlerIsActive(BattlerByID(id))) {
            continue;
        }
        DrawAt(4, y);
        DrawHudUser(PlayerTeam.Member[id].Spectra);
        // Hud tag
        if (!BattleMenuDone()) {
            int current = BattleMenuCurrentUserID();
            if (id==current) {
                al_draw_bitmap(MiscImage(HUD_UP), 200, 5, 0);
            } else if (id<current) {
                al_draw_bitmap(MiscImage(HUD_OK), 200, 5, 0);
            }
        }
        y += 29;
    }
    
    // Enemy HUDs
    for (int id=TEAM_SIZE, y=4; id<BATTLE_SIZE; id++) {
        if (!BattlerIsActive(BattlerByID(id))) {
            continue;
        }
        DrawAt(275, y);
        DrawHudEnemy(EnemyTeam.Member[id-TEAM_SIZE].Spectra);
        y += 29;
    }
}

/**********************************************************//**
 * @brief Renders the current state of the battle system on
 * the screen.
 **************************************************************/
void DrawBattle(void) {
    // Draw background image
    DrawAt(0, 0);
    BACKGROUND_ID background = Location(Player->Location)->Background;
    al_draw_bitmap(BackgroundImage(background? background: CHARCOAL), 0, 0, 0);
    DrawBattlers();
    DrawHUDs();
    
    // Battle menu phase
    if (!BattleMenuDone() && BattleState==BATTLE_STATE_ACTIVE) {
        DrawBattleMenu();
    } else {
        DrawAt(0, 0);
        DrawOutput();
    }
}

/**************************************************************/
