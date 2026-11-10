/* include/sly2/hack.h -- the hacking minigame classes (Bentley's hack: gameplay/objects/hack.cpp, their key TU):
 * HACK : PO and its subclasses HACKSHIP (the player's ship), HACKBULLET, HACKENEMY, HACKLOCK and HACKWALL, with the
 * records they own (HACKE anim entries, ship parts, enemy targeting rules, wall segments and pieces).
 *
 * The classes are registry classes (sly2/classes.h conventions: real virtual layouts, each slot's function named by an
 * asm label, sizes from the class descriptors). They must be defined at their emission position inside classes.h, so
 * classes.h includes this file there; a TU that includes this header first gets classes.h (and with it these classes)
 * in the right order.
 *
 * Field names come from how hack's functions use them; "(inferred)" marks names guessed from that use (no Sly 2
 * symbol or Sly 1 counterpart). Layouts: names/vu-objects (view unification of the file's per-function views). */
#ifndef SLY2_HACK_H
#ifndef SLY2_CLASSES_H
#include "sly2/classes.h"   /* includes this header again at the classes' emission position */
#else
#define SLY2_HACK_H

/* HACKE (0x10): an animation entry of a hack object: an oid and what it resolves to (BindHackAnimEntries: an ASEG,
   cid 0x6D, or a state machine, cid 0x6F); StartHackAnim plays the entry of the object's state. */
struct HACKE
{
    OID oid;
    ASEG *paseg;     /* +0x4 */
    SM *psm;         /* +0x8 */
    int grf;         /* +0xC flags passed to ReplaceCurrentAseg / ReplaceSma */
};
typedef char assert_HACKE_size[sizeof(HACKE) == 0x10 ? 1 : -1];

/* 71 HACK (0x1C0): hacking minigame base (damageable PO); vtable 0x2D1878, 83 slots */
struct HACK : PO
{
    int cHealthMax;         /* +0x180 */
    int cHealth;            /* +0x184 */
    int cDamage;            /* +0x188 damage it deals: KillHack's blast, the contact kills of bullets/enemies/walls
                               (HACKBULLET's Init sets 10) (inferred) */
    int fDying;             /* +0x18C StartHackDying posted the Kill callback */
    EXPL *pexpl;            /* +0x190 explosion played when it dies */
    ASEGA *pasega;          /* +0x194 current animation (StartHackAnim) */
    SMA *psma;              /* +0x198 current state machine */
    int unknown19C;
    union                   /* +0x1A0 knockback gain: a quadratic in the damage taken (TakeHackDamage) */
    {
        qword qwKnockback;  /* as InitHack stores the default */
        CLQ clqKnockback;
    };
    LM lmKnockback;         /* +0x1B0 limits of the gain (the max also limits the resulting speed) */
    int unknown1B8[2];
    virtual void RemoveHierarchy() __asm__("RemoveHackHierarchy");   /* slot 12 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHack");   /* slot 7 */
    virtual void PostLoad() __asm__("PostHackLoad");   /* slot 15 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetHackHealth");   /* slot 26 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackNewXp");   /* slot 55 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackDamage");   /* slot 75 */
    virtual void Kill(LO *ploKiller) __asm__("KillHack");   /* slot 82: the health ran out (0) or the dying callback fired */
    virtual void Reset() __asm__("ResetHack");   /* slot 83 (PostHackLoad calls it) */
};
typedef char assert_HACK_size[sizeof(HACK) == 0x1C0 ? 1 : -1];
typedef char assert_HACK_pexpl[SLY2_OFFSETOF(HACK, pexpl) == 0x190 ? 1 : -1];
typedef char assert_HACK_qwKnockback[SLY2_OFFSETOF(HACK, qwKnockback) == 0x1A0 ? 1 : -1];
typedef char assert_HACK_lmKnockback[SLY2_OFFSETOF(HACK, lmKnockback) == 0x1B0 ? 1 : -1];

/* HACKSHIPPART (0x18): a ship part (AddHackshipOidPair, BindHackshipParts): a transform of the ship moved by a position
   actuator, and an object driven by a state machine (UpdateHackship's goals 0x66/0x68). */
struct HACKSHIPPART
{
    OID oidXf;              /* +0x0 */
    OID oidObject;          /* +0x4 */
    XF *pxf;                /* +0x8 */
    ACT *pact;              /* +0xC position actuator on pxf */
    LO *plo;                /* +0x10 */
    SMA *psma;              /* +0x14 state machine applied to plo */
};
typedef char assert_HACKSHIPPART_size[sizeof(HACKSHIPPART) == 0x18 ? 1 : -1];

