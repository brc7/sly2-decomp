// carmelita.cpp -- CARMELITA (cid 0x3F, a STEPGUARD subclass) and her route behaviour. A ROUTE (cid 0x7B, an LO) is
// a list of up to 256 stations along a path shape, loaded from Splice options. The route behaviour (BHV182,
// guard flag 0x24) keeps her at the station best placed relative to the hero (distance along the route,
// screen position), walks or jumps between stations, attacks when the hero is in reach, and starts/stops a music
// cue. Also Carmelita's own Init, post-load, message handler and target choice. VU0 and FPU asm;
// UpdateBhvRouteChase has one register pin.
// TU 0x14A008..0x14B660 (21 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_carmelita
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_carmelita
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
#include "sly2/sndmgr.h"

// 0x14B508 StaticInit_carmelita and GlobalConstructors_carmelita: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Class ids used here (docs/systems/object-model.md section 1). */
enum {
    CID_STEPPLAYER = 0x34,
    CID_GUARD = 0x3A,
    CID_BHVGUARD = 0x9C,
    CID_BHVIDLE = 0x9E,
    CID_BHVATTACK = 0xA4,
    CID_BHVWALKPATH = 0xA7,
    CID_BHVLEAP = 0xAC,
    CID_BHV182 = 0xB6,
};
/* Guard flags (STEPGUARD +0x2C0, bit n = flag n; each behaviour raises its own: docs/systems/ai-guards.md
   section 2). Only the identified flags are named. */
enum {
    SGF_ROUTE = 0x24,   // BHV182 (Carmelita route)
};

typedef union { qword q; float f[4]; } U;
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
struct SW;
struct ALLOCREQ { int cb, cbAlign; };
struct SCOPE { int a[4]; };

/* The route is classes.h ROUTE (stations: ROUTESTATION), Carmelita is CARMELITA, the behaviour is BHV182; the
   sub-behaviours it pushes are BHVIDLE, BHVWALKPATH, BHVLEAP and BHVATTACK. */
/* The hero's record at STEP +0x258 while hanging (UpdateBhvRouteChase): its points (STEP types it SOPART*). */
struct PATH { char pad[0x90]; int c; qword *av; };
/* An object of the hiding-place list (*(SO **)&g_sw.dlCid98) (UpdateBhvRouteChase). */
struct XEC { char pad[0x20]; qword pos; float radius; };
struct SOV { void *scriptFrame; void *scriptVariables; void *firstSubscriber; VTE *vt; void *parent; void *root; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback; char pad38[0x3C - 0x38]; void *proxyRecord; char pad40[0x54 - 0x40]; void *transforms; void *positionXf; void *rotationXf; void *rootTransform; void *unknown64; void *unknown68; char pad6C[0x70 - 0x6C]; void *ploRemovedFirst; char pad74[0x80 - 0x74]; void *unknown80; char pad84[0xA8 - 0x84]; void *unknownA8; char padAC[0xB0 - 0xAC]; void *unknownB0; char padB4[0xBC - 0xB4]; void *unknownBC; void *unknownC0; void *unknownC4; void *unknownC8; char padCC[0xD8 - 0xCC]; void *unknownD8; void *unknownDC; char padE0[0xE4 - 0xE0]; void *parts; int flags; XEC *hidingInfo; char pad2[0x134 - 0xF0]; void *psobounds; void *pxa; char pad13C[0x144 - 0x13C]; void *prender; char pad148[0x174 - 0x148]; SOV *next; };

