/* include/sly2/tank.h -- the robot tank (RC car) and its ammo pickup (gameplay/objects/robottank.cpp, their key TU):
 * ROBOTTANK : PO with its wheel, side and ammo records, and AMMOPICKUP (the ammo pickup).
 *
 * Registry classes (sly2/classes.h conventions: real virtual layouts, each slot's function named by an asm label,
 * sizes from the class descriptors). They are defined at their emission position inside classes.h, which includes
 * this file there; a TU that includes this header first gets classes.h (and these classes) in the right order.
 *
 * Field names come from how robottank's functions use them; "(inferred)" marks names guessed from that use.
 * The weapons are JSG objects (0xB0; class_registry's _vt$3JSG, CloneRobottank constructs them); their fields are
 * still read through per-function views. Layouts: names/vu-objects. */
#ifndef SLY2_TANK_H
#ifndef SLY2_CLASSES_H
#include "sly2/classes.h"   /* includes this header again at the classes' emission position */
#else
#define SLY2_TANK_H

/* ROBOTTANKWHEEL (0x60): a wheel (AddRobottankWheel): its transform, its contacts and suspension. */
struct ROBOTTANKWHEEL
{
    OID oid;                /* +0x0 */
    XF *pxf;                /* +0x4 */
    XP *pxpFirst;           /* +0x8 its contacts */
    float steering;         /* +0xC */
    int unknown10;
    float tilt;             /* +0x14 */
    float speed;            /* +0x18 */
    float angle;            /* +0x1C spin angle */
    float sRadius;          /* +0x20 */
    LM lmTravel;            /* +0x24 suspension travel (inferred) */
    int unknown2C;
    qword pos;              /* +0x30 */
    qword posTop;           /* +0x40 */
    qword posBottom;        /* +0x50 */
};
typedef char assert_ROBOTTANKWHEEL_size[sizeof(ROBOTTANKWHEEL) == 0x60 ? 1 : -1];

/* ROBOTTANKSIDE (0x14): an attachment of the body (AddRobottankAttachment) turned within limits. */
struct ROBOTTANKSIDE
{
    OID oid;                /* +0x0 */
    XF *pxf;                /* +0x4 */
    void *pvBinding;        /* +0x8 */
    LM lmAngle;             /* +0xC */
};
typedef char assert_ROBOTTANKSIDE_size[sizeof(ROBOTTANKSIDE) == 0x14 ? 1 : -1];

/* ROBOTTANKAMMO (0xC): an ammo slot (SetRobottankAmmo, CollectRobottankPickup). */
struct ROBOTTANKAMMO
{
    int cAmmo;              /* +0x0 */
    int cAmmoMax;           /* +0x4 */
    float gPickupBias;      /* +0x8 added to a pickup's distance when the AI picks the nearest (FindNearestRobottankPickup) */
};
typedef char assert_ROBOTTANKAMMO_size[sizeof(ROBOTTANKAMMO) == 0xC ? 1 : -1];