/* 72 HACKSHIP (0x2B0); vtable 0x2D15D0, 83 slots */
struct HACKSHIP : HACK
{
    int state;              /* +0x1C0 */
    float stateTime;        /* +0x1C4 */
    HACKE ahacke[5];        /* +0x1C8 one per state */
    float heading;          /* +0x218 */
    int unknown21C;
    qword desiredVelocity;  /* +0x220 */
    int cpart;              /* +0x230 */
    HACKSHIPPART apart[4];  /* +0x234 */
    LO *ploDock;            /* +0x294 the lock's trigger object the ship docks to in state 3 (inferred) */
    ACT *pactPos;           /* +0x298 docking actuators (state 3) */
    ACT *pactRot;           /* +0x29C */
    WIDGET *pwidgetHealth;  /* +0x2A0 */
    CID151 *psndEngine;     /* +0x2A4 counted (RcPtr_Assign); the constructor clears it */
    int sfxidEngine;        /* +0x2A8 */
    float uVolEngine;       /* +0x2AC */
    HACKSHIP();
    virtual void RemoveHierarchy() __asm__("RemoveHackshipHierarchy");   /* slot 12 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackshipClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitHackship");   /* slot 7 */
    virtual void PostLoad() __asm__("PostHackshipLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHackship");   /* slot 16 */
    virtual void PresetAccel() __asm__("PresetHackshipAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackshipDamage");   /* slot 75 */
    virtual void OnActive(int fActive) __asm__("OnHackshipActive");   /* slot 78 */
    virtual void Reset() __asm__("ResetHackship");   /* slot 83 */
};
typedef char assert_HACKSHIP_size[sizeof(HACKSHIP) == 0x2B0 ? 1 : -1];
typedef char assert_HACKSHIP_ahacke[SLY2_OFFSETOF(HACKSHIP, ahacke) == 0x1C8 ? 1 : -1];
typedef char assert_HACKSHIP_desiredVelocity[SLY2_OFFSETOF(HACKSHIP, desiredVelocity) == 0x220 ? 1 : -1];
typedef char assert_HACKSHIP_apart[SLY2_OFFSETOF(HACKSHIP, apart) == 0x234 ? 1 : -1];
typedef char assert_HACKSHIP_psndEngine[SLY2_OFFSETOF(HACKSHIP, psndEngine) == 0x2A4 ? 1 : -1];
/* The constructor class_registry inlines into its factory. g++ 2.95 emits a class's inline member functions in
   its key TU (as global functions the ELF does not have), so the key TU defines SLY2_KEY_TU_HACKSHIP and sees
   only the declaration. */
#ifndef SLY2_KEY_TU_HACKSHIP
// HACKSHIP constructor: clears the engine sound pointer.
inline HACKSHIP::HACKSHIP() { psndEngine = 0; }
#endif

/* 74 HACKBULLET (0x1E0); vtable 0x2D1328, 83 slots */
struct HACKBULLET : HACK
{
    float tExpire;          /* +0x1C0 Kill when the clock passes it */
    float dtLifetime;       /* +0x1C4 0: never expires */
    SO *psoOwner;           /* +0x1C8 who fired it: contacts with its tree are ignored */
    int fBounce;            /* +0x1CC survives contacts with non-hack objects, which get elasticity 1 (inferred) */
    LO *ploTarget;          /* +0x1D0 homing target */
    float radTurnRate;      /* +0x1D4 homing turn rate (0: no homing) */
    int unknown1D8[2];
    virtual void Init() __asm__("InitHackbullet");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackbulletClassDesc");   /* slot 6 */
    virtual void Update() __asm__("UpdateHackbullet");   /* slot 16 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackbulletNewXp");   /* slot 55 */
    virtual int ShouldIgnoreIntersection(SO *psoOther) __asm__("ShouldHackbulletIgnoreIntersection");   /* slot 61 */
};
typedef char assert_HACKBULLET_size[sizeof(HACKBULLET) == 0x1E0 ? 1 : -1];
typedef char assert_HACKBULLET_ploTarget[SLY2_OFFSETOF(HACKBULLET, ploTarget) == 0x1D0 ? 1 : -1];

/* HACKRULE (0x28): when a hack enemy enters or stays in a state (UpdateHackenemy): the target's distance and its
   heading relative to the enemy, each test enabled by its flag (SetHackenemyEntryVectors / ...SecondVectors). */
struct HACKRULE
{
    int fDistance;          /* +0x0 */
    int fAngle;             /* +0x4 */
    LM lmEnterDistance;     /* +0x8 */
    LM lmStayDistance;      /* +0x10 */
    LM lmEnterAngle;        /* +0x18 */
    LM lmStayAngle;         /* +0x20 */
};
typedef char assert_HACKRULE_size[sizeof(HACKRULE) == 0x28 ? 1 : -1];

