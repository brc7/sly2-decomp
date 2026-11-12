// throw.cpp -- JTTHROW, the player carrying and throwing an object: init/load, aiming (the hold point follows the
// stick within a range around the player), the predicted arc and its marker, the throw states with their animations,
// and the launch. VU0 and EE asm, register-pinned. TU 0x21F2E0..0x220D40 (12 functions, address order).
// Entry points: InitThrow/PostJtThrowLoad (setup), UpdateJtThrow (per frame), SetJtThrowState, ReleaseJtThrowObject.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_throw
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_throw
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/cm.h"
#include "sly2/aseg.h"
#include "sly2/act.h"
#include "player_input.h"
#include "sly2/util.h"
#include "sly2/players.h"   /* STEPPLAYER (JTTHROW::player), BENTLEY (D_002DD5BC) */
#include "sly2/clock.h"
#include "sly2/sw.h"
#include "sly2/classes.h"   /* CID100 (the carried tossable) */
// 0x220BE8 StaticInit_throw and GlobalConstructors_throw: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

union V {qword q;float f[4];int n[4];};
struct M3 {V r[3];};
struct M4 {M3 basis;V position;};
/* The real classes behind the views below, for their virtual calls (the carried object is a tossable, CID100). */
static inline ::LO *AsLo(void *plo) { return (::LO *)plo; }
static inline ::CID100 *AsTossable(void *p) { return (::CID100 *)p; }
/* the clip channel FindAsegChannel returns: aseg.h CHN, its curve act.h AC */
/* Any BASIC as the register-pinned launch helper calls it: the g++ vtable pointer at +0xC. */
struct VOBJ {char pad[0xC];VTE *vt;};
/* A tossable (classes.h CID100, no fields there yet) carried as JTTHROW::ploCarried: its slot-76 virtual (not declared
   in classes.h) and two words. */
/* A frame of the aim marker's arc: the time it was written, 1 when the point exists, a length (0) and the matrix. */
struct ARCFRAME {float t,gValid,s;M4 mat;};
/* The aim marker (EFFECT in the state code; Lilly's ploMarker): vtable, active flag, its ring of arc frames. */
struct EFFECT {char pad[0xC];void *vptr;char pad10[0x144];int active,cframe;ARCFRAME *aframe;int reset;};
union SCRATCH {V vector;LM range;};
struct WORK {SCRATCH scratch;V delta;float angle,radius;V ownerPosition,speed,vector,playerPosition;};
/* The line-test filter's argument. */
struct SOPAIR { int a; void *p; };
   /* sly2/basic.h's CLQ has a constructor (no brace initializer) */
struct HIT {V pos;char unk10[0x50];};

extern BENTLEY *D_002DD5BC;
/* throw's .data (address order; D_002C4108/D_002C410C are defined below, before their function). */
float D_002C40E0 = 1.5f;                        // aim range scale with power-up 0x24
extern V g_normalZ,D_002A2DA0,D_002A2DD0;
float D_002C40E4 = 1.35f;                       // tPredict scale with power-up 0x24
extern CLQ D_002C40F0;                          // the launch elevation over the aim distance: a 16-byte CLQV
namespace THROW_DATA { float r_D_002C40F0[4] __asm__("D_002C40F0") __attribute__((aligned(16))) = {0.15f, 0.55f, 0.0f, 0.0f}; }
float D_002C4100 = 3.0f, D_002C4104 = 2.0f;     // flight time / blend rate limits per second
extern const float D_002A96A0;   /* defined below where the ELF has it (.rodata order) */
extern "C" {
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(STEPPLAYER *a, int b, int c, void *d, int e, int *f, int *g, void *h);
qword GetLoWorldPos(void *);
int IsPowerupAcquired(int);
qword SetVectorCylind(float,float,float);
void GetVectorCylind(qword,float *,float *,float *);
float ClampToLimits__FP2LMf(LM *,float);
void UpdateJtThrowArc(JTTHROW *,int);
float atan2f(float,float);
float GetStepHeading(STEPPLAYER *);
float NormalizeAngle__Ff(float);
void SetJtThrowState(JTTHROW *,int);
void RecalculateAsegBlend(ASEG *);
int IsAsegaRetracted(ASEGA *);
int IsBasicDerivedFrom__FP5BASIC3CID(void *,int);
void SetBentleyHoverState(BENTLEY *,int);
CHN *FindAsegChannel(ASEG *,STEPPLAYER *,void *,int);
int ShouldKeepSoPair(void *a, SOPAIR *b);
qword ConvertPosBetweenLos(void *,void *,qword);
qword CalculateBallisticLaunchVelocity(qword,qword,qword,float); // CalculateLaunchVelocity
void IntersectSwBoundingBox(SW *,qword,qword,int (*)(void *,SOPAIR *),void *,int *,SO ***); // FindSwObjectsInBox
int FilterJtThrowArcObject(void *,SOPAIR *);
SO *FindFirstObjectHitByLine(int,qword,qword,int,SO **,HIT *);
int SolveQuadratic__FfffPf(float,float,float,float *);
M3 *BuildOrthonormalMatrixZ(M3 *,qword,qword);
V *VECTOR_opAssign(V *,qword);
void SetTrailMeshPositions(EFFECT *,qword);
int MaxInt(int, int);
void AttachJtHandsObject(JTHANDS *,LO *);
void ReleaseJtHandsActs(JTHANDS *);
void RefreshSwObjectCollisionFilter(SW *,STEPPLAYER *);
extern "C++" { void SetStepTargetHeading(STEPPLAYER *,float) __asm__("SetStepTargetHeading"); }
void SetStepTargetHeadingFromVector(STEPPLAYER *,qword);
void SetLoParent(LO *,int,int);
void FadeSoOut(LO *,float);
void ChargeGadgetMp(STEPPLAYER *,int);
void ReleaseSma(SMA **);
void ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(ASEG *,STEPPLAYER *,float,float,int,ASEGA **);
void SetSoCnstrForce(SO *po, int a);
void SetSoCnstrTorque(SO *po, int a);
qword ConvertVecBetweenLos(void *p, int a, qword q);
}

// Set the three components of pv.
static inline void SetV(VECTOR *pv, float x, float y, float z) { pv->x = x; pv->y = y; pv->z = z; }
// Initializes the player's carry/throw controller (JTTHROW): owner, state -1, aim ranges 500/1500, timing and the
// default launch vectors.
// Also known as: InitJtThrow (upstream readable / Sly 1 name; InitJtThrow__FP5ACTORUi).
extern "C" void InitThrow(JTTHROW *thrower, STEPPLAYER *a)
{
    thrower->player = a;
    thrower->state = -1;
    thrower->lmAim.minValue = 500.0f;
    thrower->lmAim.maxValue = 1500.0f;
    thrower->tPredict = 1.5f;
    thrower->holdTurnAngle = 1.0f;
    thrower->aimTurnAngle = 1.0f;
    thrower->aimSpeed = 1000.0f;
    SetV(&thrower->wLaunchLocal, 0.0f, 0.0f, 25.0f);
    SetV(&thrower->posAimLocal, 1000.0f, 0.0f, 0.0f);
    SetV(&thrower->posLaunchLocal, 42.0f, -73.0f, -50.0f);
}

// Post-load: builds the three aim blends (2-clip, 4-clip and 2-clip).
// Also known as: PostLoadJtThrow (upstream readable / Sly 1 name; PostLoadJtThrow__FP7JTTHROW).
extern "C" void PostJtThrowLoad(JTTHROW *thrower)
{
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(thrower->player, sizeof(thrower->abl[0]), 2, thrower->abl, 0, 0, 0, &thrower->pasegbl);
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(thrower->player, sizeof(thrower->ablB[0]), 4, thrower->ablB, 0, 0, 0, &thrower->pasegblB);
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(thrower->player, sizeof(thrower->ablC[0]), 2, thrower->ablC, 0, 0, 0, &thrower->pasegblC);
}