void *D_002DD648;   /* the registered guard (carmelita's .bss; medium: by layout, just before cd's .bss) */
extern qword D_002A2DD0;
extern const float D_002A4EE8;   /* defined below where the ELF has it (.rodata order) */
extern "C" {
/* carmelita's .data (address order). */
extern float D_002BD460;
extern float D_002BD468[2];
extern float D_002BD470;
extern float D_002BD474;
extern float D_002BD478;
extern float D_002BD47C;
extern LM D_002BD480;
extern LM D_002BD488;
extern LM D_002BD490;
extern LM D_002BD498;
extern LM D_002BD4A0;
extern float D_002BD4B0[3];
SCOPE *STACKSCOPE_ctor(SCOPE *pscope);
void STACKSCOPE_ctor_1AE4A0(SCOPE *pscope, int n) __asm__("_$_10STACKSCOPE");
ROUTESTATION *AllocClearedFromStack(ALLOCREQ *preq);
ROUTESTATION *AllocCopyFromWorldHeap(ALLOCREQ *preq, ROUTESTATION *pv);
void LoadLoFromBrx(ROUTE *proute, void *pbis);
void PostLoLoad(ROUTE *proute);
SHAPE *NewSampledShape(int oid, int c);
void *FindSwObject__FP2SWi3OIDP2LO(SW *psw, int k, int oid, void *object);
qword GetLoWorldPos(void *p);
void MeasureShape(SHAPE *shape);
void InitBhv(BHV182 *p);
void StartBhv(BHV182 *p);
void CloseBhv(BHV182 *p);
void UpdateBhv(BHV182 *p);
int IsBhvBusyAbove(BHV182 *p);
void PopBhv(void *stack);
void AddMusicLayer2Request(void *p);
void RemoveMusicLayer2Request(void *p);
void AddStepguardSgf(STEP *p, int n);
void RemoveStepguardSgf(STEP *p, int n);
int IsStepguardHandsOff(STEP *p);
STEPPLAYER *GetHeroPo(SW *psw);
CID120 *ChooseStepguardAttack(STEP *p, STEPPLAYER *phero, int m);
int IsBhvpathAtEnd(void *p);
int IsBhvanimDone(void *p);
int IsBhvattackDone(void *p);
int SnapToGround(qword *pq, int a, int b, float f, float g);
int IsSwHandsOff(SW *pg);
int IsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void SetStepguardTarget(STEP *pw, void *p);
int IsBreakableDone(SOV *p);
int FindSwObjectsByClass(SW *pg, int a, int b, void *c, int d, SOV **apso);
qword SetVectorSphere(float a, float b, float c);
float AngleBetweenVectors(qword a, qword b, void *p);
float atan2f(float y, float x);
qword SetVectorCylind(float a, float b, float c);
void FindShapeClosestPoint(SHAPE *, qword, void *, qword *, qword *, float *, float *);
qword ConvertCmWorldToScreen(void *, qword);
void SetStepguardAimAct(STEP *, int, int, qword);
void SetSoCnstrForce(STEP *, int);
void SetSoCnstrTorque(STEP *, int);
BHV *PushStepBehavior(STEP *, int);
void SetBhvpathShape(BHVPATH *, SHAPE *, int);
void InitStepguard(CARMELITA *p);
void PostStepguardLoad(CARMELITA *p);
void HandleStepguardMessage(CARMELITA *p, int msgid, void *pv);
int StepguardHasSgf(CARMELITA *p, int k);
void SetDetectorTargetStateById(DETECTOR *pdet, void *object, int k);
LO *ChooseStepguardTarget(CARMELITA *p);
void *memset(void *p, int c, unsigned int n);
void *FindStepBehavior(CARMELITA *p, int n);
void SetBhvguardMode(void *p, BHVGUARDRECDATA *m);
void UpdateBhvRouteChase(BHV182 *p);
int IsBhvRouteOffGround(BHV182 *p);
int FindBhvRouteBestStation(BHV182 *p);
void AdvanceBhvRouteStation(BHV182 *p);
LO *GetBhvRouteStationKind(BHV182 *p);
void SetBhvRouteState(BHV182 *p, int n);
}
float RandomFloatInRange(float gMin, float gMax);
int RandomIntInRange(int nMin, int nMax);
int IsWithinLimits(LM *plm, float g) __asm__("IsWithinLimits__FP2LMf");   /* LM: this file's view of LM */
LO *CreateLo(int cid, void *psoParent, int oid, int isplice) __asm__("CreateLo__F3CIDP2SO3OIDi");