/* 73 HACKENEMY (0x390); vtable 0x2D1080, 83 slots */
struct HACKENEMY : HACK
{
    int state;              /* +0x1C0 */
    float stateTime;        /* +0x1C4 */
    int fHoldAim;           /* +0x1C8 animation events 0x25/0x26: states 1/2 keep their heading (inferred) */
    int fHoldState;         /* +0x1CC animation events 0x27/0x28: the state rules are not evaluated (inferred) */
    void *pvPathzone;       /* +0x1D0 path zone the chase path is found in (FindPathzonePathCurve) */
    LO *ploTarget;          /* +0x1D4 */
    SHAPE *pshapePath;      /* +0x1D8 chase path (NewSampledShape) */
    float sPath;            /* +0x1DC progress along it */
    qword posSelfPath;      /* +0x1E0 positions when the path was last found */
    qword posTargetPath;    /* +0x1F0 */
    float tPath;            /* +0x200 */
    float heading;          /* +0x204 */
    int unknown208[2];
    qword desiredVelocity;  /* +0x210 chase velocity (state 3) or animation step (event 0x15E) */
    HACKE ahacke[5];        /* +0x220 one per state */
    HACKRULE arule[5];      /* +0x270 one per state */
    float uSpeed;           /* +0x338 chase speed scale */
    int unknown33C;
    CLQ clqSpeedDistance;   /* +0x340 speed factor by the path left */
    CLQ clqSpeedTurn;       /* +0x350 speed factor by the turn */
    CLQ clqHeading;         /* +0x360 heading from the target's heading (states 1/2) */
    LM lmHeading;           /* +0x370 */
    float aScale[3];        /* +0x378 rotation spring (AccelSoTowardMatBounded's SMPA: speeds and time scale) */
    float sRange;           /* +0x384 gain toward desiredVelocity in state 3 */
    int unknown388[2];
    virtual void Reset() __asm__("ResetHackenemy");   /* slot 83 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackenemyClassDesc");   /* slot 6 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleHackenemyMessage");   /* slot 5 */
    virtual void Init() __asm__("InitHackenemy");   /* slot 7 */
    virtual void RemoveHierarchy() __asm__("RemoveHackenemyHierarchy");   /* slot 12 */
    virtual void PostLoad() __asm__("PostHackenemyLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHackenemy");   /* slot 16 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetHackenemyState");   /* slot 26 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackenemyNewXp");   /* slot 55 */
    virtual void PresetAccel() __asm__("PresetHackenemyAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackenemyDamage");   /* slot 75 */
};
typedef char assert_HACKENEMY_size[sizeof(HACKENEMY) == 0x390 ? 1 : -1];
typedef char assert_HACKENEMY_desiredVelocity[SLY2_OFFSETOF(HACKENEMY, desiredVelocity) == 0x210 ? 1 : -1];
typedef char assert_HACKENEMY_arule[SLY2_OFFSETOF(HACKENEMY, arule) == 0x270 ? 1 : -1];
typedef char assert_HACKENEMY_lmHeading[SLY2_OFFSETOF(HACKENEMY, lmHeading) == 0x370 ? 1 : -1];
typedef char assert_HACKENEMY_sRange[SLY2_OFFSETOF(HACKENEMY, sRange) == 0x384 ? 1 : -1];

/* 75 HACKLOCK (0x250); vtable 0x2D0DD8, 83 slots */
struct HACKLOCK : HACK
{
    int state;              /* +0x1C0 */
    float stateTime;        /* +0x1C4 */
    HACKSHIP *pship;        /* +0x1C8 the ship that reached the lock (state 1) */
    LO *ploTrigger;         /* +0x1CC the ship must come close to it; handed to the ship to dock to */
    HACKE ahacke[3];        /* +0x1D0 one per state */
    int crsmg;              /* +0x200 */
    OID arsmg[4][4];        /* +0x204 RSMG[4] (sw.cpp's RSMG: oidRoot, oidSM, oidTrigger, oidNoTrigger; no header yet) */
    int unknown244[3];
    virtual void Init() __asm__("InitHacklock");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHacklockClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostHacklockLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHacklock");   /* slot 16 */
    virtual void Reset() __asm__("ResetHacklock");   /* slot 83 */
};
typedef char assert_HACKLOCK_size[sizeof(HACKLOCK) == 0x250 ? 1 : -1];
typedef char assert_HACKLOCK_ahacke[SLY2_OFFSETOF(HACKLOCK, ahacke) == 0x1D0 ? 1 : -1];
typedef char assert_HACKLOCK_crsmg[SLY2_OFFSETOF(HACKLOCK, crsmg) == 0x200 ? 1 : -1];