// Update the controller target and consume state/animation transitions.
// Move a float's bits into a GPR qword ($2).
static inline qword CPUFloat(float f){register qword r asm("$2");__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;}
// Store value to home and return the stored copy.
static inline qword ScalarHome(V &home,qword value){home.q=value;return home.q;}
// Load a vector into a VU0 register.
static inline qword LoadVF(const V &v){qword q;__asm__("lqc2 %0,%1":"=j"(q):"m"(v));return q;}
// Return a - b.
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Return a*wa + b*wb.
static inline qword Lerp(qword a,qword b,qword wa,qword wb){__asm__("vmulax.xyzw ACC,%0,%2x\n\tvmaddx.xyzw %0,%1,%3x":"+j"(a):"j"(b),"j"(wa),"j"(wb));return a;}
// Throw-update asm fragment: save the owner position to home and read the throw state (register-pinned).
static inline int SaveOwnerPosition(V &home, qword value, const JTTHROW *state)
{
    register int result asm("$3");
    asm("sq %2,%1\n\tlw %0,%3" : "=r"(result), "=m"(home.q) : "r"(value), "m"(state->state));
    return result;
}
// Throw-update asm fragment: polar coordinates (angle, radius) of the movement relative to the saved position, with
// the argument pointers for the following call.
static inline qword PolarOperands(const V &saved, qword movement, float *angle, float *radius, float *&angleArgument,
                                  float *&radiusArgument)
{
    register qword sum asm("vf1"), offset asm("vf3");
    register float *a asm("$5"), *r asm("$6");
    asm("lqc2 %0,%4\n\tla %2,%a5\n\tqmtc2.ni %6,%1\n\tla %3,%a7\n\tvadd.xyzw %0,%0,%1"
        : "=&j"(sum), "=j"(offset), "=&r"(a), "=r"(r)
        : "m"(saved), "p"(angle), "r"(movement), "p"(radius));
    angleArgument = a;
    radiusArgument = r;
    return sum;
}
// Throw-update asm fragment: target = owner + polar offset, and the delta.
static inline void StoreTarget(qword &target, V &delta, const V &owner, qword polar)
{
    register qword local asm("vf1"), origin asm("vf4"), sum asm("vf2");
    asm("qmtc2.ni %5,%0\n\tlqc2 %1,%6\n\tvadd.xyzw %2,%1,%0\n\tsqc2 %0,%3\n\tsqc2 %2,%4"
        : "=j"(local), "=j"(origin), "=j"(sum), "=m"(delta.q), "=m"(target)
        : "r"(polar), "m"(owner));
}
// Throw-update asm fragment: selected point minus the owner position.
static inline qword IdleDifference(qword selected, const V &owner)
{
    register qword point asm("vf2"), origin asm("vf3"), difference asm("vf1");
    asm("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tvsub.xyzw %2,%0,%1"
        : "=j"(point), "=j"(origin), "=j"(difference)
        : "r"(selected), "m"(owner));
    return difference;
}
// Throw-update asm fragment: target minus the owner position.
static inline qword ActiveDifference(const qword &target, const V &owner)
{
    register qword origin asm("vf2"), difference asm("vf1");
    asm("lqc2 %0,%3\n\tlqc2 %1,%2\n\tvsub.xyzw %1,%1,%0" : "=j"(origin), "=j"(difference) : "m"(target), "m"(owner));
    return difference;
}
// Throw-update asm fragment: load the current and player points and the high/saved-high pointers for the projection.
static inline void ProjectionInputs(qword current, const V &player, qword &currentVF, qword &playerVF, V *&high,
                                    V *&savedHigh)
{
    register V *upper asm("$3"), *saved asm("$4");
    register qword origin asm("vf1"), point asm("vf4");
    asm("lui %0,%%hi(%6)\n\tqmtc2.ni %4,%2\n\tlqc2 %3,%5\n\tdaddu %1,%0,$zero"
        : "=&r"(upper), "=r"(saved), "=j"(origin), "=j"(point)
        : "r"(current), "m"(player), "i"(&g_normalZ));
    currentVF = origin;
    playerVF = point;
    high = upper;
    savedHigh = saved;
}
// Address of the upper vector, pinned to $3.
static inline V *InitialUpAddress(V *upper)
{
    register V *address asm("$3");
    asm("addiu %0,%1,%%lo(%2)" : "=r"(address) : "r"(upper), "i"(&g_normalZ));
    return address;
}
// player - current (register-pinned).
static inline qword PointDifference(qword player, qword current)
{
    register qword point asm("vf4") = player, origin asm("vf1") = current;
    asm("vsub.xyzw %0,%0,%1" : "+j"(point) : "j"(origin));
    return point;
}
// Move the player point into vf4.
static inline qword PlayerVF(qword player){register qword value asm("vf4");asm("qmtc2.ni %1,%0":"=j"(value):"r"(player));return value;}
// x of the launch length as a float (through $3).
static inline float LaunchLengthX(qword length)
{
    register int bits asm("$3");
    register float x asm("$f0");
    asm("qmfc2.ni %0,%2\n\tmtc1 %0,%1" : "=r"(bits), "=f"(x) : "j"(length));
    return x;
}
// Launch velocity: direction*speed + up*height.
static inline qword LaunchVelocity(qword direction, qword speed, qword up, qword height)
{
    register qword result asm("vf1") = up, d asm("vf3") = direction, s asm("vf4") = speed, h asm("vf2") = height;
    asm("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%0,%3x" : "+j"(result) : "j"(d), "j"(s), "j"(h));
    return result;
}
// Load the launch velocity from home and pass it to the owner's vector virtual.
static inline void LaunchVectorCall(BENTLEY *owner, const V &home)
{
    qword velocity = LoadVF(home);
    VTE *table = ((VOBJ *)owner)->vt;
    register int delta asm("$4");
    register qword argument asm("$5");
    asm("lh %0,%2\n\tqmfc2.ni %1,%3" : "=&r"(delta), "=r"(argument) : "m"(table[35].d), "j"(velocity));
    ((void (*)(void *,qword))table[35].pfn)((char *)owner + delta, argument);
}
// Unit vector of input with its component along up removed.
static inline qword PlanarUnit(qword input, qword up)
{
    register qword point asm("vf4") = input, normal asm("vf1") = up, projection asm("vf2"), direction asm("vf3");
    register const V *address asm("$2");
    register float epsilon asm("$f1");
    asm("lui %3,%%hi(%6)\n\tvmul.xyz %0,%4,%5\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x "
        "%0,%1,%0z\n\tli.s %2,0.0001\n\tvmulx.xyzw %0,%5,%0x\n\taddiu %3,%3,%%lo(%6)"
        : "=&j"(projection), "=&j"(direction), "=f"(epsilon), "=&r"(address)
        : "j"(point), "j"(normal), "i"(&D_002A2DA0)
        : "$1");
    asm("vsub.xyzw %0,%1,%2" : "=j"(direction) : "j"(point), "j"(projection));
    register qword fallback asm("$2");
    asm("lq %0,0(%1)" : "=r"(fallback) : "r"(address), "m"(*address));
    register qword length asm("vf1"), one asm("vf2");
    asm("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvsqrt "
        "Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q"
        : "=&j"(length), "=&j"(one)
        : "j"(direction));
    if (LaunchLengthX(length) < epsilon)
        direction = fallback;
    else
        asm("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%0,Q" : "+j"(direction) : "j"(length));
    return direction;
}
// Lift term from the upper vector: loads the speed and up values for the launch.
static inline qword LiftFromUpper(float height, const V &speed, V *upper, qword &speedValue, qword &upValue)
{
    register V *address asm("$2");
    register qword normal asm("vf1"), scalar asm("vf2"), velocity asm("vf4");
    register int bits asm("$5");
    asm("addiu %0,%5,%%lo(%9)\n\tlqc2 %1,0(%0)\n\tmfc1 %2,%6\n\tqmtc2.ni %2,%3\n\tlqc2 %4,%7"
        : "=&r"(address), "=j"(normal), "=&r"(bits), "=j"(scalar), "=j"(velocity)
        : "r"(upper), "f"(height), "m"(speed), "m"(*(const V *)((const char *)upper + 0x2DC0)), "i"(&g_normalZ));
    upValue = normal;
    speedValue = velocity;
    return scalar;
}
// Load the saved player point and the throw's owner.
static inline qword SavedPlayerAndOwner(qword player, const JTTHROW *state, STEPPLAYER *&owner)
{
    register qword point asm("vf4");
    register STEPPLAYER *receiver asm("$4");
    asm("qmtc2.ni %2,%0\n\tlw %1,%3" : "=j"(point), "=r"(receiver) : "r"(player), "m"(state->player));
    owner = receiver;
    return point;
}
// Blend from begin to end by numerator/duration, storing the scalar to home.
static inline qword TransitionBlend(const qword &begin, const qword &end, float numerator, float duration, V &home,
                                    qword &toward)
{
    register qword from asm("vf2"), to asm("vf4"), wa asm("vf1"), wb asm("vf3");
    register float alpha asm("$f0") = numerator, inverse asm("$f1") = 1.0f, total asm("$f2") = duration;
    asm("lqc2 %0,%4\n\tlqc2 %1,%5\n\tdiv.s %2,%2,%6\n\tsub.s %3,%3,%2"
        : "=&j"(from), "=&j"(to), "+&f"(alpha), "+&f"(inverse)
        : "m"(begin), "m"(end), "f"(total));
    register int alphaBits asm("$2"), inverseBits asm("$3");
    asm("mfc1 %0,%2\n\tqmtc2.ni %0,%1" : "=&r"(alphaBits), "=j"(wb) : "f"(alpha));
    asm("mfc1 %2,%4\n\tqmtc2.ni %2,%1\n\tsqc2 %1,%3\n\tvmulax.xyzw ACC,%0,%1x\n\tvmaddx.xyzw %0,%5,%6x"
        : "+j"(from), "=&j"(wa), "=&r"(inverseBits), "=m"(home.q)
        : "f"(inverse), "j"(to), "j"(wb));
    toward = wb;
    return from;
}
// Planar snapshot: store value to home and read the clock dt.
static inline float PlanarSnapshot(qword value, V &home, const float &clock)
{
    register float dt asm("$f0");
    asm("lwc1 %0,%3\n\tsqc2 %4,%1\n\tsw $zero,%2" : "=f"(dt), "=m"(home.q), "=m"(home.f[2]) : "m"(clock), "j"(value));
    return dt;
}
// Publish the launch: store value to home and fetch the owner and flag for the following call.
static inline void LaunchPublication(qword value, V &home, BENTLEY *&owner, int &flag)
{
    register BENTLEY *receiver asm("$4");
    register int mode asm("$5");
    asm volatile("lw %0,%3\n\taddiu %1,$zero,1\n\tsqc2 %4,%2"
                 : "=r"(receiver), "=r"(mode), "=m"(home.q)
                 : "m"(D_002DD5BC), "j"(value));
    owner = receiver;
    flag = mode;
}
typedef int RangeBits __attribute__((mode(DI)));
// Copy a range record through a GPR.
#ifdef SLY2_EE_ASM
static inline void CopyRange(LM &destination, const LM &source)
{
    register RangeBits value asm("$5");
    asm("ldl %0,7+%2\n\tldr %0,%2\n\tsdl %0,7+%1\n\tsdr %0,%1" : "=&r"(value), "=m"(destination) : "m"(source));
}
#else
static inline void CopyRange(LM &destination, const LM &source) { destination = source; }
#endif
// Per-frame carry/aim controller: target selection, aim angles (limited), the predicted arc (UpdateJtThrowArc) and
// state transitions (SetJtThrowState).
// Also known as: UpdateJtThrow (upstream readable / Sly 1 name; UpdateJtThrow__FP7JTTHROW).
extern "C" void UpdateJtThrow(JTTHROW *s)
{
    WORK w;
    register POCTL *control asm("$19") = s->player->controller;
    qword initial = GetLoWorldPos(s->player);
    int initialState = SaveOwnerPosition(w.ownerPosition, initial, s);
    if (initialState < 7) {
        if (initialState >= 4) {
            if (s->isBlending) {
                float begin = s->blendStartTime, end = begin + s->blendDuration, time = g_clock.time;
                if (end <= time) {
                    s->isBlending = 0;
                    s->holdPosition = s->blendToPosition;
                } else {
                    qword toward;
                    s->holdPosition = TransitionBlend(s->blendFromPosition, s->blendToPosition, time - begin, end - begin,
                                                       w.scratch.vector, toward);
                    w.scratch.vector.q = toward;
                }
            } else {
                CopyRange(w.scratch.range, s->lmAim);
                if (IsPowerupAcquired(0x24))
                    w.scratch.range.maxValue *= D_002C40E0;
                float dt = PlanarSnapshot(Sub(s->holdPosition, w.ownerPosition.q), w.delta, g_clock.deltaTime);
                float elevation;
                float scale;
                qword delta;
                float speed;
                {
                    register float rate asm("$f13"), factor asm("$f1");
                    register qword vector asm("vf1");
                    asm("lwc1 %0,%3\n\tlwc1 %1,%4\n\tmul.s %0,%0,%5\n\tlqc2 %2,%6"
                        : "=&f"(rate), "=&f"(factor), "=j"(vector)
                        : "m"(control->leftStickDeflection), "m"(s->aimSpeed), "f"(dt), "m"(w.delta));
                    scale = factor;
                    delta = vector;
                    speed = rate;
                }
                float heading, advance;
                {
                    register float z asm("$f20"), direction asm("$f12"), step asm("$f13") = speed;
                    asm("lwc1 %0,%4\n\tlwc1 %1,%5\n\tmul.s %2,%2,%6\n\tsqc2 %7,%3"
                        : "=&f"(z), "=&f"(direction), "+f"(step), "=m"(w.vector.q)
                        : "m"(w.delta.f[2]), "m"(control->leftStickAngle), "f"(scale), "j"(delta));
                    elevation = z;
                    heading = direction;
                    advance = step;
                }
                qword movement = SetVectorCylind(heading, advance, elevation);
                float *angleArgument, *radiusArgument;
                qword sum = PolarOperands(w.vector, movement, &w.angle, &w.radius, angleArgument, radiusArgument);
                w.delta.q = sum;
                GetVectorCylind(sum, angleArgument, radiusArgument, 0);
                w.radius = ClampToLimits__FP2LMf(&w.scratch.range, w.radius);
                qword polar = SetVectorCylind(w.angle, w.radius, elevation);
                StoreTarget(s->holdPosition, w.delta, w.ownerPosition, polar);
            }
            UpdateJtThrowArc(s, 1);
        }
    }
    for (;;) {
        int previous = s->state;
        ASEGA *animation = s->player->activeAnimation;
        switch (previous) {
        case 0: {
            w.scratch.vector.q = IdleDifference(GetLoWorldPos(s->ploCarried), w.ownerPosition);
            float heading = atan2f(w.scratch.vector.f[1], w.scratch.vector.f[0]);
            float angle = NormalizeAngle__Ff(heading - GetStepHeading(s->player));
            float threshold = s->holdTurnAngle;
            if (threshold < angle)
                SetJtThrowState(s, 2);
            else if (angle < -threshold)
                SetJtThrowState(s, 1);
            else {
                s->abl[1].weight = angle / threshold * 0.5f + 0.5f;
                s->abl[0].weight = 1.0f - s->abl[1].weight;
                RecalculateAsegBlend(s->pasegbl);
            }
            break;
        }
        case 1:
        case 2:
            if (!animation || IsAsegaRetracted(animation))
                SetJtThrowState(s, 0);
            break;
        case 3:
            if (!animation || IsAsegaRetracted(animation)) {
                if (D_002DD5BC->state == 8)
                    SetJtThrowState(s, 4);
                else
                    SetJtThrowState(s, 0);
            }
            break;
        case 4:
        case 5:
        case 6: {
            if (IsBasicDerivedFrom__FP5BASIC3CID(s->ploCarried, 0x64) && ((CID100 *)s->ploCarried)->pendingContact != -1) {
                s->player->SetPlayerState(0);
                break;
            }
            float margin = 0.1f;
            if (control->ConsumeButtonsPressedWithin(BUTTON_JUMP | BUTTON_ATTACK, margin)) {
                SetJtThrowState(s, 7);
                break;
            }
            if (control->ConsumeButtonsPressedWithin(BUTTON_ALT_ATTACK | BUTTON_ACTION, margin)) {
                if (s->pasegbl)
                    SetJtThrowState(s, 0);
                else
                    s->player->SetPlayerState(0);
                LO *selected = s->ploCarried;
                if (selected == D_002DD5BC && ((BENTLEY *)selected)->state == 8) {
                    w.speed.q = ScalarHome(w.scratch.vector, CPUFloat(400.0f));
                    STEPPLAYER *positionOwner;
                    w.playerPosition.q = SavedPlayerAndOwner(GetLoWorldPos(selected), s, positionOwner);
                    qword position = GetLoWorldPos(positionOwner), currentVF, playerVF;
                    V *upper, *savedUpper;
                    ProjectionInputs(position, w.playerPosition, currentVF, playerVF, upper, savedUpper);
                    qword direction = PointDifference(playerVF, currentVF);
                    qword up = LoadVF(*InitialUpAddress(upper));
                    direction = PlanarUnit(direction, up);
                    qword speedValue, upValue;
                    qword height = LiftFromUpper(600.0f, w.speed, savedUpper, speedValue, upValue);
                    BENTLEY *launchOwner;
                    int launchFlag;
                    LaunchPublication(LaunchVelocity(direction, speedValue, upValue, height), w.vector, launchOwner,
                                      launchFlag);
                    w.scratch.vector.q = height;
                    SetBentleyHoverState(launchOwner, launchFlag);
                    LaunchVectorCall(D_002DD5BC, w.vector);
                }
                if (((EFFECT *)s->ploMarker))
                    AsLo(s->ploMarker)->Remove();
                return;
            }
            if (!animation || IsAsegaRetracted(animation))
                SetJtThrowState(s, 4);
            else if (s->state == 4) {
                w.scratch.vector.q = ActiveDifference(s->holdPosition, w.ownerPosition);
                float heading = atan2f(w.scratch.vector.f[1], w.scratch.vector.f[0]);
                float angle = NormalizeAngle__Ff(heading - GetStepHeading(s->player));
                float threshold = s->aimTurnAngle;
                if (threshold < angle)
                    SetJtThrowState(s, 6);
                else if (angle < -threshold)
                    SetJtThrowState(s, 5);
            }
            break;
        }
        case 7:
            if (!animation || IsAsegaRetracted(animation)) {
                if (s->pasegbl)
                    SetJtThrowState(s, 0);
                else
                    s->player->SetPlayerState(0);
            }
            break;
        }
        if (s->state == previous || s->state == -1)
            break;
    }
    STEPPLAYER *owner = s->player;
    if (owner->steeringMode != 1)
        return;
    ASEGA *animation = owner->activeAnimation;
    CHN *reference = animation ? FindAsegChannel(animation->animation, owner, (void *)6, 1) : 0;
    if (reference) {
        reference->pac->Evaluate(animation->time, animation->speed, 0, &w.scratch.vector);
        VECTOR *direction = &s->player->vecTargetAngularVelocity;
        float height = w.scratch.vector.f[2];
        direction->x = 0.0f;
        direction->z = height;
        direction->y = 0.0f;
    } else
        s->player->targetAngularVelocity = D_002A2DD0.q;
}

