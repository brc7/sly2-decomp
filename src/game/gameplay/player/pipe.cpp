// pipe.cpp -- PIPE, a shape the player can climb along (a SHAPE with GRABSPOTDATA at +0x190), kept in a global list:
// init/load/add/remove/clone, the climbing frame at a distance (GetPipeGoalMatrix), the closest point and range, and a
// random point/direction on it. VU0 and EE asm. TU 0x1C3390..0x1C3D70 (13 functions, address order); PIPE's
// GRABSPOT owner getter GetPipeGrabspotOwner is an inline function in sly2/xfm.h.
// Entry points: InitPipe/PostPipeLoad/ClonePipe, the world pipe list, GetPipeGoalMatrix (climbing frame) and the
// GRABSPOT virtuals (GetPipeWrapRange, GetPipeClosestParam, GetPipeGrabPosition, GetPipeSparklePosition).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_pipe
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_pipe
#include "sly2/slotheap.h"
#include "sly2/vu0.h"
#include "sly2/util.h"
#include "sly2/sw.h"
#include "sly2/classes.h"
// 0x1C3C18 StaticInit_pipe and GlobalConstructors_pipe: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* View of the pipe (a SHAPE). */
struct PIPEV : BASIC 
{
    void *parent; void *root; char pad18[0x1C - 0x18]; void *unknown1C; void *unknown20; void *unknown24; void *unknown28; char pad2C[0x34 - 0x2C]; void *firstRunCallback;
    float length;           /* +0x038 1e10 at load: the shape's length (UpdateShapeBoundingSphere) plus D_002C0D00 */
    void *proxyRecord; char pad40[0x50 - 0x40]; void *parentTransform; char pad54[0x6C - 0x54]; void *unknown6C; char pad70[0xBC - 0x70]; void *unknownBC; char padC0[0xE4 - 0xC0];
    int isCyclic;           /* +0x0E4 closed shape: distances wrap, no range clamp */
    char pad0E8[0xEC - 0xE8]; void *vertexParams; void *vertexArcLengths; void *vertexPositions; char padF8[0xFC - 0xF8]; void *unknownFC; char pad100[0x12C - 0x100]; void *unknown12C; char pad130[0x140 - 0x130]; void *incomingTangents; void *outgoingTangents; char pad148[0x17C - 0x148]; void *chain; void *pranges; char pad184[0x190 - 0x184];
    char grabspot[0x1C];    /* +0x190 GRABSPOTDATA (its +0x1C, +0x24 follow) */
    int isGrabDisabled;     /* +0x1AC 1 at init (SetPipeGrabEnabled sets it to !arg) */
    void *unknown1B0;
    DLE k;                    /* +0x1B4 kept across CloneShape */
    void *unknown1BC;
    qword axis;             /* +0x1C0 the reference axis (D_002A2DA0 at init) */
    int grabParamA;         /* +0x1D0 8 at init */
    int grabParamB;         /* +0x1D4 9 at init */
};
typedef char assert_PIPE_grabParamB[(unsigned int)&((PIPEV *)0)->grabParamB == 0x1D4 ? 1 : -1];

