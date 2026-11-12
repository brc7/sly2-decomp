// places.cpp -- JTPLACES (JT +0x9E0), the player's recent safe places (the ground or shape he stood on, or a spot on an
// object): remembering them, choosing the best one to return to (line tests around it, distance to bonus objects)
// and putting the player back there, and forgetting the places of a removed object. VU0 asm.
// TU 0x19F3E8..0x1A0300 (14 functions, address order).
// Entry points: UpdateJtPlaces (per frame, records places), ReturnJtToSafePlace / ReturnJtToPosition (after a
// fall or hazard), RemoveSwPlacesOf (object removed from the world).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_places
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_places
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/stackscope.h"
#include "sly2/clock.h"
#include "sly2/cm.h"
#include "sly2/sw.h"
#include "sly2/so.h"

// 0x1A01A8 StaticInit_places and GlobalConstructors_places: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

#include "sly2/jt.h"
#include "sly2/classes.h"
#include "sly2/xp.h"
/* vec.h's quadword-aligned VECTOR (basic.h's has no alignment). */
struct VECTORV { float x, y, z; } __attribute__((aligned(16)));
typedef union { qword q; float f[4]; } U;
/* The surface the player stands on (STEP::floorContact): no shared XP definition yet. */

/* The hang object (JT::hangHandle, CID19): +0x100 its grab spot, +0x10C nonzero: no place is recorded on it. */
struct B { char pad[0x10C]; int unknown10C; };
#include "sly2/lsg.h"
typedef struct { float g, gFirst, gSecond; } CLQV;   /* 12-byte data object: basic.h's CLQ is 16 (its symbol size would change) */

float D_002BF940 = 1.25f;   /* places' .data starts here (address order) */
extern const float D_002A66A0;   /* defined below where the ELF has it (.rodata order) */
struct VECTORQ { float x, y, z; } __attribute__((aligned(16)));   /* a quadword-aligned VECTORV (vec.h's layout) */
extern VECTORV D_002A2DD0;
extern STEPPLAYER *D_002E1E40, *D_002DD5BC, *D_002F7900;
extern "C" {
qword TransformXfPos(void *a, void *b, qword q);
void LoadIdentityMatrix4(void *o, float g, int n, MATRIX4Q *pm, int m);
void *func_002333B4(void *, int, int) __asm__("memset");
void ReturnJtToSafePlace(STEPPLAYER *, JTPLACE *);
void ChooseJtSafePlace(JTPLACES *p, JTPLACE *e);
PO *GetHeroPo(SW *g);
qword GetJtPlacePosition(STEPPLAYER *p, JTPLACE *e, float g);
float ComputeMaxBallisticTime(qword a, qword b, float c, float d, float e, float f);
void SetStepTargetHeadingFromVector(STEP *p, qword q);
void CutCm__FP2CM(CM *pcm);
void SetJtAirJump(void *p, int a, int b, int c);
int IsBasicDerivedFrom__FP5BASIC3CID(LO *a, int b);
void UpdateSoBusy(SW *g, LO *a);
void ChooseJtLanding(void *p, LO *a);
void StartScreenFade(SCREENFADE *pcid116);
void IntersectSwBoundingSphere(SW *world, qword pos, void *pfn, void *ctx, int *pcpso, SOPART ***papso, float r);   /* (the radius travels in $f12) */
SOPART *FindFirstObjectHitByLine(int grfhtl, qword a, qword b, int cpso, SOPART **apso, LSG *phit);
void ShouldKeepSoPair(void);
/* VQ: a 16-byte vector that can be initialized (floats first), read as a qword (.q). */
union VQ { float af[4]; qword q; };
float D_002BF944 = 500.0f;          /* the nearby-objects sphere radius */
VQ D_002BF950[8] = {{{40.0f, 0.0f, 0.0f, 0.0f}}, {{28.3f, 28.3f, 0.0f, 0.0f}}, {{0.0f, 40.0f, 0.0f, 0.0f}}, {{-28.3f, 28.3f, 0.0f, 0.0f}},
    {{-40.0f, 0.0f, 0.0f, 0.0f}}, {{-28.3f, -28.3f, 0.0f, 0.0f}}, {{0.0f, -40.0f, 0.0f, 0.0f}}, {{28.3f, -28.3f, 0.0f, 0.0f}}};   /* 8 probe offsets */
CLQV D_002BF9D0 = {400.0f, -1.0f, 0.0f};
LM D_002BF9E0 = {0.0f, 1000.0f};
int FindSwObjectsByClass(SW *g, int k, int n, int m, int c, STEP **apv);
float ScoreJtPlace(JTPLACES *p, JTPLACE *e, int c, STEP **apv);
qword GetLoWorldPos(void *p);
void AddJtPlaceAtPosition(JTPLACES *p, SOPART *a, qword q);
void AddJtPlaceOnCurve(JTPLACES *p, GRABSPOT *b, float g);
void RecordJtGrabPlace(void *p);
void RecordJtPipePlace(void *p);
void *memmove(void *dst, const void *src, unsigned int cb);
void RemoveJtPlacesOf(JTPLACES *p, LO *a, SOPART *b);
}