// Object filter for the arc's box search: rejects objects with flag 0x59, otherwise defers to the default filter.
// Also known as: ArcEntry (upstream readable / Sly 1 name; ArcEntry__FP3OXAP3SOE).
extern "C" int FilterJtThrowArcObject(void *a, SOPAIR *b)
{
    if (IsBasicDerivedFrom__FP5BASIC3CID(b->p, 0x59))
        return 1;
    return ShouldKeepSoPair(a, b);
}

// UpdateJtThrowArc = UpdateJtThrowArc (Lilly's Sly2Port name, 0x21FB70): while the hero holds an object (JTTHROW, jts 23)
// it predicts the arc the object would fly to the aim point. The launch point (vLaunchLocal in the hero's space) and the
// carried object's gravity give CalculateLaunchVelocity (CalculateBallisticLaunchVelocity) a velocity whose elevation follows lmAim; the
// apex and the end point after tPredict seconds bound a box for FindSwObjectsInBox (IntersectSwBoundingBox). 28 samples along the
// arc are line-tested (PsoHitTestLineObjects); at the first hit the flight time is the closest non-negative root of the vertical
// motion (SolveQuadratic). The flight time may change by at most 3 * dt per frame; up to 30 points of the arc are written
// back to front into the marker's ring of frames (matrix along the arc, BuildOrthonormalMatrixZ), and the four-clip / two-clip
// blends get the distance (lmAim) and heading-error (aimTurnAngle) weights, rate limited by 2 * dt when fUpdateBlends.
// How it matched (Lilly's agent, T1604): fresh8's stack layout plus
// - the scope is an empty STACKSCOPE with its real out-of-line ctor/dtor (a sized FRAME takes the freed 0x0 home slot);
// - the VU_FLOAT homes are by-value parameters of the arc helper (all share sp+0x0; the hit block's at 0x260 is moved
//   out of the root loop by loop.c's load_mems), and the 0.5 of the sampling loop is a literal (cse2 copies it to f22);
// - the root loop's by-value VU_VECTOR copies keep the sample loop >= 96 RTL insns, so loop.c leaves FLT_MAX's lui in it;
// - accZ / startZ are written inside the root solver's arguments: loop.c hoists them, which gives the game's schedule
//   and the f23/f24/f25 order (computed before the loop, accZ ties uFar in global.c priority);
// - one counter for both point loops, `(points + i)->q`, `pc = point++`, the basis copy through a pointer (its stores
//   use the hit address register like the game), `left` before `near`, dt read into a local before the 2.0 * dt.
// A vector in a VU0 register, passed by value
struct VU_VECTOR {qword q;VU_VECTOR(){}VU_VECTOR(qword qw){q=qw;}};
// A float in x of a VU0 register; by value it has a stack home (the game's dead sqc2)
struct VU_FLOAT {qword q;};
// Broadcast a float into a VU0 register.
static inline VU_FLOAT VuFloat(float g){VU_FLOAT v;__asm__("qmtc2.ni %1,%0":"=j"(v.q):"r"(g));return v;}
// Return a - b.
static inline qword SubVec(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Return a - b.
static inline VU_VECTOR VuSub(VU_VECTOR a,VU_VECTOR b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a.q),"j"(b.q));return VU_VECTOR(r);}
// Same idiom as sly2/vu0.h VuMin; kept local: this file includes the header below the function that needs it.
static inline qword VuMin(qword a,qword b){qword r;__asm__("vmini.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Same idiom as sly2/vu0.h VuMax; kept local: this file includes the header below the function that needs it.
static inline qword VuMax(qword a,qword b){qword r;__asm__("vmax.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// Same idiom as sly2/vu0.h VuToFloat; kept local: this file includes the header below the function that needs it.
static inline float VuToFloat(qword v){float g;__asm__("qmfc2.ni %0,%1":"=r"(g):"j"(v));return g;}
// Same idiom as sly2/vu0.h QwordZ; kept local: this file includes the header below the function that needs it.
static inline float QwordZ(qword v){float g;__asm__("pextuw %0,$zero,%1":"=r"(g):"r"(v));return g;}
// XY length of v as a float.
static inline float LengthXYOf(qword v)
{
    __asm__("vmul.xy %0,%0,%0\n\tvaddy.x %0,%0,%0y" : "+j"(v));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(v));
    return VuToFloat(v);
}
// Length of w as a float.
static inline float LengthOf(VU_VECTOR w)
{
    qword v = w.q;
    qword one;
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z"
            : "+j"(v), "=&j"(one));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(v));
    return VuToFloat(v);
}
// pos + v * t + a * t2
static inline qword BallisticPos(const qword &pos, const qword &a, const qword &v, VU_FLOAT t, VU_FLOAT t2)
{
    qword r;
    __asm__("vadda.xyzw ACC,%1,$vf0\n\tvmaddax.xyzw ACC,%2,%3x\n\tvmaddx.xyzw %0,%4,%5x"
            : "=j"(r)
            : "j"(pos), "j"(v), "j"(t.q), "j"(a), "j"(t2.q));
    return r;
}
// Where the arc is after t seconds
static inline qword PosOnArc(const qword &pos,const qword &a,const qword &v,float t,float gHalf){return BallisticPos(pos,a,v,VuFloat(t),VuFloat(t*(t*gHalf)));}
// a * g0 + b * g1
static inline qword CombineVec(qword a, qword b, VU_FLOAT g0, VU_FLOAT g1)
{
    qword r;
    __asm__("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x" : "=j"(r) : "j"(a), "j"(g0.q), "j"(b), "j"(g1.q));
    return r;
}
// Evaluate the quadratic g0 + g1*x + g2*x^2.
static inline float EvaluateClq(const CLQ &clq,float x){return clq.g0+x*(clq.g1+x*clq.g2);}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A96A0 __asm__("D_002A96A0") = 3.40282347e+38f; }
// Predicts the arc of the held object, line-tests 28 samples against the world, writes up to 30 points into the aim
// marker ring and updates the aim blends.
// Also known as: UpdateJtThrowArc (upstream readable / Sly 1 name; UpdateJtThrowArc__FP7JTTHROWi).
extern "C" void UpdateJtThrowArc(JTTHROW *thrower, int fUpdateBlends)
{
    qword posLaunch = ConvertPosBetweenLos(thrower->player, 0, thrower->qwPosLaunchLocal);
    qword posHold = thrower->holdPosition;
    qword vAccel = *(qword *)&((SO *)thrower->ploCarried)->physics->gravity;
    qword posHero = GetLoWorldPos(thrower->player);
    float uFar =
        Clamp((LengthXYOf(SubVec(posHold, posHero)) - thrower->lmAim.minValue) / (thrower->lmAim.maxValue - thrower->lmAim.minValue),
               0.0f, 1.0f);
    qword vLaunch = CalculateBallisticLaunchVelocity(posLaunch, thrower->holdPosition, vAccel, EvaluateClq(D_002C40F0, uFar));
    float tApex = StdMax(-QwordZ(vLaunch) / QwordZ(vAccel), 0.0f);
    qword posApex = PosOnArc(posLaunch, vAccel, vLaunch, tApex, 0.5f);
    float t;
    if (IsPowerupAcquired(0x24))
        t = D_002C40E4 * thrower->tPredict;
    else
        t = thrower->tPredict;
    qword posEnd = PosOnArc(posLaunch, vAccel, vLaunch, t, 0.5f);
    STACKSCOPE stackscope;
    qword posMin = VuMin(posApex, posEnd);
    qword posMax = VuMax(posApex, posEnd);
    posMax = VuMax(posLaunch, posMax);
    posMin = VuMin(posLaunch, posMin);
    int cso;
    SO **apso;
    IntersectSwBoundingBox(&g_sw, posMin, posMax, FilterJtThrowArcObject, ((SO *)thrower->ploCarried)->physicsTree, &cso,
                           &apso);
    thrower->vLaunch = vLaunch;
    float dt = t * 0.034482758f;
    V apos[30];
    qword posPrev = posLaunch;
    unsigned i;
    for (i = 1; i < 29; i++) {
        t = (float)(int)i * dt;
        qword pos = PosOnArc(posLaunch, vAccel, vLaunch, t, 0.5f);
        HIT hit;
        if (FindFirstObjectHitByLine(1, posPrev, pos, cso, apso, &hit)) {
            V at;
            int ct =
                SolveQuadratic__FfffPf(QwordZ(vAccel) * 0.5f, QwordZ(vLaunch), QwordZ(posLaunch) - hit.pos.f[2], at.f);
            float tBest = 0.0f, sBest = D_002A96A0;
            for (int it = 0; it < ct; it++) {
                float tRoot = at.f[it];
                if (tRoot < 0.0f)
                    continue;
                qword posRoot = PosOnArc(posLaunch, vAccel, vLaunch, tRoot, 0.5f);
                float s = LengthOf(VuSub(posRoot, hit.pos.q));
                if (s < sBest) {
                    sBest = s;
                    tBest = tRoot;
                }
            }
            t = tBest;
            break;
        }
        posPrev = pos;
    }
    float dtMax = D_002C4100 * g_clock.deltaTime;
    thrower->tFlight = Clamp(t, thrower->tFlight - dtMax, thrower->tFlight + dtMax);
    t = thrower->tFlight;
    i = 0;
    for (;;) {
        if (t < 0.1f)
            break;
        apos[i].q = PosOnArc(posLaunch, vAccel, vLaunch, t, 0.5f);
        if (++i >= 29)
            break;
        t = thrower->tFlight - (float)(int)i * dt;
    }
    (apos + i)->q = posLaunch;
    int cpos = i + 1;
    if (((EFFECT *)thrower->ploMarker)) {
        ARCFRAME *pframe = ((EFFECT *)thrower->ploMarker)->aframe + ((EFFECT *)thrower->ploMarker)->cframe;
        qword posBefore = CombineVec(apos[0].q, apos[1].q, VuFloat(2.0f), VuFloat(-1.0f));
        V *posOut = apos;
        M4 mat;
        M3 matRot;
        for (int iframe = 0; iframe < ((EFFECT *)thrower->ploMarker)->cframe; iframe++) {
            pframe--;
            if (iframe < cpos) {
                V *pposCur = posOut++;
                qword pos = pposCur->q;
                qword dpos = SubVec(pos, posBefore);
                BuildOrthonormalMatrixZ(&matRot, dpos, g_normalZ.q);
                M4 *matrix = &mat;
                VECTOR_opAssign(&mat.position, pos);
                matrix->basis = matRot;
                posBefore = pos;
                mat.basis.r[2].n[3] = 0;
                mat.basis.r[1].n[3] = 0;
                mat.basis.r[0].n[3] = 0;
                matrix->position.f[3] = 1.0f;
                pframe->gValid = 1.0f;
            } else
                pframe->gValid = 0.0f;
            pframe->t = g_clock.time;
            pframe->s = 0.0f;
            pframe->mat = mat;
        }
        SetTrailMeshPositions(((EFFECT *)thrower->ploMarker), posLaunch);
    }
    qword posAim = thrower->holdPosition;
    qword posHeroNow = GetLoWorldPos(thrower->player);
    V dpos;
    dpos.q = SubVec(posAim, posHeroNow);
    float rad = atan2f(dpos.f[1], dpos.f[0]);
    float radHero = GetStepHeading(thrower->player);
    rad = NormalizeAngle__Ff(rad - radHero);
    float uTurn = Clamp(rad / thrower->aimTurnAngle, -1.0f, 1.0f) * 0.5f + 0.5f;
    if (fUpdateBlends) {
        float uFarOld = thrower->ablC[1].weight, uTurnOld;
        if (uFarOld > 0.5f)
            uTurnOld = thrower->ablB[3].weight / uFarOld;
        else
            uTurnOld = thrower->ablB[1].weight / (1.0f - uFarOld);
        float dtFrame = g_clock.deltaTime;
        float du = D_002C4104 * dtFrame;
        uFar = Clamp(uFar, uFarOld - du, uFarOld + du);
        uTurn = Clamp(uTurn, uTurnOld - du, uTurnOld + du);
    }
    float uTurnOpp = 1.0f - uTurn;
    float uNear = 1.0f - uFar;
    thrower->ablB[0].weight = uTurnOpp * uNear;
    thrower->ablB[1].weight = uTurn * uNear;
    thrower->ablB[2].weight = uTurnOpp * uFar;
    thrower->ablB[3].weight = uTurn * uFar;
    RecalculateAsegBlend(thrower->pasegblB);
    thrower->ablC[0].weight = uNear;
    thrower->ablC[1].weight = uFar;
    RecalculateAsegBlend(thrower->pasegblC);
}
#include "sly2/vu0.h" // here, not at the top: declared before UpdateJtThrowArc, the helpers change its code (355 of 517 off)

