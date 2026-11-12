// pickpocket.cpp -- a guard's pickpocket loot (STEPGUARD +0x3F4..+0x494): the loot list (up to eight entries of
// coins or an item), its generation from per-world tables at load, staging the next entry into the guard's loot
// pouch, handing it to Sly when he pickpockets the guard, and the pickpocket state (-1 nothing to steal, 0 guard
// alert, 1 coins only, 2 item ready) shown through two glint indicator objects and the pouch's state machine. Entry
// points: GenerateStepguardLoot, StageStepguardLoot, PickpocketStepguard, UpdateStepguardPickpocket.
// TU 0x1F1768..0x1F2108 (11 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_pickpocket
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_pickpocket
#include "sly2/slotheap.h"
#include "sly2/clock.h"
#include "sly2/task.h"
#include "sly2/guard.h"
#include "sly2/game.h"
#include "sly2/sw.h"
#include "sly2/classes.h"   /* LOOTPOUCH */

// 0x1F1FB0 StaticInit_pickpocket and GlobalConstructors_pickpocket: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_KO = 0x7,
    SGF_JUGGLED = 0x17,
    SGF_COMBAT = 0x1D,
};

typedef unsigned long ulong;


/* Class ids tested here (docs/systems/object-model.md section 1). */
enum { CID_GUARD = 0x3A, CID_KEY = 0x5C };
/* Object ids of the loot templates: a coin pile, a generated item reward, and the pouch launch kinds for a
   plain object and a key. */
enum { OID_LOOT_COINS = 0x2B3, OID_LOOT_ITEM = 0x2B5, LAUNCH_OBJECT = 0x2B0, LAUNCH_KEY = 0x2B1 };
/* Pickpocket states (P::pickpocketState). */
enum { PICKPOCKET_NONE = -1, PICKPOCKET_ALERT = 0, PICKPOCKET_COINS = 1, PICKPOCKET_ITEM = 2 };

/* The guard is guard.h STEPGUARD (loot: GUARDLOOT); its loot pouch (STEPGUARD::lootPouch) is a LOOTPOUCH (classes.h). */

struct TABLE { int minimum, maximum; float chance; struct PICK { int weight, key; } picks[6]; };
typedef TABLE::PICK PICK;
struct OBJECT { char pad[0x230]; TASK *ptask; };   /* a key: its task */

extern int D_002E1E40;
/* pickpocket's .data (address order): the pouch SMA state oids, then the per-world loot tables (index = world, 9 worlds)
 * for ordinary guards and for CID 0x3A guards: {min count, max count, chance, 6 x {weight, item key}}. */
