/* include/sly2/prize.h -- the prize classes (gameplay/progression/prize.cpp, their key TU): DPRIZE : SO, the
 * collectable dropped or placed in the world, and its classes COIN, MEDICINE, KEY, LOOT (with its LOOTE entries) and
 * CLUE (the clue bottle), the clue-bottle proxy CLUEPROXY and the clue-bottle safe VAULT (a PO, not a DPRIZE).
 *
 * The classes are registry classes (sly2/classes.h conventions: real virtual layouts, each slot's function named by an
 * asm label, sizes from the class descriptors). They must be defined inside classes.h in their emission order
 * (prize.cpp emits their vtables in the reverse order of these definitions), so classes.h includes this file at
 * DPRIZE's position; a TU that includes this header first gets classes.h (and with it these classes) in the right order.
 *
 * Field names come from how prize.cpp's functions use them; "(inferred)" marks names guessed from that use (no Sly 2
 * symbol or Sly 1 counterpart). Layouts: names/vu-objects-2 (view unification of the file's per-function views). */
#ifndef SLY2_PRIZE_H
#ifndef SLY2_CLASSES_H
#include "sly2/classes.h"   /* includes this header again at the classes' emission position */
#else
#define SLY2_PRIZE_H

/* DPRIZE states (SetDprizeDprizes, vtable slot 76). Animations: idle 0x184 (or the prize's override), airborne
 * 0x183, vanish 0x186. (inferred from the state setters and UpdateDprize) */
enum DPRIZESTATE {
    DPRIZE_STATE_NONE = -1,
    DPRIZE_STATE_IDLE = 0,      /* resting and collectable; the lifetime counts down */
    DPRIZE_STATE_COLLECTED = 1, /* transient: the class setter reacts, then moves on to GONE */
    DPRIZE_STATE_GONE = 2,      /* hidden, back in its proxy pool */
    DPRIZE_STATE_LAUNCHED = 3,  /* sprayed out, rising */
    DPRIZE_STATE_FALLING = 4,   /* falling and bouncing */
    DPRIZE_STATE_VANISHING = 5, /* lifetime over: plays the vanish animation, then GONE */
    DPRIZE_STATE_HELD = 6       /* ProjectDprizeTransform only forwards this state (inferred) */
};

/* 90 DPRIZE (0x230): abstract; vtable 0x2D6B98, 76 slots */
struct DPRIZE : SO
{
    int state;              /* +0x150 DPRIZESTATE */
    float stateTime;        /* +0x154 clock time of the last state change */
    int initialState;       /* +0x158 state PostDprizeLoad enters */
    int idleAnimOverride;   /* +0x15C idle animation in place of 0x184 (-1: none) */
    float animSkipTime;     /* +0x160 the new state machine is skipped ahead by this much (LaunchSprayedDprize) */
    SM *smaSource;          /* +0x164 state machine applied for the state animations */
    SMA *sma;               /* +0x168 its application while a state has an animation */
    LO *target;             /* +0x16C the lock-on TARGET child (cid 0x12): added only while collectable */
    qword position;         /* +0x170 integrated position (ProjectDprizeTransform) */
    qword velocity;         /* +0x180 */
    qword acceleration;     /* +0x190 gravity while airborne, zero at rest */
    float glintChance;      /* +0x1A0 chance of a glint when the camera crosses the prize's side */
    OID glintOwnerOid;      /* +0x1A4 (SetDprizeParams) */
    LO *glintOwner;         /* +0x1A8 */
    OID leftEmitterOid;     /* +0x1AC */
    LO *leftEmitter;        /* +0x1B0 glint emitter used when the camera is on the left */
    OID rightEmitterOid;    /* +0x1B4 */
    LO *rightEmitter;       /* +0x1B8 */
    int glintSide;          /* +0x1BC camera side of the last glint test */
    float glintTime;        /* +0x1C0 clock time of the last glint */
    int hasBoosted;         /* +0x1C4 the one-shot bounce boost was used (CheckDprizeBounce) */
    int isCollectable;      /* +0x1C8 */
    float expireTime;       /* +0x1CC clock time after which the prize collects itself */
    float bounceBoostThreshold; /* +0x1D0 a floor bounce slower than this gets the boost */
    float bounceBoost;      /* +0x1D4 the boost's vertical speed */
    float bounceRadius;     /* +0x1D8 collision radius */
    float collectRadius;    /* +0x1DC pickup radius */
    float normalResponse;   /* +0x1E0 bounce factor along the contact normal */
    float tangentResponse;  /* +0x1E4 bounce factor along the contact plane */
    int unknown1E8[7];
    OID linkedOid;          /* +0x204 (resolved at post-load, not used by prize.cpp) */
    LO *linked;             /* +0x208 */
    OID collectListenerOid; /* +0x20C */
    LO *collectListener;    /* +0x210 told (its slot 51, by hand) when the prize is collected */
    OID vanishListenerOid;  /* +0x214 */
    LO *vanishListener;     /* +0x218 told when the vanish animation ends */
    float unknown21C;       /* +0x21C InitDprize sets 30 */
    int unknown220;
    OID unknown224;         /* +0x224 InitDprize sets -1 */
    int unknown228[2];
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeDprizeDamage");   /* slot 75; key function (prize.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDprizeClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleDprizeMessage");   /* slot 5 */
    virtual void Init() __asm__("InitDprize");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadDprizeFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostDprizeLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateDprize");   /* slot 16 */
    virtual void UpdateXfWorld() __asm__("ProjectDprizeTransform");   /* slot 17 */
    virtual void RenderSelf() __asm__("RenderDprizeSelf");   /* slot 19 */
    virtual void SetState(int state) __asm__("SetDprizeDprizes");   /* slot 76 */
};
typedef char assert_DPRIZE_size[sizeof(DPRIZE) == 0x230 ? 1 : -1];
typedef char assert_DPRIZE_position[SLY2_OFFSETOF(DPRIZE, position) == 0x170 ? 1 : -1];
typedef char assert_DPRIZE_glintChance[SLY2_OFFSETOF(DPRIZE, glintChance) == 0x1A0 ? 1 : -1];
typedef char assert_DPRIZE_bounceBoostThreshold[SLY2_OFFSETOF(DPRIZE, bounceBoostThreshold) == 0x1D0 ? 1 : -1];
typedef char assert_DPRIZE_linkedOid[SLY2_OFFSETOF(DPRIZE, linkedOid) == 0x204 ? 1 : -1];
typedef char assert_DPRIZE_unknown21C[SLY2_OFFSETOF(DPRIZE, unknown21C) == 0x21C ? 1 : -1];