// Fill an allocation request for `a` 12-byte records with alignment b.
static inline void InitS(ALLOCREQ *ps, int a, int b)
{
    ps->cb = a * 0xC;
    ps->cbAlign = b;
}
// 0x14A008: ROUTE (cid 0x7B) vtable slot 14 (LoadFromBrx): loads into a 256-entry stack table filled by the
// AddRoutePoint Splice option, then copies the used part to the heap.
extern "C" void LoadRouteFromBrx(ROUTE *route, void *pbis)
{
    SCOPE f;
    ALLOCREQ s;
    STACKSCOPE_ctor(&f);
    InitS(&s, 0x100, 0x10);
    route->stations = AllocClearedFromStack(&s);
    LoadLoFromBrx(route, pbis);
    InitS(&s, route->stationCount, 0x10);
    route->stations = AllocCopyFromWorldHeap(&s, route->stations);
    STACKSCOPE_ctor_1AE4A0(&f, 2);
}

// 0x14A0A8: ROUTE vtable slot 15 (PostLoad): LO post-load, then builds the route's path shape (kind 0x62C) from its
// points.
// Also known as: PostLoadChasegraph (upstream readable / Sly 1 name; PostLoadChasegraph__FP2LO).
void ROUTE::PostLoad()
{
    ROUTE *route = this;
    int i;
    PostLoLoad(route);
    route->shape = NewSampledShape(0x62C, route->stationCount);
    route->shape->numControlVertices = route->stationCount;
    for (i = 0; i < route->stationCount; i++) {
        ROUTESTATION *e = &route->stations[i];
        LO *o = (LO *)FindSwObject__FP2SWi3OIDP2LO(&g_sw, 4, e->oid, route);
        e->object = o;
        if (o) {
            qword *pq = (qword *)((i << 4) + (int)route->shape->vertexPositions);   // (byte arithmetic: an indexed store does not match)
            *pq = GetLoWorldPos(o);
        }
    }
    MeasureShape(route->shape);
}

// 0x14A178: Splice option: appends a route point {oid, kind} (at most 256).
extern "C" void AddRoutePoint(ROUTE *route, int k, int oid)
{
    int c = route->stationCount;
    if (c < 0x100) {
        ROUTESTATION *prte = &route->stations[c];
        prte->kind = k;
        prte->oid = oid;
        route->stationCount++;
    }
}

// 0x14A1B0: Route behaviour (BHV182) vtable slot 7 (OnPush): base, starts the music cue, raises guard flag 0x24, no
// current or target station.
// Also known as: Init (upstream readable / Sly 1 name).
extern "C" void InitBhvRoute(BHV182 *behavior)
{
    InitBhv(behavior);
    AddMusicLayer2Request(&g_sndmgr);
    AddStepguardSgf(behavior->owner, SGF_ROUTE);
    behavior->state = -1;
    behavior->targetStation = -1;
    behavior->currentStation = -1;
}

// 0x14A200: Route behaviour vtable slot 8 (Start): state 0, start time, clears the guard's +0x4A4.
// Also known as: Enter (upstream readable / Sly 1 name).
void BHV182::Start()
{
    BHV182 *behavior = this;
    StartBhv(behavior);
    SetBhvRouteState(behavior, 0);
    behavior->lastFastHeroTime = g_clock.time;
    ((CARMELITA *)behavior->owner)->tracksRelocatePoint = 0;
}

// 0x14A248: Route behaviour vtable slot 9 (Close): state -1, lowers flag 0x24, stops the music cue, sets the
// guard's +0x4A4 again.
// Also known as: Exit (upstream readable / Sly 1 name).
extern "C" void CloseBhvRoute(BHV182 *behavior)
{
    CloseBhv(behavior);
    SetBhvRouteState(behavior, -1);
    RemoveStepguardSgf(behavior->owner, SGF_ROUTE);
    RemoveMusicLayer2Request(&g_sndmgr);
    ((CARMELITA *)behavior->owner)->tracksRelocatePoint = 1;
}