struct HACKWALLSEG;
/* HACKWALLPIECE (0x70, allocated by PostHackwallLoad): a breakable piece of a wall segment, sliding along the segment's
   shape. The Sly symbols FindNearestHackwallSegment / SetHackwallSegmentState / UpdateHackwallSegments name it a
   segment; here a segment is the 0x30-byte record AddHackwallSegment appends. */
struct HACKWALLPIECE
{
    qword pos;              /* +0x0 point on the shape (EvaluateShapeAtDistance) */
    qword dir;              /* +0x10 its tangent */
    HACKWALLSEG *pseg;      /* +0x20 */
    int state;              /* +0x24 0 intact, 1 hit (wobbling), 2 broken */
    float stateTime;        /* +0x28 */
    LM lmS;                 /* +0x2C where it sits along the shape for the ship's closest point at either end of
                               lmSClosest (LayoutHackwallPieces; UpdateHackwallPieces interpolates) */
    LM lmSClosest;          /* +0x34 */
    float s;                /* +0x3C position along the shape, smoothed toward sGoal */
    float ds;               /* +0x40 its speed */
    float sGoal;            /* +0x44 */
    float negDistanceToShip;    /* +0x48 */
    int cHealth;            /* +0x4C */
    float lateral;          /* +0x50 wobble offset and roll (springs; TakeHackwallDamage kicks their speeds) */
    float roll;             /* +0x54 */
    float dLateral;         /* +0x58 */
    float dRoll;            /* +0x5C */
    DLE dle;                /* +0x60 link in its segment's list or the wall's broken list */
    int unknown68[2];
};
typedef char assert_HACKWALLPIECE_size[sizeof(HACKWALLPIECE) == 0x70 ? 1 : -1];
typedef char assert_HACKWALLPIECE_dle[SLY2_OFFSETOF(HACKWALLPIECE, dle) == 0x60 ? 1 : -1];

/* HACKWALLSEG (0x30): a wall segment (AddHackwallSegment): its pieces spread along a shape. */
struct HACKWALLSEG
{
    OID oid;                /* +0x0 the shape's oid */
    int cpiece;             /* +0x4 pieces allocated */
    int cpieceMax;          /* +0x8 pieces laid out side by side at most */
    int cpieceActive;       /* +0xC pieces not broken */
    DL dlPiece;             /* +0x10 */
    int cPieceHealth;       /* +0x1C each piece's health (default 30) */
    float sRadius;          /* +0x20 piece radius (contacts, bounds, spacing) */
    SHAPE *pshape;          /* +0x24 */
    float sClosest;         /* +0x28 point of the shape closest to the ship */
    float tRetarget;        /* +0x2C when to search for it again */
};
typedef char assert_HACKWALLSEG_size[sizeof(HACKWALLSEG) == 0x30 ? 1 : -1];

/* 76 HACKWALL (0x2A0); vtable 0x2D0B30, 83 slots */
struct HACKWALL : HACK
{
    int ploTarget;          /* +0x1C0 UpdateHackwall moves the pieces only when set (inferred) */
    LO *ploPiece;           /* +0x1C4 model drawn for every piece */
    int cseg;               /* +0x1C8 */
    HACKWALLSEG aseg[4];    /* +0x1CC */
    DL dlBroken;            /* +0x28C broken pieces */
    int unknown298[2];
    virtual void Init() __asm__("InitHackwall");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetHackwallClassDesc");   /* slot 6 */
    virtual void PostLoad() __asm__("PostHackwallLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateHackwall");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderHackwallSelf");   /* slot 19 */
    virtual void UpdateBounds() __asm__("UpdateHackwallTreeBounds");   /* slot 49 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustHackwallNewXp");   /* slot 55 */
    virtual void AddCustomXps(SOPART *ppart, int n2, int n3, void *pvGeom, XP **ppxpFirst) __asm__("AddHackwallXps");   /* slot 60 */
    virtual int GetExtraPartCount() __asm__("GetHackwallExtraPartCount");   /* slot 63 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeHackwallDamage");   /* slot 75 */
    virtual void Kill(LO *ploKiller) __asm__("KillHackwall");   /* slot 82 */
    virtual void Reset() __asm__("ResetHackwall");   /* slot 83 */
};
typedef char assert_HACKWALL_size[sizeof(HACKWALL) == 0x2A0 ? 1 : -1];
typedef char assert_HACKWALL_aseg[SLY2_OFFSETOF(HACKWALL, aseg) == 0x1CC ? 1 : -1];
typedef char assert_HACKWALL_dlBroken[SLY2_OFFSETOF(HACKWALL, dlBroken) == 0x28C ? 1 : -1];

#endif /* SLY2_CLASSES_H */
#endif /* SLY2_HACK_H */