/* 77 ROBOTTANK (0x5D0): single instance; vtable 0x2D7CA8, 81 slots */
struct ROBOTTANK : PO
{
    int state;              /* +0x180 */
    float stateTime;        /* +0x184 */
    int tactic;             /* +0x188 AI tactic (SetRobottankTactic) */
    float tacticTime;       /* +0x18C */
    int cwheel;             /* +0x190 */
    int unknown194[3];
    ROBOTTANKWHEEL awheel[8];   /* +0x1A0 */
    float sWidth;           /* +0x4A0 */
    int cside;              /* +0x4A4 */
    ROBOTTANKSIDE aside[4]; /* +0x4A8 */
    int cjsgWeapon;         /* +0x4F8 */
    struct JSG *apjsgWeapon[8]; /* +0x4FC */
    float driveMax;         /* +0x51C drive at full stick; the traction terms scale with it */
    CLQ clqTraction;        /* +0x520 */
    LM lmSpeed;             /* +0x530 */
    float sReverseMax;      /* +0x538 */
    float gContactScale;    /* +0x53C */
    float heading;          /* +0x540 */
    float goalHeading;      /* +0x544 */
    float steering;         /* +0x548 */
    float wheelAngle;       /* +0x54C */
    float drive;            /* +0x550 */
    float turn;             /* +0x554 */
    float acceleration;     /* +0x558 */
    int cHealthMax;         /* +0x55C */
    int cHealth;            /* +0x560 */
    int fHealthMeterInverted;   /* +0x564 */
    WIDGET *pwidgetHealth;  /* +0x568 */
    void *pvAsset;          /* +0x56C */
    SMA *psma;              /* +0x570 body animation machine */
    ROBOTTANKAMMO aammo[3]; /* +0x574 */
    CID151 *psndEngine;     /* +0x598 counted (RcPtr_Assign); the constructor clears it */
    int sfxidEngine;        /* +0x59C */
    float uRateEngine;      /* +0x5A0 */
    SO *psoTarget;          /* +0x5A4 */
    void *pvPathzone;       /* +0x5A8 */
    SHAPE *pshapePath;      /* +0x5AC AI path */
    float sPath;            /* +0x5B0 cursor along it */
    float tNextAttack;      /* +0x5B4 */
    SO *psoSelected;        /* +0x5B8 last pickup chosen */
    struct JSG *pjsgSelected;   /* +0x5BC weapon chosen */
    float lastCorrection;   /* +0x5C0 */
    int unknown5C4[3];
    ROBOTTANK();
    virtual void OnActive(int fActive) __asm__("OnRobottankActive");   /* slot 78 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetRobottankClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRobottank");   /* slot 7 */
    virtual void PostLoad() __asm__("PostRobottankLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateRobottank");   /* slot 16 */
    virtual void RenderSelf() __asm__("RenderRobottankSelf");   /* slot 19 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetRobottank");   /* slot 26 */
    virtual void Clone(LO *ploBase) __asm__("CloneRobottank");   /* slot 41 */
    virtual void UpdateBounds() __asm__("UpdateRobottankBounds");   /* slot 49 */
    virtual void OnContactFreed(SO *pso) __asm__("RemoveRobottankContactsWith");   /* slot 54 */
    virtual void AdjustNewXp(XP *pxp, int ixpd) __asm__("AdjustRobottankNewXp");   /* slot 55 */
    virtual void AddCustomXps(SOPART *ppart, int cbsp, BSP *absp, BSP *pbsp, XP **ppxpFirst) __asm__("AddRobottankCustomXps");   /* slot 60 */
    virtual void PresetAccel() __asm__("PresetRobottankAccel");   /* slot 70 */
    virtual int TakeDamage(ZPR *pzpr) __asm__("TakeRobottankDamage");   /* slot 75 */
};
typedef char assert_ROBOTTANK_size[sizeof(ROBOTTANK) == 0x5D0 ? 1 : -1];
typedef char assert_ROBOTTANK_awheel[SLY2_OFFSETOF(ROBOTTANK, awheel) == 0x1A0 ? 1 : -1];
typedef char assert_ROBOTTANK_cjsgWeapon[SLY2_OFFSETOF(ROBOTTANK, cjsgWeapon) == 0x4F8 ? 1 : -1];
typedef char assert_ROBOTTANK_clqTraction[SLY2_OFFSETOF(ROBOTTANK, clqTraction) == 0x520 ? 1 : -1];
typedef char assert_ROBOTTANK_cHealthMax[SLY2_OFFSETOF(ROBOTTANK, cHealthMax) == 0x55C ? 1 : -1];
typedef char assert_ROBOTTANK_aammo[SLY2_OFFSETOF(ROBOTTANK, aammo) == 0x574 ? 1 : -1];
typedef char assert_ROBOTTANK_psndEngine[SLY2_OFFSETOF(ROBOTTANK, psndEngine) == 0x598 ? 1 : -1];
typedef char assert_ROBOTTANK_lastCorrection[SLY2_OFFSETOF(ROBOTTANK, lastCorrection) == 0x5C0 ? 1 : -1];
/* The constructor class_registry inlines into its factory. g++ 2.95 emits a class's inline member functions in
   its key TU (as global functions the ELF does not have), so the key TU defines SLY2_KEY_TU_ROBOTTANK and sees
   only the declaration. */
#ifndef SLY2_KEY_TU_ROBOTTANK
// ROBOTTANK constructor: clears the engine sound pointer.
inline ROBOTTANK::ROBOTTANK() { psndEngine = 0; }
#endif

/* 108 AMMOPICKUP (was CID108; 0x170): robot-tank ammo pickup; vtable 0x2D7A40, 75 slots */
struct AMMOPICKUP : SO
{
    int state;              /* +0x150 -1 off, 0 waiting, 1 available, 2 collected (SetRcPickupState) (inferred) */
    float stateTime;        /* +0x154 */
    int iammo;              /* +0x158 the ammo slot it refills (FindNearestRobottankPickup, CollectRobottankPickup) */
    int cAmmo;              /* +0x15C ammo it gives (default 30) */
    SM *psm;                /* +0x160 */
    SMA *psma;              /* +0x164 */
    int unknown168[2];
    virtual void Update() __asm__("UpdateRcPickup");   /* slot 16 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid108ClassDesc");   /* slot 6 */
    virtual void Init() __asm__("InitRcPickup");   /* slot 7 */
    virtual void PostLoad() __asm__("PostRcPickupLoad");   /* slot 15 */
    virtual void OnProxyAcquire(LO *plo) __asm__("ResetRobottankPickup");   /* slot 26 */
};
typedef char assert_CID108_size[sizeof(AMMOPICKUP) == 0x170 ? 1 : -1];
typedef char assert_CID108_iammo[SLY2_OFFSETOF(AMMOPICKUP, iammo) == 0x158 ? 1 : -1];

#endif /* SLY2_CLASSES_H */
#endif /* SLY2_TANK_H */