// 0x14A2A0: Route behaviour vtable slot 10 (Update): updates the chase position, then runs the state machine: 0 at
// a station (attack the hero if it is the best station, else advance), 1 travelling (back to 0 at the path end), 2
// falling/recovering animation, 3 attacking, 4 hands-off pause.
// Also known as: Update (upstream readable / Sly 1 name).
extern "C" void UpdateBhvRoute(BHV182 *behavior)
{
    int s;
    UpdateBhv(behavior);
    UpdateBhvRouteChase(behavior);
    for (;;) {
        if (IsBhvBusyAbove(behavior))
            break;
        s = behavior->state;
        switch (s) {
        case 0:
            if (IsStepguardHandsOff(behavior->owner)) {
                SetBhvRouteState(behavior, 4);
            } else if (IsBhvRouteOffGround(behavior)) {
                SetBhvRouteState(behavior, 2);
            } else {
                int v = FindBhvRouteBestStation(behavior);
                behavior->targetStation = v;
                if (v == behavior->currentStation) {
                    CID120 *r = ChooseStepguardAttack(behavior->owner, GetHeroPo(&g_sw), 0);
                    behavior->attack = r;
                    if (r)
                        SetBhvRouteState(behavior, 3);
                } else {
                    AdvanceBhvRouteStation(behavior);
                }
            }
            break;
        case 1:
            if (IsStepguardHandsOff(behavior->owner))
                SetBhvRouteState(behavior, 4);
            else if (IsBhvRouteOffGround(behavior))
                SetBhvRouteState(behavior, 2);
            else if (IsBhvpathAtEnd(behavior->pbhvNext))
                SetBhvRouteState(behavior, 0);
            else
                ChooseStepguardAttack(behavior->owner, GetHeroPo(&g_sw), 0);
            break;
        case 2:
            if (IsStepguardHandsOff(behavior->owner))
                SetBhvRouteState(behavior, 4);
            else if (IsBhvanimDone(behavior->pbhvNext))
                SetBhvRouteState(behavior, 0);
            break;
        case 3:
            if (IsStepguardHandsOff(behavior->owner))
                SetBhvRouteState(behavior, 4);
            else if (IsBhvattackDone(behavior->pbhvNext))
                SetBhvRouteState(behavior, 0);
            break;
        case 4:
            if (!IsStepguardHandsOff(behavior->owner))
                SetBhvRouteState(behavior, 0);
            break;
        }
        if (s == behavior->state)
            break;
    }
}

// 0x14A478: True if the guard is in the air and no ground lies within 100 above / 250 below it (it fell off the
// route, inferred).
// Also known as: FAirborne (upstream readable / Sly 1 name).
extern "C" int IsBhvRouteOffGround(BHV182 *behavior)
{
    STEP *guard = behavior->owner;
    qword t;
    if (guard->floorContact) return 0;
    t = GetLoWorldPos(guard);
    return SnapToGround(&t, 1, 0, 100.0f, -250.0f) == 0;
}

// a < b (kept as a helper for the match).
static inline int IsLess(float a, float b) { return a < b; }
// VU0 helper: vector difference.
static inline qword SubX(qword a, qword b)
{
    qword r;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b), "j"(b)); /* extra operand: allocation only */
    return r;
}
// VU0 helper: vector sum.
// VU0 helper: length of a vector.
static inline float Len(qword a)
{
    qword d; float g;
    d = VuDot(a, a);
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    g = VuToFloat(d);
    return g;
}
// VU0 helper: squared length of a vector.
static inline float LenSq(qword a)
{
    qword d; float g;
    d = VuDot(a, a);
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(d));
    return g;
}
// VU0 helper: length of a vector (second instruction pattern for the match).
static inline float Len2(qword a)
{
    qword d; float g;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=&j"(d) : "j"(a));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    g = VuToFloat(d);
    return g;
}
// VU0 helper: x component of a vector.
// VU0 helper: y component of a vector.
// VU0 helper: z component of a vector.
// VU0 helper: a plus the normalized direction of v scaled by s.
static inline qword AddScaledNorm(qword a, qword v, VU_FLOAT s)
{
    qword d, r;
    d = VuDot(v, v);
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(d) : "j"(d), "j"(v));
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(a), "j"(d), "j"(s.q));
    return r;
}