int D_002C3738[4] = {92, 101, 102, 105};
TABLE D_002C3748[9] = {
    {0, 0, 0.0f, {{0, -1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {4, 8, 0.2f, {{30, 1114}, {30, 1115}, {15, 1120}, {15, 1121}, {5, 1117}, {5, 1118}}},
    {4, 8, 0.2f, {{30, 1117}, {30, 1118}, {15, 1123}, {15, 1124}, {5, 1126}, {5, 1127}}},
    {4, 8, 0.2f, {{30, 1117}, {30, 1118}, {15, 1123}, {15, 1124}, {5, 1126}, {5, 1127}}},
    {4, 8, 0.2f, {{30, 1126}, {30, 1127}, {15, 1129}, {15, 1130}, {5, 1135}, {5, 1136}}},
    {4, 8, 0.2f, {{30, 1126}, {30, 1127}, {15, 1129}, {15, 1130}, {5, 1135}, {5, 1136}}},
    {4, 8, 0.2f, {{30, 1135}, {30, 1136}, {15, 1132}, {15, 1133}, {5, 1141}, {5, 1142}}},
    {4, 8, 0.2f, {{30, 1135}, {30, 1136}, {15, 1132}, {15, 1133}, {5, 1141}, {5, 1142}}},
    {4, 8, 0.2f, {{30, 1132}, {30, 1133}, {15, 1141}, {15, 1142}, {5, 1138}, {5, 1139}}},
};
TABLE D_002C3968[9] = {
    {0, 0, 0.0f, {{0, -1}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {6, 12, 0.4f, {{30, 1116}, {30, 1115}, {15, 1122}, {15, 1121}, {5, 1119}, {5, 1118}}},
    {6, 12, 0.4f, {{30, 1119}, {30, 1118}, {15, 1125}, {15, 1124}, {5, 1128}, {5, 1127}}},
    {6, 12, 0.4f, {{30, 1119}, {30, 1118}, {15, 1125}, {15, 1124}, {5, 1128}, {5, 1127}}},
    {6, 12, 0.4f, {{30, 1128}, {30, 1127}, {15, 1131}, {15, 1130}, {5, 1137}, {5, 1136}}},
    {6, 12, 0.4f, {{30, 1128}, {30, 1127}, {15, 1131}, {15, 1130}, {5, 1137}, {5, 1136}}},
    {6, 12, 0.4f, {{30, 1137}, {30, 1136}, {15, 1134}, {15, 1133}, {5, 1143}, {5, 1142}}},
    {6, 12, 0.4f, {{30, 1137}, {30, 1136}, {15, 1134}, {15, 1133}, {5, 1143}, {5, 1142}}},
    {6, 12, 0.4f, {{30, 1134}, {30, 1133}, {15, 1143}, {15, 1142}, {5, 1140}, {5, 1139}}},
};
extern "C" {
void SetSmaGoalIfReachable(void *psma, int n);
ulong GetSgfMask(int);
int GetHeroPo(SW *);
void ClearLootPouchLoot(LO *);
int StageLootPouchLoot(LO *, OBJECT *, int, int);
void *LaunchLootPouchLootAtPlayer(LO *, int);
int GetCurrentWorld(void);
int IsBasicDerivedFrom__FP5BASIC3CID(void *, int);
void *func_002333B4(void *, int, int) __asm__("memset");
OBJECT *FindSwObject__FP2SWi3OIDP2LO(SW *, int, int, LO *);
int IsTaskComplete(TASK *);
float RandomFloatInRange__Fff(float, float);
int RandomIntInRange__Fii(int, int);
void BroadcastGameEvent(GAME *, int, STEPGUARD *);
void BuildSpliceListFromUniformValues(void *, int, int, int *) SLY2_SRET_LABEL("BuildSpliceListFromUniformValues", "BuildSpliceListFromUniformValuesInto");
void IgnoreStepSlot87(STEPGUARD *, int);
void SetStepguardPickpocketState(STEPGUARD *guard, int state);
void UpdateStepguardPickpocket(STEPGUARD *guard);
void StageStepguardLoot(STEPGUARD *guard);
}

// 0x1F1768: Splice option: appends a coin entry (oid 0x2B3) worth `count` coins, when the guard's loot is fixed by
// the level (+0x490).
// Also known as: AddPocketCoins (upstream readable / Sly 1 name; AddPocketCoins__FP2LOi).
extern "C" void AddStepguardLootCoins(STEPGUARD *guard, int count)
{
    if (guard->isLootFixed) {
        GUARDLOOT *entry = &guard->loot[guard->numLoot++];
        entry->oid = (OID)OID_LOOT_COINS;
        entry->key = -1;
        entry->flags = 0;
        entry->count = count;
    }
}

// 0x1F17B0: Splice option: appends an item entry (object oid `itemOid`, count 1) when the list is empty or the loot
// is fixed by the level.
// Also known as: AddPocketPrize (upstream readable / Sly 1 name; AddPocketPrize__FP2LOi).
extern "C" void AddStepguardLootItem(STEPGUARD *guard, int itemOid)
{
    if (guard->numLoot <= 0 || guard->isLootFixed) {
        GUARDLOOT *entry = &guard->loot[guard->numLoot++];
        entry->oid = (OID)itemOid;
        entry->flags = 0;
        entry->key = -1;
        entry->count = 1;
    }
}

// 0x1F17F8: Sets the pickpocket state (-1..2): records the time, hides (vtable slot 9) or shows (slot 8) the two
// glint indicators, and moves the pouch's state machine to the matching state.
// Also known as: SetPocketMode (upstream readable / Sly 1 name; SetPocketMode__FP2LOi).
extern "C" void SetStepguardPickpocketState(STEPGUARD *guard, int state)
{
    void *stateMachine;
    if (state == guard->pickpocketState) return;
    guard->pickpocketState = state;
    guard->pickpocketStateTime = g_clock.time;
    switch (state) {
    case PICKPOCKET_NONE:
    case PICKPOCKET_ALERT:
        if (guard->pickpocketIndicator0) guard->pickpocketIndicator0->Remove();
        if (guard->pickpocketIndicator1) guard->pickpocketIndicator1->Remove();
        break;
    case PICKPOCKET_COINS:
    case PICKPOCKET_ITEM:
        if (guard->pickpocketIndicator0) guard->pickpocketIndicator0->Add();
        if (guard->pickpocketIndicator1) guard->pickpocketIndicator1->Add();
        break;
    }
    stateMachine = ((LOOTPOUCH *)guard->lootPouch)->psma;
    if (stateMachine) SetSmaGoalIfReachable(stateMachine, D_002C3738[guard->pickpocketState + 1]);
}

// 0x1F1900: Recomputes the pickpocket state: -1 with no pouch or pickpocketing disabled; 0 while the guard is
// alerted, hurt, dead, stunned or thrown; 1 when only coins are left (or the hero is not the current player); 2
// when an item is ready.
// Also known as: UpdatePocketMode (upstream readable / Sly 1 name; UpdatePocketMode__FP2LO).
extern "C" void UpdateStepguardPickpocket(STEPGUARD *guard)
{
    ulong alertMask;
    int state;
    if (!guard->lootPouch) return;
    state = PICKPOCKET_NONE;
    if (((LOOTPOUCH *)guard->lootPouch)->pdprize) {
        alertMask = GetSgfMask(SGF_COMBAT) | GetSgfMask(0x20) | GetSgfMask(5) | GetSgfMask(6)
            | GetSgfMask(SGF_KO) | GetSgfMask(SGF_JUGGLED) | GetSgfMask(0x16);
        if (guard->grfsg & alertMask)
            state = PICKPOCKET_ALERT;
        else if (!guard->numLoot || guard->loot[guard->numLoot - 1].oid == OID_LOOT_COINS
                 || !GetHeroPo(&g_sw) || GetHeroPo(&g_sw) != D_002E1E40)
            state = PICKPOCKET_COINS;
        else
            state = PICKPOCKET_ITEM;
    }
    SetStepguardPickpocketState(guard, state);
}

// 0x1F1A40: Rolls the guard's loot at load (unless the level fixed it): picks the per-world table (richer for cid
// 0x3A guards), keeps a level-given first entry unless its task is complete, otherwise rolls a weighted item reward
// with the table's chance; then splits a random coin total into 2-3 coin entries and appends the reward.
// Choose a weighted reward and distribute the remaining pickup count.
// Also known as: FillPocket (upstream readable / Sly 1 name; FillPocket__FP2LO).
extern "C" void GenerateStepguardLoot(STEPGUARD *guard)
{
    TABLE *table;
    GUARDLOOT reward;
    int world, total, remaining, numCoinEntries;
    unsigned int i;
    ClearLootPouchLoot(guard->lootPouch);
    if (guard->isLootFixed)
        return;
    world = GetCurrentWorld();
    if (world == -1)
        world = 1;
    if (IsBasicDerivedFrom__FP5BASIC3CID(guard, CID_GUARD))
        table = &D_002C3968[world];
    else
        table = &D_002C3748[world];
    func_002333B4(&reward, 0, sizeof(reward));
    if (guard->numLoot > 0) {
        // A level-given first entry: dropped once its key's task is complete.
        OBJECT *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, guard->loot[0].oid, guard->lootPouch);
        if (object && IsBasicDerivedFrom__FP5BASIC3CID(object, CID_KEY) && object->ptask && IsTaskComplete(object->ptask))
            guard->numLoot = 0;
    }
    if (guard->numLoot > 0) {
        reward = guard->loot[0];
    } else if (RandomFloatInRange__Fff(0.0f, 1.0f) > table->chance) {
        reward.key = -1;
        reward.oid = (OID)-1;
    } else {
        // Weighted pick among the table's six rewards.
        total = 0;
        for (i = 0; i < 6; i++)
            total += table->picks[i].weight;
        int choice = RandomIntInRange__Fii(0, total - 1);
        const PICK *pick = &table->picks[0];
        register int weight asm("$3") = pick->weight; /* pin: weight must be v1 (global-alloc gives it v0 and swaps pick/weight otherwise) */
        asm("" : "+r"(weight)); /* empty asm: gives the first weight load a dependent so sched2 issues it before the choice copy, as in the target */
        // (The loop is written with gotos: its rotated shape, test at the bottom, is what matches.)
        i = 0;
        goto testPick;
nextPick:
        choice -= pick->weight;
        i++;
        if (i >= 6)
            goto picked;
        pick = &table->picks[i];
        weight = pick->weight;
testPick:
        if (choice >= weight)
            goto nextPick;
        reward.key = pick->key;
picked:;
        reward.count = 1;
        reward.oid = (OID)OID_LOOT_ITEM;
    }
    // Split a random coin total into two or three coin entries, then append the reward.
    guard->numLoot = 0;
    remaining = RandomIntInRange__Fii(table->minimum, table->maximum);
    numCoinEntries = RandomIntInRange__Fii(2, 3);
    int j = 0;
    if (numCoinEntries > 0) {
        do {
            GUARDLOOT *item = &guard->loot[guard->numLoot++];
            item->oid = (OID)OID_LOOT_COINS;
            item->flags = 0;
            int amount = remaining / (numCoinEntries - j);
            if (amount < 1)
                amount = 1;
            remaining -= amount;
            item->count = amount;
            if (!remaining)
                break;
            j++;
        } while (j < numCoinEntries);
    }
    if (reward.count > 0)
        guard->loot[guard->numLoot++] = reward;
}

// 0x1F1CF8: Stages the next loot entry: from the current index on, finds the entry's object and loads it into the
// pouch launcher (skipping finished task items); then refreshes the pickpocket state.
// Also known as: NextPocketPrize (upstream readable / Sly 1 name; NextPocketPrize__FP2LO).
extern "C" void StageStepguardLoot(STEPGUARD *guard)
{
    guard->isLootStaged = 0;
    for (; guard->firstLoot < guard->numLoot; guard->firstLoot++) {
        GUARDLOOT *entry = &guard->loot[guard->firstLoot];
        OBJECT *object = FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, entry->oid, guard->lootPouch);
        int key, launchKind;
        if (!object) continue;
        key = entry->key;
        launchKind = LAUNCH_OBJECT;
        if (IsBasicDerivedFrom__FP5BASIC3CID(object, CID_KEY)) {
            TASK *task = object->ptask;
            if (task) {
                int taskState = task->state;
                if (taskState == -1 || taskState == 0) { guard->isLootStaged = 1; break; }
                if (taskState != 1) continue;
            }
            launchKind = LAUNCH_KEY;
        }
        if (StageLootPouchLoot(guard->lootPouch, object, key, launchKind) == 1) guard->isLootStaged = 1;
        break;
    }
    UpdateStepguardPickpocket(guard);
}

// 0x1F1E20: Sly pickpockets the guard: broadcasts game event 0x19, launches the staged entry's count from the
// pouch, resets the state, advances to the next entry and stages it. Returns the launched object, or NULL when
// nothing is left.
// Also known as: PloTakePocket (upstream readable / Sly 1 name; PloTakePocket__FP2LO).
extern "C" void *PickpocketStepguard(STEPGUARD *guard)
{
    void *launched;
    if (guard->firstLoot >= guard->numLoot) return 0;
    BroadcastGameEvent(&g_game, 0x19, guard);
    launched = LaunchLootPouchLootAtPlayer(guard->lootPouch, (guard->loot + guard->firstLoot)->count);   // (pointer form needed for the match)
    SetStepguardPickpocketState(guard, PICKPOCKET_NONE);
    guard->firstLoot++;
    StageStepguardLoot(guard);
    return launched;
}

// 0x1F1EB8: Splice property getter: returns the oids of the loot entries not yet handed out as a list (kind 6).
extern "C" void *GetStepguardLootRefList(void *result, STEPGUARD *guard)
{
    int oids[8];
    int count = 0;
    int i;
    for (i = guard->firstLoot; i < guard->numLoot; i++) oids[count++] = guard->loot[i].oid;
    BuildSpliceListFromUniformValues(result, count, 6, oids);
    return result;
}

// 0x1F1F38: STEPGUARD vtable slot 87 (set playable/active, inferred): STEP base, then enables the loot pouch (its
// vtable slot 21) only while the guard is not in this mode.
extern "C" void SetStepguardPlayable(STEPGUARD *guard, int isPlayable)
{
    IgnoreStepSlot87(guard, isPlayable);
    if (isPlayable) {
        if (guard->lootPouch) guard->lootPouch->SetFlag8(0);
    } else if (guard->lootPouch) {
        guard->lootPouch->SetFlag8(1);
    }
}