// Complete vector-state update with the observed scalar scratch writes.
struct G {char pad[0x10];qword v;};
struct Q {char pad[0x10];qword v,w;};
union F32 {float f;int bits;};
float D_002C4108 = -150.0f;
float D_002C410C = 300.0f;   /* a plain float: 4-aligned in the ELF, where the F32 union would be 8-aligned */
// In states 4..6 moves the hold point toward its goal (gain D_002C4108). (inferred)
// Also known as: GetJtThrowCpdefi (upstream readable / Sly 1 name; GetJtThrowCpdefi__FP7JTTHROWP6CPDEFI).
extern "C" void UpdateJtThrowHoldPosition(JTTHROW *owner, Q *value)
{
    V plane, scratch;
    register float gain asm("$f1");
    register G *movementAddress asm("$3");
    register qword prior asm("vf2"), position asm("vf5");
    asm("lui $2,%%hi(%3)\n\tlui %1,%%hi(%4)\n\tlwc1 %0,%%lo(%3)($2)\n\taddiu %1,%1,%%lo(%4)\n\tlqc2 %2,%5"
        : "=f"(gain), "=&r"(movementAddress), "=j"(prior)
        : "i"(&D_002C4108), "i"(&g_cm.mat), "m"(value->v)
        : "$2", "memory");
    asm("mfc1 $2,%2\n\tqmtc2.ni $2,$vf3\n\tlqc2 $vf1,%4\n\tsqc2 $vf3,%1\n\tvadda.xyzw ACC,%3,$vf0\n\tvmaddx.xyzw "
        "%0,$vf1,$vf3x"
        : "=j"(position), "=m"(plane.q)
        : "f"(gain), "j"(prior), "m"(movementAddress->v)
        : "$2", "vf1", "vf3");
    value->v = position;
    if (!(owner->state < 7))
        return;
    qword *destination = &value->w;
    if (!(owner->state >= 4))
        return;
    register float minusOne asm("$f0"), negativeGain asm("$f1") = gain;
    register qword motion asm("vf2");
    asm("li.s %0,-1.0\n\tneg.s %1,%1\n\tlqc2 %2,%3"
        : "=&f"(minusOne), "+f"(negativeGain), "=j"(motion)
        : "m"(movementAddress->v)
        : "$1");
    register qword target asm("vf1"), negative asm("vf4");
    register unsigned high asm("$2");
    asm("mfc1 %2,%4\n\tqmtc2.ni %2,$vf3\n\tlqc2 %0,%8\n\tmfc1 %2,%5\n\tqmtc2.ni %2,%1\n\tsqc2 "
        "$vf3,%3\n\tvadda.xyzw ACC,%0,$vf0\n\tvmaddax.xyzw ACC,%6,$vf3x\n\tvmaddx.xyzw %0,%7,%1x\n\tlui "
        "%2,%%hi(%9)"
        : "=&j"(target), "=&j"(negative), "=&r"(high), "=m"(scratch.q)
        : "f"(minusOne), "f"(negativeGain), "j"(position), "j"(motion), "m"(owner->holdPosition),
          "i"(&D_002C410C)
        : "vf3");
    plane.q = target;
    plane.n[2] = 0;
    register int raw asm("$2");
    register qword direction asm("vf3"), scale asm("vf6");
    asm("lw %0,%%lo(%5)(%3)\n\tlqc2 %1,%4\n\tqmtc2.ni %0,%2"
        : "=r"(raw), "=j"(direction), "=j"(scale)
        : "r"(high), "m"(plane.q), "i"(&D_002C410C), "m"(*(const int *)(high + 0x410C)));
    register qword length asm("vf1"), unit asm("vf2");
    register float tolerance asm("$f1");
    asm("vmul.xyz %0,%3,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tli.s "
        "%2,0.0001\n\tvsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q"
        : "=&j"(length), "=&j"(unit), "=f"(tolerance)
        : "j"(direction)
        : "$1");
    register qword fallback asm("$2");
    register int bits asm("$3");
    register float size asm("$f0");
    asm("lui %0,%%hi(%4)\n\tqmfc2.ni %1,%3\n\taddiu %0,%0,%%lo(%4)\n\tmtc1 %1,%2\n\tlq %0,0(%0)"
        : "=&r"(fallback), "=&r"(bits), "=f"(size)
        : "j"(length), "i"(&D_002A2DD0)
        : "memory");
    // Preserve the full native negative-gain scratch publication.
    *(volatile qword *)&scratch.q = negative;
    scratch.q = scale;
    scale = scratch.q;
    if (size < tolerance)
        length = fallback;
    else
        asm("vdiv Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "+j"(length) : "j"(direction));
    asm("vmulx.xyzw %0,%0,%1x" : "+j"(length) : "j"(scale));
    asm("sqc2 %1,%0" : "=m"(*destination) : "j"(length));
}

// Intersection-flag hook: for the carried object while carrying (state >= 3, or the player in state 0x12) adds the
// ignore flags so the player does not collide with it; otherwise returns the flags unchanged.
// Also known as: OverlapKindJtThrow (upstream readable / Sly 1 name; OverlapKindJtThrow__FP7JTTHROWP3ALOi).
extern "C" int AdjustJtThrowFindFlags(JTTHROW *thrower, LO *object, int n)
{
    if (object == thrower->ploCarried) {
        int k = thrower->state;
        if (k != -1) {
            if (k >= 3 || (object == D_002DD5BC && ((BENTLEY *)object)->jtair.state == 0x12))
                return MaxInt(n, 3);
            return n;
        }
        return n;
    }
    return n;
}

// bits (as float) divided by denominator (register-pinned).
static inline float DivideBits(int bits,float denominator){register float value asm("$f0");register float rate asm("$f2")=denominator;__asm__ ("mtc1 %1,%0\n\tdiv.s %0,%0,%2":"=&f"(value):"r"(bits),"f"(rate));return value;}
// The frame clock's current time.
static inline float ClockNow(void){float value;__asm__ __volatile__ ("lwc1 %0,%1":"=f"(value):"m"(g_clock.time));return value;}
// Store the throw's active flag, start time and span in one asm block.
static inline void PublishStart(JTTHROW *thrower,int active,float now,float span){asm("sw %3,%0\n\tswc1 %4,%1\n\tswc1 %5,%2":"=m"(thrower->isBlending),"=m"(thrower->blendStartTime),"=m"(thrower->blendDuration):"r"(active),"f"(now),"f"(span));}
// The constant 1 in $3.
static inline int Enabled(void){register int value asm("$3");asm("addiu %0,$zero,1":"=r"(value));return value;}
// In states 4..6: stores the launch and target points, flight time = distance / speed, start time = now, and marks
// the flight active.
// Also known as: SetJtThrowTarget (upstream readable / Sly 1 name; SetJtThrowTarget__FP7JTTHROWP2LO).
extern "C" void StartJtThrowFlight(JTTHROW *thrower, void *q)
{
    int k = thrower->state;
    if (k < 7) if (k >= 4) {
        qword v = GetLoWorldPos(q), a = thrower->holdPosition, d, r;
        int len;
        int enabled=Enabled();
        d = VuSub(a, v);
        thrower->blendToPosition = v;
        r = VuDot(d, d);
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(r));
        thrower->blendFromPosition = a;
        __asm__("qmfc2.ni %0, %1" : "=r"(len) : "j"(r));
        {
            float f = DivideBits(len,thrower->aimSpeed);
            float now=ClockNow();
            PublishStart(thrower,enabled,now,f);
        }
    }
}