// 0x14A4D0: Compares the hero's and the guard's positions and velocities along the route to pick the chase speed
// and lead the hero (inferred).
// Also known as: Aim (upstream readable / Sly 1 name).
extern "C" void UpdateBhvRouteChase(BHV182 *behavior)
{
    SW *pg = &g_sw;
    STEPPLAYER *hero = GetHeroPo(pg);
    qword posPlayer = GetLoWorldPos(hero);
    qword posMe = GetLoWorldPos(behavior->owner);
    qword d = VuSub(posPlayer, posMe);
    qword v;
    qword pos;
    v = hero->GetVelocity();
    if (IsLess(D_002BD460, Len(v)))
        behavior->lastFastHeroTime = g_clock.time;
    float dt = g_clock.time - behavior->lastFastHeroTime;
    if (RandomFloatInRange(D_002BD468[0], D_002BD468[1]) < dt && !IsSwHandsOff(pg)) {
        if (IsBasicDerivedFrom__FP5BASIC3CID(hero, CID_STEPPLAYER) && hero->jtair.state == 0x11) {
            SOPART *ppart = hero->psopartBody;
            pos = GetLoWorldPos(hero);
            int c = ppart->meshWorld.numPositions;
            for (int i = 0; i < c; i++) {   /* the highest vertex of the body mesh */
                if (QwordZ(pos) < QwordZ(*(qword *)&ppart->meshWorld.positions[i]))
                    pos = *(qword *)&ppart->meshWorld.positions[i];
            }
            goto moveMarker;
        }
        SetStepguardTarget(behavior->owner, hero);
        return;
    }
    for (SOV *object = (*(SOV **)&g_sw.dlCid98); object; object = object->next) {
        if (!IsBreakableDone(object)) {
            SOV *apso[8];
            qword posT = GetLoWorldPos(object);
            float r = D_002BD478;
            if (object->hidingInfo) {
                posT = object->hidingInfo->pos;
                r = object->hidingInfo->radius;
            }
            int c = FindSwObjectsByClass(&g_sw, 1, 0x12, object, 8, apso);
            int k = 0;
            for (int i = 0; i < c; i++) {
                if (apso[i]->flags & 0x17E)
                    apso[k++] = apso[i];
            }
            if (k) {
                SOV *pt = apso[RandomIntInRange(0, k - 1)];
                posT = GetLoWorldPos(pt);
                r = *(float *)&pt->hidingInfo;
            }
            if (!IsLess(D_002BD478 * D_002BD478, LenSq(VuSub(posT, posPlayer)))) {
                float pan = RandomFloatInRange(-3.1415927f, 3.1415927f);
                float tilt = RandomFloatInRange(-3.1415927f, 3.1415927f);
                register qword q asm("vf3"); /* register pin: the target's vadd writes posT's new value to vf3 */
                q = VuAdd(posT, SetVectorSphere(pan, tilt, RandomFloatInRange(0.0f, r)));
                qword entry = SubX(q, posMe);
                posT = q;
                if (IsLess(Len2(entry), Len2(d))) {
                    pos = posT;
                    goto moveMarker;
                }
                float g = AngleBetweenVectors(d, entry, 0);
                if (!(__builtin_fabsf(g) * 57.295776f <= D_002BD47C)) {
                    pos = posT;
                    goto moveMarker;
                }
            }
        }
    }
    if (IsLess(Len2(v), D_002BD460)) {
        qword entry = VuSub(posMe, posPlayer);
        float rad = atan2f(QwordY(entry), QwordX(entry));
        qword pp = GetLoWorldPos(hero);
        pos = VuAdd(pp, SetVectorCylind(RandomFloatInRange(rad - 1.5707964f, rad + 1.5707964f), D_002BD470, D_002BD474));
    } else {
        pos = AddScaledNorm(posPlayer, v, VU_FLOAT(-D_002BD470));
    }
moveMarker:   // (a shared tail: each search above jumps here with its pick)
    {
        LO *plo = ((CARMELITA *)behavior->owner)->marker;
        plo->TranslateToPos(pos);
    }
    SetStepguardTarget(behavior->owner, ((CARMELITA *)behavior->owner)->marker);
}

