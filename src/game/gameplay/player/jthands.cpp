// jthands.cpp -- JTHANDS (sly2/stepplayer.h), the hero's hands record: two hold clips sampled into curves for the hand
// and the chain top (InitJtHands), fitting them to the grab target every frame (UpdateJtHands), choosing a target,
// and carrying / attaching / releasing a held object with position and orientation acts.
// TU 0x1CC158..0x1CD5F0 (17 functions, address order).
// Entry points: InitJtHands__FP7JTHANDSP2SOPP5ASEGA (setup), SetJtHandsTarget, UpdateJtHands (per frame),
// AttachJtHandsObject / ReleaseJtHandsActs.
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_jthands
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_jthands
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/clock.h"
#include "sly2/sw.h"

// 0x1CD498 StaticInit_jthands and GlobalConstructors_jthands: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Representation (docs/guides/tu-merging.md "Representation decisions"): every function works on the header JTHANDS
   (the per-file P/S records mapped onto it field for field). Matrices that inline asm moves as quadword rows use the
   TU-private M3/M4 (qword rows) and are converted at the boundary to the header's float MATRIX3/MATRIX4. The hero is
   the header STEPPLAYER (its target rows through STEP::aqwTarget); the hand target the headers do not name is a view
   derived from LO (HANDATTACH). */

union U { qword q; float f[4]; };
struct M3 { qword r[3]; };
struct M4 { qword r[4]; };

/* Hand-made virtual calls (the vtable pointer of an LO is at +0xC). */
/* a macro, not an inline function: an inline function's parameter becomes a new pseudo, which moves a pinned object
   pointer into another register (StartJtHandsCarry) */
#define PVT(plo) (*(VTE **)((char *)(plo) + 0xC))

/* The hero (a JT) as far as this TU reads it beyond SO: the hand-target class mask, the hero's rotation and the ASEGA
   currently playing. */

/* A hand target of CID 0x14 (placeholder name): its class mask at +0xE0. */
struct HANDATTACH : LO
{
    char pad50[0xE0 - 0x50];
    uint grfHand;           /* +0xE0 */
};


/* Context of the curve-distance callbacks (MinimizeRange's pv): the hands and the point to fit. */
struct JTHANDSCTX { JTHANDS *phands; qword pos; };

/* The callback PMF passed by value from the `.c` file (see "callback" in docs/guides/tu-merging.md). */

extern "C" {
/* jthands' .data (address order): the carried-shape point count, the default gravity (VQ: initializable, read as a
 * qword), the hand-follow distance and the act delay. */
union VQ { float af[4]; qword q; };
int D_002C1ED0 = 9;
VQ D_002C1EE0 = {{0.0f, 0.0f, -2250.0f, 0.0f}};   /* gravity when the object has no SOPHYS */
float D_002C1EF0 = 400.0f, D_002C1EF4 = 0.25f;
extern qword D_002A2DD0;
extern const LONGU D_002A77C8;   /* PMF of UpdateJtHandsCallback */
void EvaluateShapeAtParam(SHAPE *pshape, float u, qword *ppos, qword *pdir);
void FindClosestPointOnLineSegment(qword q, qword a, qword b, qword *ppos, float *pu, float *ps);
qword GetLoWorldPos(LO *plo);
void FindShapeClosestPointFromDistance(void *pshape, qword pos, void *p, qword *ppos, qword *pdir, float *pu, float *ps, float s);
void FindShapeClosestPoint(LO *pshape, qword pos, void *a, qword *b, qword *c, float *d, float *ps);
M4 InvertMatrix4Rigid(const MATRIX4 *pmat);
qword ConvertPosBetweenLos(LO *ploFrom, LO *ploTo, qword pos);
M4 ConvertMat4BetweenLos(LO *ploFrom, LO *ploTo, const M4 *pmat);
M3 *ConvertXfMat(M3 *pmatOut, XF *pxfFrom, XF *pxfTo, const M3 *pmat);
qword ConvertXfVec(XF *pxfFrom, XF *pxfTo, qword vec);
qword TransformXfPos(XF *pxfFrom, XF *pxfTo, qword pos);
qword DecomposeRotationMatrixToEuler(const M3 *pmat);
void SetActPositionGoal(ACT *pact, qword pos);
void SeekAsega(ASEGA *pasega, int n, float t, float g);
void RecalculateAsegBlend(ASEGBL *pasegbl);
void SetStepTargetHeadingFromVector(LO *plo, qword vec);
void FixStepAngularVelocity(LO *plo);
void ReleaseAct(ACT **ppact);
SO *GetSoFromLo(LO *plo);
int MaxInt(int grf, int n);   /* NMaxIgnore */
int FindSwObjectsByClass(SW *psw, int grffso, int cid, LO *plo, int cploMax, LO **aplo);
int IsBasicDerivedFrom__FP5BASIC3CID(LO *plo, int cid);
qword GetXfValueFromMatrix(XF *pxfA, XF *pxfB);
ACT *NewPositionAct(LO *plo, XF *pxf, int a, int b, int c, qword qw);
ACT *NewBezierAct(LO *plo, XF *pxf, int n, float t, qword pos, qword vel);
void EnsureSwCallback(SW *psw, SO *pso, long pmf, JTHANDS *phands);
float GetAsegLabelTime(ASEG *paseg, int n);
void PredictAsegTransform(ASEG *paseg, float t, float g, LO *plo, XF *pxfA, XF *pxfB, M4 *pmatA, M4 *pmatB);
qword *VECTOR_opAssign(qword *pdst, qword src);
void SetLoParent(LO *plo, int a, int b);
float ComputeMaxBallisticTime(qword dpos, qword vel, float a, float b, float c, float d);
void SetAsegaSpeed__FP5ASEGAf(ASEGA *pasega, float svt);
void SetSoTreeFlag2(LO *plo, int n);
void SetLoXfFromMat(LO *plo, M4 *pmat);
void SetSoCnstrForce(SO *pso, int n);
void SetSoCnstrTorque(SO *pso, int n);
void FadeSoIn(SO *pso, float t);
void *GetLoWorldMatrix(LO *plo);

void GetClosestPointOnJtHandsCurveA(JTHANDS *phands, qword pos, float *pu, float *ps, float u);
void GetClosestPointOnJtHandsCurveB(JTHANDS *phands, qword pos, float *pu, float *ps, float u);
float EvaluateJtHandsCurveA(JTHANDSCTX *pctx, float u);
float EvaluateJtHandsCurveB(JTHANDSCTX *pctx, float u);
void UpdateJtHands(JTHANDS *phands);
HANDATTACH *FindJtHandsTarget(STEPPLAYER *pjt, LO *plo);
void SetJtHandsTarget(JTHANDS *phands, int nMode);
}
/* Per-caller views of callees that other callers declare differently (see the mismatch table). */
/* operator*(const MATRIX4 &, const MATRIX4 &) with its hidden result pointer passed by hand, on quadword rows (an
   extern "C" declaration of the literal mangled name next to the C++ operator* hits ICE 378). */