// Initializes the player's safe-place table (STEPPLAYER+0x9E0): stores the owner.
extern "C" void InitJtPlaces(JTPLACES *places, STEPPLAYER *jt)
{
    places->player = jt;
}

// Post-load of the safe-place table: nothing to do.
extern "C" void PostLoadJtPlaces(void)
{
}

// World position of a recorded place: an object-local position converted through the object's transform, a curve
// distance evaluated on the object, or a stored world position.
// Also known as: PosSafeEntry (upstream readable / Sly 1 name; PosSafeEntry__FP5ACTORPvf).
extern "C" qword GetJtPlacePosition(STEPPLAYER *jt, JTPLACE *q, float g)
{
    U t;
    MATRIX4Q m;
    void *o;
    qword v;
    qword r;
    if (q->grabSpot) {
        GRABSPOT *s = q->grabSpot;
        s->GetGrabPos(q->grabSpotParam, jt, &t.q, 0);
        o = q->object->GetLocalTransform(0);
        v = TransformXfPos(0, o, t.q);
    } else {
        o = q->part ? q->part->transform : 0;
        v = q->position;
    }
    if (o && 0.0f < g) {
        LoadIdentityMatrix4(o, g, 0, &m, 0);
        r = VuMulMatrix4(m.r[0].q, m.r[1].q, m.r[2].q, m.r[3].q, v);
    } else {
        r = TransformXfPos(o, 0, v);
    }
    if (!q->grabSpot)
        r = VuSub(r, jt->pivotOffset);
    return r;
}

// Builds a place record for a world position and returns the player there (ReturnJtToSafePlace).
// Also known as: RecoverTo (upstream readable / Sly 1 name; RecoverTo__FP5ACTORG9VU_VECTOR).
extern "C" void ReturnJtToPosition(STEPPLAYER *jt, qword q) { JTPLACE f; func_002333B4(&f, 0, sizeof f); f.position = q; ReturnJtToSafePlace(jt, &f); }

// Puts the player back at a safe place (the best recorded one when `place` is null): a ballistic launch toward it
// when reachable (ComputeMaxBallisticTime, SetJtAirJump, ChooseJtLanding), otherwise a teleport with a camera cut.
// Used after falling into hazards and by the hurt callbacks.
// Also known as: RecoverActorTo (upstream readable / Sly 1 name; RecoverActorTo__FP5ACTORPv).
extern "C" void ReturnJtToSafePlace(STEPPLAYER *jt, JTPLACE *q)
{
    JTPLACE e;
    if (!q) {
        ChooseJtSafePlace(&jt->jtplaces, &e);
        q = &e;
        if (jt == GetHeroPo(&g_sw)) {
            qword r = GetJtPlacePosition(jt, &e, 0.0f);
            float g = ComputeMaxBallisticTime(VuSub(r, *(qword *)jt->positionXf->worldMatrix.mat[3]), *(qword *)&jt->physics->gravity, 1000.0f, -1000.0f, 1.0f, -1.0f);
            if (D_002BF940 < g) {
                int save = jt->jtplaces.cplace;
                jt->TranslateToPos(VuSub(r, jt->pivotOffset));
                jt->jtplaces.cplace = save;
                SetStepTargetHeadingFromVector(jt, g_cm.matq.rot[0]);
                jt->RotateToMat(&jt->matTarget);
                CutCm__FP2CM(&g_cm);
                SetJtAirJump(&jt->jtair, 0xB, 0, 0);
                if (e.grabSpot) {
                    if (IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0x13) || IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0xB) || IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0xD) ||
                        IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0x27) || IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0xC) || IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0x22) ||
                        IsBasicDerivedFrom__FP5BASIC3CID(e.object, 0x26)) {
                        UpdateSoBusy(&g_sw, e.object);
                        ChooseJtLanding(&jt->jtair, e.object);
                    }
                }
                StartScreenFade(g_sw.pcid116Fade);
                return;
            }
        }
    }
    jt->jtair.placeTarget = *q;
    SetJtAirJump(&jt->jtair, 0xA, 0, 0);
}