// Remaps g through an LM range (inferred: clamped linear mapping).
static inline float NormalizeToRange(LM *pr, float g)
{
    if (pr->minValue == pr->maxValue)
        return 0.0f;
    return (g - pr->minValue) / (pr->maxValue - pr->minValue);
}
// 0x14AA30: Returns the index of the route station best placed relative to the hero: each station is scored by its
// distance along the route from the hero's closest point (weight 0.5), its depth on screen (0.15) and its distance
// from the screen centre (0.35), each mapped through a .data range.
// Also known as: IBest (upstream readable / Sly 1 name).
extern "C" int FindBhvRouteBestStation(BHV182 *behavior)
{
    int iBest = -1;
    int i = 0;
    float ag[1];
    void *pv = GetHeroPo(&g_sw);
    float gBest = -1.0f;
    FindShapeClosestPoint(behavior->route->shape, GetLoWorldPos(pv), 0, 0, 0, 0, ag);
    for (; i < behavior->route->stationCount; i++) {
        float g, u1, u2, u3, s;
        U v;
        ROUTESTATION *station = &behavior->route->stations[i];
        if (!station->object)
            continue;
        g = ag[0] - behavior->route->shape->vertexArcLengths[i];
        if (behavior->route->isReversed == 1)
            g = -g;
        if (IsWithinLimits(&D_002BD480, g))
            u1 = NormalizeToRange(&D_002BD480, g);
        else if (IsWithinLimits(&D_002BD488, g))
            u1 = 1.0f - NormalizeToRange(&D_002BD488, g);
        else
            u1 = 0.0f;
        v.q = ConvertCmWorldToScreen(&g_cm, *(qword *)&behavior->route->shape->vertexPositions[i]);
        if (IsWithinLimits(&D_002BD490, v.f[2]))
            u2 = NormalizeToRange(&D_002BD490, v.f[2]);
        else if (IsWithinLimits(&D_002BD498, v.f[2]))
            u2 = 1.0f - NormalizeToRange(&D_002BD498, v.f[2]);
        else
            u2 = 0.0f;
        { float l2 = v.f[0] * v.f[0] + v.f[1] * v.f[1]; __asm__ __volatile__("sqrt.s %0, %1" : "=f"(g) : "f"(l2)); }
        if (IsWithinLimits(&D_002BD4A0, g))
            u3 = 1.0f - NormalizeToRange(&D_002BD4A0, g);
        else
            u3 = 0.0f;
        s = u1 * 0.5f + u2 * 0.15f + u3 * 0.35f;
        if (gBest < s) {
            gBest = s;
            iBest = i;
        }
    }
    return iBest;
}

// 0x14AD70: Advances one station toward the target station and starts the move its kind asks for (walk or leap).
// Also known as: Step (upstream readable / Sly 1 name).
extern "C" void AdvanceBhvRouteStation(BHV182 *behavior)
{
    int k;
    if (behavior->currentStation < behavior->targetStation) {
        behavior->currentStation++;
        k = behavior->route->stations[behavior->currentStation].kind;
    } else {
        k = behavior->route->stations[behavior->currentStation].kind;
        behavior->currentStation--;
    }
    switch (k) {
    case 0:
        SetBhvRouteState(behavior, 1);
        break;
    case 1:
        SetBhvRouteState(behavior, 2);
        break;
    }
}

// 0x14AE18: Returns the kind of the current route station.
// Also known as: PloEntry (upstream readable / Sly 1 name).
extern "C" LO *GetBhvRouteStationKind(BHV182 *behavior)
{
    return behavior->route->stations[behavior->currentStation].object;
}