struct DL;
/* The TU's .data (0x2C0D00..0x2C0D14, address order = definition order). Only pipe's code reads them. */
float D_002C0D00 = 1250.0f;     /* added to the shape's length at load (PIPE length) */
float D_002C0D04 = -50.0f;      /* not read by the matched code; between pipe's own data, so pipe's */
float D_002C0D08 = 1.8f;        /* random direction: radial scale */
float D_002C0D0C = 25.0f;       /* random direction: magnitude range */
float D_002C0D10 = 150.0f;      /* random point: range along the shape */
extern "C" {
extern qword D_002A2DA0, g_normalZ;   // fallback unit vectors
void InitShape(PIPEV *p);
void InitGrabspot(void *p);
void OnLoAdd(PIPEV *p);
void OnLoRemove(PIPEV *p);
void PostLoLoad(PIPEV *p);
float UpdateShapeBoundingSphere(PIPEV *p);
void CloneShape(PIPEV *p, LO *ploBase);
float GetShapeMinDistance(PIPEV *);           // low distance bound
float GetShapeMaxDistance(PIPEV *);           // high distance bound
float GetShapeMinParam(PIPEV *);           // start distance
void EvaluateShapeAtParam(PIPEV *, float, qword *, qword *);
float ModPositive__Fff(float, float);     // wrap into range
float ClampShapeDistance(PIPEV *, float);
void EvaluateShapeAtDistance(PIPEV *, float, qword *, qword *);
MATRIX3Q BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(qword, qword);
qword ConvertVecBetweenLos(PIPEV *, PIPEV *, qword);
qword GetNormalVector(qword);
qword GetLoWorldPos(void *q);
void FindShapeClosestPoint(PIPEV *p, qword q, int a, void *pv, int b, int c, float *pf);
void GetPipeGoalMatrix(PIPEV *p, float distance, int forward, MATRIX3Q *out);
float GetRandomShapeDistanceInRange(PIPEV *,float,float);
float RandomFloatInRange__Fff(float,float);
qword SetVectorCylind(float,float,float);
}
void AppendDlEntry(DL *pdl, void *pv);   /* AppendDlEntry__FP2DLPv */
void RemoveDlEntry(DL *pdl, void *pv);   /* RemoveDlEntry__FP2DLPv */

// PIPE::Init (vtable slot 7): shape init, its GRABSPOT part, grabbing enabled, the default reference axis and two
// grab parameters (8 and 9, as HND uses).
extern "C" void InitPipe(PIPEV *pipe)
{
    InitShape(pipe);
    InitGrabspot(pipe->grabspot);
    pipe->axis = D_002A2DA0;
    pipe->grabParamB = 9;
    pipe->grabParamA = 8;
    pipe->isGrabDisabled = 1;
}

// PIPE slot 23 (OnAdd: the object enters the active world): LO base, then joins the world's pipe list (g_sw.dlCid11).
void PIPE::OnAdd()
{
    PIPEV *pipe = (PIPEV *)this;
    OnLoAdd(pipe);
    AppendDlEntry(&g_sw.dlCid11, pipe);
}

// PIPE slot 22 (OnRemove): LO base, then leaves the world's pipe list.
extern "C" void OnPipeRemove(PIPEV *pipe)
{
    OnLoRemove(pipe);
    RemoveDlEntry(&g_sw.dlCid11, pipe);
}

// PIPE post-load (slot 15): if the level left the length at the 1e10 sentinel, uses the shape's length plus
// D_002C0D00 (1250).
extern "C" void PostPipeLoad(PIPEV *pipe)
{
    PostLoLoad(pipe);
    if (pipe->length == 1e10f) pipe->length = UpdateShapeBoundingSphere(pipe) + D_002C0D00;
}

// PIPE::Clone (slot 41): clones the shape but keeps this pipe's own 8-byte link at +0x1B4.
extern "C" void ClonePipe(PIPEV *pipe, LO *ploBase)
{
    DLE k = pipe->k;
    CloneShape(pipe, ploBase);
    pipe->k = k;
}

// GetPipeGoalMatrix (lane 6, 2026-10-24): builds an orientation basis (M3) for a position `distance` along a path.
// It samples the path tangent at the start, then tries sample distances distance, +10, -10, +20, -20 ... (wrapped
// for cyclic paths, rejected outside [low,high] otherwise) until the direction between the samples at +-50 is within
// ~25 degrees of the tangent, rotates the path axis by the tangent->direction rotation, and builds the basis
// (side, up) from that.  Optionally flips side/up (forward == 0) and copies the basis to *out.
union U { qword q; float f[4]; };
// Float broadcast into a VU0 register on construction.
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
// Wrap a qword as a VU_VECTOR.
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } };
// Return v scaled by f.
static inline qword operator*(VU_FLOAT f, VU_VECTOR v) { qword r; __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q)); return r; }

