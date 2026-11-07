/* include/sly2/rip.h -- rips: the particles a RIPG (particle group) owns and updates, one class per particle kind
 * (RIPT: bublets, flying debris, ripples, glints, matches, shrapnel...). engine/fx/rip.cpp defines their members
 * and is their key TU (it emits their vtables); engine/fx/emitter.cpp emits them (EmitRips). */
#ifndef SLY2_RIP_H
#define SLY2_RIP_H
#include "sly2/basic.h"
struct RIPG; struct EMITB; struct EXPLO;

/* A subscription node (pool D_003E1144). */
struct RIPSUB
{
    LO *plo;
    int pad4;
    RIPSUB *pnext;          /* +0x08 */
};
/* a rotation as three quadword rows (copied whole) */
struct RIPM3 { qword r[3]; };
/* RIP: share count, owning group, start time and the per-kind fields used by the m2c bodies; the vtable pointer
   follows the fields (+0xEC). Sly 2's RIP kinds (RIPT, PvtripFromRipt) are real classes with the eight slots of Sly 1's
   VTRIP table; engine/fx/rip.cpp is their key TU (docs/systems/rendering.md section 8). */
struct RIP
{
    int kind;
    int refCount;               /* +0x04 */
    RIPG *ripGroup;             /* +0x08 the group it lives in (on its ripList) */
    char padC[0x14 - 0xC];
    float startTime;           /* +0x14 game time the rip starts (it is skipped until then) */
    float duration;            /* +0x18 */
    LO *renderObject;          /* +0x1C */
    union
    {
        qword scale;           /* +0x20 the scale curve as one quadword */
        CLQ clqScale;          /* +0x20 the same as a quadratic in the rip's age fraction (CreateStuck evaluates it) */
        struct
        {
            float scaleStart;  /* +0x20 its coefficients: the initial scale, */
            float scaleSlope;  /* +0x24 slope */
            float scaleCurve;  /* +0x28 and curvature (EmitRips: the emitter's, when set) */
        };
    };
    qword alpha;            /* +0x30 */
    RIPM3 orientation;      /* +0x40 */
    qword currentPosition;           /* +0x70 */
    qword previousPosition;          /* +0x80 */
    qword velocity;            /* +0x90 */
    qword acceleration;          /* +0xA0 */
    char padB0[0xD0 - 0xB0];
    float angularVelocity[3];  /* +0xD0 spin (SHRAPNEL: random, from the impact speed) */
    char padDC[0xE0 - 0xDC];
    LO *touchObject;           /* +0xE0 */
    char padE4[4];
    RIPSUB *firstSubscription;      /* +0xE8 */
    virtual void Init(qword pos, float scale, SO *psoTouch) __asm__("InitRip");   /* slot 1; key function (parameter order from Sly 1's InitMatch symbol) */
    virtual void PostEmit(EMITB *pemitb) __asm__("PostRipEmit");   /* slot 2 */
    virtual void OnRemove() __asm__("OnRipRemove");   /* slot 3 */
    virtual void Project() __asm__("ProjectRipTransform");   /* slot 4 */
    virtual void Update() __asm__("UpdateRip");   /* slot 5: no time step (UpdateRipg passes none; no override reads one) */
    virtual void Render() __asm__("RenderRip");   /* slot 6: no camera (RenderRipgSelf passes none; RenderRip takes only the rip) */
    virtual void Touch(int fRemove) __asm__("TouchRip__FP3RIPi");   /* slot 7 */
    virtual int HandleBounce(void *pa, qword q0, qword q1) __asm__("HandleRipBounce");   /* slot 8 */
};
typedef char assert_RIP_size[sizeof(RIP) == 0xF0 ? 1 : -1];
typedef char assert_RIP_renderObject[SLY2_OFFSETOF(RIP, renderObject) == 0x1C ? 1 : -1];
typedef char assert_RIP_orientation[SLY2_OFFSETOF(RIP, orientation) == 0x40 ? 1 : -1];
typedef char assert_RIP_velocity[SLY2_OFFSETOF(RIP, velocity) == 0x90 ? 1 : -1];
typedef char assert_RIP_touchObject[SLY2_OFFSETOF(RIP, touchObject) == 0xE0 ? 1 : -1];
/* The kinds, in the reverse of their emission order (vtables 0x2D7738 .. 0x2D7468; RIP's 0x2D7788 last). DRAFT
   names: FLYING, STUCK, GLINT, SMACK, LEAF, REMIT after their functions, RIPT0/RIPT1 after their kind numbers; MATCH and
   SHRAPNEL are the Sly 1 symbols' class names. Fields: only the two words a GLINT/RIPT1 body reads (RIPF). */
