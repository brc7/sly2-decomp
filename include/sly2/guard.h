/* include/sly2/guard.h -- STEPGUARD (CID 0x38) and the guard root behaviour BHV156 (CID 0x9C).
 * Written by hand in types_round2 from runtime findings (runtime, runtime2, runtime4, runtime7a FINDINGS; devtools/ram_inspector
 * guard decoder). Only offsets with evidence are named; the rest is padding. Tags: [R] runtime-confirmed (offset and meaning
 * checked in save-state RAM), [S] static, [H] hypothesis. Included by sly2/classes.h and the guard TUs under
 * src/game/gameplay/ai/ (stepguard, guardbhv, detector, pickpocket, gomer, ...). */
#ifndef SLY2_GUARD_H
#define SLY2_GUARD_H
#include "sly2/leap.h"
#include "sly2/basic.h"
#include "sly2/step.h"
#include "sly2/xfm.h"
#include "sly2/stepplayer.h"   /* ZPR (BHVGUARDRECDATA) */

struct CID113;  /* the guard script class (classes.h) */
struct CID147;   /* NOISE: a noise event heard by the guards' listeners (noise.cpp; fields in classes.h) */

/* one pickpocket-able loot entry carried by a guard (16 bytes) */
struct GUARDLOOT
{
      OID oid;              /* +0x0 [R] loot object oid */
      undefined4 unknown04[2]; /* [?] */
      int count;            /* +0xC [S] how many (StartBhvko sums the coin entries, oid 0x2B3) */
};

/* A guard script command (SSGE, from SW::slotheapSsge; tu_1709A8.cpp): its kind (SSGK_*: 0 set the context object,
   1/2 walk/pause times, 3 teleport, 4 message, 5 wait, 6 follow a path, 7 go to a point, 8 animation, 9 label,
   10 jump, 11/12 busy on/off, 13 leap, 14 play the behaviour's binding, 15 relay alarm), the target id (an oid, -1
   none), the kind-dependent arguments and the script's list link (DLE at +0x18). [S] */
struct SSGE
{
      int kind;             /* +0x00 */
      int id;               /* +0x04 */
      union
      {
            struct
            {
                  union     /* +0x08 by kind: 5 a time, 3/7 a point, 6/13..15 a number */
                  {
                        int n;
                        float g;
                        qword *ppos;
                  };
                  union     /* +0x0C kind 6: an oid for the patrol path's second target; kinds 3/7: a heading */
                  {
                        int secondOid;
                        float heading;
                  };
            };
            LM range;       /* +0x08 kinds 1/2: the walk/pause time range */
      };
      int lookAtOid;        /* +0x10 kind 7 */
      float gotoParam;      /* +0x14 kind 7: BHVGOTO +0xB0 (meaning unknown) */
      SSGE *next;           /* +0x18 */
      SSGE *previous;       /* +0x1C */
};
typedef char assert_SSGE_size[sizeof(SSGE) == 0x20 ? 1 : -1];
/* Where a guard script resumes (STEPGUARD::scriptResumePoint, 12 bytes: SaveBhvScriptResumeState). [S] */
struct SSGRESUME
{
      SSGE *command;        /* +0x0 */
      LO *context;          /* +0x4 */
      float pathParam;      /* +0x8 kind 6: where along the path */
};

/* A stimulus template (the noise type; only what the guards' sense tests read). [S] */
struct STIMTEMPLATE
{
      undefined4 unknown0[0x50 / 4];
      int kind;             /* +0x50 [S] 0 noise (audible), 1 sight (CanStepguardSenseStimulus) */
      float range;          /* +0x54 [S] how far it carries (CanRajanSenseStimulus doubles it) */
      LM heightRange;       /* +0x58 [S] the height window */
};
/* A stimulus a guard may sense (0x60 bytes; stepguard.cpp's stimulus functions, tu_1CBEB0 emitter). [S] */
struct STIMULUS
{
      undefined4 unknown0[5];
      STIMTEMPLATE *stimTemplate; /* +0x14 [S] what kind of stimulus */
      undefined4 unknown18[2];
      qword pos;            /* +0x20 [S] where it happened */
      qword targetPos;      /* +0x30 [S] */
      LO *source;           /* +0x40 [S] its emitter (a guard does not sense its own) */
      LO *subject;          /* +0x44 [S] who it is about (level 4: only a tracked subject is sensed) */
      undefined4 unknown48;
      float time;           /* +0x4C [S] */
      undefined4 unknown50;
      int level;            /* +0x54 [S] loudness, compared with STEPGUARD::minStimulusLevel */
      STIMULUS *next;       /* +0x58 [S] */
      undefined4 unknown5C;
};
typedef char assert_STIMULUS_level[SLY2_OFFSETOF(STIMULUS, level) == 0x54 ? 1 : -1];
typedef char assert_STIMULUS_size[sizeof(STIMULUS) == 0x60 ? 1 : -1];