extern "C" M4 *MulMatrix4Ret(M4 *pmatOut, const M4 *pmatL, const M4 *pmatR) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");
extern "C" void MulMatrix4RetV(M4 *pmatOut, const M4 *pmatL, const M4 *pmatR) SLY2_SRET_LABEL("__ml__FRC7MATRIX4T0", "MulMatrix4Into");   /* as AttachJtHandsObject calls it */
/* the same ABI as NewBezierAct (the float goes to $f12 either way); the order decides argument evaluation */
extern "C" ACT *func_0012DE58Q(LO *plo, XF *pxf, int n, qword pos, qword vel, float t) __asm__("NewBezierAct");
extern "C" M4 InvertMatrix4RigidP(const M4 *pmat) __asm__("InvertMatrix4Rigid");
extern "C" M4 InvertMatrix4RigidV(M4 mat) __asm__("InvertMatrix4Rigid");
extern "C" M3 ConvertXfMatR(XF *pxfFrom, XF *pxfTo, void *pmat) __asm__("ConvertXfMat");
extern "C" qword func_0011BFF0V(M3 mat) __asm__("DecomposeRotationMatrixToEuler");
extern "C" void MinimizeRange(float (*pfn)(void *, float), void *pv, float a, float b, float c, float d, float *pu, float *pg) __asm__("MinimizeRange__FPFPvf_fPvffffPfT6");

// Clears the hands' two target ids (+0x90, +0xA4) to -1.
extern "C" void ResetJtHands(JTHANDS *hands) { hands->mode = -1; hands->holdKind = -1; }

// Closest point to `point` on the segment between the two A curves (the hand transform's path) evaluated at t.
// Also known as: GReachDist (upstream readable / Sly 1 name; GReachDist__FP5REACHG9VU_VECTORP6VECTORPff).
extern "C" void GetClosestPointOnJtHandsCurveA(JTHANDS *hands, qword pos, float *pu, float *ps, float u)
{
    qword a, b;
    EvaluateShapeAtParam(hands->abl[0].pshapeA, u, &a, 0);
    EvaluateShapeAtParam(hands->abl[1].pshapeA, u, &b, 0);
    FindClosestPointOnLineSegment(pos, a, b, 0, pu, ps);
}

// Same as GetClosestPointOnJtHandsCurveA, for the B curves (the chain-top transform's path).
// It returns the closest point on the segment between the two B shapes at t.
extern "C" void GetClosestPointOnJtHandsCurveB(JTHANDS *hands, qword pos, float *pu, float *ps, float u)
{
    qword a, b;
    EvaluateShapeAtParam(hands->abl[0].pshapeB, u, &a, 0);
    EvaluateShapeAtParam(hands->abl[1].pshapeB, u, &b, 0);
    FindClosestPointOnLineSegment(pos, a, b, 0, pu, ps);
}

// Parameter callback for curve A, used by FindShapeClosestPointFromDistance.
extern "C" float EvaluateJtHandsCurveA(JTHANDSCTX *context, float u) { float g; GetClosestPointOnJtHandsCurveA(context->phands, context->pos, 0, &g, u); return g; }

// Parameter callback for curve B.
extern "C" float EvaluateJtHandsCurveB(JTHANDSCTX *context, float u) { float g; GetClosestPointOnJtHandsCurveB(context->phands, context->pos, 0, &g, u); return g; }

/* UpdateJtHands: find the target point (mode 0: closest point on the target path; 1: the reference object; 2: the
   reference object's frame relative to the hand chain), fit the hand curves to it (MinimizeRange over the clip
   parameter), seek the hold clip there and blend the two hands, set the position goal (mode 2) and the step's turn
   target. Hand-scheduled: the work area is one union of the early (matrix) and late (fit) stages, the quadword copy
   of the product and basis is one asm block, and the pins reproduce the target's register choice. This form exists
   to reproduce the original codegen. */
struct CTXCOPY { JTHANDSCTX context; U local; M3 transformed, temporary; };
struct EARLY { M4 product, temporary, inverse; };
union STAGE { EARLY early; CTXCOPY late; };
struct WORK { U position; M3 basis; STAGE stage; float t, result, projection; U saved; };
// Placement new: construct at destination.
inline void *operator new(unsigned size, void *destination) { return destination; }
struct COPYPREFIX { U position; M3 basis; M4 product; };
// Publish the product and basis words of the work record (register-pinned) and return the hands context.
static inline JTHANDSCTX *PublishProductAndBasis(WORK *w)
{
    register qword first __asm__("$6"), last __asm__("$4"), second __asm__("$2"), third __asm__("$3");
    register JTHANDSCTX *address __asm__("$5"), *context __asm__("$16");
    __asm__("lq %0,128(%7)\n\taddiu %4,%7,64\n\tlq %1,176(%7)\n\tdaddu %5,%4,$zero\n\tlq %2,144(%7)\n\tlq %3,160(%7)\n\tsq %2,80+%6\n\tsq %3,96+%6\n\tsq %1,%6\n\tsq %0,16+%6\n\tsq %0,64+%6\n\tsq %1,112+%6\n\tlq %2,16(%4)\n\tsq %2,32+%6\n\tlq %3,32(%4)\n\tsq %3,48+%6"
            : "=&r"(first), "=&r"(last), "=&r"(second), "=&r"(third), "=&r"(address), "=&r"(context), "=m"(*(COPYPREFIX *)w)
            : "r"(w), "m"(w->stage.early.temporary));
    return context;
}
// XY length of v (x lane, vf2).
static inline qword LengthXY(qword v)
{
    register qword d __asm__("vf2");
    qword word;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(word) : "j"(d));
    return word;
}
// Store the x lane of q to *p.
static inline void StoreX(float *p, qword q) { __asm__("sw %1,%0" : "=m"(*p) : "r"(q)); }
// *saved - raw, also fetching the hands' carried object (register-pinned).
static inline qword DifferenceFromSaved(const U *saved, qword raw, JTHANDS *hands, SO *&receiver)
{
    register qword point __asm__("vf1"), current __asm__("vf2");
    register SO *owner __asm__("$4");
    __asm__("lqc2 %0,%3\n\tqmtc2.ni %4,%1\n\tlw %2,%5\n\tvsub.xyzw %0,%0,%1"
            : "=&j"(point), "=&j"(current), "=r"(owner) : "m"(*saved), "r"(raw), "m"(hands->owner));
    receiver = owner;
    return point;
}
// Copy a matrix pointer into $6.
static inline const M4 *InverseArgument(const M4 *value)
{
    register const M4 *arg __asm__("$6");
    __asm__("daddu %0,%1,$zero" : "=r"(arg) : "r"(value));
    return arg;
}