struct RIPF : RIP
{
    LO *followObject;          /* +0xF0 (GLINT) */
    int nextRenderObject;       /* +0xF4 (RIPT1): the EMITB's next-render object, what the rip leaves on touching */
};
struct FLYING : RIP   /* RIPT 3 */
{
    LO *ploStuckRender;        /* +0xF0 what it becomes when it sticks (the EMITB's next-render object) */
    virtual int HandleBounce(void *pa, qword q0, qword q1) __asm__("HandleFlyingBounce");   /* key function */
    virtual void PostEmit(EMITB *pemitb) __asm__("PostFlyingEmit");
    virtual void Render() __asm__("RenderFlying");
};
struct STUCK : RIP   /* RIPT 10 */
{
    LO *ploHost;               /* +0xF0 the object it sticks to: its position and orientation are in that object's space
                                  (CreateStuck, RenderStuck) */
    virtual void Update() __asm__("UpdateStuck");   /* key function */
    virtual void Render() __asm__("RenderStuck");
};
typedef char assert_STUCK_size[sizeof(STUCK) == 0x100 ? 1 : -1];   /* PvtripFromRipt clears 0x100 bytes */
struct SHRAPNEL : RIP   /* RIPT 8 */
{
    VECTOR dir;                /* +0xF0 the emitter record's direction (PostShrapnelEmit) */
    virtual int HandleBounce(void *pa, qword q0, qword q1) __asm__("HandleShrapnelBounce");   /* key function */
    virtual void PostEmit(EMITB *pemitb) __asm__("PostShrapnelEmit__FP8SHRAPNELP5EMITB");
    virtual void Render() __asm__("RenderShrapnel__FP8SHRAPNELP2CM");
};
struct MATCH : RIP   /* RIPT 6 */
{
    char padF0[0x104 - 0xF0];
    int shouldLaunch;          /* +0x104 launch once its start time has come */
    virtual void Update() __asm__("UpdateMatch");   /* key function */
    virtual void Init(qword pos, float scale, SO *psoTouch) __asm__("InitMatch__FP5MATCHP6VECTORfP2SO");
    virtual void Project() __asm__("ProjectMatchTransform");
    virtual void Render() __asm__("RenderMatch");
};
struct GLINT : RIP   /* RIPT 4 */
{
    virtual void Init(qword pos, float scale, SO *psoTouch) __asm__("InitGlint");   /* key function */
    virtual void Project() __asm__("ProjectGlint");
    virtual void Render() __asm__("RenderGlint");
};
struct SMACK : RIP   /* RIPT 9 */
{
    virtual void Update() __asm__("UpdateSmack");   /* key function */
    virtual void Init(qword pos, float scale, SO *psoTouch) __asm__("InitSmack");
    virtual void Project() __asm__("ProjectSmackTransform");
    virtual void Render() __asm__("RenderSmack");
};
struct RIPT1 : RIP   /* RIPT 1 */
{
    virtual void Init(qword pos, float scale, SO *psoTouch) __asm__("InitRipt1");   /* key function */
    virtual void PostEmit(EMITB *pemitb) __asm__("PostRipt1Emit");
    virtual void Touch(int fRemove) __asm__("UpdateRipt1");
};
struct RIPT0 : RIP   /* RIPT 0 */
{
    virtual void Touch(int fRemove) __asm__("TouchBublet");   /* key function */
    virtual void Init(qword pos, float scale, SO *psoTouch) __asm__("InitBublet");
    virtual void Project() __asm__("ProjectBubletTransform");
};
struct LEAF : RIP   /* RIPT 5 */
{
    virtual void PostEmit(EMITB *pemitb) __asm__("PostLeafEmit");   /* key function */
    virtual void Project() __asm__("ProjectLeafTransform");
    virtual int HandleBounce(void *pa, qword q0, qword q1) __asm__("StickLeafOnBounce");
};
struct REMIT : RIP   /* RIPT 2 */
{
    EXPLO *pexpls;             /* +0xF0 the explosion it re-emits (an EXPLS) */
    float rate;                /* +0xF4 explosions per second */
    float accumulated;         /* +0xF8 explosions owed (fractional) */
    virtual void Update() __asm__("UpdateRemit");   /* key function */
    virtual void PostEmit(EMITB *pemitb) __asm__("PostRemitEmit");
};
#endif