// Evaluate the quadratic g + gFirst*x + gSecond*x^2.
static inline float EvaluateClq(CLQV *clq, float g) { return clq->g + g * (clq->gFirst + g * clq->gSecond); }
// Scores a recorded place: distance from nearby STEP objects (guards) and a line-of-sight / bounding-sphere test
// against them; lower is better. (inferred)
// Also known as: GCostSafeEntry (upstream readable / Sly 1 name; GCostSafeEntry__FP5ACTORPviPP3LO2).
extern "C" float ScoreJtPlace(JTPLACES *places, JTPLACE *q, int n, STEP **apo)
{
    float g = 0.0f;
    qword pos = GetJtPlacePosition(places->player, q, g);
    if (q->part) {
        STACKSCOPE frame;
        U a, b;
        LSG hit;
        int cpso;
        SOPART **apso;
        IntersectSwBoundingSphere(&g_sw, pos, (void *)ShouldKeepSoPair, places->player->physicsTree, &cpso, &apso, D_002BF944);
        a.q = pos;
        b.q = pos;
        a.f[2] += 125.0f;
        b.f[2] -= 175.0f;
        if (!FindFirstObjectHitByLine(0, a.q, b.q, cpso, apso, &hit) || hit.au[0] == 0.0f || ((float *)&hit.anormal[0])[2] < 0.7f)
            g = Max(g, 2000.0f);
        else
            pos = hit.apos[0];
        b.q = pos;
        a.q = pos;
        a.f[2] += 180.0f;
        b.f[2] += 5.0f;
        if (FindFirstObjectHitByLine(0, a.q, b.q, cpso, apso, &hit))
            g = Max(g, 2000.0f);
        for (unsigned k = 0; k < 8; k++) {
            qword v;
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(v) : "j"(pos), "j"(D_002BF950[k].q));
            b.q = v;
            a.q = v;
            a.f[2] += 50.0f;
            b.f[2] -= 30.0f;
            if (!FindFirstObjectHitByLine(0, a.q, b.q, cpso, apso, &hit)) {
                g = Max(g, 1500.0f);
                break;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        STEP *lo = apo[i];
        if (lo == places->player) continue;
        qword d;
        float dist;
        d = VuSub(*lo->probeBottomPos, *(qword *)(void *)&pos);
        d = VuDot(d, d);
        __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d));
        dist = VuToFloat(d);
        g = Max(g, Clamp(EvaluateClq(&D_002BF9D0, dist), D_002BF9E0.minValue, D_002BF9E0.maxValue));
    }
    return g;
}

// .rodata order (docs/guides/tu-merging.md "Phase 5: data order"): a named constant is emitted where it is
// defined, a function's literals and jump tables with the function, so each constant is defined where the ELF
// has it among the functions, under an asm label (no function sees a definition under the name it reads).
namespace RO_ORDER { extern const float r_D_002A66A0 __asm__("D_002A66A0") = 3.40282347e+38f; }
// Picks the best recorded place by ScoreJtPlace against up to 16 STEP objects (CID 0x33).
// Also known as: EntryBestSafe (upstream readable / Sly 1 name; EntryBestSafe__FP5ACTORPv).
extern "C" void ChooseJtSafePlace(JTPLACES *places, JTPLACE *pe)
{
    STEP *apv[16];
    int c, i;
    float best;
    JTPLACE *e;
    c = FindSwObjectsByClass(&g_sw, 0x505, 0x33, 0, 0x10, apv);
    best = D_002A66A0;
    for (i = 0; i < places->cplace; i++) {
        float g;
        e = &places->aplace[i];
        g = ScoreJtPlace(places, e, c, apv);
        if (g < best) {
            best = g;
            *pe = *e;
            if (g == 0.0f)
                return;
        }
    }
    if (best != D_002A66A0)
        return;
    qword r;
    func_002333B4(pe, 0, 0x20);
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(GetLoWorldPos(places->player)), "j"(places->player->pivotOffset));
    pe->position = r;
}

// Per frame: when standing on stable ground (a ground state, floor normal z > 0.7, floor not flagged) records the floor
// object and position; the grab and pipe states record their own spot (RecordJtGrabPlace, RecordJtPipePlace).
// Also known as: UpdateSafeGround (upstream readable / Sly 1 name; UpdateSafeGround__FP5ACTOR).
extern "C" void UpdateJtPlaces(JT *jt)
{
    VECTORV v = D_002A2DD0;
    switch (jt->state) {
    case 0: case 1: case 5: case 6: case 9: case 10: case 11: case 12:
    case 15: case 16: case 17: case 20: case 23: case 24:
    {
        XP *pa = jt->floorContact;
        if (pa && pa->normalVec.z > 0.7f && !pa->axps[1].pso->isNotLandable && !jt->pwater)
            AddJtPlaceAtPosition(&jt->jtplaces, pa->axps[1].ppart, pa->axps[1].posLocal);
        break;
    }
    case 3: case 22:
    {
        B *pb = (B *)jt->hangHandle;
        if (!pb->unknown10C)
            AddJtPlaceOnCurve(&jt->jtplaces, pb ? (GRABSPOT *)((char *)pb + 0x100) : 0, 0.0f);
        break;
    }
    case 7:
        RecordJtGrabPlace(&jt->jtgrab);
        break;
    case 8:
        RecordJtPipePlace(&jt->jtpipe);
        break;
    }
}