// Fits the hand curves to the grab target: finds the best clip parameter, seeks/reblends the hold clips, and sets the
// actor's position goal and the step's turn target.
// Also known as: UpdateReach (upstream readable / Sly 1 name; UpdateReach__FP5REACH).
extern "C" void UpdateJtHands(JTHANDS *hands)
{
    WORK w;
    register void *cursor __asm__("$16");
    if (hands->mode == -1) return;
    switch (hands->holdKind) {
    default:
        cursor = &w.stage.late.context;
        break;
    case 0: {
        cursor = hands->ploTarget;
        FindShapeClosestPointFromDistance(cursor, GetLoWorldPos(hands->owner), 0, &w.position.q, 0, 0, &hands->distanceAlong, hands->distanceAlong);
        cursor = &w.stage.late.context;
        break;
    }
    case 1: {
        qword found = GetLoWorldPos(hands->ploRef);
        __asm__("sq %1,%0" : "=m"(w.position.q) : "r"(found));
        cursor = &w.stage.late.context;
        break;
    }
    case 2: {
        M4 *output = &w.stage.early.temporary;
        M4 *world = (M4 *)((LO *)hands->ploRef)->GetWorldMatrix();
        cursor = &w.stage.early.inverse;
        new (cursor) M4(InvertMatrix4Rigid(&hands->matChain));
        const M4 *inverseArg = InverseArgument((M4 *)cursor);
        MulMatrix4Ret(output, world, inverseArg);
        cursor = PublishProductAndBasis(&w);
        break;
    }
    }
    w.stage.late.context.phands = hands;
    {
        qword raw = ConvertPosBetweenLos(0, hands->owner, w.position.q);
        register qword point __asm__("vf1");
        __asm__("qmtc2.ni %1,%0" : "=j"(point) : "r"(raw));
        qword length = LengthXY(point);
        w.stage.late.local.q = point;
        w.stage.late.local.f[1] = 0.0f;
        StoreX(&w.stage.late.local.f[0], length);
    }

    w.stage.late.context.pos = ConvertPosBetweenLos(hands->owner, 0, w.stage.late.local.q);
    if (hands->holdKind == 2) {
        MinimizeRange((float (*)(void *, float))EvaluateJtHandsCurveB, cursor, 0.5f, 0.1f, 0.0f, 1.0f, &w.t, &w.result);
        GetClosestPointOnJtHandsCurveB(hands, w.stage.late.context.pos, &w.projection, 0, w.t);
        ConvertXfMat(&w.stage.late.temporary, 0, hands->handTransform->parent, &w.basis);
        w.stage.late.transformed = w.stage.late.temporary;
        SetActPositionGoal(hands->pact1Euler, DecomposeRotationMatrixToEuler(&w.stage.late.transformed));
    } else {
        MinimizeRange((float (*)(void *, float))EvaluateJtHandsCurveA, cursor, 0.5f, 0.1f, 0.0f, 1.0f, &w.t, &w.result);
        GetClosestPointOnJtHandsCurveA(hands, w.stage.late.context.pos, &w.projection, 0, w.t);
    }
    SeekAsega(*hands->ppasega, 0, w.t * hands->blendedAnimation->duration, 0.0f);
    hands->abl[1].weight = w.projection;
    hands->abl[0].weight = 1.0f - w.projection;
    RecalculateAsegBlend(hands->blendedAnimation);
    w.saved.q = w.position.q;
    SO *receiver;
    qword delta = DifferenceFromSaved(&w.saved, GetLoWorldPos(hands->owner), hands, receiver);
    SetStepTargetHeadingFromVector(receiver, delta);
    FixStepAngularVelocity(hands->owner);
}

/* InitJtHands (Lilly's port of Sly 2's jthands.c on the header classes): the matrix of the hand object's XF chain up
   to the first rotation XF (xfk 6), the hand's frame in the hero's; for each hand that has a hold clip, two curves
   (CID 10 SHAPEs of D_002C1ED0 points, removed from the world again): the path the hand object's XF (A) and the
   chain's top XF (B) take through the clip, sampled with PredictAsegEffect and carried to the hand's frame. With both
   clips the blend of the two (weights 1 and 0 to start with) is made. */
typedef unsigned size_t;
extern "C" void *memset(void *ptr, int value, size_t num);
// Set matrix to the identity.
inline void SetMatrix4Identity(MATRIX4 *matrix)
{
    memset(matrix, 0, sizeof(MATRIX4));
    matrix->mat[3][3] = 1.0f;
    matrix->mat[2][2] = 1.0f;
    matrix->mat[1][1] = 1.0f;
    matrix->mat[0][0] = 1.0f;
}
MATRIX4 operator*(const MATRIX4 &matL, const MATRIX4 &matR);
// Transform pos by the 4x4 matrix.
inline qword TransformPos(MATRIX4 *matrix, qword pos)
{
    qword result;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(result)
            : "j"(*(qword *)matrix->mat[0]), "j"(*(qword *)matrix->mat[1]), "j"(*(qword *)matrix->mat[2]), "j"(*(qword *)matrix->mat[3]),
              "j"(pos));
    return result;
}
void *AllocFromWorldHeap(SWALLOC *pswalloc) __asm__("AllocFromWorldHeap");
void EnsureAsegBlendDynamic(ALO *palo, int cbBl, int cbl, void *abl, int cmrsgc, int *mpimrsgcib, int *mpimrsgccmrsg, ASEGBL **ppasegbl);
LO *CreateLo(CID cid, SO *psoParent, OID oid, int isplice);
extern "C" void PredictAsegEffect(float tLocal, ASEG *paseg, SO *psoRoot, float svt, XF *pxfEffect, XF *pxfRef, MATRIX4 *pmatOut, int f) __asm__("PredictAsegTransform");
extern "C" void MeasureShape(SHAPE *pshape);
// Allocate cb bytes with the given alignment from the world heap.
inline void *AllocFromWorldHeapAligned(int cb, int alignment)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *allocator = &swalloc;
    allocator->cbAlign = alignment;
    return AllocFromWorldHeap(allocator);
}