/* 91 COIN (0x240): COIN pool; vtable 0x2D6928, 76 slots */
struct COIN : DPRIZE
{
    float lifetimeMin;      /* +0x230 the idle lifetime is random in [min, max] */
    float lifetimeMax;      /* +0x234 */
    float lifetimeLeft;     /* +0x238 */
    int unknown23C;
    virtual void Update() __asm__("UpdateCoin");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCoinClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitCoin");   /* slot 7 */
    virtual void SetState(int state) __asm__("SetCoinDprizes");   /* slot 76 */
};
typedef char assert_COIN_size[sizeof(COIN) == 0x240 ? 1 : -1];

/* 93 MEDICINE (0x250): health refill; vtable 0x2D66B8, 76 slots */
struct MEDICINE : DPRIZE
{
    float lifetimeMin;      /* +0x230 as COIN's */
    float lifetimeMax;      /* +0x234 */
    float lifetimeLeft;     /* +0x238 */
    int healArg0;           /* +0x23C hit points given (AddHpMp) */
    int healArg1;           /* +0x240 second AddHpMp argument */
    int unknown244[3];
    virtual void Update() __asm__("UpdateMedicine");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetMedicineClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitMedicine");   /* slot 7 */
    virtual void SetState(int state) __asm__("SetMedicineDprizes");   /* slot 76 */
};
typedef char assert_MEDICINE_size[sizeof(MEDICINE) == 0x250 ? 1 : -1];

/* 92 KEY (0x240); vtable 0x2D6448, 76 slots */
struct KEY : DPRIZE
{
    void *task;             /* +0x230 task told (state 2) when the key is collected */
    int unknown234[3];
    virtual void Init() __asm__("InitKey");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetKeyClassDesc");   /* slot 6 */
    virtual void SetState(int state) __asm__("SetKeyDprizes");   /* slot 76 */
};
typedef char assert_KEY_size[sizeof(KEY) == 0x240 ? 1 : -1];

/* LOOTE (0x2C): a loot entry (AddLootEntry): the item id, its save id ("loot" "mplootkc" counter key) and the globs
   shown for it (SelectLootEntry). */
struct LOOTE
{
    int id;
    int saveId;             /* +0x4 */
    char pad8[0x14 - 0x8];
    int unknown14;          /* +0x14 AddLootEntry sets -1 */
    int unknown18;          /* +0x18 AddLootEntry sets -1 */
    int globCount;          /* +0x1C */
    void *globIds[3];       /* +0x20 (ShowSoGlobsById) */
};
typedef char assert_LOOTE_size[sizeof(LOOTE) == 0x2C ? 1 : -1];
enum { MAX_LOOT_ENTRIES = 32 };   /* LoadLootFromBrx reserves 32 * sizeof(LOOTE) = 0x580 bytes */