// Detaches the carried object and gives it its launch linear and angular velocity; removes the arc marker.
// Also known as: ReleaseJtThrow (upstream readable / Sly 1 name; ReleaseJtThrow__FP7JTTHROW).
extern "C" void ReleaseJtThrowObject(JTTHROW *thrower)
{
    if (thrower->ploCarried == D_002DD5BC) {
        if (((BENTLEY *)thrower->ploCarried)->state != 8)
            goto tail;
        SetBentleyHoverState((BENTLEY *)thrower->ploCarried, 1);
    } else {
        SetLoParent(thrower->ploCarried, 0, 3);
        SetSoCnstrForce((SO *)thrower->ploCarried, 0);
        SetSoCnstrTorque((SO *)thrower->ploCarried, 0);
    }
    ((SO *)thrower->ploCarried)->SetVelocity(thrower->vLaunch);
    ((SO *)thrower->ploCarried)->SetAngularVelocity(ConvertVecBetweenLos(thrower->player, 0, thrower->qwWLaunchLocal));
    if (IsBasicDerivedFrom__FP5BASIC3CID(thrower->ploCarried, 100))
        AsTossable(thrower->ploCarried)->SetState(1);
tail:
    if (((EFFECT *)thrower->ploMarker))
        AsLo(thrower->ploMarker)->Remove();
}