// 0x14AE38: Route behaviour state change: pushes the child behaviour for the new state (path walk, leap, animation,
// attack, idle).
// Also known as: SetState (upstream readable / Sly 1 name; SetState__8JOBCHASEi).
extern "C" void SetBhvRouteState(BHV182 *behavior, int state)
{
    if (state == behavior->state)
        return;
    switch (behavior->state) {
    case 0:
        SetStepguardAimAct(behavior->owner, 0, 0, D_002A2DD0);
        break;
    case -1:
    case 1:
        SetSoCnstrForce(behavior->owner, 1);
        SetSoCnstrTorque(behavior->owner, 1);
        break;
    case 3: {
        float t = g_clock.time;
        ((CARMELITA *)behavior->owner)->nextAttackTime = t + RandomFloatInRange(behavior->attack->cooldownMin, behavior->attack->cooldownMax);
        behavior->attack = 0;
        break;
    }
    }
    PopBhv(&behavior->pbhvNext);
    behavior->state = state;
    behavior->stateTime = g_clock.time;
    switch (state) {
    case 0: {
        BHVIDLE *po = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
        po->lookAt = GetHeroPo(&g_sw);
        po->Start();
        behavior->owner->SetVelocity(D_002A2DD0);
        behavior->owner->SetAngularVelocity(D_002A2DD0);
        {
            VECTOR *pv = &behavior->owner->vecLocalVelocity;
            *(int *)&pv->x = *(int *)&pv->y = *(int *)&pv->z = 0;   /* integer zero stores (sw $0), as the target */
        }
        break;
    }
    case 1: {
        SHAPE *psp = behavior->owner->routeShape;
        BHVWALKPATH *po;
        psp->numControlVertices = 2;
        {
            qword *pq = (qword *)psp->vertexPositions;
            *pq = GetLoWorldPos(behavior->owner);
            pq = (qword *)psp->vertexPositions;
            pq[1] = GetLoWorldPos(GetBhvRouteStationKind(behavior));
        }
        MeasureShape(psp);
        po = (BHVWALKPATH *)PushStepBehavior(behavior->owner, CID_BHVWALKPATH);
        SetBhvpathShape(po, psp, 0);
        po->animSgf = 8;
        po->ignoresBlockers = 1;
        po->arrivalRadius = 25.0f;
        po->lookAtTarget = GetHeroPo(&g_sw);
        po->Start();
        ((CARMELITA *)behavior->owner)->nextAttackTime = g_clock.time;
        SetSoCnstrForce(behavior->owner, 0);
        SetSoCnstrTorque(behavior->owner, 0);
        break;
    }
    case 2: {
        BHVLEAP *po = (BHVLEAP *)PushStepBehavior(behavior->owner, CID_BHVLEAP);
        po->landingPoint = GetLoWorldPos(GetBhvRouteStationKind(behavior));
        po->extra = 1;
        po->Start();
        break;
    }
    case 3: {
        BHVATTACK *po = (BHVATTACK *)PushStepBehavior(behavior->owner, CID_BHVATTACK);
        po->victim = ((CARMELITA *)behavior->owner)->target;
        po->attack = behavior->attack;
        po->Start();
        ((CARMELITA *)behavior->owner)->nextAttackTime = 3.40282346638528859812e+38f;
        break;
    }
    case 4: {
        BHVIDLE *po = (BHVIDLE *)PushStepBehavior(behavior->owner, CID_BHVIDLE);
        po->lookAt = GetHeroPo(&g_sw);
        po->sgf = 1;
        po->Start();
        break;
    }
    case -1:
        SetSoCnstrForce(behavior->owner, 0);
        SetSoCnstrTorque(behavior->owner, 0);
        break;
    }
}

// 0x14B1A0: CARMELITA vtable slot 7 (Init): STEPGUARD Init, registers her as the world's Carmelita (D_002DD648),
// sets +0x4A4.
void CARMELITA::Init()
{
    CARMELITA *guard = this;
    InitStepguard(guard);
    D_002DD648 = guard;
    guard->tracksRelocatePoint = 1;
}

// 0x14B1D8: CARMELITA vtable slot 15 (PostLoad): STEPGUARD post-load, then creates her helper object (oid 0x62B) at
// +0x1010.
extern "C" void PostCarmelitaLoad(CARMELITA *guard)
{
    PostStepguardLoad(guard);
    guard->marker = CreateLo(3, 0, 0x62B, -1);
}