/* One detector target (16 bytes): the object, its detection state (0 none, 1 seen, 2 close; set by
   UpdateDetectorTargets) and when the state was entered. [S] */
struct DETECTORTARGET
{
      LO *object;           /* +0x0 [S] */
      int state;            /* +0x4 [S] */
      float stateTime;      /* +0x8 [S] */
      int unknownC;
};

/* A target record (STEPGUARD::pvEngage; func_001F8278 finds the one of a target), shared by every guard engaging
   that target: when the first guard engaged, the record's state and its time, the target, how the engagement token
   is granted (DoesTargetRecordAllowGuard: facing kind and tolerance) and the engaged guards from +0x2C. [S] */
struct TARGETREC
{
      float firstEngageTime; /* +0x00 */
      float stateTime;      /* +0x04 */
      int state;            /* +0x08 -1 none */
      LO *target;           /* +0x0C */
      int facingKind;       /* +0x10 -1 none, 0 camera heading, 1 target facing */
      float unknown14;      /* +0x14 */
      float facingTolerance; /* +0x18 radians */
      int isDisabled;       /* +0x1C */
      char pad20[0x8];
      int guardCount;       /* +0x28 */
      STEPGUARD *guards[1]; /* +0x2C */
};

/* DETECTOR (CID 0x10, cb 0x180): a guard's vision detector. Only the fields the option accessors below use. [S] */
struct DETECTOR : public XFM
{
      STEPGUARD *owner;     /* +0xE0 [S] the guard owning the detector (ResetStepguardToHome) */
      SOTREE *ownerPhysicsTree; /* +0xE4 [S] the owner's physics tree (copied from SO::physicsTree) */
      VOL *pvolNear;        /* +0xE8 [S] near volume (tu_1B9F80's view; VOL type [H]) */
      VOL *pvolFar;         /* +0xEC [S] far volume */
      int unknownF0;        /* +0xF0 [S] cleared by ClearStepguardState */
      int unknownF4;        /* +0xF4 [S] */
      undefined4 unk_0xf8;
      int ctarget;          /* +0xFC [S] entries used in atarget: the designated targets first, then the guards' generated ones (STEPGUARD::generatedTargetCount, the tail) */
      DETECTORTARGET atarget[8]; /* +0x100 [S] the tracked targets (TryAddStepguardDetectTarget, ChooseStepguardTarget) */
    virtual void Update() __asm__("UpdateDetector");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetDetectorClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitDetector");   /* slot 7 */
};
typedef char assert_DETECTOR_size[sizeof(DETECTOR) == 0x180 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_DETECTOR_ctarget[SLY2_OFFSETOF(DETECTOR, ctarget) == 0xFC ? 1 : -1];
typedef char assert_DETECTOR_atarget[SLY2_OFFSETOF(DETECTOR, atarget) == 0x100 ? 1 : -1];
typedef char assert_DETECTOR_owner[SLY2_OFFSETOF(DETECTOR, owner) == 0xE0 ? 1 : -1];
typedef char assert_DETECTOR_unknownF0[SLY2_OFFSETOF(DETECTOR, unknownF0) == 0xF0 ? 1 : -1];
typedef char assert_DETECTOR_pvolNear[SLY2_OFFSETOF(DETECTOR, pvolNear) == 0xE8 ? 1 : -1];

/* An animation binding of a guard (16 bytes; stepguard.cpp ResolveAnimBinding/ReleaseAnimBinding): its kind (2 and 3
   hold a handle released by FreeLeap / FreeDirAnim; -1 none), the bound animation and its instance, and the start
   flags. The second and third words hold the load-time oid/instance until ResolveAnimBinding replaces them, and a
   handle for kinds 2/3 (union selected by kind and by the resolve phase). [S] */
struct ANIMBINDING
{
      int kind;             /* +0x0 [S] 0 animation segment, 2 leap, 3 directional animation, -1 none */
      union {
            OID oid;        /* +0x4 [S] before ResolveAnimBinding: the animation's oid */
            int h;          /* +0x4 [S] kinds 2/3: the LEAP / DIRANIM handle */
            ASEG *aseg;     /* +0x4 [S] kind 0: the animation segment */
            SM *sm;         /* +0x4 [S] kind 1: the state machine */
            void *payload4; /* +0x4 [S] kinds 2/3 as the Splice options allocate them (AllocLeap / AllocDirAnim) */
      };
      union {
            int instance;   /* +0x8 [S] before ResolveAnimBinding */
            ASEGA *asega;   /* +0x8 [S] kind 0: the playing instance */
            void *payload;  /* +0x8 [S] kinds 1/2: the state machine application (kind 1) or the leap (kind 2) */
            SMA *sma;       /* +0x8 [S] kind 1: the playing state machine application */
            BASIC *pbasic;  /* +0x8 [S] the running instance as a counted object: the ASEGA, the SMA, or the guard's own
                               LEAP / DIRANIM (StartAnimBinding takes a reference with slot 1, StopAnimBinding drops it) */
      };
      int flags;            /* +0xC [S] start flags passed to StartAnimBinding */
};
/* The payload of message 9 (an event label of a playing animation reached; the behaviours' HandleMessage, slot 13,
   compare the instance with their binding's): the animation instance, the time and the event label. [S] */
struct ASEGEVENTMSG
{
      ASEGA *asega;         /* +0x0 [S] the playing instance (GetAnimBindingInstance) */
      float time;           /* +0x4 [S] */
      int event;            /* +0x8 [S] the event label (0x15E root motion, 0x25..0x36 guard events) */
};
typedef char assert_ASEGEVENTMSG_event[SLY2_OFFSETOF(ASEGEVENTMSG, event) == 0x8 ? 1 : -1];
/* The guard's mover (STEPGUARD +0xFD0, 16 bytes; tu_144B28.cpp reads the same bytes as a JTGOAL): the driver (the
   behaviour whose motion update moves the guard: SetStepguardMoverDriver / ResetStepguardMover), a heading lock and
   the owning guard. [S] */
struct GUARDMOVER
{
      BHV *driver;          /* +0x0 [S] the driving behaviour (0 none): UpdateStepguardMover runs its UpdateMotion (slot 11) */
      int isHeadingLocked;  /* +0x4 [S] */
      int unknown8;         /* +0x8 [S] */
      STEPGUARD *guard;     /* +0xC [S] the owning guard */
};
/* A guard's general animation binding record (32 bytes): the binding and 16 more bytes. [S] */
struct ANIMBINDINGREC
{
      ANIMBINDING binding;  /* +0x0 [S] */
      unsigned long sgfMask; /* +0x10 [S] the guard-flag set the binding plays for (FindBestAnimBinding) */
      undefined4 unknown18[2];
};

/* CID120 (classes.h's family, defined here): its vtable follows _vt$9STEPGUARD in stepguard.cpp's run, and g++
   emits a TU's vtables in the reverse order of their classes' definitions. */
/* 120 CID120 (0xD0): guard attack definition; vtable 0x2DA840, 48 slots */
struct CID120 : LO
{
    LM distance;            /* +0x50 [S] horizontal distance window of the target (InitGuardAttack 0..300; ChooseStepguardAttack) */
    LM height;              /* +0x58 [S] height window of the target (-5000..5000) */
    float cooldownMin;      /* +0x60 [S] delay range before the attack can be chosen again */
    float cooldownMax;      /* +0x64 [S] */
    float angle;            /* +0x68 [S] radians (SetGuardAttackAngleDegrees; 10 degrees by default) */
    float minLineOfSight;   /* +0x6C [S] a line-of-sight hit nearer than this blocks the attack (ChooseStepguardAttack) */
    int needsLineOfSight;   /* +0x70 [S] */
    int ignoresClass98;     /* +0x74 [S] the line-of-sight test ignores cid 0x62 objects (FilterAttackLineOfSightHit) */
    int aimsAtTarget;       /* +0x78 [S] starting it aims at the target's local transform (ChooseStepguardAttack) */
    int aimObjectOid;       /* +0x7C [S] the transform that aims (a gun arm; -1 none) */
    int effectorOid;        /* +0x80 [S] the transform that strikes */
    int strikeLabel;        /* +0x84 [S] label of the attack animation at the strike */
    int unknown88[2];
    qword strikeOffset;     /* +0x90 [S] where the effector is, relative to the aim object, at the strike (PostGuardAttackLoad) */
    XF *aimObject;          /* +0xA0 [S] FindSoXf(aimObjectOid) */
    XF *effector;           /* +0xA4 [S] FindSoXf(effectorOid) */
    int isEnabled;          /* +0xA8 [S] (InitGuardAttack 1; ChooseStepguardAttack skips disabled attacks) */
    STEPGUARD *guard;       /* +0xAC [S] the guard owning the attack */
    ANIMBINDING binding;    /* +0xB0 [S] the attack's animation */
    ANIMBINDING *activeBinding; /* +0xC0 [S] */
    int unknownC4[3];
    virtual void Init() __asm__("InitGuardAttack");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid120ClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostGuardAttackLoad");   /* slot 15 */
    virtual void Clone(LO *ploBase) __asm__("CloneGuardAttack");   /* slot 41 */
};
typedef char assert_CID120_size[sizeof(CID120) == 0xD0 ? 1 : -1];
typedef char assert_CID120_aimObjectOid[SLY2_OFFSETOF(CID120, aimObjectOid) == 0x7C ? 1 : -1];
typedef char assert_CID120_guard[SLY2_OFFSETOF(CID120, guard) == 0xAC ? 1 : -1];

/* STEPGUARD: the guard: a STEP driven by its behaviour stack (root BHV156), with hit points, a target, pickpocket
   loot, a vision detector and animation bindings.
   class descriptor cb 0x1010, vtable 89 entries (types/cid_table.csv, CONFIRMED runtime) */
struct STEPGUARD : public STEP
{
      unsigned long grfsg;          /* +0x2C0 [R] guard flag word == union of the flags of the behaviours on bhvstk (all
                                       guards in all states, ram_inspector check). 64-bit: on ee-gcc long = 8 bytes, long long = 16 */
      undefined4 unk_0x2c8;
      LM walkTimes;         /* +0x2CC [S] patrol walk duration range (BHVPATROL; script command 1 copies it whole) */
      LM pauseTimes;        /* +0x2D4 [S] patrol pause duration range (BHVPATROL; script command 2) */
      SSGRESUME scriptResumePoint; /* +0x2DC [S] where a patrol script resumes (BHVPATROL saves/restores it) */
      CID113 *patrolScript; /* +0x2E8 [S] the patrol script (BHVPATROL) */
      CID113 *altPatrolScript; /* +0x2EC [S] the alternative patrol script (PostStepguardLoad creates it) */
      SSGE *homeRecord;     /* +0x2F0 [S] the home marker the guard returns to: the alternative patrol script's first command (ResetStepguardToHome) */
      undefined4 unk_0x2f4[3];
      qword homePosition;   /* +0x300 [S] */
      float homeHeading;    /* +0x310 [S] */
      LM attackRange;       /* +0x314 [S] close/far attack distances (InitStepguard 800/1200; UpdateBhvcombat) */
      LM unknownRange31C;   /* +0x31C [S] (InitStepguard 300/450; guardbhv func_0017BFF8 picks between it and circleDistance) */
      LM circleDistance;    /* +0x324 [S] (InitStepguard 300/600) */
      int isInvulnerable32C; /* +0x32C [S] */
      int isKnockedOut;     /* +0x330 [R] knocked out */
      undefined4 unknown334;
      int maxHitPoints;     /* +0x338 [R] max hp */
      int hitPoints;        /* +0x33C [R] current hp */
      int staggerCapacity;  /* +0x340 [S] flinches absorbed before a stagger */
      int staggerProgress;  /* +0x344 [S] */
      ASEG *staggerAseg;    /* +0x348 [S] the flinch animation played while the stagger meter absorbs a zap (HandleBhvguardMessage) */
      ASEGA *heldAnimation; /* +0x34C [S] the flinch animation's instance (ReplaceCurrentAseg; released with ReleaseAsega(&pasega)) */
      int hurtZapMask;      /* +0x350 [S] zap kinds that hurt (InitStepguard) */
      int knockdownZapMask; /* +0x354 [S] zap kinds that knock down */
      int koZapMask;        /* +0x358 [S] zap kinds that knock out */
      CID147 *heldNoise;    /* +0x35C [S] the noise the guard keeps making (ReplaceHeldNoise) */
      float pulseStartTime; /* +0x360 [S] */
      int unknown364;       /* +0x364 [S] */
      LM closeAngleRangeHeld; /* +0x368 [S] interaction angle range while held (GuardEvalInteractMode) */
      LM closeAngleRange;   /* +0x370 [S] */
      float closeRange;     /* +0x378 [S] */
      int canInteract;      /* +0x37C [S] */
      LM interactAngleRange; /* +0x380 [S] */
      float interactRange;  /* +0x388 [S] */
      LO *target;           /* +0x38C [R] current target (Sly while chasing) */
      LO *forcedTarget;     /* +0x390 [S] target forced on the guard (ChooseStepguardTarget) */
      int preferClassCount; /* +0x394 [S] classes the guard prefers as targets (AddStepguardPreferClass, at most 3) */
      int preferClasses[3]; /* +0x398 [S] */
      int generatedTargetCount; /* +0x3A4 [S] detector targets the guard generated (the tail of pdetector->atarget) */
      int ignoreClassCount; /* +0x3A8 [S] classes the guard ignores (AddStepguardIgnoreClass, at most 5) */
      int ignoreClasses[5]; /* +0x3AC [S] */
      int forceClassCount;  /* +0x3C0 [S] classes forced as targets (AddStepguardForceClass, at most 2) */
      int forceClasses[2];  /* +0x3C4 [S] */
      TARGETREC *pvEngage;  /* +0x3CC [R] engagement record (non-zero while in combat): the target's shared TARGETREC */
      int unknown3D0;       /* +0x3D0 [S] */
      int unknown3D4;       /* +0x3D4 [S] read with pvEngage (FBhvcombatLostTarget) */
      int coinCountOverride; /* +0x3D8 [S] coins dropped when knocked out (-1 default; StartBhvko) */
      undefined4 unk_0x3dc;
      qword coinDropPos;    /* +0x3E0 [S] where the coins are sprayed from, in the guard's frame (StartBhvko) */
      int unknown3F0;       /* +0x3F0 [S] (InitStepguard -200) */
      VOL *pickpocketIndicator0; /* +0x3F4 [S] */
      VOL *pickpocketIndicator1; /* +0x3F8 [S] */
      int unknown3FC;       /* +0x3FC [S] (InitStepguard -1) */
      undefined4 unk_0x400;
      GUARDLOOT loot[8];    /* +0x404 [R] pickpocket loot list */
      int numLoot;          /* +0x484 [R] entries used in aloot */
      int firstLoot;        /* +0x488 [S] */
      int isLootStaged;     /* +0x48C [S] */
      int unknown490;       /* +0x490 [S] */
      int lootEnabled;      /* +0x494 [S] */
      void *stuckWatch;     /* +0x498 [S] */
      void *hazardWatch;    /* +0x49C [S] */
      int unknown4A0;       /* +0x4A0 [S] */
      int tracksRelocatePoint; /* +0x4A4 [S] */
      undefined4 unk_0x4a8[2];
      qword relocatePoint;  /* +0x4B0 [S] where BHVRELOCATE moves the guard */
      int cbindingrec;      /* +0x4C0 [S] entries used in abindingrec */
      undefined4 unknown4C4[3];
      ANIMBINDINGREC abindingrec[32];   /* +0x4D0 [S] */
      ANIMBINDING knockdownReactions[26]; /* +0x8D0 [S] zap-reaction bindings by zap kind (ResolveZapReactions): knockdown */
      ANIMBINDING hurtReactions[26];      /* +0xA70 [S] hurt */
      ANIMBINDING koReactions[26];        /* +0xC10 [S] KO */
      LEAP leap;        /* +0xDB0 constructed by STEPGUARD_ctor (ConstructLeap) */
      CID146 cid146;    /* +0xE20 (DIRANIM) constructed inline by STEPGUARD_ctor */
      int noiseType;        /* +0xE60 [S] */
      int sensesEnabled;    /* +0xE64 [S] */
      int minStimulusLevel; /* +0xE68 [S] weakest stimulus the guard reacts to */
      LM lookTimes;         /* +0xE6C [S] how long a search looks around (InitStepguard 5.5/6.5; SetBhvSearchLookTimers) */
      LO *searchTarget;     /* +0xE74 [S] a search ends when its last known position is nearer than this object (HasBhvSearchReachedTarget) */
      int searchMode;       /* +0xE78 [S] */
      float sleepTimeMin;   /* +0xE7C [S] sleep duration range (BHVSLEEP) */
      float sleepTimeMax;   /* +0xE80 [S] */
      int sleepDartCapacity; /* +0xE84 [S] sleep darts absorbed before sleeping */
      int sleepDartProgress; /* +0xE88 [S] */
      ASEG *sleepDartAseg;  /* +0xE8C [S] the animation a sleep dart that does not yet put the guard to sleep plays */
      ASEGA *sleepDartAsega; /* +0xE90 [S] its instance */
      int numAllowedZones;  /* +0xE94 [S] path zones the guard may use */
      int allowedZones[8];  /* +0xE98 [S] */
      int attackOids[8];    /* +0xEB8 [S] oids of the attack definitions (resolved into attacks) */
      int attackOidCount;   /* +0xED8 [S] */
      CID120 *attacks[8];   /* +0xEDC [S] the guard's attack definitions (ChooseStepguardAttack) */
      int numAttacks;       /* +0xEFC [S] */
      CID120 *currentAttack; /* +0xF00 [S] the attack being run */
      float nextAttackTime; /* +0xF04 [S] earliest time of the next attack choice (guardbhv TryBhvcombatChooseAttack) */
      CID120 *aimAttack;    /* +0xF08 [S] the attack being aimed */
      ACT *aimAct;          /* +0xF0C [S] */
      int aimTarget;        /* +0xF10 [S] */
      undefined4 unk_0xf14[3];
      qword aimOffset;      /* +0xF20 [S] */
      int ignoresBusyGuards; /* +0xF30 [S] */
      int ready;            /* +0xF34 [S] */
      undefined4 unk_0xf38;
      int isEngagedBusy;    /* +0xF3C [S] set by StartBhvstrike, cleared by CloseBhvstrike (inferred: a strike is in progress) */
      int unknownF40;       /* +0xF40 [S] (InitStepguard 1) */
      float alertCooldownEnd; /* +0xF44 [S] */
      LM shootRange;        /* +0xF48 [S] distance range of a shot (FBhvcombatCanShoot) */
      LM shootPan;          /* +0xF50 [S] pan (yaw) range of a shot */
      LM shootTilt;         /* +0xF58 [S] tilt (pitch) range of a shot */
      int firingWindow;     /* +0xF60 [R] firing window (shooting guards) */
      DETECTOR *pdetector;  /* +0xF64 [R] vision detector (CID 0x10; its +0xE0 owner == this guard) */
      LO *escortObject;     /* +0xF68 [S] escort object, copied with escortPos into the root behaviour's sub-behaviour in mode 0x10 (SetStepguardEscortPoint) */
      undefined4 unknownF6C;
      qword escortPos;      /* +0xF70 [S] escort point */
      int attachKind;       /* +0xF80 [S] what the guard is attached to (rail, pipe, ...) */
      LO *attachObject;     /* +0xF84 [S] the object the guard is attached to (rail/pipe behaviours) */
      float attachParam;    /* +0xF88 [S] position along it */
      int isMovingForward;  /* +0xF8C [S] */
      void *attachPoint;    /* +0xF90 [S] */
      int designatedTargetCount; /* +0xF94 [S] targets designated by script (SetStepguardTargetLists); with avoidObjectCount, non-zero means a target list (FBhvguardHasTargetList) */
      LO *designatedTargets[4]; /* +0xF98 [S] */
      int avoidObjectCount; /* +0xFA8 [S] objects to avoid (SetStepguardTargetLists) */
      LO *avoidObjects[4];  /* +0xFAC [S] */
      int specialState;     /* +0xFBC [S] */
      float specialStartTime; /* +0xFC0 [S] */
      void *scale;          /* +0xFC4 [S] a scale record (value at +0x20) */
      int isHandsOff;       /* +0xFC8 [S] (IsStepguardHandsOff) */
      LO *handsOffLookAt;   /* +0xFCC [S] object faced while hands-off (BHVIDLE lookAt) */
      GUARDMOVER mover;     /* +0xFD0 [S] movement arbiter */
      DLE dleGuard;         /* +0xFE0 [R] link in the in-world guard list (DL at 0x3E1444, ibDle 0xFE0) */
      uint capabilityFlags; /* +0xFE8 [R] capability bits (bit 3 = may shoot) */
      LO *knockdownHelper;  /* +0xFEC [S] an object the knockdown behaviour adds to the world while it runs (StartBhvKnockdown / CloseBhvKnockdown); removed on reset */
      float walkPhase;      /* +0xFF0 [S] walk-cycle phase saved by BHVANIM when its walk animation stops */
      float walkPhaseTime;  /* +0xFF4 [S] the time it was saved */
      float removalTime;    /* +0xFF8 [S] time the guard was removed from the world (OnStepguardRemove) */
      int busyCount;        /* +0xFFC [R] busy count */
      int removalMode;      /* +0x1000 [S] how the guard leaves the world (OnStepguardRemove / InitStepguard: D_002C3CF0) */
      undefined4 unknown1004[3];
    STEPGUARD() __asm__("STEPGUARD_ctor");   /* inline in the game (linkonce copy at 0x256360), called by the guard factories: defined in app/class_registry.cpp */
    virtual void RemoveHierarchy() __asm__("RemoveStepguardHierarchy");   /* slot 12 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetStepguardClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleStepguardMessage");   /* slot 5 */
    virtual void Init() __asm__("InitStepguard");   /* slot 7 */
    virtual void PostLoad() __asm__("PostStepguardLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateStepguard");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderStepguardSelf");   /* slot 19 */
    virtual void OnRemove() __asm__("OnStepguardRemove");   /* slot 22 */
    virtual void OnAdd() __asm__("OnStepguardAdd");   /* slot 23 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetStepguardToHome");   /* slot 26 */
    virtual void OnProxyRelease(LO *plo) __asm__("ClearStepguardState");   /* slot 27 */
    virtual void Clone(LO *ploBase) __asm__("CloneStepguard");   /* slot 41 */
    virtual void AddExtraXps() __asm__("AddStepguardAttachContacts");   /* slot 59 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldStepguardIgnoreIntersection");   /* slot 61 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeStepguardDamage");   /* slot 75 */
    virtual int IsInvulnerable(ZPK zpk) __asm__("IsStepguardInvulnerable");   /* slot 76 */
    virtual int GetHealthState() __asm__("GetStepguardHealthState");   /* slot 77 */
    virtual int GetGroundProbeFlags() __asm__("GetStepguardGroundProbeFlags");   /* slot 85 */
    virtual int IsZapLethal(ZPR *pzpr) __asm__("IsStepguardZapLethal");   /* slot 86 */
    virtual void SetPlayable(int fOn) __asm__("SetStepguardPlayable");   /* slot 87 */
    virtual LO *ChooseTarget() __asm__("ChooseStepguardTarget");   /* slot 88: STEPGUARD adds 2 slots to STEP's 0..87; returns the target to pursue (ResumeBhvguard) */
    virtual int CanSenseStimulus(struct STIMULUS *pstim) __asm__("CanStepguardSenseStimulus");   /* slot 89 (stepguard.cpp calls vtable entry 89 with the stimulus) */
};
typedef char assert_STEPGUARD_size[sizeof(STEPGUARD) == 0x1010 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_STEPGUARD_grfsg[SLY2_OFFSETOF(STEPGUARD, grfsg) == 0x2C0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_hitPoints[SLY2_OFFSETOF(STEPGUARD, hitPoints) == 0x33C ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_STEPGUARD_target[SLY2_OFFSETOF(STEPGUARD, target) == 0x38C ? 1 : -1];         /* CONFIRMED runtime */
typedef char assert_STEPGUARD_loot[SLY2_OFFSETOF(STEPGUARD, loot) == 0x404 ? 1 : -1];     /* CONFIRMED runtime */
typedef char assert_STEPGUARD_numLoot[SLY2_OFFSETOF(STEPGUARD, numLoot) == 0x484 ? 1 : -1]; /* CONFIRMED runtime */
typedef char assert_STEPGUARD_abindingrec[SLY2_OFFSETOF(STEPGUARD, abindingrec) == 0x4D0 ? 1 : -1];   /* stepguard.cpp */
typedef char assert_STEPGUARD_knockdownReactions[SLY2_OFFSETOF(STEPGUARD, knockdownReactions) == 0x8D0 ? 1 : -1];   /* stepguard.cpp */
typedef char assert_STEPGUARD_koReactions[SLY2_OFFSETOF(STEPGUARD, koReactions) == 0xC10 ? 1 : -1];
typedef char assert_STEPGUARD_nextAttackTime[SLY2_OFFSETOF(STEPGUARD, nextAttackTime) == 0xF04 ? 1 : -1]; /* guardbhv.cpp */
typedef char assert_STEPGUARD_avoidObjectCount[SLY2_OFFSETOF(STEPGUARD, avoidObjectCount) == 0xFA8 ? 1 : -1]; /* guardbhv.cpp */
typedef char assert_STEPGUARD_homePosition[SLY2_OFFSETOF(STEPGUARD, homePosition) == 0x300 ? 1 : -1];
typedef char assert_STEPGUARD_forceClasses[SLY2_OFFSETOF(STEPGUARD, forceClasses) == 0x3C4 ? 1 : -1];
typedef char assert_STEPGUARD_pickpocketIndicator1[SLY2_OFFSETOF(STEPGUARD, pickpocketIndicator1) == 0x3F8 ? 1 : -1];
typedef char assert_STEPGUARD_tracksRelocatePoint[SLY2_OFFSETOF(STEPGUARD, tracksRelocatePoint) == 0x4A4 ? 1 : -1];
typedef char assert_STEPGUARD_attackOidCount[SLY2_OFFSETOF(STEPGUARD, attackOidCount) == 0xED8 ? 1 : -1];
typedef char assert_STEPGUARD_aimOffset[SLY2_OFFSETOF(STEPGUARD, aimOffset) == 0xF20 ? 1 : -1];
typedef char assert_STEPGUARD_removalTime[SLY2_OFFSETOF(STEPGUARD, removalTime) == 0xFF8 ? 1 : -1];
typedef char assert_STEPGUARD_escortPos[SLY2_OFFSETOF(STEPGUARD, escortPos) == 0xF70 ? 1 : -1]; /* stepguard.cpp */
typedef char assert_STEPGUARD_walkTimes[SLY2_OFFSETOF(STEPGUARD, walkTimes) == 0x2CC ? 1 : -1];
typedef char assert_STEPGUARD_isInvulnerable32C[SLY2_OFFSETOF(STEPGUARD, isInvulnerable32C) == 0x32C ? 1 : -1];
typedef char assert_STEPGUARD_unknown3D4[SLY2_OFFSETOF(STEPGUARD, unknown3D4) == 0x3D4 ? 1 : -1];
typedef char assert_STEPGUARD_relocatePoint[SLY2_OFFSETOF(STEPGUARD, relocatePoint) == 0x4B0 ? 1 : -1];
typedef char assert_STEPGUARD_sleepTimeMin[SLY2_OFFSETOF(STEPGUARD, sleepTimeMin) == 0xE7C ? 1 : -1];
typedef char assert_STEPGUARD_attacks[SLY2_OFFSETOF(STEPGUARD, attacks) == 0xEDC ? 1 : -1];
typedef char assert_STEPGUARD_isEngagedBusy[SLY2_OFFSETOF(STEPGUARD, isEngagedBusy) == 0xF3C ? 1 : -1];
typedef char assert_STEPGUARD_shootTilt[SLY2_OFFSETOF(STEPGUARD, shootTilt) == 0xF58 ? 1 : -1];
typedef char assert_STEPGUARD_attachPoint[SLY2_OFFSETOF(STEPGUARD, attachPoint) == 0xF90 ? 1 : -1];
typedef char assert_STEPGUARD_specialState[SLY2_OFFSETOF(STEPGUARD, specialState) == 0xFBC ? 1 : -1];
typedef char assert_STEPGUARD_mover[SLY2_OFFSETOF(STEPGUARD, mover) == 0xFD0 ? 1 : -1];
typedef char assert_STEPGUARD_walkPhaseTime[SLY2_OFFSETOF(STEPGUARD, walkPhaseTime) == 0xFF4 ? 1 : -1];
typedef char assert_STEPGUARD_pdetector[SLY2_OFFSETOF(STEPGUARD, pdetector) == 0xF64 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_dleGuard[SLY2_OFFSETOF(STEPGUARD, dleGuard) == 0xFE0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_STEPGUARD_attackRange[SLY2_OFFSETOF(STEPGUARD, attackRange) == 0x314 ? 1 : -1];
typedef char assert_STEPGUARD_hurtZapMask[SLY2_OFFSETOF(STEPGUARD, hurtZapMask) == 0x350 ? 1 : -1];
typedef char assert_STEPGUARD_coinCountOverride[SLY2_OFFSETOF(STEPGUARD, coinCountOverride) == 0x3D8 ? 1 : -1];
typedef char assert_STEPGUARD_unknown3FC[SLY2_OFFSETOF(STEPGUARD, unknown3FC) == 0x3FC ? 1 : -1];
typedef char assert_STEPGUARD_lookTimes[SLY2_OFFSETOF(STEPGUARD, lookTimes) == 0xE6C ? 1 : -1];
typedef char assert_STEPGUARD_unknownF40[SLY2_OFFSETOF(STEPGUARD, unknownF40) == 0xF40 ? 1 : -1];
typedef char assert_STEPGUARD_removalMode[SLY2_OFFSETOF(STEPGUARD, removalMode) == 0x1000 ? 1 : -1];
typedef char assert_STEPGUARD_busyCount[SLY2_OFFSETOF(STEPGUARD, busyCount) == 0xFFC ? 1 : -1]; /* CONFIRMED runtime */

/* Option accessors (inline). g_aeopid takes their addresses, which is why out-of-line copies of them exist in the
 * linkonce area; g_aeopid's TU (class_registry.cpp) emits them that way. See docs/guides/tu-merging.md
 * "Inline functions and linkonce copies". Free functions (not members): see there why. Names are placeholders. */
inline void func_00259D38(STEPGUARD *pstepguard, VOL *pvol) { pstepguard->pdetector->pvolNear = pvol; }   /* set */
inline void func_00259D48(STEPGUARD *pstepguard, VOL *pvol) { pstepguard->pdetector->pvolFar = pvol; }    /* set */
inline VOL *func_00259D60(STEPGUARD *pstepguard) { return pstepguard->pdetector->pvolNear; }   /* get */
inline VOL *func_00259D70(STEPGUARD *pstepguard) { return pstepguard->pdetector->pvolFar; }    /* get */

/* BHVGUARD's mode record (0x70; BHVGUARD.md): the awareness mode and its argument, or for the zap modes the zap.
   SetBhvguardMode copies it whole; IsBhvguardModeSame compares two. The data part; classes.h's BHVGUARDREC adds the
   clearing constructor (a POD base, so BHV156 can overlay it). [S] */
struct BHVGUARDRECDATA
{
      int kind;             /* +0x00 the mode (GUARD_MODE_*, guardbhv.cpp) */
      int unknown4[3];
      union {
            struct {
                  void *object; /* +0x10 what the mode is about: the target, the noise, the hero, the escorted object */
                  float amount; /* +0x14 a time or amount (stalk delay, watch time) */
            } arg;
            ZPR zap;        /* +0x10 hurt/knockdown/KO modes: the zap that caused them */
      };
};
typedef char assert_BHVGUARDRECDATA_size[sizeof(BHVGUARDRECDATA) == 0x70 ? 1 : -1];

/* BHV156 (CID 0x9C, cb 0xB0): root of every guard's behaviour stack; its mode drives calm/search/combat */
struct BHV156 : public BHV
{
      union {
            BHVGUARDRECDATA modeRec; /* +0x30 [S] the current mode record (SetBhvguardMode) */
            struct {
                  int mode; /* +0x30 [R] 0 start, 1 calm, 2 search, 3 alert anim, 4 combat, 5 script, 7 juggled, 11 KO
                               (runtime2/4/7a; ram_inspector ROOT_MODE); 17 also seen (S28, meaning unknown) */
                  undefined4 unknown34[3];
                  LO *modeObject; /* +0x40 [R] object of the mode (stimulus / target) */
                  float modeAmount; /* +0x44 [S] the mode's amount (SetStepguardSpecial compares it with FLT_MAX in mode 0x12) */
            };
      };
      float modeTime;       /* +0xA0 [S] when the mode was entered */
      float watchEndTime;   /* +0xA4 [S] mode 0x1A (watch): when it ends */
      undefined4 unk_0xa8[2];
};
typedef char assert_BHV156_size[sizeof(BHV156) == 0xB0 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_BHV156_mode[SLY2_OFFSETOF(BHV156, mode) == 0x30 ? 1 : -1];     /* CONFIRMED runtime */

#endif /* SLY2_GUARD_H */