// Builds the hands record: the hand-frame matrix, the sampled A/B curves per hold clip (PredictAsegEffect) and the
// blend between the clips.
// Also known as: BindReach (upstream readable / Sly 1 name; BindReach__FP5REACHP5ACTORPP5ASEGA).
void InitJtHands(JTHANDS *hands, SO *so, ASEGA **asegaOut)
{
    hands->owner = so;
    hands->ppasega = asegaOut;
    if (!hands->ploHand)
        return;
    MATRIX4 matHand;
    XF *pxfHand = hands->ploHand->GetLocalTransform(&matHand);
    hands->matChain = matHand;
    hands->handTransform = pxfHand;
    while (hands->handTransform->kind != 6)
    {
        hands->matChain = hands->handTransform->localMatrix * hands->matChain;
        hands->handTransform = hands->handTransform->parent;
    }
    int cHands = 0;
    for (int i = 0; i < 2; i++)
    {
        ASEG *aseg = hands->abl[i].animation;
        if (!aseg)
            continue;
        cHands++;
        for (int j = 0; j < 2; j++)
        {
            SHAPE *shape = (SHAPE *)CreateLo((CID)10, hands->owner, (OID)0xB2, -1);
            shape->curveKind = (CRVK)0;
            shape->numControlVertices = D_002C1ED0;
            shape->vertexParams = (float *)AllocFromWorldHeapAligned(D_002C1ED0 << 2, 16);
            shape->vertexArcLengths = (float *)AllocFromWorldHeapAligned(D_002C1ED0 << 2, 16);
            shape->vertexPositions = (VECTOR4 *)AllocFromWorldHeapAligned(D_002C1ED0 << 4, 16);
            MATRIX4 matSample;
            XF *pxfEffect;
            if (j == 0)
            {
                matSample = matHand;
                pxfEffect = pxfHand;
            }
            else
            {
                pxfEffect = hands->handTransform;
                SetMatrix4Identity(&matSample);
            }
            for (int k = 0; k < D_002C1ED0; k++)
            {
                MATRIX4 matOut;
                float u = (float)k / (float)(D_002C1ED0 - 1);
                PredictAsegEffect(u * aseg->duration, aseg, hands->owner, 0.0f, pxfEffect, hands->owner->rootTransform, &matOut, 0);
                shape->vertexParams[k] = u;
                qword pos = TransformPos(&matOut, *(qword *)matSample.mat[3]);
                shape->vertexPositions[k] = *(VECTOR4 *)&pos;
            }
            shape->Remove();
            MeasureShape(shape);
            if (j == 0)
                hands->abl[i].pshapeA = shape;
            else
                hands->abl[i].pshapeB = shape;
        }
    }
    if (cHands == 2)
    {
        hands->abl[0].weight = 1.0f;
        EnsureAsegBlendDynamic(hands->owner, sizeof(hands->abl[0]), 2, hands->abl, 0, 0, 0, &hands->blendedAnimation);
    }
}

/* The grab target's tree is ignored while it is held (state 1). */
// While the hands hold an object (state 1) adds intersection-ignore bits for its object tree.
extern "C" int GetJtHandsIgnoreFlags(JTHANDS *hands, SO *so, int grf)
{
    if (hands->mode == 1) {
        SO *psoTarget = GetSoFromLo(hands->ploTarget);
        if (psoTarget && psoTarget->physicsTree && so->physicsTree == psoTarget->physicsTree)
            grf = MaxInt(grf, 3);
    }
    return grf;
}

// World callback wrapper that runs UpdateJtHands.
extern "C" void UpdateJtHandsCallback(void *pv, JTHANDS *hands)
{
    UpdateJtHands(hands);
}

// The first nearby object of class 0x14 whose flags match the hands' flags.
// Also known as: PloReachGrip (upstream readable / Sly 1 name; PloReachGrip__FP5ACTORi).
extern "C" HANDATTACH *FindJtHandsTarget(STEPPLAYER *jt, LO *lo)
{
    HANDATTACH *apcid[4];
    int c = FindSwObjectsByClass(&g_sw, 0x401, 0x14, lo, 4, (LO **)apcid);
    int i;
    for (i = 0; i < c; i++) {
        if (apcid[i]->grfHand & jt->grfGrabTarget) return apcid[i];
    }
    return 0;
}

// Chooses the grab target (closest point on a shape, or a class-0x14 object), creates the position act, reblends and
// schedules UpdateJtHandsCallback.
// Also known as: SetReachState (upstream readable / Sly 1 name; SetReachState__FP5REACHi).
extern "C" void SetJtHandsTarget(JTHANDS *hands, int nMode)
{
    if (nMode == hands->mode) return;
    if (hands->mode == 1) {
        ReleaseAct(&hands->pactPos);
        ReleaseAct(&hands->pactTurn);
    }
    hands->mode = nMode;
    hands->modeStartTime = g_clock.time;
    switch (nMode) {
    case 0:
    case 1:
        hands->ploRef = hands->ploTarget;
        hands->holdKind = 1;
        if (IsBasicDerivedFrom__FP5BASIC3CID(hands->ploTarget, 10)) {
            LO *shape = hands->ploTarget;
            FindShapeClosestPoint(shape, GetLoWorldPos(hands->owner), 0, 0, 0, 0, &hands->distanceAlong);
            hands->holdKind = 0;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(hands->ploTarget, 0x14) && (((HANDATTACH *)hands->ploTarget)->grfHand & ((STEPPLAYER *)hands->owner)->grfGrabTarget)) {
            hands->holdKind = 2;
        } else if (IsBasicDerivedFrom__FP5BASIC3CID(hands->ploTarget, 0x30)) {
            HANDATTACH *pcid = FindJtHandsTarget((STEPPLAYER *)hands->owner, hands->ploTarget);
            if (pcid) {
                hands->ploRef = pcid;
                hands->holdKind = 2;
            }
        }
        if (hands->holdKind == 2)
            hands->pact1Euler = NewPositionAct(hands->owner, hands->handTransform, 5, 7, 3, GetXfValueFromMatrix(hands->handTransform, hands->handTransform));
        hands->abl[1].weight = hands->abl[0].weight = 0.5f;
        RecalculateAsegBlend(hands->blendedAnimation);
        EnsureSwCallback(&g_sw, hands->owner, D_002A77C8.l, hands);
        break;
    case -1:
        ReleaseAct(&hands->pact1Euler);
        hands->holdKind = nMode;
        hands->ploTarget = 0;
        hands->distanceAlong = 0;
        hands->ploRef = 0;
        break;
    }
}