// 0x14B218: CARMELITA vtable slot 5 (HandleMessage): STEPGUARD handler; message 0xB about herself while not on her
// route (flag 0x24 clear) marks the hero as seen on her detector (inferred).
extern "C" void HandleCarmelitaMessage(CARMELITA *guard, int msgid, void **ppv)
{
    HandleStepguardMessage(guard, msgid, ppv);
    if (StepguardHasSgf(guard, SGF_ROUTE) == 0 && msgid == 0xB) {
        CARMELITA *pguard = (CARMELITA *)ppv[0];
        if (pguard == guard) {
            SetDetectorTargetStateById(pguard->pdetector, ppv[1], 1);
        }
    }
}

// VU0 helper: distance between two points.
static __inline__ float Distance(qword a, qword b)
{
    qword d;
    float g;
    d = VuSub(a, b);
    d = VuDot(d, d);
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    g = VuToFloat(d);
    return g;
}
// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A4EE8 __asm__("D_002A4EE8") = 3.40282347e+38f; }
// 0x14B290: CARMELITA vtable slot 88 (choose target): picks the best candidate object by kind and distance.
// Also known as: PloGuardTarget3F (upstream readable / Sly 1 name; PloGuardTarget3F__FP5GUARD).
extern "C" LO *ChooseCarmelitaTarget(CARMELITA *guard)
{
    LO *poBest;
    LO *object;
    float gBest;
    int kBest;
    qword pos;
    int i;
    if (StepguardHasSgf(guard, SGF_ROUTE))
        return ChooseStepguardTarget(guard);
    poBest = 0;
    object = guard->forcedTarget;
    if (object && (uchar)object->isInWorld) {   /* (uchar): the bit extract of the target (see brx) */
        poBest = object;
    } else {
    gBest = D_002A4EE8;
    kBest = 999;
    pos = GetLoWorldPos(guard);
    for (i = 0; i < guard->pdetector->ctarget; i++) {
        DETECTORTARGET *pe = &guard->pdetector->atarget[i];
        int k;
        float g;
        if (!pe->state)
            continue;
        if (IsBasicDerivedFrom__FP5BASIC3CID(pe->object, CID_STEPPLAYER))
            k = 0;
        else if (IsBasicDerivedFrom__FP5BASIC3CID(pe->object, CID_GUARD))
            k = 1;
        else
            k = 2;
        g = Distance(pos, GetLoWorldPos(pe->object));
        if (pe->object == guard->target)
            g = D_002BD4B0[0] + g * (D_002BD4B0[1] + g * D_002BD4B0[2]);
        if (k < kBest || (k == kBest && g < gBest)) {
            kBest = k;
            gBest = g;
            poBest = pe->object;
        }
    }
    }
    return poBest;
}

// 0x14B460: CARMELITA vtable slot 86 (is zap lethal): never.
extern "C" int IsCarmelitaZapLethal()
{
    return 0;
}

// 0x14B468: Splice option: sends a value (mode record 0xF) to her root guard behaviour (BHVGUARD, cid 0x9C).
extern "C" void SendCarmelitaRootBhvValue(CARMELITA *guard, int val)
{
    BHVGUARDRECDATA m;
    void *o;
    memset(&m, 0, sizeof(m));
    o = FindStepBehavior(guard, CID_BHVGUARD);
    if (val) {
        m.arg.object = (void *)val;
        m.kind = 0xE;
        SetBhvguardMode(o, &m);
    } else if (FindStepBehavior(guard, CID_BHV182)) {
        m.kind = 1;
        SetBhvguardMode(o, &m);
    }
}

// ---- carmelita's .data in address order (tu-merging.md "Phase 6: one link mode"): one block after the functions (definition order is emission order); the declarations above are what the functions see.
extern "C" {
float D_002BD460 = 100.0f;
float D_002BD468[2] = {4.0f, 6.0f};
float D_002BD470 = 200.0f;
float D_002BD474 = -75.0f;
float D_002BD478 = 600.0f;
float D_002BD47C = 10.0f;
LM D_002BD480 = {900.0f, 1400.0f};
LM D_002BD488 = {1400.0f, 4000.0f};
LM D_002BD490 = {600.0f, 1000.0f};
LM D_002BD498 = {1000.0f, 4000.0f};
LM D_002BD4A0 = {0.0f, 10.0f};
float D_002BD4B0[3] __attribute__((aligned(16))) = {-400.0f, 0.8f, 0.0f};
}