// Inserts (object, curve distance) at the front of the place table, merging an entry on the same object within 50.
// Also known as: RecordSafeTarget (upstream readable / Sly 1 name; RecordSafeTarget__FP5ACTORP2LOf).
extern "C" void AddJtPlaceOnCurve(JTPLACES *places, GRABSPOT *s, float g)
{
    int i;
    JTPLACE *e;
    for (i = 1; i < places->cplace; i++) {
        e = &places->aplace[i];
        if (s == e->grabSpot && __builtin_fabsf(g - e->grabSpotParam) < 50.0f) {
            if (i != 1) {
                JTPLACE t = *e;
                memmove(&places->aplace[2], &places->aplace[1], (i - 1) * sizeof(JTPLACE));
                places->aplace[1] = t;
            }
            break;
        }
    }
    if (i >= places->cplace) {
        if ((unsigned int)places->cplace < 32)
            places->cplace++;
        memmove(&places->aplace[1], &places->aplace[0], places->cplace * sizeof(JTPLACE) - sizeof(JTPLACE));
    }
    e = &places->aplace[0];
    func_002333B4(e, 0, sizeof(JTPLACE));
    places->aplace[0].object = s->GetOwner();
    e->grabSpot = s;
    e->grabSpotParam = g;
    e->rememberedTime = g_clock.time;
    places->aplace[0].object->clearsPlacesOnRemove = 1;
}

// Distance between a and b as a float.
static __inline__ float Dist(qword a, qword b)
{
    qword r, t;
    float g;
    r = VuSub(a, b);
    __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q"
            : "+j"(r), "=&j"(t));
    g = VuToFloat(r);
    return g;
}
// Inserts (object, position) at the front of the place table, merging an entry within 50 units.
// Also known as: RecordSafeGround (upstream readable / Sly 1 name; RecordSafeGround__FP5ACTORP3SOEG9VU_VECTOR).
extern "C" void AddJtPlaceAtPosition(JTPLACES *places, SOPART *s, qword q)
{
    int i;
    JTPLACE *e;
    for (i = 1; i < places->cplace; i++) {
        e = &places->aplace[i];
        if (s == e->part && Dist(q, e->position) < 50.0f) {
            if (i != 1) {
                JTPLACE t = *e;
                memmove(&places->aplace[2], &places->aplace[1], (i - 1) * sizeof(JTPLACE));
                places->aplace[1] = t;
            }
            break;
        }
    }
    if (i >= places->cplace) {
        if ((unsigned int)places->cplace < 32)
            places->cplace++;
        memmove(&places->aplace[1], &places->aplace[0], places->cplace * sizeof(JTPLACE) - sizeof(JTPLACE));
    }
    e = &places->aplace[0];
    func_002333B4(e, 0, sizeof(JTPLACE));
    if (s) {
        places->aplace[0].object = s->owner;
        e->part = s;
        s->hasPlaces = 1;
    }
    places->aplace[0].position = q;
    e->rememberedTime = g_clock.time;
}

// Compacts the place table, dropping entries that reference the given object.
// Also known as: RemoveSafeGroundReferences (upstream readable / Sly 1 name; RemoveSafeGroundReferences__FP2LOP3SOEPv).
extern "C" void RemoveJtPlacesOf(JTPLACES *s, LO *p0, SOPART *p8)
{
    JTPLACE *dst = s->aplace;
    JTPLACE *src = dst;
    JTPLACE *end = &s->aplace[s->cplace];
    for (; src < end; src++) {
        if (p0 && p0 == src->object)
            continue;
        if (p8 && p8 == src->part)
            continue;
        if (src != dst)
            *dst = *src;
        dst++;
    }
    s->cplace = dst - s->aplace;
}

// Purges a removed object from the place tables of all three players (Sly g_pjt, and the two other player globals
// D_002DD5BC and D_002F7900, presumably Bentley and Murray).
extern "C" void RemoveSwPlacesOf(void *g, LO *a, SOPART *b)
{
    if (D_002E1E40)
        RemoveJtPlacesOf(&D_002E1E40->jtplaces, a, b);
    if (D_002DD5BC)
        RemoveJtPlacesOf(&D_002DD5BC->jtplaces, a, b);
    if (D_002F7900)
        RemoveJtPlacesOf(&D_002F7900->jtplaces, a, b);
    if (a)
        a->clearsPlacesOnRemove = 0;
    if (b)
        b->hasPlaces = 0;
}