/* StartJtHandsCarry (StartJtHandsCarry): create the position and orientation acts that carry the object along the
   predicted clip path to the hand, timed ballistically (ComputeMaxBallisticTime), parented to the world (SetLoParent). The body
   is a transcription of the target's schedule into register-pinned asm blocks (a matching island: this form exists
   to reproduce the original codegen); its data is the header JTHANDS and SO, STEPPLAYER, and the work area
   below. */
union VIEWS { struct { M4 intermediate, product, inverse; } matrices; struct { M3 angles, converted; } basis; U scalar; };
struct CARRYWORK { M4 original, sample, start, final; VIEWS views; qword velocity, destination, delta, endVelocity; };
// The object's source matrix into out (virtual slot 32).
static inline XF *SourceMatrix(LO *lo, M4 *out) { return lo->GetLocalTransform((MATRIX4 *)out); }
// The object's world matrix (virtual slot 31).
static inline M4 *WorldMatrix(LO *lo) { return (M4 *)lo->GetWorldMatrix(); }
// Address of the work record's translation slot (+0xB0), in $4.
static inline qword *TranslationSlot(CARRYWORK &w) { register qword *slot __asm__("$4"); __asm__("addiu %0,%1,0xB0" : "=r"(slot) : "r"(&w)); return slot; }
// Copy the matrix pointer into $8.
static inline M4 *SampleOutput(M4 *value) { register M4 *result __asm__("$8"); __asm__("daddu %0,%1,$zero" : "=r"(result) : "r"(value)); return result; }
// Difference to the destination and the speed needed to cover it in the duration (register-pinned).
static inline void DeltaForDuration(CARRYWORK &w, qword raw, float &bound, qword &speed)
{
    register qword destination __asm__("vf1"), difference __asm__("vf2");
    register unsigned page __asm__("$3");
    __asm__("lqc2 %0,%3\n\tlui %1,%%hi(D_002C1EF0)\n\tqmtc2.ni %4,%2" : "=j"(destination), "=&r"(page), "=j"(difference) : "m"(w.destination), "r"(raw));
    register float limit __asm__("$f12");
    register qword velocity __asm__("$5");
    __asm__("lwc1 %0,%%lo(D_002C1EF0)(%4)\n\tvsub.xyzw %1,%5,%1\n\tlq %2,%6\n\tsqc2 %1,%3"
            : "=f"(limit), "+j"(difference), "=r"(velocity), "=m"(w.delta) : "r"(page), "j"(destination), "m"(w.velocity), "m"(*(float *)(page + 0x1EF0)));
    bound = limit;
    speed = velocity;
}
struct __attribute__((packed)) BASIS_TAIL { float firstW; qword row1, row2; };