// Length of the xyz part of v (x lane).
static inline qword Length(qword v)
{
    qword d, len;
    d = VuDot(v, v);
    __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(d));
    return len;
}
// Unit vector along v, or fallback when its length is too small.
static inline qword Normalize(qword v, qword fallback)
{
    qword len = Length(v);
    if (QwordX(len) < 0.0001f)
        return fallback;
    return VuDivByX(v, len);
}
// Dot product of the xyz parts of a and b as a float.
static inline float Dot(qword a, qword b)
{
    qword d;
    float f;
    d = VuDot(a, b);
    f = VuToFloat(d);
    return f;
}
// earlyclobber output: the result must not share a register with the matrix rows
static inline qword Transform(const MATRIX3Q &m, qword q)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
            : "=&j"(r) : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q), "j"(q));
    return r;
}

struct WORK { MATRIX3Q basis; qword tangent, first, direction; };

// The climbing frame at `distance` along the pipe: searches sample distances around it until the local direction
// agrees with the start tangent, rotates the pipe's reference axis accordingly and builds an orthonormal basis
// (side, up, along). With forward == 0 the side and up rows are flipped (climbing the other way). Used by the
// player's pipe state to orient Sly and by the grab-spot interface.
// Also known as: MatPipeFrame (upstream readable / Sly 1 name; MatPipeFrame__FP2LOfiPv).
extern "C" void GetPipeGoalMatrix(PIPEV *pipe, float distance, int forward, MATRIX3Q *out)
{
    WORK w;
    U fwd;   // declared outside the inner block on purpose: the stack slot of `back` (declared inside) is freed at the
             // end of that block and is reused by the VU_FLOAT temporary of the final negate, like the original.
    float low = GetShapeMinDistance(pipe), high = GetShapeMaxDistance(pipe);
    float t0 = GetShapeMinParam(pipe);
    EvaluateShapeAtParam(pipe, t0, 0, &w.tangent);
    w.tangent = Normalize(w.tangent, D_002A2DA0);
    {
        U back;
        MATRIX3Q rot;
        for (int i = 0;; ++i) {
            float offset = (i & 1) ? 10.0f * (float)(i + 1) : (float)i * -10.0f;
            float sample = distance + offset;
            if (!pipe->isCyclic) {
                if (sample < low || high < distance)
                    continue;
            } else
                sample = low + ModPositive__Fff(sample - low, high - low);
            float r = 50.0f;
            EvaluateShapeAtDistance(pipe, ClampShapeDistance(pipe, sample + r), &fwd.q, 0);
            EvaluateShapeAtDistance(pipe, ClampShapeDistance(pipe, sample - r), &back.q, 0);
            w.direction = Normalize(VuSub(fwd.q, back.q), g_normalZ);
            if (i == 0)
                w.first = w.direction;
            if (Dot(w.tangent, w.direction) > -0.9f)
                break;
        }
        rot = BuildRotateVectorsMatrix__FP6VECTORT0P7MATRIX3(w.tangent, w.direction);
        qword ref = ConvertVecBetweenLos(pipe, 0, pipe->axis);
        fwd.q = Transform(rot, ref);
        qword up = Normalize(w.first, g_normalZ);
        w.basis.r[2].q = up;
        back.q = VuCross(up, fwd.q);
        w.basis.r[1].q = back.q;
        qword len = Length(back.q);
        w.basis.r[1].q = VuDivByX(back.q, len);
        if (QwordX(len) < 0.0001f)
            w.basis.r[1].q = GetNormalVector(up);
        w.basis.r[0].q = VuCross(w.basis.r[1].q, w.basis.r[2].q);
    }
    if (!forward) {
        w.basis.r[1].q = VU_FLOAT(-1.0f) * VU_VECTOR(w.basis.r[1].q);
        w.basis.r[2].q = VU_FLOAT(-1.0f) * VU_VECTOR(w.basis.r[2].q);
    }
    if (out)
        *out = w.basis;
}

