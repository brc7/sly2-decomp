/* include/sly2/leap.h -- LEAP and CID146, the BASIC-derived helper objects that STEP subclasses embed (MURRAY +0x1890,
 * STEPGUARD +0xDB0 / +0xE20), from app/class_registry.cpp's construction views with the real virtual layout from
 * the ELF vtables (types/views-2). Its own header, included by guard.h and players.h only: player/gait.cpp
 * (which includes stepplayer.h) still has its own field-level LEAP view. */
#ifndef SLY2_LEAP_H
#define SLY2_LEAP_H
#include "sly2/basic.h"

/* LEAP (cid 145): the leap helper object STEP subclasses embed (MURRAY +0x1890, STEPGUARD +0xDB0). From
   app/class_registry.cpp's construction view; real virtual layout from the ELF vtable.
   145 LEAP (0x70); vtable 0x2DAA10, 6 slots */
struct LEAP : BASIC
{
    int state;              /* +0x10 (gait.cpp's LEAPV) */
    float stateTime;        /* +0x14 */
    undefined4 unknown18[2];
    qword previousVelocity; /* +0x20 */
    struct ASEGARC *tuning; /* +0x30 the animation arc (stepplayer.h ASEGARC) */
    SO *subject;            /* +0x34 the leaping object */
    ASEGA *pasegaCur;       /* +0x38 [S] the animation the leap plays (stepguard GetAnimBindingInstance) */
    int index;              /* +0x3C */
    qword posGoal;          /* +0x40 the goal position (murray.cpp; gait.cpp's LEAPV calls it target) */
    float tArrival;         /* +0x50 */
    float tEnd;             /* +0x54 */
    LO *target;             /* +0x58 the object leapt onto (StartBhvleap) (inferred; the snapshots mostly show 0 here) */
    int isReversed;         /* +0x5C (StartBhvleap) (inferred; gait.cpp: kind) */
    int extra;              /* +0x60 (StartBhvleap) (inferred; gait.cpp: ballistic) */
    int steep;              /* +0x64 (gait.cpp) */
    struct ACT *constraint; /* +0x68 (gait.cpp) */
    undefined4 unknown6C;
    LEAP() __asm__("ConstructLeap");              /* out of line: player/gait.cpp */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleLeapMessage");   /* slot 5; key function (gait) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetLeapClassDesc");   /* slot 6 */
};
typedef char assert_LEAP_size[sizeof(LEAP) == 0x70 ? 1 : -1]; typedef char assert_LEAP_target[SLY2_OFFSETOF(LEAP, target) == 0x58 ? 1 : -1];
/* CID146 (cid 146, DIRANIM): embedded in STEPGUARD at +0xE20. From app/class_registry.cpp.
   146 CID146 (0x40): DIRANIM; vtable 0x2DA9D0, 6 slots */
struct CID146 : BASIC
{
    char pad10[0x24 - 0x10];
    ASEGA *pasegaCur;       /* +0x24 [S] the animation the directional animation plays (stepguard GetAnimBindingInstance) */
    char pad28[0x40 - 0x28];
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleCid146Message");   /* slot 5 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid146ClassDesc");   /* slot 6 */
};
typedef char assert_CID146_size[sizeof(CID146) == 0x40 ? 1 : -1];

#endif /* SLY2_LEAP_H */