// Creates position and orientation jobs that carry the selected animated object along the predicted clip path to the
// hand (ballistic time, reparenting).
// Also known as: StartReachGrab (upstream readable / Sly 1 name; StartReachGrab__FP5REACHP2LO).
#ifdef SLY2_EE_ASM
extern "C" void StartJtHandsCarry(JTHANDS *input, SO *other)
{
    register JTHANDS *p __asm__("$22");
    register SO *object __asm__("$20");
    __asm__("daddu %0,%1,$zero" : "=r"(p) : "r"(input));
    __asm__("daddu %0,%1,$zero" : "=r"(object) : "r"(other));
    CARRYWORK work;
    ReleaseAct(&p->pactPos);
    register float zero __asm__("$f21") = 0.0f;
    ReleaseAct(&p->pactHold);
    ReleaseAct(&p->pactTurn);
    ASEGA *anim = ((STEPPLAYER *)p->owner)->activeAnimation;
    ASEG *motion = anim->animation;
    float time = GetAsegLabelTime(motion, 0x1C7), phase = (time - anim->time) / anim->speed;
    XF *space = SourceMatrix(p->ploHand, &work.original);
    register M4 *workingBase __asm__("$19") = &work.sample;
    PredictAsegTransform(motion, time, zero, p->owner, space, p->owner->rootTransform, SampleOutput(workingBase), 0);
    STEPPLAYER *owner = (STEPPLAYER *)p->owner;
    M4 *start = &work.start;
    qword *basis = owner->aqwTarget;
    register M4 *product __asm__("$21");
    qword location = GetLoWorldPos(owner);
    VECTOR_opAssign(TranslationSlot(work), location);
    {
        register qword firstRow __asm__("$2");
        register M4 *intermediate __asm__("$16");
        register float homogeneous __asm__("$f0");
        __asm__("lq %0,%2\n\taddiu %1,%3,0x140" : "=&r"(firstRow), "=r"(product) : "m"(owner->aqwTarget[0]), "r"(&work));
        __asm__(".set noat\n\tlui $at,0x3F80\n\tmtc1 $at,%0\n\t.set at\n\taddiu %1,%3,0x100\n\tsq %4,%2"
                : "=f"(homogeneous), "=&r"(intermediate), "=m"(work.start.r[0])
                : "r"(&work), "r"(firstRow)
                : "$1");
        register const M4 *left __asm__("$5"), *right __asm__("$6");
        register M4 *destination __asm__("$4");
        __asm__("daddu %0,%3,$zero\n\tdaddu %1,%4,$zero\n\tdaddu %2,%5,$zero"
                : "=&r"(left), "=&r"(right), "=r"(destination)
                : "r"(start), "r"(workingBase), "r"(product));
        register qword row1 __asm__("$3"), row2 __asm__("$2");
        __asm__("lq %0,16(%4)\n\tdaddu %2,%6,$zero\n\tsq %0,16(%5)\n\tlq %1,32(%4)\n\tsq %1,32(%5)\n\tswc1 "
                "%7,%3\n\tswc1 %7,32+%3\n\tswc1 %7,16+%3"
                : "=&r"(row1), "=&r"(row2), "=&r"(workingBase), "=m"(*(BASIS_TAIL *)((char *)&work.start + 12))
                : "r"(basis), "r"(start), "r"(intermediate), "f"(zero));
        ((U *)&start->r[3])->f[3] = homogeneous;
        MulMatrix4Ret(destination, left, right);
        register const M4 *originalArgument __asm__("$6");
        register M4 *secondDestination __asm__("$4");
        __asm__("daddu %0,%2,$zero\n\tdaddu %1,%3,$zero"
                : "=r"(originalArgument), "=r"(secondDestination)
                : "r"(&work.original), "r"(intermediate));
        MulMatrix4Ret(secondDestination, product, originalArgument);
    }
    HANDATTACH *counterpart;
    {
        register qword first __asm__("$7"), row1 __asm__("$2"), row2 __asm__("$3"), row3 __asm__("$6");
        register SO *objectArgument __asm__("$5");
        register STEPPLAYER *ownerArgument __asm__("$4");
        __asm__("lq %0,%2\n\tdaddu %1,%3,$zero"
                : "=&r"(first), "=r"(objectArgument)
                : "m"(work.views.matrices.intermediate.r[0]), "r"(object));
        __asm__("lq %0,%6\n\tlq %1,%7\n\tlq %2,%8\n\tlw %3,%9\n\tsq %5,%4\n\tsq %0,16+%4\n\tsq %1,32+%4"
                : "=&r"(row1), "=&r"(row2), "=&r"(row3), "=&r"(ownerArgument), "=m"(*(M3 *)&work.final)
                : "r"(first), "m"(work.views.matrices.intermediate.r[1]), "m"(work.views.matrices.intermediate.r[2]),
                  "m"(work.views.matrices.intermediate.r[3]), "m"(p->owner));
        work.final.r[3] = row3;
        counterpart = FindJtHandsTarget(ownerArgument, objectArgument);
    }
    register M4 *finalMatrix __asm__("$17") = &work.final;
    if (counterpart) {
        M4 *counterMatrix = WorldMatrix(counterpart);
        register const M4 *counterArgument __asm__("$7");
        register SO *counterSpace __asm__("$6");
        __asm__("daddu %0,%2,$zero\n\tdaddu %1,%3,$zero"
                : "=&r"(counterArgument), "=r"(counterSpace)
                : "r"(counterMatrix), "r"(object));
        new (product) M4(ConvertMat4BetweenLos(0, counterSpace, counterArgument));
        register M4 *inverse __asm__("$16");
        {
            register qword row1 __asm__("$6"), row2 __asm__("$3"), row3 __asm__("$2"), row0 __asm__("$7");
            register M4 *inverseArgument __asm__("$4");
            register const M4 *sourceArgument __asm__("$5");
            __asm__("lq %0,%4\n\taddiu %1,%5,0x180\n\tlq %2,%6\n\tdaddu %3,%1,$zero"
                    : "=&r"(row1), "=&r"(inverse), "=r"(row2), "=r"(inverseArgument)
                    : "m"(work.views.matrices.product.r[1]), "r"(&work), "m"(work.views.matrices.product.r[2]));
            __asm__("lq %0,%4\n\tdaddu %1,%5,$zero\n\tlq %2,%6\n\tsq %7,%3\n\tsq %8,16+%3\n\tsq %0,32+%3"
                    : "=&r"(row3), "=&r"(sourceArgument), "=&r"(row0),
                      "=m"(*(M3 *)&work.views.matrices.intermediate.r[1])
                    : "m"(work.views.matrices.product.r[3]), "r"(workingBase), "m"(work.views.matrices.product.r[0]),
                      "r"(row1), "r"(row2));
            work.views.matrices.intermediate.r[0] = row0;
            new (inverseArgument) M4(InvertMatrix4RigidP(sourceArgument));
        }
        MulMatrix4Ret(product, finalMatrix, inverse);
        {
            register qword row0 __asm__("$2"), row1 __asm__("$3"), row2 __asm__("$4"), row3 __asm__("$5");
            __asm__(
                "lq %0,%5\n\tlq %1,%6\n\tlq %2,%7\n\tlq %3,%8\n\tsq %0,%4\n\tsq %1,16+%4\n\tsq %2,32+%4\n\tsq %3,48+%4"
                : "=&r"(row0), "=&r"(row1), "=&r"(row2), "=&r"(row3), "=m"(work.final)
                : "m"(work.views.matrices.product.r[0]), "m"(work.views.matrices.product.r[1]),
                  "m"(work.views.matrices.product.r[2]), "m"(work.views.matrices.product.r[3]));
        }
    }
    SetLoParent(object, 0, 15);
    const qword *velocityAddress = object->physics ? (const qword *)&object->physics->gravity : &D_002C1EE0.q;
    register float unit __asm__("$f20");
    register SO *initialOwner __asm__("$4");
    {
        register qword initialVelocity __asm__("$2");
        __asm__("lq %0,%4\n\tdaddu %1,%5,$zero\n\t.set noat\n\tlui $at,0x3F80\n\tmtc1 $at,%2\n\t.set at\n\tsq %0,%3"
                : "=r"(initialVelocity), "=r"(initialOwner), "=f"(unit), "=m"(work.velocity)
                : "m"(*velocityAddress), "r"(object)
                : "$1");
    }
    {
        register qword destination __asm__("$2");
        __asm__("lq %0,%1" : "=r"(destination) : "m"(work.final.r[3]));
        work.destination = destination;
    }
    float limit;
    qword initialSpeed;
    DeltaForDuration(work, GetLoWorldPos(initialOwner), limit, initialSpeed);
    register float negativeLimit __asm__("$f13"), minimum __asm__("$f15"), maximum __asm__("$f14");
    __asm__("neg.s %0,%3\n\t.set noat\n\tlui $at,0xBF80\n\tmtc1 $at,%1\n\t.set at\n\tmov.s %2,%4"
            : "=f"(negativeLimit), "=f"(minimum), "=f"(maximum)
            : "f"(limit), "f"(unit)
            : "$1");
    float duration =
        FpuMax(phase, ComputeMaxBallisticTime(work.delta, initialSpeed, limit, negativeLimit, maximum, minimum));
    float speed = phase / duration;
    phase = duration;
    SetAsegaSpeed__FP5ASEGAf(anim, speed);
    {
        register float negativeTime __asm__("$f0"), positiveTime __asm__("$f1");
        register qword first __asm__("vf4"), speedVector __asm__("vf5");
        __asm__("div.s %4,%4,%5\n\t.set noat\n\tlui $at,0xBF00\n\tmtc1 $at,%0\n\tlui $at,0x3F00\n\tmtc1 $at,%1\n\t.set "
                "at\n\tmul.s %0,%5,%0\n\tlqc2 %2,%6\n\tmul.s %1,%5,%1\n\tlqc2 %3,%7"
                : "=&f"(negativeTime), "=&f"(positiveTime), "=j"(speedVector), "=j"(first), "+&f"(unit)
                : "f"(phase), "m"(work.velocity), "m"(work.delta)
                : "$1");
        register qword negativeOrEnd __asm__("vf2"), inverse __asm__("vf3");
        register int positiveBits __asm__("$3");
        register VTE *table __asm__("$2");
        __asm__("mfc1 %2,%4\n\tmfc1 %3,%5\n\tqmtc2.ni %2,%0\n\tqmtc2.ni %3,%1\n\tmfc1 %2,%6\n\tlw %3,%7"
                : "=j"(negativeOrEnd), "=j"(inverse), "=&r"(positiveBits), "=&r"(table)
                : "f"(negativeTime), "f"(unit), "f"(positiveTime), "m"(PVT(object)));
        __asm__("vmulax.xyzw $ACC,%0,%2x\n\tvmaddx.xyzw %0,%3,%4x\n\tsqc2 %2,%1"
                : "+j"(first), "=m"(work.views.scalar.q)
                : "j"(inverse), "j"(speedVector), "j"(negativeOrEnd));
        register qword positive __asm__("vf1"), callbackVelocity __asm__("$5");
        __asm__("qmtc2.ni %4,%0\n\tsqc2 %1,%3\n\tlqc2 %1,%5\n\tvmulax.xyzw $ACC,%1,%6x\n\tvmaddx.xyzw "
                "%1,%7,%0x\n\tqmfc2.ni %2,%8"
                : "=&j"(positive), "+&j"(negativeOrEnd), "=r"(callbackVelocity), "=m"(work.views.scalar.q)
                : "r"(positiveBits), "m"(work.delta), "j"(inverse), "j"(speedVector), "j"(first));
        typedef void (*APPLY)(LO *, qword);
        register APPLY apply __asm__("$6");
        register LO *receiver __asm__("$4");
        __asm__("lh %0,%4\n\tlw %1,%5\n\tsqc2 %6,%2\n\taddu %0,%8,%0\n\tsqc2 %7,%3"
                : "=&r"(receiver), "=&r"(apply), "=m"(work.views.scalar.q), "=m"(work.endVelocity)
                : "m"(table[35].d), "m"(table[35].pfn), "j"(inverse), "j"(negativeOrEnd),
                  "r"(object)); /* LO slot 35, SetVelocity */
        work.views.scalar.q = positive;
        apply(receiver, callbackVelocity);
    }
    float endTime = g_clock.time + phase;
    qword position = TransformXfPos(0, object->positionXf->parent, work.final.r[3]);
    qword endVelocity = ConvertXfVec(0, object->positionXf->parent, work.endVelocity);
    {
        register XF *channel __asm__("$5");
        register qword velocityArgument __asm__("$8"), positionArgument __asm__("$7");
        register float timeArgument __asm__("$f12");
        register SO *receiver __asm__("$4");
        __asm__("lw %0,%5\n\tpor %1,$zero,%6\n\tmov.s %2,%7\n\tpor %3,$zero,%8\n\tdaddu %4,%9,$zero"
                : "=&r"(channel), "=&r"(velocityArgument), "=f"(timeArgument), "=&r"(positionArgument), "=r"(receiver)
                : "m"(object->positionXf), "r"(endVelocity), "f"(endTime), "r"(position), "r"(object));
        p->pactPos = NewBezierAct(receiver, channel, 5, timeArgument, positionArgument, velocityArgument);
    }
    ConvertXfMat(&work.views.basis.converted, 0, object->rotationXf->parent, (M3 *)finalMatrix);
    work.views.basis.angles = work.views.basis.converted;
    qword angles = DecomposeRotationMatrixToEuler((M3 *)workingBase);
    register XF *orientationChannel __asm__("$5");
    register SO *orientationOwner __asm__("$4");
    register qword anglesArgument __asm__("$9");
    __asm__("lw %0,%3\n\tdaddu %1,%4,$zero\n\tpor %2,$zero,%5"
            : "=&r"(orientationChannel), "=&r"(orientationOwner), "=r"(anglesArgument)
            : "m"(object->rotationXf), "r"(object), "r"(angles));
    ACT *job = NewPositionAct(orientationOwner, orientationChannel, 5, 9, 2, anglesArgument);
    p->pactTurn = job;
    XF *pxf = object->rotationXf;
    if (pxf->firstActuator == job)
        pxf->arrivalTime = endTime;   /* the end time of the current act (XF+0xA8 union) */
    SetSoTreeFlag2(object, 1);
}
#else
/* StartJtHandsCarry in C (the EE body above is the register-pinned transcription of the target's schedule, with a hand
   slot-35 SetVelocity call): the same calls in the same order on the same values. */