// GRABSPOT interface: the shape distance of the pipe point closest to the player; on open pipes clamped to
// [100, length - 35].
extern "C" void GetPipeClosestParam(PIPEV *pipe, void *q, float *pf)
{
    FindShapeClosestPoint(pipe, GetLoWorldPos(q), 0, 0, 0, 0, pf);
    if (pipe->isCyclic == 0) {
        *pf = Clamp(*pf, 100.0f, GetShapeMaxDistance(pipe) - 35.0f);
    }
}

enum { SCALE_WORD_ADDRESS = 0x002C0D04 };
// Load the scale word (raw bits) from SCALE_WORD_ADDRESS.
static inline int ScaleBits(void) {
    register int *page __asm__("$2") = (int *)(SCALE_WORD_ADDRESS & ~0xFFFF);
    register int bits __asm__("$2");
    __asm__("lw %0,%1":"=r"(bits):"m"(page[(SCALE_WORD_ADDRESS & 0xFFFF) / sizeof(int)]));
    return bits;
}
// GRABSPOT interface: the pipe point at distance `distance`, pushed out along the climbing frame's side axis by
// D_002C0D04 (-50) so the hands meet the pipe surface.
extern "C" void GetPipeGrabPosition(PIPEV *pipe, int unused, qword *pvec, float g)
{
    qword a[3];
    VECTOR4 b;
    union {
        qword q;
        float f;
    } s;
    register qword r __asm__("vf2");
    register qword scale __asm__("vf3"), axis __asm__("vf1"), position __asm__("vf2");
    GetPipeGoalMatrix(pipe, g, 1, (MATRIX3Q *)a);
    EvaluateShapeAtDistance(pipe, g, (qword *)&b, 0);
    if (!pvec)
        return;
    __asm__("lqc2 %0,%1" : "=j"(position) : "m"(b));
    __asm__("qmtc2.ni %2,%0\n\tlqc2 %1,%3" : "=j"(scale), "=j"(axis) : "r"(ScaleBits()), "m"(a[0]));
    s.q = scale;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(position), "j"(axis), "j"(s.q));
    *pvec = r;
}

// Broadcast a float into a VU0 register (with a stack home copy).
static inline qword Bc(float g) {
    union { qword q; float f; } u;
    qword q;
    q = VuFromFloat<qword>(g);
    u.q=q;
    return q;
}
// GRABSPOT interface: a random point near the pipe around `distance` and a random outward direction, used to place the
// grab-hint sparkle effect.
extern "C" void GetPipeSparklePosition(PIPEV *pipe, qword *posOut, qword *dirOut, float t)
{
    union {
        qword q;
        float f[4];
    } vp;
    MATRIX3Q m;
    qword dir, s, r, pos2;
    float n, mag;
    t = GetRandomShapeDistanceInRange(pipe, t, D_002C0D10);
    GetPipeGoalMatrix(pipe, t, 1, &m);
    EvaluateShapeAtDistance(pipe, t, &vp.q, 0);
    n = RandomFloatInRange__Fff(-1.0f, 1.0f);
    mag = __builtin_fabsf(n);
    dir = SetVectorCylind((n * mag) * D_002C0D08, -1.0f, 0.0f);
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z"
            : "+j"(dir)
            : "j"(m.r[0].q), "j"(m.r[1].q), "j"(m.r[2].q));
    pos2 = vp.q;
    mag = mag * RandomFloatInRange__Fff(0.0f, D_002C0D0C);
    s = Bc(mag);
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(r) : "j"(pos2), "j"(dir), "j"(s));
    *posOut = r;
    *dirOut = dir;
}

// GRABSPOT interface: for a closed (looping) pipe returns 1 and the [min, max] distance range a grab position wraps
// around; open pipes return 0.
extern "C" int GetPipeWrapRange(PIPEV *pipe, float *pg)
{
    if (pipe->isCyclic) {
        pg[0] = GetShapeMinDistance(pipe);
        pg[1] = GetShapeMaxDistance(pipe);
        return 1;
    }
    return 0;
}

// Enables or disables grabbing this pipe (+0x1AC holds the inverted flag). (inferred)
extern "C" void SetPipeGrabEnabled(PIPEV *pipe, int f)
{
    pipe->isGrabDisabled = (f == 0);
}