/* 94 LOOT (0x250): pickpocket item; vtable 0x2D61D8, 76 slots */
struct LOOT : DPRIZE
{
    float lifetimeMin;      /* +0x230 as COIN's */
    float lifetimeMax;      /* +0x234 */
    float lifetimeLeft;     /* +0x238 */
    int entryCount;         /* +0x23C */
    LOOTE *entries;         /* +0x240 */
    LOOTE *currentEntry;    /* +0x244 */
    int unknown248[2];
    virtual void Update() __asm__("UpdateLoot");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLootClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitLoot");   /* slot 7 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadLootFromBrx");   /* slot 14 */
    virtual void SetState(int state) __asm__("SetLootDprizes");   /* slot 76 */
};
typedef char assert_LOOT_size[sizeof(LOOT) == 0x250 ? 1 : -1];
typedef char assert_LOOT_entryCount[SLY2_OFFSETOF(LOOT, entryCount) == 0x23C ? 1 : -1];

/* 95 CLUE (0x230): clue bottle; vtable 0x2D5F68, 76 slots */
struct CLUE : DPRIZE
{
    virtual void Init() __asm__("InitClue");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetClueClassDesc");   /* slot 6 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetClue");   /* slot 26 */
    virtual void OnProxyRelease(LO *plo) __asm__("ClearClueState");   /* slot 27 */
    virtual void SetState(int state) __asm__("SetClueDprizes");   /* slot 76 */
};
typedef char assert_CLUE_size[sizeof(CLUE) == 0x230 ? 1 : -1];

/* 7 CLUEPROXY (0x110): PROXY child; clue-bottle proxy (REVIEW, medium); vtable 0x2D5DC8, 50 slots */
struct CLUEPROXY : PROXY
{
    int clueIndex;          /* +0x100 the level's clue bit (taken from SW iclueNext at post-load) */
    int unknown104[3];
    virtual void PostLoad() __asm__("PostClueproxyLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetClueproxyClassDesc");   /* slot 6 */
};
typedef char assert_CLUEPROXY_size[sizeof(CLUEPROXY) == 0x110 ? 1 : -1];

/* 78 VAULT (0x230): clue-bottle safe; vtable 0x2D5B30, 81 slots. States (UpdateVault): 0 closed, 1 intro relay,
   2 dialling, 3 opening, 4 wrong code, 5 open; the dial has its own sub-state (SetVaultDialState: -1 off, 0 choosing,
   1 turning, 2 entering, 3 done). Its parts are bound by oid (D_002C0EC8). */
struct VAULT : PO
{
    int state;              /* +0x180 */
    float stateTime;        /* +0x184 */
    int dialState;          /* +0x188 */
    float dialStateTime;    /* +0x18C */
    PO *user;               /* +0x190 the player dialling (TriggerVault) */
    int selectedDigit;      /* +0x194 dial position 0..8 (-1: none) */
    int enteredCode;        /* +0x198 code = code * 10 % 1000 + digit + 1 */
    void *relay;            /* +0x19C relay of the current state (a20C[state]) */
    ASEGA *pasegaDial;      /* +0x1A0 digit animation playing (ReplaceCurrentAseg's output) */
    int dialogId;           /* +0x1A4 dialog started by the vault */
    LO *digitTargets[9];    /* +0x1A8 grab targets of the nine dial positions */
    ASEG *digitAnims[9];    /* +0x1CC their animations */
    int combination;        /* +0x1F0 default 123 */
    void *introDialog;      /* +0x1F4 (-1 until set) */
    int notEnoughCluesDialog; /* +0x1F8 */
    void *openedDialog;     /* +0x1FC */
    int rewardPowerup;      /* +0x200 powerup granted on opening (-1: none) */
    XF *dialTimer;          /* +0x204 the dial timer frame (oid 0x645) */
    LO *dialCenter;         /* +0x208 grab target while turning */
    void *relays[6];        /* +0x20C relay fired on entering each state */
    int unknown224[3];
    virtual void PostLoad() __asm__("PostVaultLoad");   /* slot 15 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetVaultClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitVault");   /* slot 7 */
    virtual void Update() __asm__("UpdateVault");   /* slot 16 */
};
typedef char assert_VAULT_size[sizeof(VAULT) == 0x230 ? 1 : -1];
typedef char assert_VAULT_digitTargets[SLY2_OFFSETOF(VAULT, digitTargets) == 0x1A8 ? 1 : -1];
typedef char assert_VAULT_dialTimer[SLY2_OFFSETOF(VAULT, dialTimer) == 0x204 ? 1 : -1];

#endif /* SLY2_CLASSES_H */
#endif /* SLY2_PRIZE_H */