// Transition controller states and select the corresponding animation.
struct WORKS {V scalar,saved,coefficient,velocity,point;};   /* (WORK in the per-function file) */
// Load the up vector g_normalZ into vf1.
static inline qword LoadUp(){register qword q __asm__("vf1");__asm__("lqc2 %0,0(%1)":"=j"(q):"r"(&g_normalZ),"m"(g_normalZ));return q;}
// Move a GPR point into vf3.
static inline qword PointVF(qword q){register qword r __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(q));return r;}
// Load a saved point into vf3.
static inline qword RestorePoint(const V &v){register qword q __asm__("vf3");__asm__("lqc2 %0,%1":"=j"(q):"m"(v));return q;}
// Load the up vector g_normalZ into vf2.
static inline qword LaunchUp(){register qword q __asm__("vf2");__asm__("lqc2 %0,0(%1)":"=j"(q):"r"(&g_normalZ),"m"(g_normalZ));return q;}
// Load the launch speed into vf4.
static inline qword LaunchSpeed(const V &v){register qword q __asm__("vf4");__asm__("lqc2 %0,%1":"=j"(q):"m"(v));return q;}
// Move the current position into vf1.
static inline qword LaunchCurrent(qword position){register qword value asm("vf1");asm("qmtc2.ni %1,%0":"=j"(value):"r"(position));return value;}
// Move a 128-bit GPR value into vf2.
static inline qword ToVF(qword q){register qword r asm("vf2");__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(q));return r;}
// Move a float's bits into a GPR qword.
static inline qword CPUFloatS(float f){qword r;__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;}
// Broadcast a float into vf1.
static inline qword ScalarS(float f){register qword r __asm__("vf1");r = VuFromFloat<qword>(f);return r;}
// Return a - b.
static inline qword SubS(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
// Dot product of a and b (x lane, register-pinned).
static inline qword DotV(qword a,qword b){register qword d __asm__("vf2"),one __asm__("vf4");__asm__("vmul.xyz %0,%1,%2":"=j"(d):"j"(a),"j"(b));__asm__("vaddw.x %0,$vf0,$vf0w":"=j"(one));__asm__("vadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d):"j"(one));return d;}
// Length of v (x lane, vf1).
static inline qword LengthV(qword v){register qword d __asm__("vf1");d = VuDot(v, v);__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));return d;}
// Return v scaled by k.x (computed in k's register).
static inline qword Scale(qword v,qword k){__asm__("vmulx.xyzw %0,%1,%0x":"+j"(k):"j"(v));return k;}
// Divide v by the x lane of length.
static inline qword NormalizeLength(qword v,qword length){__asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%0,Q":"+j"(v):"j"(length));return v;}
// Launch velocity: direction*speed + up*height.
static inline qword Launch(qword direction,qword speed,qword up,qword height){__asm__("vmulax.xyzw ACC,%1,%0x\n\tvmaddx.xyzw %0,%2,%3x":"+j"(speed):"j"(direction),"j"(up),"j"(height));return speed;}
// Launch asm fragment: broadcast height, load the coefficient and fetch the owner and flag.
static inline qword LaunchOperands(float height, const V &coefficient, qword &speed, BENTLEY *&receiver, int &flag)
{
    register qword scalar asm("vf1"), velocity asm("vf4");
    register BENTLEY *player asm("$4");
    register int one asm("$5");
    asm("addiu %3,$zero,1\n\tmfc1 $3,%4\n\tqmtc2.ni $3,%0\n\tlqc2 %1,%5\n\tlw %2,%6"
        : "=j"(scalar), "=j"(velocity), "=r"(player), "=&r"(one)
        : "f"(height), "m"(coefficient.q), "m"(D_002DD5BC)
        : "$3");
    speed = velocity;
    receiver = player;
    flag = one;
    return scalar;
}
// Zero the xyz floats of v.
static inline void ZeroXYZ(VECTOR &v){v.x=v.y=v.z=0.0f;}
// Restore the saved target and the throw's owner.
static inline qword RestoreTarget(STEPPLAYER **receiver, const JTTHROW *state, const V &saved)
{
    register STEPPLAYER *owner asm("$4");
    register qword target asm("vf1");
    asm("lw %0,%2\n\tlqc2 %1,%3" : "=r"(owner), "=j"(target) : "m"(state->player), "m"(saved));
    *receiver = owner;
    return target;
}

// Projection asm fragment: load the current and saved points and the saved-high pointer.
static inline qword ProjectionStart(qword current, const V &saved, qword &point, V *&savedHigh)
{
    register V *upper asm("$3"), *retained asm("$4");
    register qword position asm("vf1"), target asm("vf3");
    asm("lui %0,%%hi(%6)\n\tqmtc2.ni %4,%2\n\tlqc2 %3,%5\n\tdaddu %1,%0,$zero"
        : "=&r"(upper), "=r"(retained), "=j"(position), "=j"(target)
        : "r"(current), "m"(saved.q), "i"(&g_normalZ));
    point = target;
    savedHigh = retained;
    register V *full asm("$3");
    target = VuSub(target, position);
    asm("addiu %0,%1,%%lo(%2)" : "=r"(full) : "r"(upper), "i"(&g_normalZ));
    register qword normal asm("vf1");
    asm("lqc2 %0,%1" : "=j"(normal) : "m"(*full));
    point = target;
    return normal;
}
// Unit vector of input with its component along up removed (register-pinned).
static inline qword ProjectUnit(qword input, qword up)
{
    register qword direction asm("vf3") = input, normal asm("vf1") = up, projection asm("vf2"), one asm("vf4");
    register V *address asm("$2");
    register float epsilon asm("$f1");
    asm("lui %3,%%hi(%6)\n\tvmul.xyz %0,%4,%5\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x "
        "%0,%1,%0z\n\tli.s %2,0.0001\n\tvmulx.xyzw %0,%5,%0x\n\taddiu %3,%3,%%lo(%6)"
        : "=&j"(projection), "=&j"(one), "=f"(epsilon), "=&r"(address)
        : "j"(direction), "j"(normal), "i"(&D_002A2DA0)
        : "$1");
    direction = VuSub(direction, projection);
    register qword fallback asm("$2");
    asm("lq %0,0(%1)" : "=r"(fallback) : "r"(address), "m"(*address));
    qword length = LengthV(direction);
    register float size asm("$f0");
    size = VuToFloat(length);
    if (size < epsilon)
        direction = fallback;
    else
        direction = NormalizeLength(direction, length);
    return direction;
}
// Load the lift height and the upper vector address.
static inline qword LiftInputs(V *upper, float &height)
{
    register float scalar asm("$f0");
    register V *address asm("$2");
    register qword up asm("vf2");
    asm("li.s %0,600.0\n\taddiu %1,%3,%%lo(%5)\n\tlqc2 %2,0(%1)"
        : "=f"(scalar), "=&r"(address), "=j"(up)
        : "r"(upper), "m"(*(const V *)((const char *)upper + 0x2DC0)), "i"(&g_normalZ)
        : "$1");
    height = scalar;
    return up;
}
// Copy the selected object into $4 for the following call.
static inline LO*PositionReceiver(LO*selected){register LO*argument asm("$4");asm("daddu %0,%1,$zero":"=r"(argument):"r"(selected));return argument;}
// Load the saved target and the throw's owner.
static inline qword SavedTargetAndOwner(const JTTHROW *state, STEPPLAYER *&receiver)
{
    register qword target asm("$2");
    register STEPPLAYER *owner asm("$4");
    asm("lq %0,%2\n\tlw %1,%3" : "=r"(target), "=r"(owner) : "m"(state->holdPosition), "m"(state->player));
    receiver = owner;
    return target;
}
// Load the saved player position and the throw's owner.
static inline qword SavedPlayerAndOwnerS(qword position, const JTTHROW *state, STEPPLAYER *&receiver)
{
    register qword point asm("vf3");
    register STEPPLAYER *owner asm("$4");
    asm("qmtc2.ni %2,%0\n\tlw %1,%3" : "=j"(point), "=r"(owner) : "r"(position), "m"(state->player));
    receiver = owner;
    return point;
}
// State transitions of the carry/throw controller and selection of the matching animation.
// Also known as: SetJtThrowState (upstream readable / Sly 1 name; SetJtThrowState__FP7JTTHROWi).
extern "C" void SetJtThrowState(JTTHROW *s, int state)
{
    WORKS w;
    int old = s->state;
    if (state == old)
        return;
    switch (old) {
    case -1:
        if (s->ploCarried != D_002DD5BC)
            AttachJtHandsObject(&s->player->jtgrab.hands, s->ploCarried);
        if (IsBasicDerivedFrom__FP5BASIC3CID(s->ploCarried, 0x64))
            AsTossable(s->ploCarried)->SetState(0);
        ZeroXYZ(s->player->vecLocalVelocity);
        break;
    case 3:
        if (state != 4)
            D_002DD5BC->SetCtlState(1);
        break;
    }
    int previous = s->state;
    s->state = state;
    s->stateStartTime = g_clock.time;
    RefreshSwObjectCollisionFilter(&g_sw, s->player);
    switch (s->state) {
    case -1:
        SetStepTargetHeading(s->player, GetStepHeading(s->player));
        s->player->steeringMode = 0;
        break;
    case 7: {
        STEPPLAYER *getterOwner;
        w.saved.q = SavedTargetAndOwner(s, getterOwner);
        qword position = GetLoWorldPos(getterOwner);
        qword current = ToVF(position);
        STEPPLAYER *receiver;
        qword target = RestoreTarget(&receiver, s, w.saved);
        SetStepTargetHeadingFromVector(receiver, SubS(target, current));
    }
        s->player->steeringMode = 0;
        break;
    default:
        s->player->targetAngularVelocity = D_002A2DD0.q;
        s->player->steeringMode = 1;
        break;
    }
    ASEG *animation = 0;
    int flags = 1;
    switch (s->state) {
    case -1:
        if (s->ploCarried == D_002DD5BC && !((BENTLEY *)s->ploCarried)->hoverState) {
            LO *chosen = s->ploCarried;
            w.scalar.q = CPUFloatS(400.0f);
            w.coefficient.q = w.scalar.q;
            STEPPLAYER *getterOwner;
            w.point.q = SavedPlayerAndOwnerS(GetLoWorldPos(PositionReceiver(chosen)), s, getterOwner);
            qword position = GetLoWorldPos(getterOwner), direction;
            V *savedUpper;
            qword up = ProjectionStart(position, w.point, direction, savedUpper);
            direction = ProjectUnit(direction, up);
            float liftHeight;
            qword liftUp = LiftInputs(savedUpper, liftHeight), speed;
            BENTLEY *receiver;
            int flag;
            qword height = LaunchOperands(liftHeight, w.coefficient, speed, receiver, flag);
            qword velocity = Launch(direction, speed, liftUp, height);
            asm volatile("sqc2 %1,%0" : "=m"(w.velocity.q) : "j"(velocity));
            w.scalar.q = height;
            SetBentleyHoverState(receiver, flag);
            D_002DD5BC->SetVelocity(w.velocity.q);
        } else if (s->ploCarried->parent == s->ploHoldParent->parent) {
            if (IsBasicDerivedFrom__FP5BASIC3CID(s->ploCarried, 0x64))
                AsTossable(s->ploCarried)->SetState(4);
            else {
                SetLoParent(s->ploCarried, 0, 0xF);
                FadeSoOut(s->ploCarried, 0.25f);
            }
        }
        ReleaseJtHandsActs(&s->player->jtgrab.hands);
        if (((EFFECT *)s->ploMarker))
            AsLo(s->ploMarker)->Remove();
        s->ploCarried = 0;
        return;
    case 0:
        if (previous != 1 && previous != 2) {
            s->abl[0].weight = s->abl[1].weight = 0.5f;
            RecalculateAsegBlend(s->pasegbl);
        }
        animation = s->pasegbl;
        flags = 9;
        break;
    case 1: animation = s->pasegHoldTurnA; break;
    case 2: animation = s->pasegHoldTurnB; break;
    case 3:
        animation = s->unknown64;
        s->player->SetCtlState(1);
        break;
    case 4:
        if (((EFFECT *)s->ploMarker)) {
            ((EFFECT *)s->ploMarker)->active = 1;
            ((EFFECT *)s->ploMarker)->reset = 0;
            AsLo(s->ploMarker)->Add();
        }
        if (previous != 5 && previous != 6) {
            s->tFlight = 0.0f;
            s->holdPosition = ConvertPosBetweenLos(s->player, 0, s->qwPosAimLocal);
            UpdateJtThrowArc(s, 0);
        }
        animation = s->pasegblB;
        flags = 9;
        s->isBlending = 0;
        break;
    case 5: animation = s->pasegAimTurnA; break;
    case 6: animation = s->pasegAimTurnB; break;
    case 7:
        animation = s->pasegblC;
        if (IsBasicDerivedFrom__FP5BASIC3CID(s->ploCarried, 0x64))
            ChargeGadgetMp(s->player, ((CID100 *)s->ploCarried)->gadgetKind);
        break;
    }
    ReleaseSma(&s->player->activeStateMachine);
    ReplaceCurrentAseg__FP4ASEGP3ALOffiPP5ASEGA(animation, s->player, 0.0f, 1.0f, flags, &s->player->activeAnimation);
    s->player->ApplyStateStepParams();
    RefreshSwObjectCollisionFilter(&g_sw, s->player);
}