extern "C" void StartJtHandsCarry(JTHANDS *p, SO *object)
{
    CARRYWORK work;
    ReleaseAct(&p->pactPos);
    float zero = 0.0f;
    ReleaseAct(&p->pactHold);
    ReleaseAct(&p->pactTurn);
    ASEGA *anim = ((STEPPLAYER *)p->owner)->activeAnimation;
    ASEG *motion = anim->animation;
    float time = GetAsegLabelTime(motion, 0x1C7), phase = (time - anim->time) / anim->speed;
    /* the hand's transform at the clip's grab label (0x1C7), in the player's frame */
    XF *space = SourceMatrix(p->ploHand, &work.original);
    PredictAsegTransform(motion, time, zero, p->owner, space, p->owner->rootTransform, &work.sample, 0);
    STEPPLAYER *owner = (STEPPLAYER *)p->owner;
    M4 *start = &work.start;
    VECTOR_opAssign(&start->r[3], GetLoWorldPos(owner));
    /* start = the player's heading basis (w 0) at its position (w 1); intermediate = start * sample * original */
    M4 *product = &work.views.matrices.product;
    M4 *intermediate = &work.views.matrices.intermediate;
    start->r[0] = owner->aqwTarget[0];
    start->r[1] = owner->aqwTarget[1];
    start->r[2] = owner->aqwTarget[2];
    ((U *)&start->r[0])->f[3] = zero;
    ((U *)&start->r[2])->f[3] = zero;
    ((U *)&start->r[1])->f[3] = zero;
    ((U *)&start->r[3])->f[3] = 1.0f;
    MulMatrix4Ret(product, start, &work.sample);
    MulMatrix4Ret(intermediate, product, &work.original);
    work.final = *intermediate;
    HANDATTACH *counterpart = FindJtHandsTarget((STEPPLAYER *)p->owner, object);
    M4 *finalMatrix = &work.final;
    if (counterpart) {
        /* relative to the counterpart: final = final * inverse(counterpart's matrix in the object's frame) */
        M4 *counterMatrix = WorldMatrix(counterpart);
        new (product) M4(ConvertMat4BetweenLos(0, object, counterMatrix));
        M4 *inverse = &work.views.matrices.inverse;
        *intermediate = *product;
        new (inverse) M4(InvertMatrix4RigidP(intermediate));
        MulMatrix4Ret(product, finalMatrix, inverse);
        work.final = *product;
    }
    SetLoParent(object, 0, 15);
    const qword *velocityAddress = object->physics ? (const qword *)&object->physics->gravity : &D_002C1EE0.q;
    work.velocity = *velocityAddress;
    work.destination = work.final.r[3];
    work.delta = VuSub(work.destination, GetLoWorldPos(object));
    float limit = D_002C1EF0;
    qword gravity = work.velocity;
    float duration = FpuMax(phase, ComputeMaxBallisticTime(work.delta, gravity, limit, -limit, 1.0f, -1.0f));
    float speed = phase / duration;
    phase = duration;
    SetAsegaSpeed__FP5ASEGAf(anim, speed);
    {
        /* launch velocity delta / T - g T / 2, end velocity delta / T + g T / 2 */
        qword inverseTime = VuFromFloat<qword>(1.0f / phase);
        qword negativeHalf = VuFromFloat<qword>(phase * -0.5f);
        qword positiveHalf = VuFromFloat<qword>(phase * 0.5f);
        qword launch = VuScaleX2(work.delta, inverseTime, work.velocity, negativeHalf);
        qword arrival = VuScaleX2(work.delta, inverseTime, work.velocity, positiveHalf);
        work.endVelocity = arrival;
        work.views.scalar.q = positiveHalf;
        object->SetVelocity(launch);
    }
    float endTime = g_clock.time + phase;
    qword position = TransformXfPos(0, object->positionXf->parent, work.final.r[3]);
    qword endVelocity = ConvertXfVec(0, object->positionXf->parent, work.endVelocity);
    p->pactPos = NewBezierAct(object, object->positionXf, 5, endTime, position, endVelocity);
    ConvertXfMat(&work.views.basis.converted, 0, object->rotationXf->parent, (M3 *)finalMatrix);
    work.views.basis.angles = work.views.basis.converted;
    qword angles = DecomposeRotationMatrixToEuler((M3 *)intermediate);
    ACT *job = NewPositionAct(object, object->rotationXf, 5, 9, 2, angles);
    p->pactTurn = job;
    XF *pxf = object->rotationXf;
    if (pxf->firstActuator == job)
        pxf->arrivalTime = endTime;   /* the end time of the current act (XF+0xA8 union) */
    SetSoTreeFlag2(object, 1);
}
#endif

/* Attach the held object to the hand: parent it to the hand's XF, place it at the hand (relative to a CID 0x14
   counterpart when there is one), constrain it, and give it position/orientation acts. */
// Attaches the held object to the hand: force/torque constraints, transform from the hand matrix, acts, fade-in.
// Also known as: AttachReachObject (upstream readable / Sly 1 name; AttachReachObject__FP5REACHP2LO).
extern "C" void AttachJtHandsObject(JTHANDS *hands, SO *object)
{
    M4 buf;
    M4 m;
    if (object == hands->psoHeld) return;
    ReleaseAct(&hands->pactPos);
    ReleaseAct(&hands->pactHold);
    ReleaseAct(&hands->pactTurn);
    XF *xf = hands->ploHand->GetLocalTransform((MATRIX4 *)&buf);
    object->SetParent(hands->owner, xf, 0);
    m = *(M4 *)hands->ploHand->GetWorldMatrix();
    HANDATTACH *pcid = FindJtHandsTarget((STEPPLAYER *)hands->owner, object);
    if (pcid) {
        M4 u;
        M4 t = ConvertMat4BetweenLos(0, object, (M4 *)pcid->GetWorldMatrix());
        u = t;
        M4 w = InvertMatrix4RigidV(u);
        MulMatrix4RetV(&t, &m, &w);
        m = t;
    }
    SetLoXfFromMat(object, &m);
    if (object->physics) {
        SetSoCnstrForce(object, 1);
        SetSoCnstrTorque(object, 1);
    }
    XF *pxfAct = object->positionXf;
    hands->pactHold = NewPositionAct(object, pxfAct, 5, 0, 2, GetXfValueFromMatrix(pxfAct, pxfAct));
    pxfAct = object->rotationXf;
    hands->pactTurn = NewPositionAct(object, pxfAct, 5, 0, 2, GetXfValueFromMatrix(pxfAct, pxfAct));
    FadeSoIn(object, 0.25f);
    hands->psoHeld = object;
}

// Frees the three hand acts that are active.
// Also known as: ReleaseReachObject (upstream readable / Sly 1 name; ReleaseReachObject__FP5REACH).
extern "C" void ReleaseJtHandsActs(JTHANDS *hands)
{
    if (hands->psoHeld == 0)
        return;
    ReleaseAct(&hands->pactPos);
    ReleaseAct(&hands->pactHold);
    ReleaseAct(&hands->pactTurn);
    hands->psoHeld = 0;
}

/* Release the held object onto po (its parent, or the world) and move it there with acts; then target po. */
// Variant of SetJtHandsTarget that parents the object and builds the position/rotation acts.
// Also known as: PlaceReachObject (upstream readable / Sly 1 name; PlaceReachObject__FP5REACHP2LO).
extern "C" void SetJtHandsTargetAttached(JTHANDS *hands, SO *object)
{
    M3 mat;
    int unused[4];          /* with mat, the 0x40 bytes GetLocalTransform fills (a MATRIX4); part of the frame */
    if (!hands->psoHeld) return;
    SetJtHandsTarget(hands, -1);
    ReleaseAct(&hands->pactPos);
    ReleaseAct(&hands->pactHold);
    ReleaseAct(&hands->pactTurn);
    if (object->parent) {
        XF *xf = object->GetLocalTransform((MATRIX4 *)&mat);
        hands->psoHeld->SetParent(object->parent, xf, 0xF);
    } else {
        SetLoParent(hands->psoHeld, 0, 0xF);
    }
    XF *pxfAct = hands->psoHeld->positionXf;
    hands->pactPos = func_0012DE58Q(hands->psoHeld, pxfAct, 5, TransformXfPos(0, pxfAct->parent, GetLoWorldPos(object)), D_002A2DD0, g_clock.time + D_002C1EF4);
    pxfAct = hands->psoHeld->rotationXf;
    mat = ConvertXfMatR(0, pxfAct->parent, GetLoWorldMatrix(object));
    hands->pactTurn = NewPositionAct(hands->psoHeld, pxfAct, 5, 7, 2, func_0011BFF0V(mat));
    hands->ploTarget = hands->psoHeld;
    SetJtHandsTarget(hands, 1);
    hands->psoHeld = 0;
}

// ---- Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_jthands {
void p_func_001CCA20() __asm__("UpdateJtHandsCallback");

extern const unsigned int D_002A77C8[2] __asm__("D_002A77C8") = {0xFFFF0000, (unsigned int)p_func_001CCA20};

} // namespace ANPTR_jthands
