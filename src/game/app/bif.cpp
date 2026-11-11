// bif.cpp -- the Splice builtin functions (Builtin*: list, arithmetic, comparison, vector/matrix, world queries).
// TU 0x11F440..0x1258F0 (155 functions, address order; then the three linkonce VU math copies 0x256CE8..0x256D28).
// Splice is the game's Scheme-like scripting language (splice/). The evaluator (splice/eval.cpp) calls a builtin
// through the 169-entry table g_mpbifkbif at the end of this file ({function, minimum argument count, takes rest
// arguments}) with the evaluated arguments as a CRef array and returns its CRef result. Sly 1's builtin table gives
// most script spellings; docs/systems/splice-scripting.md section 7 describes the library.
// Entry points: the table g_mpbifkbif and the Builtin* functions (Sly 1 named them RefOp*; the suffix is the script op).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_bif
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_bif
// Every builtin is `CRef BuiltinX(int cref, CRef *args, CFrame *pframe)` (the mangled symbols say so); the per-function
// files define them under their literal symbols with C linkage, keep their other private views in namespaces named
// after the function (docs/guides/tu-merging.md "Representation decisions"), and use sly2/cref.h's CRef (the C++
// bodies) or its POD base CRefData (the C-style bodies). The former m2c bodies
// follow the same convention at file scope: C linkage under the literal symbol, the hidden result pointer passed by
// hand, sly2/cref.h's POD CRefData, and the C-named CRef constructors of splice/ref.cpp.
#include "sly2/port.h"
#include "sly2/slotheap.h"
#include "sly2/stackscope.h"
#include "sly2/pair.h"
#include "sly2/cframe.h"
#include "sly2/clock.h"
#include "sly2/sw.h"
#include "sly2/so.h"
#include "sly2/util.h"
#include "sly2/cm.h"
#include "sly2/joy.h"
#include "sly2/save.h"
#include "sly2/sndmgr.h"
/* SLY2_BUILTIN_C(sym), before each C-style builtin (result through a hand-passed hidden pointer, sly2/cref.h's
   CRefData): on the EE nothing, the table (g_mpbifkbif) calls the C-style body itself (g++ 2.95 passes a CRef result's
   address first, as these bodies take it). Natively the body is renamed sym_c and sym becomes the CRef-returning entry
   the table's type says (eval.cpp BIF::function), because a native ABI passes the result's address elsewhere (x8 on
   arm64). */
#if defined(SLY2_EE_LAYOUT)
#define SLY2_BUILTIN_C(sym)
#else
#define SLY2_BUILTIN_C(sym) \
    extern "C" CRefData *sym(CRefData *ret, int cref, CRefData *args, CFrame *frame) __asm__(#sym "_c"); \
    CRef sym##_cref(int cref, CRef *args, CFrame *frame) __asm__(#sym); \
    CRef sym##_cref(int cref, CRef *args, CFrame *frame) { CRef ref; sym(&ref, cref, args, frame); return ref; }
#endif
// 0x125798 StaticInit_bif and GlobalConstructors_bif: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

/* Splice value tags (CRef::m_tagk; docs/systems/splice-scripting.md section 2), named for this file. The shared TAGK
   enum (sly2/basic.h, sly2/cref.h) defines only TAGK_Nil = -1 so far; these are plain constants so that the views below
   can keep taking `int` tags. 0x12 is the quote special form's head tag, used when the builtins build (quote x). */
enum
{
    TAGK_QUOTE = 0x12,
    TAGK_S32 = 0x15,
    TAGK_F32 = 0x16,
    TAGK_BOOL = 0x17,
    TAGK_SYMID = 0x18,
    TAGK_BIFK = 0x19,          /* a builtin function id */
    TAGK_VECTOR = 0x1A,
    TAGK_MATRIX = 0x1B,
    TAGK_CLQ = 0x1C,           /* quadratic coefficients */
    TAGK_LM = 0x1D,            /* limits (min, max) */
    TAGK_SMP = 0x1E,           /* smoothing parameters */
    TAGK_PAIR = 0x1F,
    TAGK_PROC = 0x20,
    TAGK_BASIC = 0x21,         /* an object */
    TAGK_METHOD = 0x22,
    TAGK_VOID = 0x24,
};

/* 16-byte vector as the builtins pass it (the payload heaps hold quadwords; sly2/basic.h's VECTOR is 12 bytes, its
   VECTOR4 the same layout under another name). VU_VECTOR's constructor keeps its mangled name (RC6VECTOR). */
struct VECTORQ { float x, y, z, w; } __attribute__((aligned(16)));
struct BASIC;
/* The VU wrappers of the linkonce copies at the end of the file. */
struct VU_FLOAT { qword q; VU_FLOAT(float f); };
struct VU_VECTOR { qword q; VU_VECTOR(const VECTORQ &vec) __asm__("__9VU_VECTORRC6VECTOR"); };

extern char D_002DE578[];       /* the difficulty record */
int GetCurrentLevelIndex();
extern "C" {
void SetTag(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void Decref(CRefData *pref) __asm__("Decref__4CRef");
void CompareNumericArgs(CRefData *pref, CRefData *args, int cmpk) __asm__("CompareNumericArgs__FP4CRef4CMPK");
void ApplyUnaryFloatOp(CRefData *pref, CRefData *args, int ufok) __asm__("ApplyUnaryFloatOp__FP4CRef4UFOK");
void AreRefsStructurallyEqual(CRefData *pref, CRefData *pref1, CRefData *pref2) __asm__("AreRefsStructurallyEqual__FP4CRefT0");
CRefData *__4CRefRC4CRef(CRefData *pref, CRefData *prefSrc);             /* CRef copy constructor */
CRefData *CRef_ctor_CPair(CRefData *pref, CPair *ppair);
CRefData *CRef_ctor_VECTOR(CRefData *pref, VECTORQ *pvec);
CRefData *CRef_ctor_MATRIX4(CRefData *pref, MATRIX4 *pmat);
CRefData *CRef_ctor_BASIC(CRefData *pref, BASIC *pbasic);
void InvertMatrix4(MATRIX4 *pmat, MATRIX4 *pmatInverse);
void CalculateDmat4(MATRIX4 *pmat1, MATRIX4 *pmat2, MATRIX4 *pmatResult);
int GetCurrentEpisode();
int GetCurrentWorld();
int SwStubFalse();
float GetDifficulty(char *pdifficulty);
void CancelSwSpliceCallback(SW *psw, int n);
}

// 0x11F440 BuiltinAdd__FiP4CRefP6CFrame
namespace M_BuiltinAdd__FiP4CRefP6CFrame {
struct VECTOR; struct MATRIX4;
// (Claude agent, lane A9, 2026-10-23) script `+` builtin (probably). The first node is read through an inline accessor in an
// if/else: the inline blocks jump.c's "x = b; if (c) x = a" rewrite, so `pn = 0` is laid out after the branch and
// reorg steals it into the beqz delay slot as in the original.
struct VECTOR { float x, y, z, w; };
struct MATRIX4 { float m[4][4]; };
extern CRef BuiltinAdd__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinAdd__FiP4CRefP6CFrame");

// The node a script reference points to.
static inline CPair *NodeFromRef(CRef *p) { return p->u.ppair; }
// Splice builtin "+": n-ary numeric add over arg list (int/float/vector/matrix promotion).
// Also known as: RefOpAdd (upstream readable / Sly 1 name).
CRef BuiltinAdd__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    int tagk = args[0].m_tagk;
    CPair *pn;
    if (args[1].m_tagk)
        pn = NodeFromRef(&args[1]);
    else
        pn = 0;
    switch (tagk) {
    case TAGK_S32:
    case TAGK_F32: {
        float g;
        int fInt;
        if (tagk == TAGK_S32) {
            g = (float)args[0].u.n;
            fInt = 1;
        } else {
            g = args[0].u.g;
            fInt = 0;
        }
        for (; pn; pn = pn->next.p) {
            if (pn->ref.m_tagk == TAGK_S32) {
                g += (float)pn->ref.u.n;
            } else {
                fInt = 0;
                g += pn->ref.u.g;
            }
        }
        if (fInt)
            ref = (int)g;
        else
            ref = g;
        break;
    }
    case TAGK_VECTOR:
        ref.SetVector((const ::VECTOR *)args[0].u.pv);
        for (; pn; pn = pn->next.p) {
            qword *pq = (qword *)ref.u.pv;
            qword a = *pq, b = *(qword *)pn->ref.u.pv, r;
            __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
            *pq = r;
        }
        break;
    case TAGK_MATRIX:
        ref.SetMatrix((const ::MATRIX4 *)args[0].u.pv);
        for (; pn; pn = pn->next.p) {
            MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
            MATRIX4 *pmB = (MATRIX4 *)pn->ref.u.pv;
            pm->m[0][0] += pmB->m[0][0]; pm->m[0][1] += pmB->m[0][1]; pm->m[0][2] += pmB->m[0][2]; pm->m[0][3] += pmB->m[0][3];
            pm->m[1][0] += pmB->m[1][0]; pm->m[1][1] += pmB->m[1][1]; pm->m[1][2] += pmB->m[1][2]; pm->m[1][3] += pmB->m[1][3];
            pm->m[2][0] += pmB->m[2][0]; pm->m[2][1] += pmB->m[2][1]; pm->m[2][2] += pmB->m[2][2]; pm->m[2][3] += pmB->m[2][3];
            pm->m[3][0] += pmB->m[3][0]; pm->m[3][1] += pmB->m[3][1]; pm->m[3][2] += pmB->m[3][2]; pm->m[3][3] += pmB->m[3][3];
        }
        break;
    }
    return ref;
}
} // namespace M_BuiltinAdd__FiP4CRefP6CFrame

// 0x11F6D0 BuiltinSubtract__FiP4CRefP6CFrame
namespace M_BuiltinSubtract__FiP4CRefP6CFrame {
struct VECTOR; struct MATRIX4;
// (Claude agent, lane A9, 2026-10-23) script `-` builtin (probably), same shape as BuiltinAdd: the inline accessor in the
// if/else keeps `pn = 0` after the branch (no jump.c rewrite), so reorg puts it in the beqz delay slot.
struct VECTOR { float x, y, z, w; };
struct MATRIX4 { float m[4][4]; };
extern CRef BuiltinSubtract__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinSubtract__FiP4CRefP6CFrame");

// The node a script reference points to.
static inline CPair *NodeFromRef(CRef *p) { return p->u.ppair; }
// Splice builtin "-": n-ary subtract, same shape as +.
// Also known as: RefOpSub (upstream readable / Sly 1 name).
CRef BuiltinSubtract__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    int tagk = args[0].m_tagk;
    CPair *pn;
    if (args[1].m_tagk)
        pn = NodeFromRef(&args[1]);
    else
        pn = 0;
    switch (tagk) {
    case TAGK_S32:
    case TAGK_F32: {
        float g;
        int fInt;
        if (tagk == TAGK_S32) {
            g = (float)args[0].u.n;
            fInt = 1;
        } else {
            g = args[0].u.g;
            fInt = 0;
        }
        for (; pn; pn = pn->next.p) {
            if (pn->ref.m_tagk == TAGK_S32) {
                g -= (float)pn->ref.u.n;
            } else {
                fInt = 0;
                g -= pn->ref.u.g;
            }
        }
        if (fInt)
            ref = (int)g;
        else
            ref = g;
        break;
    }
    case TAGK_VECTOR:
        ref.SetVector((const ::VECTOR *)args[0].u.pv);
        for (; pn; pn = pn->next.p) {
            qword *pq = (qword *)ref.u.pv;
            qword a = *pq, b = *(qword *)pn->ref.u.pv, r;
            __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b));
            *pq = r;
        }
        break;
    case TAGK_MATRIX:
        ref.SetMatrix((const ::MATRIX4 *)args[0].u.pv);
        for (; pn; pn = pn->next.p) {
            MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
            MATRIX4 *pmB = (MATRIX4 *)pn->ref.u.pv;
            pm->m[0][0] -= pmB->m[0][0]; pm->m[0][1] -= pmB->m[0][1]; pm->m[0][2] -= pmB->m[0][2]; pm->m[0][3] -= pmB->m[0][3];
            pm->m[1][0] -= pmB->m[1][0]; pm->m[1][1] -= pmB->m[1][1]; pm->m[1][2] -= pmB->m[1][2]; pm->m[1][3] -= pmB->m[1][3];
            pm->m[2][0] -= pmB->m[2][0]; pm->m[2][1] -= pmB->m[2][1]; pm->m[2][2] -= pmB->m[2][2]; pm->m[2][3] -= pmB->m[2][3];
            pm->m[3][0] -= pmB->m[3][0]; pm->m[3][1] -= pmB->m[3][1]; pm->m[3][2] -= pmB->m[3][2]; pm->m[3][3] -= pmB->m[3][3];
        }
        break;
    }
    return ref;
}
} // namespace M_BuiltinSubtract__FiP4CRefP6CFrame

// 0x11F960 BuiltinMultiply__FiP4CRefP6CFrame
namespace M_BuiltinMultiply__FiP4CRefP6CFrame {
struct VECTOR; struct MATRIX4; struct V8; struct VU_FLOAT; struct VU_FLOAT_M; struct VU_FLOAT_V; struct VU_FLOAT_I; struct VU_FLOAT_D; struct VU_FLOAT_F; struct VU_FLOAT_X; struct VU_VECTOR;
// BuiltinMultiply MATCH (Claude agent, lane A12, 2026-10-23), from fresh4 (6): 0x1A/0x16 MulVectorR builds the VU float from the int bits with ONE commented pin ($17, pr's register, as the target), and MulMatrix (0x1B/0x16) has a commented volatile empty asm that keeps &mat after the mtc1. The other old pins were removed.
struct VECTOR { float x, y, z, w; };
struct MATRIX4 { float af[16]; } __attribute__((aligned(16)));
struct V8 { float x, y; };
struct VU_FLOAT {
    qword q;
    // Carry a float's bits in a GPR qword (no instruction).
    VU_FLOAT(const float &f) { __asm__("" : "=r"(q) : "0"(*(const int *)&f)); }
    // The value as an FPU float.
    float F() const { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
};
struct VU_FLOAT_M {
    qword q;
    // Move a float's bits into a GPR qword (mfc1).
    VU_FLOAT_M(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); }
    // The value as an FPU float.
    float F() const { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
};
struct VU_FLOAT_V {
    qword q;
    // Broadcast the float f (by its bits) into a VU0 register.
    VU_FLOAT_V(const float &f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(*(const int *)&f)); }
};
struct VU_FLOAT_I {
    qword q;
    // Broadcast int bits into a VU0 register.
    VU_FLOAT_I(int n) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(n)); }
};
struct VU_FLOAT_D { qword q; };
struct VU_FLOAT_F {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT_F(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
struct VU_FLOAT_X { qword q; VU_FLOAT_X(float f) __asm__("__8VU_FLOATf"); };
struct VU_VECTOR { qword q; VU_VECTOR(const VECTOR &v) __asm__("__9VU_VECTORRC6VECTOR"); };
qword operator*(VU_FLOAT_X f, VU_VECTOR v) __asm__("__ml__FG8VU_FLOATG9VU_VECTOR");
extern qword *VECTOR_opAssign(VECTOR *p, qword v) __asm__("VECTOR_opAssign");
MATRIX4 __ml__FRC7MATRIX4T0(const MATRIX4 &a, const MATRIX4 &b) __asm__("__ml__FRC7MATRIX4T0");
CRef CRef_CloneTo(CRef *pref, void *a, void *b) __asm__("CRef_CloneTo");
extern CRef *CRef_SetVector_VU_VECTOR(CRef *pref, qword vec) __asm__("CRef_SetVector_VU_VECTOR");   /* CRef = VU_VECTOR (tag 0x1A) */
extern CRef BuiltinMultiply__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMultiply__FiP4CRefP6CFrame");

// Return *pv scaled by g.
static inline qword MulVF(const qword *pv, float g)
{
    VU_FLOAT_F s(g);
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(s.q), "j"(*pv));
    return r;
}
// Return the vector the reference points to, scaled by g.
static inline qword MulVFR(const CRef *pr, float g)
{
    VU_FLOAT_F s(g);
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(s.q), "j"(*(const qword *)pr->u.pv));
    return r;
}
// Scale *pq in place by g (g's bits pinned to $17).
static inline void MulVectorR(qword *pq, const float &g)
{
    register int bits __asm__("$17") = *(const int *)&g; /* pin */
    VU_FLOAT_D s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(bits));
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(s.q), "j"(*pq));
    *pq = r;
}
// Scale *pq in place by g.
static inline void MulVector(qword *pq, float g)
{
    VU_FLOAT_F s(g);
    qword a = *pq, r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(s.q), "j"(a));
    *pq = r;
}
// Scale all 16 entries of *pm by d.
static inline void MulMatrixM(MATRIX4 *pm, float d)
{
    MATRIX4 mat;
    float g = VU_FLOAT_M(d).F();
    for (int i = 0; i < 16; i++)
        mat.af[i] = g * pm->af[i];
    *pm = mat;
}
// Scale all 16 entries of *pm by d.
static inline void MulMatrix(MATRIX4 *pm, const float &d)
{
    MATRIX4 mat;
    float g = VU_FLOAT(d).F();
    __asm__ volatile(""); /* scheduling barrier: keeps &mat after the mtc1 like the target */
    for (int i = 0; i < 16; i++)
        mat.af[i] = g * pm->af[i];
    *pm = mat;
}
// Transform the point *pv by the 4x4 matrix pm.
static inline qword MulMatVec(const MATRIX4 *pm, const qword *pv)
{
    const qword *aq = (const qword *)pm;
    qword r;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(r) : "j"(aq[0]), "j"(aq[1]), "j"(aq[2]), "j"(aq[3]), "j"(*pv));
    return r;
}

// Scale the x and y of p by d.
static inline void Mul2(V8 *p, float d)
{
    p->x *= d;
    p->y *= d;
}
// Splice builtin "*": n-ary multiply incl. vector*scalar, matrix*matrix (VU).
// Also known as: RefOpMult (upstream readable / Sly 1 name).
CRef BuiltinMultiply__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    int isIntResult = 0;
    switch (args[0].m_tagk) {
    case TAGK_S32:
        ref = (float)args[0].u.n;
        isIntResult = 1;
        break;
    case TAGK_F32:
    case TAGK_VECTOR:
    case TAGK_MATRIX:
    case TAGK_CLQ:
    case TAGK_LM:
        ref = CRef_CloneTo(&args[0], 0, 0);
        break;
    }
    CPair *node = 0;
    int bits;
    if (args[1].m_tagk)
        node = args[1].u.ppair;
    for (; node; node = node->next.p) {
        CRef *operand = &node->ref;
        switch (ref.m_tagk) {
        case TAGK_F32:
            switch (operand->m_tagk) {
            case TAGK_F32:
                isIntResult = 0;
                ref.u.g *= operand->u.g;
                break;
            case TAGK_S32:
                ref.u.g *= (float)operand->u.n;
                break;
            case TAGK_VECTOR:
                CRef_SetVector_VU_VECTOR(&ref, MulVFR(operand, ref.u.g));
                break;
            case TAGK_MATRIX: {
                MATRIX4 *pm = (MATRIX4 *)operand->u.pv;
                VU_FLOAT_M s(ref.u.g);
                float g = s.F();
                MATRIX4 mat;
                for (int i = 0; i < 16; i++)
                    mat.af[i] = g * pm->af[i];
                ref.SetMatrix((const ::MATRIX4 *)&mat);
                break;
            }
            case TAGK_CLQ: {
                float g = ref.u.g;
                ref.Decref();
                ref.InitTag(TAGK_CLQ);
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                VECTOR *pvSrc = (VECTOR *)operand->u.pv;
                VECTOR_opAssign(pvec, VU_FLOAT_X(g) * VU_VECTOR(*pvSrc));
                break;
            }
            case TAGK_LM: {
                float g = ref.u.g;
                ref.Decref();
                ref.InitTag(TAGK_LM);
                V8 *ps = (V8 *)operand->u.pv;
                V8 *pd = (V8 *)ref.u.pv;
                pd->x = ps->x * g;
                pd->y = ps->y * g;
                break;
            }
            }
            break;
        case TAGK_VECTOR:
            switch (operand->m_tagk) {
            case TAGK_S32: MulVector((qword *)ref.u.pv, (float)operand->u.n); break;
            case TAGK_F32: MulVectorR((qword *)ref.u.pv, operand->u.g); break;
            }
            break;
        case TAGK_MATRIX:
            switch (operand->m_tagk) {
            case TAGK_S32: MulMatrixM((MATRIX4 *)ref.u.pv, (float)operand->u.n); break;
            case TAGK_F32: MulMatrix((MATRIX4 *)ref.u.pv, operand->u.g); break;
            case TAGK_VECTOR: CRef_SetVector_VU_VECTOR(&ref, MulMatVec((MATRIX4 *)ref.u.pv, (qword *)operand->u.pv)); break;
            case TAGK_MATRIX: {
                MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
                *pm = __ml__FRC7MATRIX4T0(*pm, *(MATRIX4 *)operand->u.pv);
                break;
            }
            }
            break;
        case TAGK_CLQ:
            switch (operand->m_tagk) {
            case TAGK_S32: {
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                VECTOR_opAssign(pvec, VU_FLOAT_X((float)operand->u.n) * VU_VECTOR(*pvec));
                break;
            }
            case TAGK_F32: {
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                VECTOR_opAssign(pvec, VU_FLOAT_X(operand->u.g) * VU_VECTOR(*pvec));
                break;
            }
            }
            break;
        case TAGK_LM:
            switch (operand->m_tagk) {
            case TAGK_S32: Mul2((V8 *)ref.u.pv, (float)operand->u.n); break;
            case TAGK_F32: Mul2((V8 *)ref.u.pv, operand->u.g); break;
            }
            break;
        }
    }
    if (isIntResult && ref.m_tagk == TAGK_F32)
        ref = (int)ref.u.g;
    return ref;
}
} // namespace M_BuiltinMultiply__FiP4CRefP6CFrame

// 0x11FEC8 BuiltinDivide__FiP4CRefP6CFrame
namespace M_BuiltinDivide__FiP4CRefP6CFrame {
struct VECTOR; struct MATRIX4; struct V8; struct VU_FLOAT; struct VU_FLOAT_X; struct VU_VECTOR; struct VU_FLOAT_V;
// `/` script builtin (Claude agent). The int-matrix path builds its VU_FLOAT temp in the caller, before the inline scale
// helper frame, so the temp reuses the freed CRef slot at sp+0x10 as in the original.
struct VECTOR { float x, y, z, w; };
struct MATRIX4 { float af[16]; } __attribute__((aligned(16)));
struct V8 { float x, y; };
struct VU_FLOAT {
    qword q;
    // Move a float's bits into a GPR qword (mfc1).
    VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); }
    // The value as an FPU float.
    float F() const { float g; __asm__("mtc1 %1, %0" : "=f"(g) : "r"(q)); return g; }
};
struct VU_FLOAT_X { qword q; VU_FLOAT_X(float f) __asm__("__8VU_FLOATf"); };
struct VU_VECTOR { qword q; VU_VECTOR(const VECTOR &v) __asm__("__9VU_VECTORRC6VECTOR"); };
qword operator*(VU_FLOAT_X f, VU_VECTOR v) __asm__("__ml__FG8VU_FLOATG9VU_VECTOR");
extern qword *VECTOR_opAssign(VECTOR *p, qword v) __asm__("VECTOR_opAssign");
CRef CRef_CloneTo(CRef *pref, void *a, void *b) __asm__("CRef_CloneTo");
extern CRef BuiltinDivide__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinDivide__FiP4CRefP6CFrame");

struct VU_FLOAT_V {
    qword q;
    // Broadcast a float into a VU0 register.
    VU_FLOAT_V(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); }
};
// Divide *pq in place by d.
static inline void DivVector(qword *pq, float d)
{
    VU_FLOAT_V s(1.0f / d);
    qword a = *pq, r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(s.q), "j"(a));
    *pq = r;
}

// Scale all 16 entries of *pm by g.
static inline void ScaleM(MATRIX4 *pm, float g)
{
    MATRIX4 mat;
    for (int i = 0; i < 16; i++)
        mat.af[i] = g * pm->af[i];
    *pm = mat;
}
// Divide all 16 entries of *pm by d.
static inline void DivMatrix(MATRIX4 *pm, float d)
{
    MATRIX4 mat;
    float g = VU_FLOAT(1.0f / d).F();
    for (int i = 0; i < 16; i++)
        mat.af[i] = g * pm->af[i];
    *pm = mat;
}

// Splice builtin "/": n-ary divide incl. int-matrix path.
// Also known as: RefOpDiv (upstream readable / Sly 1 name).
CRef BuiltinDivide__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    int fInt = 0;
    switch (args[0].m_tagk) {
    case TAGK_S32:
        ref = (float)args[0].u.n;
        fInt = 1;
        break;
    case TAGK_F32:
    case TAGK_VECTOR:
    case TAGK_MATRIX:
    case TAGK_CLQ:
    case TAGK_LM:
        ref = CRef_CloneTo(&args[0], 0, 0);
        break;
    }
    CPair *pn = 0;
    if (args[1].m_tagk)
        pn = args[1].u.ppair;
    for (; pn; pn = pn->next.p) {
        CRef *pr = &pn->ref;
        switch (ref.m_tagk) {
        case TAGK_F32: {
            float d;
            switch (pr->m_tagk) {
            case TAGK_F32:
                d = pr->u.g;
                fInt = 0;
                break;
            case TAGK_S32:
                d = (float)pr->u.n;
                break;
            default:
                continue;
            }
            ref.u.g /= d;
            break;
        }
        case TAGK_VECTOR:
            switch (pr->m_tagk) {
            case TAGK_S32: DivVector((qword *)ref.u.pv, (float)pr->u.n); break;
            case TAGK_F32: DivVector((qword *)ref.u.pv, pr->u.g); break;
            }
            break;
        case TAGK_MATRIX:
            switch (pr->m_tagk) {
            case TAGK_S32: ScaleM((MATRIX4 *)ref.u.pv, VU_FLOAT(1.0f / (float)pr->u.n).F()); break;
            case TAGK_F32: DivMatrix((MATRIX4 *)ref.u.pv, pr->u.g); break;
            }
            break;
        case TAGK_CLQ:
            switch (pr->m_tagk) {
            case TAGK_S32: {
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                VECTOR_opAssign(pvec, VU_FLOAT_X(1.0f / (float)pr->u.n) * VU_VECTOR(*pvec));
                break;
            }
            case TAGK_F32: {
                VECTOR *pvec = (VECTOR *)ref.u.pv;
                VECTOR_opAssign(pvec, VU_FLOAT_X(1.0f / pr->u.g) * VU_VECTOR(*pvec));
                break;
            }
            }
            break;
        case TAGK_LM: {
            float d;
            switch (pr->m_tagk) {
            case TAGK_S32: d = (float)pr->u.n; break;
            case TAGK_F32: d = pr->u.g; break;
            default: continue;
            }
            float s = 1.0f / d;
            V8 *p = (V8 *)ref.u.pv;
            p->x *= s;
            p->y *= s;
            break;
        }
        }
    }
    if (fInt && ref.m_tagk == TAGK_F32)
        ref = (int)ref.u.g;
    return ref;
}
} // namespace M_BuiltinDivide__FiP4CRefP6CFrame

// 0x1202D0 BuiltinPrint__FiP4CRefP6CFrame
// Splice builtin "print": returns Void (tag 0x24): print compiled out.
// Also known as: RefOpPrint (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinPrint__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinPrint__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    SetTag(pref, TAGK_VOID);
    return pref;
}

// 0x120300 BuiltinPrintFrame__FiP4CRefP6CFrame
// Splice builtin "print-frame": returns Void.
// Also known as: RefOpPrintFrame (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinPrintFrame__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinPrintFrame__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    SetTag(pref, TAGK_VOID);
    return pref;
}

// 0x120330 BuiltinPrintSidebag__FiP4CRefP6CFrame
namespace M_BuiltinPrintSidebag__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
// Splice builtin "print-sidebag": returns Void (local CRef then InitTag 0x24).
// Also known as: RefOpPrintSidebag (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinPrintSidebag__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinPrintSidebag__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    SetTag__4CRef4TAGK(ret, 0x24);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinPrintSidebag__FiP4CRefP6CFrame

// 0x120370 BuiltinIntEqual__FiP4CRefP6CFrame
namespace M_BuiltinIntEqual__FiP4CRefP6CFrame {
// Splice builtin "=": bool(tag0==tag1 payload ints equal).
// Compares only the two payload words, never the tags (Sly 1's '=').
// Also known as: RefOpIntEqual (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIntEqual__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIntEqual__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    int a = args[0].u.n;
    int b = args[1].u.n;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = a == b;
    return ret;
}
} // namespace M_BuiltinIntEqual__FiP4CRefP6CFrame

// 0x120398 CompareNumericArgs__FP4CRef4CMPK
namespace M_CompareNumericArgs__FP4CRef4CMPK {
// Numeric compare of args[0],args[1] (int->float promote) by CMPK 0..3 (<,<=,>,>=) -> Bool.
// Also known as: RefCmp (upstream readable / Sly 1 name).
extern "C" CRefData *CompareNumericArgs__FP4CRef4CMPK(CRefData *ret, CRefData *args, int op)
{
    float ag[2];
    int i;
    int f;
    for (i = 0; i < 2; i++) {
        if (args[i].m_tagk == TAGK_F32)
            ag[i] = args[i].u.g;
        else
            ag[i] = (float)args[i].u.n;
    }
    switch (op) {
    case 0:
        f = ag[0] < ag[1];
        break;
    case 1:
        f = ag[0] <= ag[1];
        break;
    case 2:
        f = ag[0] > ag[1];
        break;
    case 3:
        f = ag[0] >= ag[1];
        break;
    }
    ret->u.n = f;
    ret->m_tagk = TAGK_BOOL;
    return ret;
}
} // namespace M_CompareNumericArgs__FP4CRef4CMPK

// 0x120488 BuiltinLess__FiP4CRefP6CFrame
// Splice builtin "<": CompareNumericArgs(args,0) <.
// Also known as: RefOpL (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinLess__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinLess__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CompareNumericArgs(pref, args, 0);
    return pref;
}

// 0x1204B8 BuiltinLessOrEqual__FiP4CRefP6CFrame
// Splice builtin "<=": CompareNumericArgs(args,1) <=.
// Also known as: RefOpLE (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinLessOrEqual__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinLessOrEqual__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CompareNumericArgs(pref, args, 1);
    return pref;
}

// 0x1204E8 BuiltinGreater__FiP4CRefP6CFrame
// Splice builtin ">": CompareNumericArgs(args,2) >.
// Also known as: RefOpG (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinGreater__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinGreater__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CompareNumericArgs(pref, args, 2);
    return pref;
}

// 0x120518 BuiltinGreaterOrEqual__FiP4CRefP6CFrame
// Splice builtin ">=": CompareNumericArgs(args,3) >=.
// Also known as: RefOpGE (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinGreaterOrEqual__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinGreaterOrEqual__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CompareNumericArgs(pref, args, 3);
    return pref;
}

// 0x120548 BuiltinEqv__FiP4CRefP6CFrame
namespace M_BuiltinEqv__FiP4CRefP6CFrame {
// MATCH (Claude agent laneB14, 2026-10-23), from BuiltinEqv__FiP4CRefP6CFrame.cpp (6). FEq body written out with one commented register pin (pb in a1)
// plus one empty asm whose only job is extra uses of f (f is live from entry on the default path, so its priority was below the return pointer's).
extern CRef BuiltinEqv__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinEqv__FiP4CRefP6CFrame");
// Splice builtin "eqv?": shallow eqv: same tag and same payload / vector compare.
// Also known as: RefOpEqv (upstream readable / Sly 1 name).
CRef BuiltinEqv__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef *pa = &args[0];
    register CRef *pb __asm__("$5") = &args[1];  /* register pin: &aref[1] reuses the dead cref register (a1) as in the target */
    int f;
    if (pa->m_tagk != pb->m_tagk) {
        f = 0;
    } else {
        switch (pa->m_tagk) {
        case TAGK_S32: case TAGK_BOOL: case TAGK_SYMID: case TAGK_BIFK:
        case TAGK_PAIR: case TAGK_PROC: case TAGK_BASIC: case TAGK_METHOD:
            f = pa->u.n == pb->u.n;
            break;
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18:
        case 19: case 20: case TAGK_VOID:
            f = 1;
            break;
        default:
            break;
        }
    }
    __asm__ __volatile__("" : : "r"(f), "r"(f), "r"(f));  /* allocation-only operands: raise f's global-alloc priority above the return pointer (f a3, ret t0 as in the target) */
    return CRef(f, TAGK_BOOL);
}
} // namespace M_BuiltinEqv__FiP4CRefP6CFrame

// 0x1205B8 AreRefsStructurallyEqual__FP4CRefT0
namespace M_AreRefsStructurallyEqual__FP4CRefT0 {
struct W; struct X;
// CRef equality (recursive). Matched by Claude agent (2026-10-17): case 0x22 as `f = n8 == n8 && nC == nC`.
struct W { int cref; int n4; int n8; CRef ref; int n14; CPair *pnext; };
struct X { int cref; int n4; int n8; int nC; };

extern CRef AreRefsStructurallyEqual__FP4CRefT0(const CRef &a, const CRef &b) __asm__("AreRefsStructurallyEqual__FP4CRefT0");
// Recursive structural equality (lists, vectors, matrices) -> Bool.
// Also known as: RefEqualHelper (upstream readable / Sly 1 name).
CRef AreRefsStructurallyEqual__FP4CRefT0(const CRef &a, const CRef &b)
{
    int f;
    if (a.m_tagk != b.m_tagk) {
        f = 0;
    } else {
        switch (a.m_tagk) {
        case TAGK_PAIR: {
            CPair *pa = a.u.ppair;
            CPair *pb = b.u.ppair;
            if (pa == pb) {
                f = 1;
            } else if ((pa->next.p == 0) != (pb->next.p == 0)) {
                f = 0;
            } else {
                CRef r;
                r = AreRefsStructurallyEqual__FP4CRefT0(pa->ref, pb->ref);
                if (r.u.n == 0) {
                    f = 0;
                } else if (pa->next.p == 0) {
                    f = 1;
                } else {
                    CRef ra(pa->next.p);
                    CRef rb(pb->next.p);
                    r = AreRefsStructurallyEqual__FP4CRefT0(ra, rb);
                    f = r.u.n;
                }
            }
            break;
        }
        case TAGK_PROC: {
            W *pa = (W *)a.u.pv;
            W *pb = (W *)b.u.pv;
            if (pa == pb) {
                f = 1;
            } else if (pa->ref.m_tagk != pb->ref.m_tagk) {
                f = 0;
            } else if (pa->ref.u.n != pb->ref.u.n) {
                f = 0;
            } else if (pa->n8 != pb->n8) {
                f = 0;
            } else {
                CRef ra(pa->pnext);
                CRef rb(pb->pnext);
                f = AreRefsStructurallyEqual__FP4CRefT0(ra, rb).u.n;
            }
            break;
        }
        case TAGK_S32: case TAGK_BOOL: case TAGK_SYMID: case TAGK_BIFK: case TAGK_BASIC:
            f = a.u.n == b.u.n;
            break;
        case TAGK_METHOD: {
            X *pa = (X *)a.u.pv;
            X *pb = (X *)b.u.pv;
            if (pa == pb) {
                f = 1;
            } else {
                f = pa->n8 == pb->n8 && pa->nC == pb->nC;
            }
            break;
        }
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18:
        case 19: case 20: case TAGK_VOID:
            f = 1;
            break;
        }
    }
    return CRef(f, TAGK_BOOL);
}
} // namespace M_AreRefsStructurallyEqual__FP4CRefT0

// 0x1207F0 BuiltinEqual__FiP4CRefP6CFrame
// Splice builtin "equal?": calls AreRefsStructurallyEqual (recursive).
// Also known as: RefOpEqual (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinEqual__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinEqual__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    AreRefsStructurallyEqual(pref, &args[0], &args[1]);
    return pref;
}

// 0x120820 BuiltinIsBoolean__FiP4CRefP6CFrame
namespace M_BuiltinIsBoolean__FiP4CRefP6CFrame {
// Splice builtin "boolean?": tag==0x17.
// Also known as: RefOpIsBoolean (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsBoolean__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsBoolean__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_BOOL;
    return ret;
}
} // namespace M_BuiltinIsBoolean__FiP4CRefP6CFrame

// 0x120840 BuiltinIsNum__FiP4CRefP6CFrame
namespace M_BuiltinIsNum__FiP4CRefP6CFrame {
// Splice builtin "number?": tag==0x15||0x16.
// Also known as: RefOpIsNum (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsNum__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsNum__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_S32 || tagk == TAGK_F32;
    return ret;
}
} // namespace M_BuiltinIsNum__FiP4CRefP6CFrame

// 0x120860 BuiltinIsFloat__FiP4CRefP6CFrame
namespace M_BuiltinIsFloat__FiP4CRefP6CFrame {
// Splice builtin "float?": tag==0x16 (F32).
// Also known as: RefOpIsFloat (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsFloat__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsFloat__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_F32;
    return ret;
}
} // namespace M_BuiltinIsFloat__FiP4CRefP6CFrame

// 0x120880 BuiltinIsInteger__FiP4CRefP6CFrame
namespace M_BuiltinIsInteger__FiP4CRefP6CFrame {
// Splice builtin "integer?": tag==0x15 (S32).
// Also known as: RefOpIsInteger (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsInteger__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsInteger__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_S32;
    return ret;
}
} // namespace M_BuiltinIsInteger__FiP4CRefP6CFrame

// 0x1208A0 BuiltinIsSymbol__FiP4CRefP6CFrame
namespace M_BuiltinIsSymbol__FiP4CRefP6CFrame {
// Splice builtin "symbol?": tag==0x18.
// Also known as: RefOpIsSymbol (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsSymbol__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsSymbol__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_SYMID;
    return ret;
}
} // namespace M_BuiltinIsSymbol__FiP4CRefP6CFrame

// 0x1208C0 BuiltinIsVector__FiP4CRefP6CFrame
namespace M_BuiltinIsVector__FiP4CRefP6CFrame {
// Splice builtin "vector?": tag==0x1A.
// Also known as: RefOpIsVector (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsVector__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsVector__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_VECTOR;
    return ret;
}
} // namespace M_BuiltinIsVector__FiP4CRefP6CFrame

// 0x1208E0 BuiltinIsMatrix__FiP4CRefP6CFrame
namespace M_BuiltinIsMatrix__FiP4CRefP6CFrame {
// Splice builtin "matrix?": tag==0x1B.
// Also known as: RefOpIsMatrix (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsMatrix__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsMatrix__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_MATRIX;
    return ret;
}
} // namespace M_BuiltinIsMatrix__FiP4CRefP6CFrame

// 0x120900 BuiltinIsClq__FiP4CRefP6CFrame
namespace M_BuiltinIsClq__FiP4CRefP6CFrame {
// Splice builtin "clq?": tag==0x1C.
// Also known as: RefOpIsClq (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsClq__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsClq__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_CLQ;
    return ret;
}
} // namespace M_BuiltinIsClq__FiP4CRefP6CFrame

// 0x120920 BuiltinIsLm__FiP4CRefP6CFrame
namespace M_BuiltinIsLm__FiP4CRefP6CFrame {
// Splice builtin "lm?": tag==0x1D.
// Also known as: RefOpIsLm (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsLm__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsLm__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_LM;
    return ret;
}
} // namespace M_BuiltinIsLm__FiP4CRefP6CFrame

// 0x120940 BuiltinIsSmp__FiP4CRefP6CFrame
namespace M_BuiltinIsSmp__FiP4CRefP6CFrame {
// Splice builtin "smp?": tag==0x1E.
// Also known as: RefOpIsSmp (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsSmp__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsSmp__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_SMP;
    return ret;
}
} // namespace M_BuiltinIsSmp__FiP4CRefP6CFrame

// 0x120960 BuiltinIsList__FiP4CRefP6CFrame
namespace M_BuiltinIsList__FiP4CRefP6CFrame {
extern CRef BuiltinIsList__FiP4CRefP6CFrame(int cref, CRef *pref, CFrame *frame) __asm__("BuiltinIsList__FiP4CRefP6CFrame");

// Splice builtin "list?": tag==0x1F||tag==0.
// Also known as: RefOpIsList (upstream readable / Sly 1 name).
CRef BuiltinIsList__FiP4CRefP6CFrame(int cref, CRef *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    return CRef(tagk == TAGK_PAIR || tagk == 0);
}
} // namespace M_BuiltinIsList__FiP4CRefP6CFrame

// 0x120990 BuiltinIsNull__FiP4CRefP6CFrame
namespace M_BuiltinIsNull__FiP4CRefP6CFrame {
// Splice builtin "null?": tag==0.
// Also known as: RefOpIsNull (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsNull__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsNull__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == 0;
    return ret;
}
} // namespace M_BuiltinIsNull__FiP4CRefP6CFrame

// 0x1209B0 BuiltinIsObject__FiP4CRefP6CFrame
namespace M_BuiltinIsObject__FiP4CRefP6CFrame {
// Splice builtin "object?": tag==0x21.
// Also known as: RefOpIsObject (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsObject__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsObject__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_BASIC;
    return ret;
}
} // namespace M_BuiltinIsObject__FiP4CRefP6CFrame

// 0x1209D0 BuiltinIsNullObj__FiP4CRefP6CFrame
// Splice builtin "null-obj?": tag==0x21 && ptr==NULL.
/* (null-object? x): x is an object reference (tag 0x21) holding no object. */
// Also known as: RefOpIsNullObj (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsNullObj__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsNullObj__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    int f;
    f = 0;
    if (args->m_tagk == TAGK_BASIC) {
        f = args->u.pv == 0;
    }
    pref->u.n = f;
    pref->m_tagk = TAGK_BOOL;
    return pref;
}

// 0x120A00 BuiltinIsMethod__FiP4CRefP6CFrame
namespace M_BuiltinIsMethod__FiP4CRefP6CFrame {
// Splice builtin "method?": tag==0x22.
// Also known as: RefOpIsMethod (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinIsMethod__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinIsMethod__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = tagk == TAGK_METHOD;
    return ret;
}
} // namespace M_BuiltinIsMethod__FiP4CRefP6CFrame

// 0x120A20 BuiltinIsProcedure__FiP4CRefP6CFrame
namespace M_BuiltinIsProcedure__FiP4CRefP6CFrame {
extern CRef BuiltinIsProcedure__FiP4CRefP6CFrame(int cref, CRef *pref, CFrame *frame) __asm__("BuiltinIsProcedure__FiP4CRefP6CFrame");

// Splice builtin "procedure?": tag==0x20||0x19.
// Also known as: RefOpIsProcedure (upstream readable / Sly 1 name).
CRef BuiltinIsProcedure__FiP4CRefP6CFrame(int cref, CRef *pref, CFrame *frame)
{
    int tagk = pref->m_tagk;
    return CRef(tagk == TAGK_PROC || tagk == TAGK_BIFK);
}
} // namespace M_BuiltinIsProcedure__FiP4CRefP6CFrame

// 0x120A58 BuiltinAreNear__FiP4CRefP6CFrame
namespace M_BuiltinAreNear__FiP4CRefP6CFrame {
int AreFloatsNear__Ffff(float a, float b, float c) __asm__("AreFloatsNear__Ffff");
// Splice builtin "near?": AreFloatsNear(a,b,eps).
// Also known as: RefOpAreNear (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinAreNear__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAreNear__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float ag[3];
    int i;
    int f;
    for (i = 0; i < 3; i++) {
        CRefData *p = &args[i];
        if (p->m_tagk == TAGK_S32) ag[i] = p->u.n; else ag[i] = p->u.g;
    }
    f = AreFloatsNear__Ffff(ag[0], ag[1], ag[2]) != 0;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = f;
    return ret;
}
} // namespace M_BuiltinAreNear__FiP4CRefP6CFrame

// 0x120AE0 BuiltinNot__FiP4CRefP6CFrame
// Splice builtin "not": true iff arg is Bool false.
/* (not x): true only for the boolean false. */
// Also known as: RefOpNot (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinNot__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinNot__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    int f;
    f = 0;
    if (args->m_tagk == TAGK_BOOL) {
        f = args->u.n == 0;
    }
    pref->m_tagk = TAGK_BOOL;
    pref->u.n = f;
    return pref;
}

// 0x120B08 BuiltinCons__FiP4CRefP6CFrame
namespace M_BuiltinCons__FiP4CRefP6CFrame {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern CRef BuiltinCons__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinCons__FiP4CRefP6CFrame");
// Splice builtin "cons": new pair node (ref=a0, next=a1 if list).
// Also known as: RefOpCons (upstream readable / Sly 1 name).
CRef BuiltinCons__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    SPN sp;
    sp.Set(CGcObj<CPair>::PtNew());
    sp->ref = args[0];
    CRef *pref = &args[1];
    if (pref->m_tagk == TAGK_PAIR)
        sp->next = pref->u.ppair;
    return CRef(sp.p);
}
} // namespace M_BuiltinCons__FiP4CRefP6CFrame

// 0x120BD0 BuiltinCar__FiP4CRefP6CFrame
// Splice builtin "car": copy of node->ref.
// Also known as: RefOpCar (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinCar__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinCar__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    __4CRefRC4CRef(pref, &args->u.ppair->ref);
    return pref;
}

// 0x120C00 BuiltinCdr__FiP4CRefP6CFrame
// Splice builtin "cdr": CRef(node->pnext) or Nil.
// Also known as: RefOpCdr (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinCdr__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinCdr__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CPair *ppairNext;
    ppairNext = args->u.ppair->next.p;
    if (ppairNext == 0) {
        SetTag(pref, 0);
    } else {
        CRef_ctor_CPair(pref, ppairNext);
    }
    return pref;
}

// 0x120C48 BuiltinSetCadr__FiP4CRefP6CFrame4BIFK
namespace M_BuiltinSetCadr__FiP4CRefP6CFrame4BIFK {
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void func_00129658(CRefData *pref, CRefData *prefSrc) __asm__("__as__4CRefRC4CRef");
void PairPtr_Assign(void *p, void *q) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
// Shared body of set-car!/set-cdr!: bifk 0x23 sets node->ref, else node->pnext; returns Void.
// Also known as: RefOpSetCadr (upstream readable / Sly 1 name).
extern "C" CRefData *BuiltinSetCadr__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k)
{
    char *p = (__typeof__(p))args[0].u.pv;
    if (k == 0x23)
        func_00129658((CRefData *)(p + 8), &args[1]);
    else if (args[1].m_tagk == TAGK_PAIR)
        PairPtr_Assign(p + 0x10, args[1].u.pv);
    else
        PairPtr_Assign(p + 0x10, 0);
    SetTag__4CRef4TAGK(ret, 0x24);
    return ret;
}
} // namespace M_BuiltinSetCadr__FiP4CRefP6CFrame4BIFK

// 0x120CC8 BuiltinSetCar__FiP4CRefP6CFrame
namespace M_BuiltinSetCar__FiP4CRefP6CFrame {
void BuiltinSetCadr__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k) __asm__("BuiltinSetCadr__FiP4CRefP6CFrame4BIFK");
// Splice builtin "set-car!": BuiltinSetCadr(...,0x23).
// Also known as: RefOpSetCar (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinSetCar__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinSetCar__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    BuiltinSetCadr__FiP4CRefP6CFrame4BIFK(ret, cref, args, frame, 0x23);
    return ret;
}
} // namespace M_BuiltinSetCar__FiP4CRefP6CFrame

// 0x120CF8 BuiltinSetCdr__FiP4CRefP6CFrame
namespace M_BuiltinSetCdr__FiP4CRefP6CFrame {
void BuiltinSetCadr__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k) __asm__("BuiltinSetCadr__FiP4CRefP6CFrame4BIFK");
// Splice builtin "set-cdr!": BuiltinSetCadr(...,0x24).
// Also known as: RefOpSetCdr (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinSetCdr__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinSetCdr__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    BuiltinSetCadr__FiP4CRefP6CFrame4BIFK(ret, cref, args, frame, 0x24);
    return ret;
}
} // namespace M_BuiltinSetCdr__FiP4CRefP6CFrame

// 0x120D28 BuiltinLength__FiP4CRefP6CFrame
// Splice builtin "length": count of list nodes -> S32.
// Also known as: RefOpLength (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinLength__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinLength__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    int c;
    CPair *ppair;
    c = 0;
    if (args->m_tagk == TAGK_PAIR) {
        ppair = args->u.ppair;
        if (ppair != 0) {
            do {
                ppair = ppair->next.p;
                c += 1;
            } while (ppair != 0);
        }
    }
    pref->u.n = c;
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x120D78 BuiltinNth__FiP4CRefP6CFrame
namespace M_BuiltinNth__FiP4CRefP6CFrame {
struct PAIR;
typedef struct PAIR { int a; int b; CRefData ref; struct PAIR *pnext; } PAIR;
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void func_00129658(CRefData *pref, CRefData *prefSrc) __asm__("__as__4CRefRC4CRef");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "nth": walk list to index a1.
// Also known as: RefOpNth (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinNth__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinNth__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    SetTag__4CRef4TAGK(&ref, 0);
    if (args[0].m_tagk == TAGK_PAIR) {
        PAIR *p = (PAIR *)args[0].u.pv;
        CRefData *pn = &args[1];
        int i;
        for (i = 0; p; p = p->pnext, i++) {
            if (i == pn->u.n) {
                func_00129658(&ref, &p->ref);
                break;
            }
        }
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinNth__FiP4CRefP6CFrame

// 0x120E10 BuiltinIsMember__FiP4CRefP6CFrame
namespace M_BuiltinIsMember__FiP4CRefP6CFrame {


CRef BuiltinEqual__FiP4CRefP6CFrame(int carg, CRef *args, CFrame *frame) __asm__("BuiltinEqual__FiP4CRefP6CFrame");
CRef BuiltinIsMember__FiP4CRefP6CFrame(int carg, CRef *args, CFrame *frame) __asm__("BuiltinIsMember__FiP4CRefP6CFrame");

// Splice builtin "member?": loop calling BuiltinEqual on each element.
// Also known as: RefOpIsMember (upstream readable / Sly 1 name).
CRef BuiltinIsMember__FiP4CRefP6CFrame(int carg, CRef *args, CFrame *frame)
{
    CRef ref;
    CRef arefT[2];
    CRef res(false);
    CRef *pres = &res;
    if (args[1].m_tagk == TAGK_PAIR) {
        CPair *p;
        arefT[0] = args[0];
        for (p = args[1].u.ppair; p; p = p->next.p) {
            arefT[1] = p->ref;
            ref = BuiltinEqual__FiP4CRefP6CFrame(2, arefT, frame);
            if (ref.m_tagk == TAGK_BOOL && ref.u.n) {
                pres->u.n = 1;
                break;
            }
        }
    }
    return res;
}
} // namespace M_BuiltinIsMember__FiP4CRefP6CFrame

// 0x120F80 BuiltinList__FiP4CRefP6CFrame
// Splice builtin "list": returns the arg list itself (varargs).
// Also known as: RefOpList (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinList__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinList__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    __4CRefRC4CRef(pref, args);
    return pref;
}

// 0x120FB0 BuiltinShuffle
namespace M_BuiltinShuffle {
struct SPN;
// (Claude agent, laneB10) MATCH: the !fShuffle reassign goes through an inline SetSpn(SPN*, CPair*) so &spRes is substituted directly (no pseudo for gcse to share with pspHead).

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");
int RandomIntInRange__Fii(int nLo, int nHi) __asm__("RandomIntInRange__Fii");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    void operator=(CPair *pn) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
    // Access the held pair.
    CPair *operator->() { return p; }
    int pad[3];
};
extern CRef BuiltinShuffle(int cref, CRef *args, CFrame *frame) __asm__("BuiltinShuffle");
// Assign pn to a pair pointer.
static inline void SetSpn(SPN *psp, CPair *pn) { *psp = pn; }
// Insert the pair held by sp at the front of the list psp.
static inline void InsertAfter(CPairPtr *psp, SPN &sp) { sp->next = psp->p; *psp = sp.p; }
// Random permutation of a list (Fisher-Yates via RandomIntInRange 0x11D3F8), guarantees first element changes; Nil entry
// pins tail. Scripts use it to reshuffle dialogue line lists.
// Also known as: RefOpShuffle (upstream readable / Sly 1 name; RefOpShuffle__FiP4CRefP6CFrame).
CRef BuiltinShuffle(int cref, CRef *args, CFrame *frame)
{
    if (args[0].m_tagk == 0) return CRef(0);
    if (args[0].m_tagk != TAGK_PAIR) return CRef(0);
    CRef *apref[128];
    int c = 0;
    int iNil = -1;
    SPN spRes;
    CPairPtr *pspHead = (CPairPtr *)&spRes;
    CPair *pn;
    for (pn = args[0].u.ppair; pn; pn = pn->next.p) {
        if (pn->ref.m_tagk == 0) iNil = c;
        else apref[c++] = &pn->ref;
    }
    int fShuffle = c >= 2 && iNil < 1;
    if (fShuffle) {
        CRef *prefFirst = apref[0];
        int i;
        for (i = 0; i < c; i++) {
            int j = RandomIntInRange__Fii(0, c - 1);
            CRef *pref = apref[j];
            apref[j] = apref[i];
            apref[i] = pref;
        }
        if (apref[0] == prefFirst) {
            apref[0] = apref[c - 1];
            apref[c - 1] = prefFirst;
        }
        iNil = c;
    }
    spRes.p = 0;
    CPair *pnLast = 0;
    CPairPtr *psp = pspHead;
    int i;
    for (i = 0; i < c; i++) {
        *psp = CGcObj<CPair>::PtNew();
        CPair *pnNew = psp->p;
        pnNew->ref = *apref[i];
        psp = &pnNew->next;
        if (i == iNil - 1) pnLast = pnNew;
    }
    if (pnLast && pnLast != spRes.p) {
        if (!fShuffle) {
            SPN sp;
            sp.Set(spRes.p);
            SetSpn(&spRes, spRes->next.p);
            InsertAfter(&pnLast->next, sp);
        }
        if (spRes.p != pnLast) {
            SPN sp;
            CPair *pnNew = CGcObj<CPair>::PtNew();
            sp.Set(pnNew);
            sp->ref = CRef(0);
            InsertAfter(&pnLast->next, sp);
        }
    }
    return CRef(spRes.p);
}
} // namespace M_BuiltinShuffle

// 0x1212F8 BuiltinAppend__FiP4CRefP6CFrame
namespace M_BuiltinAppend__FiP4CRefP6CFrame {
struct SP;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SP {
    CPair *p;
    // Empty pair pointer.
    SP() { p = 0; }
    SP(const SP &);
    // Drop the held pair's reference, deleting it on the last one.
    ~SP() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    void operator=(CPair *pn) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern SP CPair_CloneTo(CPair *src, void *a, void *b) __asm__("CPair_CloneTo");
extern CRef BuiltinAppend__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinAppend__FiP4CRefP6CFrame");
// Splice builtin "append": concatenates list args (copies all but last).
// Also known as: RefOpAppend (upstream readable / Sly 1 name).
CRef BuiltinAppend__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    if (args[0].m_tagk == 0)
        return args[0];
    SP result;
    CPair *tail;
    for (CPair *argPair = args[0].u.ppair; argPair; argPair = argPair->next.p) {
        if (argPair->ref.m_tagk != TAGK_PAIR)
            continue;
        SP piece;   /* every list but the last is copied; the last is shared */
        if (argPair->next.p) {
            SP copy = CPair_CloneTo(argPair->ref.u.ppair, 0, 0);
            piece = copy.p;
        } else {
            piece = argPair->ref.u.ppair;
        }
        if (!result.p) {
            result = CGcObj<CPair>::PtNew();
            result->ref = piece.p;
        } else {
            tail->next = piece.p;
        }
        tail = piece.p;
        while (tail->next.p)
            tail = tail->next.p;
    }
    if (!result.p)
        return CRef(0);
    return CRef(result->ref.u.ppair);
}
} // namespace M_BuiltinAppend__FiP4CRefP6CFrame

// 0x121578 BuildCallListForTwoArgs
namespace M_BuildCallListForTwoArgs {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Uninitialised pair pointer.
    SPN() {}
    // Hold pn (adds a reference).
    SPN(CPair *pn) { Set(pn); }
    SPN(const SPN &);
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern SPN BuildCallListForTwoArgs(CRef **ppref1, CRef **ppref2) __asm__("BuildCallListForTwoArgs");
// Allocates a 3-node call list (proc a b) and returns pointers to the two arg slots, for map/filter/for-each.
SPN BuildCallListForTwoArgs(CRef **ppref1, CRef **ppref2)
{
    SPN sp;
    sp.Set(CGcObj<CPair>::PtNew());
    sp->ref = CGcObj<CPair>::PtNew();
    CPair *pn1 = (CPair *)sp->ref.u.pv;
    CRef *pref1 = &pn1->ref;
    *pref1 = CGcObj<CPair>::PtNew();
    pn1->SetNext(CGcObj<CPair>::PtNew());
    CPair *pn2 = pref1->u.ppair;
    pn2->ref.Retag(TAGK_QUOTE);
    pn2->SetNext(CGcObj<CPair>::PtNew());
    *ppref1 = &pn2->next.p->ref;
    CRef *pref2 = &pn1->next.p->ref;
    *pref2 = CGcObj<CPair>::PtNew();
    CPair *pn3 = pref2->u.ppair;
    pn3->ref.Retag(TAGK_QUOTE);
    pn3->SetNext(CGcObj<CPair>::PtNew());
    *ppref2 = &pn3->next.p->ref;
    return SPN(sp.p);
}
} // namespace M_BuildCallListForTwoArgs

// 0x121710 BuiltinMap__FiP4CRefP6CFrame
namespace M_BuiltinMap__FiP4CRefP6CFrame {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
    int pad[3];
};
extern void BuildCallListForTwoArgs(SPN *psp, CRef **ppa, CRef **ppb) __asm__("BuildCallListForTwoArgs");
extern CRef EvalExpression__FP5CPairP6CFrame(CPair *pn, CFrame *frame) __asm__("EvalExpression__FP5CPairP6CFrame");
extern CRef BuiltinMap__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMap__FiP4CRefP6CFrame");
// Splice builtin "map": builds (proc a b) via helper 0x121578, eval 0x127838 per elem, collects results.
// Also known as: RefOpMap (upstream readable / Sly 1 name).
CRef BuiltinMap__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    if (args[1].m_tagk == 0)
        return args[1];
    SPN result;
    result.Set(CGcObj<CPair>::PtNew());
    SPN call;   /* (proc 'element) */
    CRef *procSlot;
    CRef *elementSlot;
    BuildCallListForTwoArgs(&call, &procSlot, &elementSlot);
    *procSlot = args[0];
    CPair *tail;
    CPair *element;
    element = args[1].u.ppair;
    while (element) {
        *elementSlot = element->ref;
        SPN newPair;
        CPair *pair = CGcObj<CPair>::PtNew();
        newPair.Set(pair);
        newPair->ref = EvalExpression__FP5CPairP6CFrame(call.p, frame);
        if (result->ref.m_tagk != TAGK_PAIR)
            result->ref = newPair.p;
        else
            tail->next = newPair.p;
        CPair *appended = newPair.p;
        element = element->next.p;
        tail = appended;
    }
    return CRef((CPair *)result->ref.u.pv);
}
} // namespace M_BuiltinMap__FiP4CRefP6CFrame

// 0x121938 BuiltinFilter__FiP4CRefP6CFrame
namespace M_BuiltinFilter__FiP4CRefP6CFrame {
struct SP; struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SP {
    CPair *p;
    // Empty pair pointer.
    SP() { p = 0; }
    SP(const SP &);
    // Drop the held pair's reference, deleting it on the last one.
    ~SP() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    void operator=(CPair *pn) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
    // Access the held pair.
    CPair *operator->() { return p; }
};
struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
    int pad[3];
};
extern void BuildCallListForTwoArgs(SPN *psp, CRef **ppa, CRef **ppb) __asm__("BuildCallListForTwoArgs");
extern CRef EvalExpression__FP5CPairP6CFrame(CPair *pn, CFrame *frame) __asm__("EvalExpression__FP5CPairP6CFrame");
extern CRef BuiltinFilter__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFilter__FiP4CRefP6CFrame");
// Splice builtin "filter": same, keeps elems where result true.
// Also known as: RefOpFilter (upstream readable / Sly 1 name).
CRef BuiltinFilter__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    if (args[1].m_tagk == 0)
        return args[1];
    SPN call;   /* (proc 'element) */
    CRef *procSlot;
    CRef *elementSlot;
    BuildCallListForTwoArgs(&call, &procSlot, &elementSlot);
    *procSlot = args[0];
    SP result;
    CPair *tail;
    CPair *element = args[1].u.ppair;
    while (element) {
        *elementSlot = element->ref;
        CRef keep = EvalExpression__FP5CPairP6CFrame(call.p, frame);
        if (keep.m_tagk == TAGK_BOOL && keep.u.n) {
            SPN newPair;
            CPair *pair = CGcObj<CPair>::PtNew();
            newPair.Set(pair);
            newPair->ref = element->ref;
            if (!result.p) {
                CPair *head = CGcObj<CPair>::PtNew();
                result = head;
                result->ref = newPair.p;
            } else {
                tail->next = newPair.p;
            }
            CPair *appended = newPair.p;
            tail = appended;
        }
        element = element->next.p;
    }
    if (!result.p)
        return CRef(0);
    return CRef(result->ref.u.ppair);
}
} // namespace M_BuiltinFilter__FiP4CRefP6CFrame

// 0x121C20 BuiltinForEach__FiP4CRefP6CFrame
namespace M_BuiltinForEach__FiP4CRefP6CFrame {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn);
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern void BuildCallListForTwoArgs(SPN *psp, CRef **ppa, CRef **ppb) __asm__("BuildCallListForTwoArgs");
extern CRef EvalExpression__FP5CPairP6CFrame(CPair *pn, CFrame *frame) __asm__("EvalExpression__FP5CPairP6CFrame");
extern CRef BuiltinForEach__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinForEach__FiP4CRefP6CFrame");
// Splice builtin "for-each": same, discards results.
// Also known as: RefOpForEach (upstream readable / Sly 1 name).
CRef BuiltinForEach__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    if (args[1].m_tagk == TAGK_PAIR) {
        SPN sp;
        CRef *pa;
        CRef *pb;
        CPair *pn;
        BuildCallListForTwoArgs(&sp, &pa, &pb);
        *pa = args[0];
        pn = args[1].u.ppair;
        while (pn) {
            *pb = pn->ref;
            CRef ref = EvalExpression__FP5CPairP6CFrame(sp.p, frame);
            pn = pn->next.p;
        }
    }
    return CRef(TAGK_VOID);
}
} // namespace M_BuiltinForEach__FiP4CRefP6CFrame

// 0x121D20 BuiltinEval__FiP4CRefP6CFrame
namespace M_BuiltinEval__FiP4CRefP6CFrame {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern CRef EvalExpression__FP5CPairP6CFrame(CPair *pn, CFrame *frame) __asm__("EvalExpression__FP5CPairP6CFrame");
extern CRef BuiltinEval__FiP4CRefP6CFrame(int cref, CRef *pref, CFrame *frame) __asm__("BuiltinEval__FiP4CRefP6CFrame");
// Splice builtin "eval": wraps arg in list and applies 0x127838.
// Also known as: RefOpEval (upstream readable / Sly 1 name).
CRef BuiltinEval__FiP4CRefP6CFrame(int cref, CRef *pref, CFrame *frame)
{
    SPN sp;
    sp.Set(CGcObj<CPair>::PtNew());
    sp->ref = *pref;
    return EvalExpression__FP5CPairP6CFrame(sp.p, frame);
}
} // namespace M_BuiltinEval__FiP4CRefP6CFrame

// 0x121DD8 BuiltinVector__FiP4CRefP6CFrame
namespace M_BuiltinVector__FiP4CRefP6CFrame {


CRef CRef_CoerceF32(CRef *pref) __asm__("CRef_CoerceF32");
CRef BuiltinVector__FiP4CRefP6CFrame(int c, CRef *pref, CFrame *frame) __asm__("BuiltinVector__FiP4CRefP6CFrame");

// Splice builtin "vector": 0/1/3 args -> VECTOR (Sly2 made it varargs).
// Also known as: RefOpVector (upstream readable / Sly 1 name).
CRef BuiltinVector__FiP4CRefP6CFrame(int c, CRef *pref, CFrame *frame)
{
    CRef ref(TAGK_VECTOR);
    VECTOR *pv = (VECTOR *)ref.u.pv;
    CPair *pn;
    if (c > 0)
        pn = pref->u.ppair;
    switch (c) {
    case 0:
        pv->z = 0.0f;
        pv->y = 0.0f;
        pv->x = 0.0f;
        break;
    case 1: {
        CRef r = CRef_CoerceF32(&pn->ref);
        float g = r.u.g;
        pv->z = g;
        pv->y = g;
        pv->x = g;
        break;
    }
    case 3: {
        float *pg = &pv->x;
        int i;
        for (i = 0; i < 3; i++) {
            CRef r = CRef_CoerceF32(&pn->ref);
            *pg = r.u.g;
            pg++;
            pn = pn->next.p;
        }
        break;
    }
    }
    return ref;
}
} // namespace M_BuiltinVector__FiP4CRefP6CFrame

// 0x121F00 BuiltinMatrix__FiP4CRefP6CFrame
namespace M_BuiltinMatrix__FiP4CRefP6CFrame {
struct MATRIX4;
struct MATRIX4 { float m[4][4]; };

void *func_002333B4(void *pv, int b, unsigned int cb) __asm__("memset");
CRef CRef_CoerceF32(CRef *pref) __asm__("CRef_CoerceF32");
CRef BuiltinMatrix__FiP4CRefP6CFrame(int carg, CRef *pref, CFrame *frame) __asm__("BuiltinMatrix__FiP4CRefP6CFrame");

// Splice builtin "matrix": 0/1/3/4 args -> MATRIX4 identity-based.
// Also known as: RefOpMatrix (upstream readable / Sly 1 name).
CRef BuiltinMatrix__FiP4CRefP6CFrame(int carg, CRef *pref, CFrame *frame)
{
    CRef ref(TAGK_MATRIX);
    MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
    CPair *pn;
    if (carg > 0)
        pn = pref->u.ppair;
    func_002333B4(pm, 0, sizeof(*pm));
    pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = pm->m[3][3] = 1.0f;
    switch (carg) {
    case 0:
        break;
    case 1:
        pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = CRef_CoerceF32(&pn->ref).u.g;
        break;
    case 3:
    case 4: {
        int i;
        for (i = 0; i < carg; i++) {
            *(qword *)pm->m[i] = *(qword *)pn->ref.u.pv;
            pn = pn->next.p;
        }
        break;
    }
    case 9:
    case 16: {
        int n = (carg == 9) ? 3 : 4;
        int i, j;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                CRef r = CRef_CoerceF32(&pn->ref);
                pm->m[i][j] = r.u.g;
                pn = pn->next.p;
            }
        }
        break;
    }
    }
    return ref;
}
} // namespace M_BuiltinMatrix__FiP4CRefP6CFrame

// 0x1220B8 BuiltinClq__FiP4CRefP6CFrame
namespace M_BuiltinClq__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
// Splice builtin "clq": 3 floats -> tag 0x1C.
// Also known as: RefOpClq (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinClq__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinClq__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    int i;
    float *pg;
    SetTag__4CRef4TAGK(&ref, 0x1C);
    pg = (float *)ref.u.pv;
    for (i = 0; i < 3; i++) {
        CRefData tmp;
        CRef_CoerceF32(&tmp, &args[i]);
        pg[i] = tmp.u.g;
        Decref__4CRef(&tmp);
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinClq__FiP4CRefP6CFrame

// 0x122168 BuiltinLm__FiP4CRefP6CFrame
namespace M_BuiltinLm__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
// Splice builtin "lm": 2 floats -> tag 0x1D.
// Also known as: RefOpLm (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinLm__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinLm__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    int i;
    float *pg;
    SetTag__4CRef4TAGK(&ref, 0x1D);
    pg = (float *)ref.u.pv;
    for (i = 0; i < 2; i++) {
        CRefData tmp;
        CRef_CoerceF32(&tmp, &args[i]);
        pg[i] = tmp.u.g;
        Decref__4CRef(&tmp);
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinLm__FiP4CRefP6CFrame

// 0x122218 BuiltinSmp__FiP4CRefP6CFrame
namespace M_BuiltinSmp__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
// Splice builtin "smp": 3 floats -> tag 0x1E.
// Also known as: RefOpSmp (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinSmp__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinSmp__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    int i;
    float *pg;
    SetTag__4CRef4TAGK(&ref, 0x1E);
    pg = (float *)ref.u.pv;
    for (i = 0; i < 3; i++) {
        CRefData tmp;
        CRef_CoerceF32(&tmp, &args[i]);
        pg[i] = tmp.u.g;
        Decref__4CRef(&tmp);
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinSmp__FiP4CRefP6CFrame

// 0x1222C8 BuiltinGetElement__FiP4CRefP6CFrame
namespace M_BuiltinGetElement__FiP4CRefP6CFrame {
struct VECTOR; struct IDX;
/* MATCH (Claude agent laneB9, 2026-10-23): v3 + second commented pin, pv in $5 (a1), pf = (i << 4) + pv. */
/* v3 (Claude agent laneB8, 2026-10-23): aligned 10 -> 2. ONE commented register pin: pf in $2 (v0), plus the tail written as `pf += j; f(*pf)` in all three arms (the sum then reuses pf like the target). Left: 1B-else `sll v0,a1,4; lw a1,pv` (target puts i<<4 in pf/v0, pv in a1; ours sll a1 in place, pv in v0). Tried: (i<<4)+char*, int sums, pf = base + i*4, &[i].x, j pin instead, i pin: 2-18. */
/* v2 (Claude agent, 2026-10-17): aligned 22 -> 10. Shared function-scope i / j / float *pf across cases (pidx = pidx->pnext reused) gives the target t0/a1/a3 registers and the cross-jumped pf[j] tail. Left: pf in v1, j in v0 (target pf v0, j v1) in the pf[j] tail. */
typedef struct VECTOR { float x, y, z, w; } VECTOR;
typedef struct IDX { int pad[3]; int i; struct IDX *pnext; } IDX;
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
CRefData *SetF32__4CReff(CRefData *pref, float g) __asm__("SetF32__4CReff");
CRefData *CRef_SetVector(CRefData *pref, VECTOR *pv) __asm__("CRef_SetVector");
// Splice builtin "get-element": element of vector/matrix by index list.
// Also known as: RefOpGetElement (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinGetElement__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinGetElement__FiP4CRefP6CFrame(CRefData *ret, int carg, CRefData *args, CFrame *frame)
{
    CRefData ref;
    IDX *pidx;
    int i;
    int j;
    register float *pf __asm__("$2"); /* register pin: pf must be v0 (j gets v1) in the shared pf[j] tail */
    ref.m_tagk = -1;
    pidx = (IDX *)args[1].u.pv;
    switch (args[0].m_tagk) {
    case TAGK_MATRIX: {
        i = pidx->i;
        if (carg == 2) {
            CRef_SetVector(&ref, &((VECTOR *)args[0].u.pv)[i]);
        } else {
            { register int pv __asm__("$5") = (int)args[0].u.pv; /* register pin: pv must be a1 so i<<4 lands in pf (v0) */
              pf = (float *)((i << 4) + pv); }
            pidx = pidx->pnext;
            j = pidx->i;
            pf += j;
            SetF32__4CReff(&ref, *pf);
        }
        break;
    }
    case TAGK_VECTOR: case TAGK_CLQ: case TAGK_LM:
        pf = (float *)args[0].u.pv;
        j = pidx->i;
        pf += j;
            SetF32__4CReff(&ref, *pf);
        break;
    case TAGK_SMP:
        pf = (float *)args[0].u.pv;
        j = pidx->i;
        pf += j;
            SetF32__4CReff(&ref, *pf);
        break;
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinGetElement__FiP4CRefP6CFrame

// 0x1223C0 BuiltinRandom__FiP4CRefP6CFrame
namespace M_BuiltinRandom__FiP4CRefP6CFrame {

CRef CRef_CoerceF32(CRef *pref) __asm__("CRef_CoerceF32");
int RandomIntInRange__Fii(int a, int b) __asm__("RandomIntInRange__Fii");
float RandomFloatInRange__Fff(float a, float b) __asm__("RandomFloatInRange__Fff");
CRef BuiltinRandom__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinRandom__FiP4CRefP6CFrame");

// Splice builtin "random": RandomIntInRange for ints else RandomFloatInRange.
// Also known as: RefOpRandom (upstream readable / Sly 1 name).
CRef BuiltinRandom__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    if (args[0].m_tagk == TAGK_S32 && args[1].m_tagk == args[0].m_tagk) {
        ref = RandomIntInRange__Fii(args[0].u.n, args[1].u.n);
    } else {
        float g0 = CRef_CoerceF32(&args[0]).u.g;
        float g1 = CRef_CoerceF32(&args[1]).u.g;
        ref = RandomFloatInRange__Fff(g0, g1);
    }
    return ref;
}
} // namespace M_BuiltinRandom__FiP4CRefP6CFrame

// 0x1224A8 ApplyUnaryFloatOp__FP4CRef4UFOK
namespace M_ApplyUnaryFloatOp__FP4CRef4UFOK {
typedef float (*PFNG)(float);
extern PFNG D_002BBB70[] __asm__("D_002BBB70");
void SetF32__4CReff(CRefData *pref, float g) __asm__("SetF32__4CReff");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Unary float op dispatcher (sqrt, sin, cos, tan, asin, acos, atan, rad-normalize by UFOK 0..7).
// Also known as: RefUfo (upstream readable / Sly 1 name).
extern "C" CRefData *ApplyUnaryFloatOp__FP4CRef4UFOK(CRefData *ret, CRefData *parg, int i)
{
    CRefData ref;
    float g;
    ref.m_tagk = -1;
    if (parg->m_tagk == TAGK_F32) {
        g = *(float *)&parg->u.n;
    } else {
        g = (float)parg->u.n;
    }
    SetF32__4CReff(&ref, D_002BBB70[i](g));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_ApplyUnaryFloatOp__FP4CRef4UFOK

// 0x122530 BuiltinSqrt__FiP4CRefP6CFrame
// Splice builtin "sqrt": ApplyUnaryFloatOp(arg,0).
// Also known as: RefOpSqrt (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinSqrt__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinSqrt__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 0);
    return pref;
}

// 0x122560 BuiltinSin__FiP4CRefP6CFrame
// Splice builtin "sin": ApplyUnaryFloatOp(arg,1).
// Also known as: RefOpSin (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinSin__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinSin__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 1);
    return pref;
}

// 0x122590 BuiltinCos__FiP4CRefP6CFrame
// Splice builtin "cos": ApplyUnaryFloatOp(arg,2).
// Also known as: RefOpCos (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinCos__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinCos__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 2);
    return pref;
}

// 0x1225C0 BuiltinTan__FiP4CRefP6CFrame
// Splice builtin "tan": ApplyUnaryFloatOp(arg,3).
// Also known as: RefOpTan (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinTan__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinTan__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 3);
    return pref;
}

// 0x1225F0 BuiltinAsin__FiP4CRefP6CFrame
// Splice builtin "asin": ApplyUnaryFloatOp(arg,4).
// Also known as: RefOpAsin (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinAsin__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAsin__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 4);
    return pref;
}

// 0x122620 BuiltinAcos__FiP4CRefP6CFrame
// Splice builtin "acos": ApplyUnaryFloatOp(arg,5).
// Also known as: RefOpAcos (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinAcos__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAcos__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 5);
    return pref;
}

// 0x122650 BuiltinAtan__FiP4CRefP6CFrame
// Splice builtin "atan": ApplyUnaryFloatOp(arg,6).
// Also known as: RefOpAtan (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinAtan__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAtan__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 6);
    return pref;
}

// 0x122680 BuiltinRadNormalize__FiP4CRefP6CFrame
// Splice builtin "rad-normalize": ApplyUnaryFloatOp(arg,7).
// Also known as: RefOpRadNormalize (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinRadNormalize__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinRadNormalize__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    ApplyUnaryFloatOp(pref, args, 7);
    return pref;
}

// 0x1226B0 BuiltinAtan2__FiP4CRefP6CFrame
namespace M_BuiltinAtan2__FiP4CRefP6CFrame {
float atan2f(float y, float x) __asm__("atan2f");
// Splice builtin "atan2": atan2f(a0,a1).
// Also known as: RefOpAtan2 (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinAtan2__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAtan2__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float ag[2];
    int i;
    float g;
    for (i = 0; i < 2; i++) {
        CRefData *p = &args[i];
        if (p->m_tagk == TAGK_F32) ag[i] = p->u.g; else ag[i] = p->u.n;
    }
    g = atan2f(ag[0], ag[1]);
    ret->u.g = g;
    ret->m_tagk = TAGK_F32;
    return ret;
}
} // namespace M_BuiltinAtan2__FiP4CRefP6CFrame

// 0x122730 BuiltinVectorDotProduct__FiP4CRefP6CFrame
namespace M_BuiltinVectorDotProduct__FiP4CRefP6CFrame {
// Dot product of the xyz parts of *pa and *pb, as raw float bits.
extern "C" { static __inline__ int DotProduct3(qword *pa, qword *pb)
{
    register qword a __asm__("vf1") = *pa;
    register qword b __asm__("vf2") = *pb;
    register qword t __asm__("vf3");
    int g;
    __asm__ ("vmul.xyz %0, %0, %2\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t) : "j"(b));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
} }
// Splice builtin "vector-dot-product": VU dot xyz.
// Also known as: RefOpVectorDotProduct (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinVectorDotProduct__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinVectorDotProduct__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    int g = DotProduct3((qword *)args[0].u.pv, (qword *)args[1].u.pv);
    ret->m_tagk = TAGK_F32;
    *(volatile int *)&ret->u.n = g;
    return ret;
}
} // namespace M_BuiltinVectorDotProduct__FiP4CRefP6CFrame

// 0x122770 BuiltinVectorCrossProduct__FiP4CRefP6CFrame
namespace M_BuiltinVectorCrossProduct__FiP4CRefP6CFrame {
CRefData *CRef_ctor(CRefData *pref, qword v) __asm__("CRef_ctor");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "vector-cross-product": VU outer product (cross).
// Also known as: RefOpVectorCrossProduct (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinVectorCrossProduct__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinVectorCrossProduct__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    register qword a __asm__("vf1") = *(qword *)args[0].u.pv;
    register qword b __asm__("vf2") = *(qword *)args[1].u.pv;
    register qword v __asm__("vf3");
    __asm__ ("vopmula.xyz ACC, %1, %2\n"
        "vopmsub.xyz %0, %2, %1" : "=j"(v) : "j"(a), "j"(b));
    CRef_ctor(&ref, v);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinVectorCrossProduct__FiP4CRefP6CFrame

// 0x1227D0 BuiltinVectorLength__FiP4CRefP6CFrame
namespace M_BuiltinVectorLength__FiP4CRefP6CFrame {
// Length of the xyz part of *pa, as raw float bits.
extern "C" { static __inline__ int VectorLength3(qword *pa)
{
    register qword a __asm__("vf1") = *pa;
    register qword t __asm__("vf2");
    int g;
    __asm__ ("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t));
    __asm__ ("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(a));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
} }
// Splice builtin "vector-lth": VU length.
// Also known as: RefOpVectorLth (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinVectorLength__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinVectorLength__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    int g = VectorLength3((qword *)args[0].u.pv);
    ret->m_tagk = TAGK_F32;
    *(volatile int *)&ret->u.n = g;
    return ret;
}
} // namespace M_BuiltinVectorLength__FiP4CRefP6CFrame

// 0x122810 BuiltinVectorDistance__FiP4CRefP6CFrame
namespace M_BuiltinVectorDistance__FiP4CRefP6CFrame {
struct W;
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");

CRef BuiltinVectorDistance__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinVectorDistance__FiP4CRefP6CFrame");

// Distance between *pa and *pb.
static inline float SDist(qword *pa, qword *pb)
{
    register qword a __asm__("vf1") = *pa;
    register qword b __asm__("vf2") = *pb;
    register qword t __asm__("vf3");
    int n;
    float g;
    __asm__ ("vsub.xyzw %0, %0, %2\n"
        "vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z\n"
        "vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(a), "=j"(t) : "j"(b));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}
// Splice builtin "vector-distance": distance to a1 or origin.
// Also known as: RefOpVectorDistance (upstream readable / Sly 1 name).
CRef BuiltinVectorDistance__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    if (args[1].m_tagk == 0)
        ref = SDist((qword *)args[0].u.pv, &D_002A2DD0);
    else
    {
        register W *pw __asm__("$5") = (W *)args[1].u.pv;
        ref = SDist((qword *)args[0].u.pv, pw->pv);
    }
    return ref;
}
} // namespace M_BuiltinVectorDistance__FiP4CRefP6CFrame

// 0x1228F0 BuiltinVectorDistanceSquared__FiP4CRefP6CFrame
namespace M_BuiltinVectorDistanceSquared__FiP4CRefP6CFrame {
struct W;
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");

CRef BuiltinVectorDistanceSquared__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinVectorDistanceSquared__FiP4CRefP6CFrame");

// Squared distance between *pa and *pb.
static inline float SDistSq(qword *pa, qword *pb)
{
    register qword a __asm__("vf1") = *pa;
    register qword b __asm__("vf2") = *pb;
    register qword t __asm__("vf3");
    int n;
    float g;
    __asm__ ("vsub.xyzw %0, %0, %2\n"
        "vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t) : "j"(b));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}
// Splice builtin "vector-distance-squared": squared distance.
// Also known as: RefOpVectorDistanceSquared (upstream readable / Sly 1 name).
CRef BuiltinVectorDistanceSquared__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef result;
    if (args[1].m_tagk == 0) {
        result = SDistSq((qword *)args[0].u.pv, &D_002A2DD0);   /* no second vector: distance to the origin */
    } else {
        register W *other __asm__("$5") = (W *)args[1].u.pv;
        result = SDistSq((qword *)args[0].u.pv, other->pv);
    }
    return result;
}
} // namespace M_BuiltinVectorDistanceSquared__FiP4CRefP6CFrame

// 0x1229B8 BuiltinVectorNormalize__FiP4CRefP6CFrame
namespace M_BuiltinVectorNormalize__FiP4CRefP6CFrame {
struct W;
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DD0 __asm__("D_002A2DD0");
extern CRef BuiltinVectorNormalize__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinVectorNormalize__FiP4CRefP6CFrame");
// Length of the xyz part of c, in the x lane.
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
    return l;
}
// Normalize *pv in place, or replace it with *pdef when its length is below 1e-4.
static __inline__ void Normalize(qword *pv, qword *pdef)
{
    __asm__("" : "+r"(pdef));
    qword c = *pv;
    qword l = VLen(c);
    float gl;
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        *pv = *pdef;
    } else {
        qword rn;
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
        *pv = rn;
    }
}
// Normalize *pv in place, or replace it with *pdef when its length is below 1e-4.
static __inline__ void Normalize1(qword *pv, qword *pdef)
{
    qword c;
    __asm__("qmtc2.ni %1, %0" : "=j"(c) : "r"(*pv));
    qword l = VLen(c);
    float gl;
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        *pv = *pdef;
    } else {
        qword rn;
        __asm__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(rn) : "j"(c), "j"(l));
        *pv = rn;
    }
}
// Splice builtin "vector-normalize": normalize with 1e-4 guard/default.
// Also known as: RefOpVectorNormalize (upstream readable / Sly 1 name).
CRef BuiltinVectorNormalize__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef result((const ::VECTOR *)args[0].u.pv);
    if (args[1].m_tagk == 0)
        Normalize1((qword *)result.u.pv, &D_002A2DD0);   /* fallback for a zero-length vector: the default */
    else
        Normalize((qword *)result.u.pv, ((W *)args[1].u.pv)->pv);
    return result;
}
} // namespace M_BuiltinVectorNormalize__FiP4CRefP6CFrame

// 0x122B08 BuiltinVectorProjectNormal__FiP4CRefP6CFrame
namespace M_BuiltinVectorProjectNormal__FiP4CRefP6CFrame {
CRefData *CRef_ctor(CRefData *pref, qword v) __asm__("CRef_ctor");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "vector-project-normal": (a.b)b.
// Also known as: RefOpVectorProjectNormal (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinVectorProjectNormal__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinVectorProjectNormal__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    register qword a __asm__("vf1") = *(qword *)args[0].u.pv;
    register qword b __asm__("vf2") = *(qword *)args[1].u.pv;
    register qword t __asm__("vf3");
    __asm__ ("vmul.xyz %0, %0, %2\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z\n"
        "vmulx.xyzw %1, %2, %0x" : "+j"(a), "=j"(t) : "j"(b));
    CRef_ctor(&ref, t);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinVectorProjectNormal__FiP4CRefP6CFrame

// 0x122B70 BuiltinVectorProjectTangent__FiP4CRefP6CFrame
namespace M_BuiltinVectorProjectTangent__FiP4CRefP6CFrame {
CRefData *CRef_ctor(CRefData *pref, qword v) __asm__("CRef_ctor");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "vector-project-tangent": a-(a.b)b.
// Also known as: RefOpVectorProjectTangent (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinVectorProjectTangent__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinVectorProjectTangent__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    register qword a __asm__("vf3") = *(qword *)args[0].u.pv;
    register qword b __asm__("vf2") = *(qword *)args[1].u.pv;
    register qword t __asm__("vf1");
    register qword u __asm__("vf4");
    __asm__ ("vmul.xyz %0, %2, %3\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z\n"
        "vmulx.xyzw %0, %3, %0x\n"
        "vsub.xyzw %3, %2, %0" : "=&j"(t), "=&j"(u), "+j"(a), "+j"(b));
    CRef_ctor(&ref, b);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinVectorProjectTangent__FiP4CRefP6CFrame

// 0x122BE0 BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame
namespace M_BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame {
union VQ; struct XFM; struct O; struct W;
union VQ { struct { float x, y, z, w; } v; qword q; };
struct XFM { char pad[0x80]; qword pos; };
struct O { char pad[0xE8]; XFM *pxfm; };
struct W { char pad[0xC]; O *po; };

CRef CRef_CoerceF32(CRef *pref) __asm__("CRef_CoerceF32");
qword CalculateBallisticLaunchVelocity(qword a, qword b, qword c, float g) __asm__("CalculateBallisticLaunchVelocity");
CRef BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame");

// Splice builtin "vector-ballistic-velocity": calls FCalculateMuzzleVelocity-like 0x1D6848 with gravity (0,0,-1750)
// or object xfm.
// Also known as: RefOpVectorBallisticVelocity (upstream readable / Sly 1 name).
CRef BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    VQ v;
    v.v.x = 0.0f;
    v.v.y = 0.0f;
    v.v.z = -1750.0f;
    if (args[3].m_tagk) {
        O *po = ((W *)args[3].u.pv)->po;
        if (po && po->pxfm)
            v.q = po->pxfm->pos;
    }
    CRef ref(TAGK_VECTOR);
    qword *pq = (qword *)ref.u.pv;
    *pq = CalculateBallisticLaunchVelocity(*(qword *)args[0].u.pv, *(qword *)args[1].u.pv, v.q, CRef_CoerceF32(&args[2]).u.g);
    return ref;
}
} // namespace M_BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame

// 0x122CE0 BuiltinVectorRadianNormal__FiP4CRefP6CFrame
namespace M_BuiltinVectorRadianNormal__FiP4CRefP6CFrame {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern float AngleBetweenVectors(qword a, qword b, void *pvec) __asm__("AngleBetweenVectors");
extern CRef BuiltinVectorRadianNormal__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinVectorRadianNormal__FiP4CRefP6CFrame");
// Splice builtin "vector-radian-normal": list (rad vector) from AngleBetweenVectors-like 0x11E4B0.
// Also known as: RefOpVectorRadianNormal (upstream readable / Sly 1 name).
CRef BuiltinVectorRadianNormal__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    SPN sp;
    sp.Set(CGcObj<CPair>::PtNew());
    sp->SetNext(CGcObj<CPair>::PtNew());
    CRef *pref = &sp->next.p->ref;
    pref->Retag(TAGK_VECTOR);
    sp->ref.SetF32(AngleBetweenVectors(*(qword *)args[0].u.pv, *(qword *)args[1].u.pv, pref->u.pv));
    return CRef(sp.p);
}
} // namespace M_BuiltinVectorRadianNormal__FiP4CRefP6CFrame

// 0x122DE0 BuiltinMatrixTranspose__FiP4CRefP6CFrame
namespace M_BuiltinMatrixTranspose__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void TransposeMatrix4(MATRIX4 *pmat, int n) __asm__("TransposeMatrix4");
// Splice builtin "matrix-transpose": 0x11C1D8.
// Also known as: RefOpMatrixTranspose (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinMatrixTranspose__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinMatrixTranspose__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    MATRIX4 mat;
    TransposeMatrix4(&mat, args->u.n);
    CRef_ctor_MATRIX4(&ref, &mat);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinMatrixTranspose__FiP4CRefP6CFrame

// 0x122E40 BuiltinMatrixInvert__FiP4CRefP6CFrame
// Splice builtin "matrix-invert": 0x11C2F0.
// Also known as: RefOpMatrixInvert (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinMatrixInvert__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinMatrixInvert__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    SetTag(&ref, TAGK_MATRIX);
    InvertMatrix4(args->u.pmat, ref.u.pmat);
    __4CRefRC4CRef(pref, &ref);
    Decref(&ref);
    return pref;
}

// 0x122EA0 BuiltinMatrixCalculateDmat__FiP4CRefP6CFrame
// Splice builtin "matrix-calculate-dmat": 0x11C3D0 (CalculateDmat).
// Also known as: RefOpMatrixCalculateDmat (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinMatrixCalculateDmat__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinMatrixCalculateDmat__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    SetTag(&ref, TAGK_MATRIX);
    CalculateDmat4(args[0].u.pmat, args[1].u.pmat, ref.u.pmat);
    __4CRefRC4CRef(pref, &ref);
    Decref(&ref);
    return pref;
}

// 0x122F00 BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame
namespace M_BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame {
struct MATRIX4; // Script matrix-slerp builtin. Matched by Claude agent (2026-10-17): MulM3 uses the g++ named return value (`return m`),
// stores each row right after its asm, and also writes a second (dead) M3 copy t, which is the target's 0xB0 slot.
typedef union { qword q; float g; } UF;
struct MATRIX4 { float m[4][4]; };

MATRIX3Q MulMatrix3Matrix3(MATRIX3Q *pa, MATRIX3Q *pb) __asm__("MulMatrix3Matrix3");
void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(MATRIX3Q *pmat, float *prad, qword *pvec) __asm__("DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR");
MATRIX3Q BuildAxisAngleRotationMatrix(float rad, qword axis) __asm__("BuildAxisAngleRotationMatrix");
void *func_002333B4(void *pv, int n, int cb) __asm__("memset");

// Return the 3x3 matrix product a * *pb.
static inline MATRIX3Q MulM3(const MATRIX3Q &a, MATRIX3Q *pb) return m
{
    MATRIX3Q t;
    qword r0, r1, r2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r0) : "j"(a.r[0].q), "j"(a.r[1].q), "j"(a.r[2].q), "j"(pb->r[0].q));
    m.r[0].q = r0; t.r[0].q = r0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r1) : "j"(a.r[0].q), "j"(a.r[1].q), "j"(a.r[2].q), "j"(pb->r[1].q));
    m.r[1].q = r1; t.r[1].q = r1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(r2) : "j"(a.r[0].q), "j"(a.r[1].q), "j"(a.r[2].q), "j"(pb->r[2].q));
    m.r[2].q = r2; t.r[2].q = r2;
}
extern CRef BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame");
// Splice builtin "matrix-interpolate-rotate": decompose+rebuild rotation (0x11BC00/0x11B500).
// Also known as: RefOpMatrixInterpolateRotate (upstream readable / Sly 1 name).
CRef BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    MATRIX3Q mat = MulMatrix3Matrix3((MATRIX3Q *)args[0].u.pv, (MATRIX3Q *)args[1].u.pv);
    UF normal;
    float rad;
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(&mat, &rad, &normal.q);
    CRef ref(TAGK_MATRIX);
    MATRIX4 *pm = (MATRIX4 *)ref.u.pv;
    func_002333B4(pm, 0, 0x40);
    pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = pm->m[3][3] = 1.0f;
    *(MATRIX3Q *)ref.u.pv = MulM3(BuildAxisAngleRotationMatrix(rad * args[2].u.g, normal.q), (MATRIX3Q *)args[0].u.pv);
    return ref;
}
} // namespace M_BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame

// 0x123038 BuiltinMatrixDecomposeToTranslate__FiP4CRefP6CFrame
// Splice builtin "matrix-decompose-to-translate": translation column -> vector.
// Also known as: RefOpMatrixDecomposeToTranslate (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinMatrixDecomposeToTranslate__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinMatrixDecomposeToTranslate__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    CRef_ctor_VECTOR(&ref, (VECTORQ *)args->u.pmat->mat[3]);
    __4CRefRC4CRef(pref, &ref);
    Decref(&ref);
    return pref;
}

// 0x123080 BuiltinMatrixDecomposeToRotate__FiP4CRefP6CFrame
// Splice builtin "matrix-decompose-to-rotate": rotation part -> matrix.
// Also known as: RefOpMatrixDecomposeToRotate (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinMatrixDecomposeToRotate__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinMatrixDecomposeToRotate__FiP4CRefP6CFrame(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    CRef_ctor_MATRIX4(&ref, args->u.pmat);
    __4CRefRC4CRef(pref, &ref);
    Decref(&ref);
    return pref;
}

// 0x1230C8 BuiltinMatrixDecomposeToEuler__FiP4CRefP6CFrame
namespace M_BuiltinMatrixDecomposeToEuler__FiP4CRefP6CFrame {
typedef int TI __attribute__((mode(TI)));
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_ctor(CRefData *pref, TI vec) __asm__("CRef_ctor");
TI DecomposeRotationMatrixToEuler(int n) __asm__("DecomposeRotationMatrixToEuler");
// Splice builtin "matrix-decompose-to-euler": 0x11BFF0 euler.
// Also known as: RefOpMatrixDecomposeToEuler (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinMatrixDecomposeToEuler__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinMatrixDecomposeToEuler__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    CRef_ctor(&ref, DecomposeRotationMatrixToEuler(args->u.n));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinMatrixDecomposeToEuler__FiP4CRefP6CFrame

// 0x123118 BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame
namespace M_BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame {
struct SPN;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern void DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(void *pv, float *pg, void *pvec) __asm__("DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR");
extern CRef BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame");
// Splice builtin "matrix-decompose-to-radian-normal": list(rad, normal) via DecomposeRotateMatrixRad.
// Also known as: RefOpMatrixDecomposeToRadianNormal (upstream readable / Sly 1 name).
CRef BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    SPN sp;
    struct { float g, h, i; } g;
    sp.Set(CGcObj<CPair>::PtNew());
    sp->SetNext(CGcObj<CPair>::PtNew());
    CRef *pref = &sp->next.p->ref;
    pref->Retag(TAGK_VECTOR);
    DecomposeRotateMatrixRad__FP7MATRIX3PfP6VECTOR(args[0].u.pv, &g.g, pref->u.pv);
    sp->ref.SetF32(g.g);
    return CRef(sp.p);
}
} // namespace M_BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame

// 0x123208 BuiltinMatrixLookAt__FiP4CRefP6CFrame
namespace M_BuiltinMatrixLookAt__FiP4CRefP6CFrame {
struct MATRIX3; struct W;
struct MATRIX3 { VECTOR4 mat[3]; };
struct W { char pad[0xC]; qword *pv; };
extern qword g_normalZ __asm__("g_normalZ");

MATRIX3 BuildOrthonormalMatrixZ(qword a, qword b) __asm__("BuildOrthonormalMatrixZ");
CRef BuiltinMatrixLookAt__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMatrixLookAt__FiP4CRefP6CFrame");

// Return *pb - *pa.
static inline qword SubtractVectors(qword *pb, qword *pa)
{
    register qword a __asm__("vf2") = *pa;
    register qword b __asm__("vf1") = *pb;
    __asm__ ("vsub.xyzw %0, %0, %1" : "+j"(b) : "j"(a));
    return b;
}
// Splice builtin "matrix-look-at": BuildOrthonormal from (to-from), up default g_normalZ.
// Also known as: RefOpMatrixLookAt (upstream readable / Sly 1 name).
CRef BuiltinMatrixLookAt__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    qword *pup;
    if (args[2].m_tagk == 0)
        pup = &g_normalZ;
    else
        pup = ((W *)args[2].u.pv)->pv;
    union { qword q; } v;
    v.q = SubtractVectors((qword *)args[1].u.pv, (qword *)args[0].u.pv);
    CRef ref(TAGK_MATRIX);
    MATRIX3 *pmat = (MATRIX3 *)ref.u.pv;
    *pmat = BuildOrthonormalMatrixZ(v.q, *pup);
    pmat->mat[2].w = 0.0f;
    pmat->mat[1].w = 0.0f;
    pmat->mat[0].w = 0.0f;
    return ref;
}
} // namespace M_BuiltinMatrixLookAt__FiP4CRefP6CFrame

// 0x1232D0 BuiltinMatrixTiltUpright__FiP4CRefP6CFrame
namespace M_BuiltinMatrixTiltUpright__FiP4CRefP6CFrame {
struct MATRIX3; struct MATRIX4; struct XF;
struct MATRIX3 { VECTOR4 mat[3]; };
struct MATRIX4 { VECTOR4 mat[3]; VECTOR4 pos; };
struct XF { char pad[0x30]; qword pos; };

void TiltMatUpright(XF *pxf, int f, MATRIX3 *pmat) __asm__("TiltMatUpright");
VECTOR4 *VECTOR_opAssign(VECTOR4 *pv, qword q) __asm__("VECTOR_opAssign");
CRef BuiltinMatrixTiltUpright__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMatrixTiltUpright__FiP4CRefP6CFrame");

// Splice builtin "matrix-tilt-upright": 0x11B938 tilt upright.
// Also known as: RefOpMatrixTiltUpright (upstream readable / Sly 1 name).
CRef BuiltinMatrixTiltUpright__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    MATRIX3 mat;
    TiltMatUpright((XF *)args[0].u.pv, 0, &mat);
    CRef ref(TAGK_MATRIX);
    MATRIX4 *pmat = (MATRIX4 *)ref.u.pv;
    VECTOR_opAssign(&pmat->pos, ((XF *)args[0].u.pv)->pos);
    *(MATRIX3 *)pmat = mat;
    pmat->mat[2].w = 0.0f;
    pmat->mat[1].w = 0.0f;
    pmat->mat[0].w = 0.0f;
    pmat->pos.w = 1.0f;
    return ref;
}
} // namespace M_BuiltinMatrixTiltUpright__FiP4CRefP6CFrame

// 0x123380 BuiltinClqEvaluate__FiP4CRefP6CFrame
namespace M_BuiltinClqEvaluate__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
// Splice builtin "clq-evaluate": a0+t(a1+t a2).
// Also known as: RefOpClqEvaluate (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinClqEvaluate__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinClqEvaluate__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float *a = (float *)args[0].u.pv;
    CRefData ref;
    float t, g;
    CRef_CoerceF32(&ref, &args[1]);
    t = ref.u.g;
    g = a[0] + t * (a[1] + t * a[2]);
    ret->m_tagk = TAGK_F32;
    ret->u.g = g;
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinClqEvaluate__FiP4CRefP6CFrame

// 0x1233F0 BuiltinClqEvaluateLm__FiP4CRefP6CFrame
namespace M_BuiltinClqEvaluateLm__FiP4CRefP6CFrame {
struct LM;
typedef struct LM { float minValue; float maxValue; } LM;
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
// Splice builtin "clq-evaluate-lm": clq eval clamped to lm.
// Also known as: RefOpClqEvaluateLm (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinClqEvaluateLm__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinClqEvaluateLm__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float *c = (float *)args[0].u.pv;
    LM *plm = (LM *)args[1].u.pv;
    CRefData ref;
    float g;
    CRef_CoerceF32(&ref, &args[2]);
    g = ref.u.g;
    ret->u.g = Clamp(c[0] + g * (c[1] + g * c[2]), plm->minValue, plm->maxValue);
    ret->m_tagk = TAGK_F32;
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinClqEvaluateLm__FiP4CRefP6CFrame

// 0x123498 BuiltinClqFit__FiP4CRefP6CFrame
namespace M_BuiltinClqFit__FiP4CRefP6CFrame {

CRef CRef_CoerceF32(CRef *pref) __asm__("CRef_CoerceF32");
void FitClq__FffffP3CLQ(void *pv, float a, float b, float c, float d) __asm__("FitClq__FffffP3CLQ");
CRef BuiltinClqFit__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinClqFit__FiP4CRefP6CFrame");

// Splice builtin "clq-fit": FitClq.
// Also known as: RefOpClqFit (upstream readable / Sly 1 name).
CRef BuiltinClqFit__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref(TAGK_CLQ);
    {
        CRef r0 = CRef_CoerceF32(&args[0]);
        float g0 = r0.u.g;
        CRef r1 = CRef_CoerceF32(&args[1]);
        float g1 = r1.u.g;
        CRef r2 = CRef_CoerceF32(&args[2]);
        float g2 = r2.u.g;
        CRef r3 = CRef_CoerceF32(&args[3]);
        FitClq__FffffP3CLQ(ref.u.pv, g0, g1, g2, r3.u.g);
    }
    return ref;
}
} // namespace M_BuiltinClqFit__FiP4CRefP6CFrame

// 0x1235A8 BuiltinLmLimit__FiP4CRefP6CFrame
namespace M_BuiltinLmLimit__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
float ClampToLimits__FP2LMf(LM *plm, float g) __asm__("ClampToLimits__FP2LMf");
// Splice builtin "lm-limit": ClampToLimits.
// Also known as: RefOpLmLimit (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinLmLimit__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinLmLimit__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    LM *plm = args[0].u.plm;
    CRefData ref;
    float g;
    CRef_CoerceF32(&ref, &args[1]);
    g = ClampToLimits__FP2LMf(plm, ref.u.g);
    ret->u.g = g;
    ret->m_tagk = TAGK_F32;
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinLmLimit__FiP4CRefP6CFrame

// 0x123608 BuiltinLmCheck__FiP4CRefP6CFrame
namespace M_BuiltinLmCheck__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
int IsWithinLimits__FP2LMf(LM *plm, float g) __asm__("IsWithinLimits__FP2LMf");
// Splice builtin "lm-check?": FCheckLm.
// Also known as: RefOpLmCheck (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinLmCheck__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinLmCheck__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    LM *plm = args[0].u.plm;
    CRefData ref;
    int f;
    CRef_CoerceF32(&ref, &args[1]);
    f = IsWithinLimits__FP2LMf(plm, ref.u.g) != 0;
    ret->m_tagk = TAGK_BOOL;
    ret->u.n = f;
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinLmCheck__FiP4CRefP6CFrame

// 0x123668 BuiltinFloor__FiP4CRefP6CFrame
namespace M_BuiltinFloor__FiP4CRefP6CFrame {
// Splice builtin "floor": (int)g.
// Also known as: RefOpFloor (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinFloor__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinFloor__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float g = args->u.g;
    ret->m_tagk = TAGK_S32;
    *(int *)&ret->u.g = (int)g;
    return ret;
}
} // namespace M_BuiltinFloor__FiP4CRefP6CFrame

// 0x123688 BuiltinCeiling__FiP4CRefP6CFrame
namespace M_BuiltinCeiling__FiP4CRefP6CFrame {
// Splice builtin "ceiling": (int)(g+1).
// Also known as: RefOpCeiling (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinCeiling__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinCeiling__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float g = args->u.g;
    ret->m_tagk = TAGK_S32;
    *(int *)&ret->u.g = (int)(g + 1.0f);
    return ret;
}
} // namespace M_BuiltinCeiling__FiP4CRefP6CFrame

// 0x1236B0 BuiltinRound__FiP4CRefP6CFrame
namespace M_BuiltinRound__FiP4CRefP6CFrame {
// Splice builtin "round": (int)(g+0.5).
// Also known as: RefOpRound (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinRound__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinRound__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float g = args->u.g;
    ret->m_tagk = TAGK_S32;
    *(int *)&ret->u.g = (int)(g + 0.5f);
    return ret;
}
} // namespace M_BuiltinRound__FiP4CRefP6CFrame

// 0x1236D8 BuiltinTruncate__FiP4CRefP6CFrame
namespace M_BuiltinTruncate__FiP4CRefP6CFrame {
// Splice builtin "truncate": (int)g.
// Also known as: RefOpTruncate (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinTruncate__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinTruncate__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    float g = args->u.g;
    ret->m_tagk = TAGK_S32;
    *(int *)&ret->u.g = (int)g;
    return ret;
}
} // namespace M_BuiltinTruncate__FiP4CRefP6CFrame

// 0x1236F8 BuiltinAbs__FiP4CRefP6CFrame
namespace M_BuiltinAbs__FiP4CRefP6CFrame {
void SetS32__4CRefi(CRefData *pref, int n) __asm__("SetS32__4CRefi");
void SetF32__4CReff(CRefData *pref, float g) __asm__("SetF32__4CReff");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "abs": abs int/float.
// Also known as: RefOpAbs (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinAbs__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAbs__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    if (args->m_tagk == TAGK_S32) {
        int n = args->u.n;
        SetS32__4CRefi(&ref, n < 0 ? -n : n);
    } else {
        float g = *(float *)&args->u.n;
        if (g < 0.0f) g = -g;
        SetF32__4CReff(&ref, g);
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinAbs__FiP4CRefP6CFrame

// 0x123788 BuiltinMaximum__FiP4CRefP6CFrame
namespace M_BuiltinMaximum__FiP4CRefP6CFrame {

// same shape as BuiltinMinimum (max instead of min): list pointer reuses the args parameter variable
extern CRef BuiltinMaximum__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMaximum__FiP4CRefP6CFrame");
// Splice builtin "max": max over varargs.
// Also known as: RefOpMaximum (upstream readable / Sly 1 name).
CRef BuiltinMaximum__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref(args[0]);
    if (args[1].m_tagk != 0) args = (CRef *)args[1].u.pv; else args = 0;
    CPair *pn = (CPair *)args;
    for (; pn; pn = pn->next.p) {
        if (ref.m_tagk == TAGK_S32) {
            if (pn->ref.m_tagk == TAGK_S32) {
                CRef *p = &pn->ref;
                if (ref.u.n < p->u.n)
                    ref = *p;
            } else {
                CRef *p = &pn->ref;
                if ((float)ref.u.n < p->u.g)
                    ref = *p;
            }
        } else {
            if (pn->ref.m_tagk == TAGK_S32) {
                CRef *p = &pn->ref;
                if ((float)p->u.n > ref.u.g)
                    ref = *p;
            } else {
                CRef *p = &pn->ref;
                if (ref.u.g < p->u.g)
                    ref = *p;
            }
        }
    }
    return ref;
}
} // namespace M_BuiltinMaximum__FiP4CRefP6CFrame

// 0x1238B0 BuiltinMinimum__FiP4CRefP6CFrame
namespace M_BuiltinMinimum__FiP4CRefP6CFrame {

// pn reuses the args parameter variable (if/else, tag != 0 arm first) -> beql s0=0
extern CRef BuiltinMinimum__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinMinimum__FiP4CRefP6CFrame");
// Splice builtin "min": min over varargs.
// Also known as: RefOpMinimum (upstream readable / Sly 1 name).
CRef BuiltinMinimum__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref(args[0]);
    if (args[1].m_tagk != 0) args = (CRef *)args[1].u.pv; else args = 0;
    CPair *pn = (CPair *)args;
    for (; pn; pn = pn->next.p) {
        if (ref.m_tagk == TAGK_S32) {
            if (pn->ref.m_tagk == TAGK_S32) {
                CRef *p = &pn->ref;
                if (ref.u.n > p->u.n)
                    ref = *p;
            } else {
                CRef *p = &pn->ref;
                if ((float)ref.u.n > p->u.g)
                    ref = *p;
            }
        } else {
            if (pn->ref.m_tagk == TAGK_S32) {
                CRef *p = &pn->ref;
                if ((float)p->u.n < ref.u.g)
                    ref = *p;
            } else {
                CRef *p = &pn->ref;
                if (ref.u.g > p->u.g)
                    ref = *p;
            }
        }
    }
    return ref;
}
} // namespace M_BuiltinMinimum__FiP4CRefP6CFrame

// 0x1239D8 BuiltinModulo__FiP4CRefP6CFrame
namespace M_BuiltinModulo__FiP4CRefP6CFrame {
// Splice builtin "modulo": int %.
// Also known as: RefOpModulo (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinModulo__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinModulo__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    int r = args[0].u.n % args[1].u.n;
    ret->m_tagk = TAGK_S32;
    ret->u.n = r;
    return ret;
}
} // namespace M_BuiltinModulo__FiP4CRefP6CFrame

// 0x123A08 BuiltinCurrentTime__FiP4CRefP6CFrame
namespace M_BuiltinCurrentTime__FiP4CRefP6CFrame {
extern float g_clock __asm__("g_clock");   /* g_clock.time as a float: the CLOCK field access schedules the load differently */
// Splice builtin "current-time": F32 = g_clock.time (g_clock).
// Also known as: RefOpCurrentTime (upstream readable / Sly 1 name).
SLY2_BUILTIN_C(BuiltinCurrentTime__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinCurrentTime__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    ret->m_tagk = TAGK_F32;
    ret->u.g = g_clock;
    return ret;
}
} // namespace M_BuiltinCurrentTime__FiP4CRefP6CFrame

// 0x123A28 BuiltinCurrentLevel
// S32 of global D_003E1110 (0x210A88) = current load-filter/job id used by IsCurrentLevelInList.
// That is the current level index (GetCurrentLevelIndex); scripts switch on it.
SLY2_BUILTIN_C(BuiltinCurrentLevel)
extern "C" CRefData *BuiltinCurrentLevel(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    pref->u.n = GetCurrentLevelIndex();
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x123A60 BuiltinCurrentEpisode
// S32 = table D_003E1C48[cur] (0x210A98) or -1.
// That is the current episode (g_anEpisodeOfLevel[level]) or -1; unused by retail scripts.
SLY2_BUILTIN_C(BuiltinCurrentEpisode)
extern "C" CRefData *BuiltinCurrentEpisode(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    pref->u.n = GetCurrentEpisode();
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x123A98 BuiltinCurrentWorld
// S32 = table D_003E1C44[cur] (0x210AE0) or -1. That is the current world (g_anWorldOfLevel[level]) or -1.
SLY2_BUILTIN_C(BuiltinCurrentWorld)
extern "C" CRefData *BuiltinCurrentWorld(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    pref->u.n = GetCurrentWorld();
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x123AD0 BuiltinFindCurrentJob
// CRef(BASIC*) of g_sw slot D_003E0F9C. That slot is the current JOB (null in free roam).
SLY2_BUILTIN_C(BuiltinFindCurrentJob)
extern "C" CRefData *BuiltinFindCurrentJob(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRef_ctor_BASIC(pref, (BASIC *)g_sw.currentJob);
    return pref;
}

// 0x123B00 BuiltinFindCurrentTask
// CRef(BASIC*) of D_003E0FA0. That slot is the current TASK.
SLY2_BUILTIN_C(BuiltinFindCurrentTask)
extern "C" CRefData *BuiltinFindCurrentTask(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRef_ctor_BASIC(pref, (BASIC *)g_sw.currentTask);
    return pref;
}

// 0x123B30 BuiltinFindPendingTask
// CRef(BASIC*) of D_003E0FA4.
// That slot is the pending TASK (RestartJob prefers it); scripts call RequestTaskState on it.
SLY2_BUILTIN_C(BuiltinFindPendingTask)
extern "C" CRefData *BuiltinFindPendingTask(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CRef_ctor_BASIC(pref, (BASIC *)g_sw.pendingTask);
    return pref;
}

// 0x123B60 BuiltinGetDifficulty
// F32 = 0x154C90(&D_002DE578): D_003E0F9C->pg value or average of 3 Splice values.
// That is the difficulty scale (GetDifficulty); scripts scale numbers by it.
SLY2_BUILTIN_C(BuiltinGetDifficulty)
extern "C" CRefData *BuiltinGetDifficulty(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    pref->u.g = GetDifficulty(D_002DE578);
    pref->m_tagk = TAGK_F32;
    return pref;
}

// 0x123BA0 BuiltinGetDevFlag
// Splice builtin slot 104: returns SwStubFalse() as an integer, which is 0 in the retail build. Scripts gate extra
// HUD hint tickers on it, so it reads as a development/build flag (name inferred).
SLY2_BUILTIN_C(BuiltinGetDevFlag)
extern "C" CRefData *BuiltinGetDevFlag(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    pref->u.n = SwStubFalse();
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x123BD8 BuiltinScheduleCallback__FiP4CRefP6CFrame
namespace M_BuiltinScheduleCallback__FiP4CRefP6CFrame {
struct R; struct SPN;

struct OBJ;
extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");
struct R { char pad[0xC]; int handle; };   /* a scheduled callback: its handle at +0xC */

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");
CRef CRef_CoerceF32(CRef *pref) __asm__("CRef_CoerceF32");
R *ScheduleSwSpliceCallback(SW *pg, int n, CPair *pnode, float t) __asm__("ScheduleSwSpliceCallback");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
};
extern CRef BuiltinScheduleCallback__FiP4CRefP6CFrame(int c, CRef *args, CFrame *frame) __asm__("BuiltinScheduleCallback__FiP4CRefP6CFrame");
// Splice builtin "schedule-callback": builds (proc obj args...) and calls 0x133040(g_sw,..,t) -> S32 handle.
CRef BuiltinScheduleCallback__FiP4CRefP6CFrame(int c, CRef *args, CFrame *frame)
{
    CRef result(-1, TAGK_S32);
    SPN body;                                   /* the callback body: a one-expression list */
    OBJ *proc = (OBJ *)args[1].u.pv;
    CPair *bodyPair = CGcObj<CPair>::PtNew();
    body.Set(bodyPair);
    body->ref = CGcObj<CPair>::PtNew();
    CPair *call = body->ref.u.ppair;            /* (proc 'arg ...) */
    call->ref.SetProc((CProc *)proc);
    if (c >= 3) {
        CPair *tail = call;
        for (CPair *arg = args[2].u.ppair; arg; arg = arg->next.p) {
            tail->SetNext(CGcObj<CPair>::PtNew());
            tail = tail->next.p;
            CRef *element = &tail->ref;
            *element = CGcObj<CPair>::PtNew();
            CPair *quoted = element->u.ppair;   /* (quote arg): the argument is passed unevaluated */
            quoted->ref.Retag(TAGK_QUOTE);
            quoted->SetNext(CGcObj<CPair>::PtNew());
            quoted->next.p->ref = arg->ref;
        }
    }
    SW *sw = &g_sw;
    float now = g_clock.time;
    R *callback = ScheduleSwSpliceCallback(sw, 0, body.p, now + CRef_CoerceF32(args).u.g);
    result.SetS32(callback->handle);
    return result;
}
} // namespace M_BuiltinScheduleCallback__FiP4CRefP6CFrame

// 0x123DD8 BuiltinCancelCallback
// Cancels a scheduled callback by handle: 0x133130(g_sw, id) unlinks and frees; returns Void.
SLY2_BUILTIN_C(BuiltinCancelCallback)
extern "C" CRefData *BuiltinCancelCallback(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    CancelSwSpliceCallback(&g_sw, args->u.n);
    SetTag(pref, TAGK_VOID);
    return pref;
}

// 0x123E18 BuiltinDeferObjectUpdate__FiP4CRefP6CFrame
namespace M_BuiltinDeferObjectUpdate__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
// Splice builtin "defer-object-update": obj+0x30 = g_clock.time + dt.
SLY2_BUILTIN_C(BuiltinDeferObjectUpdate__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinDeferObjectUpdate__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    CRefData refT;
    char *p;
    float g;
    SetTag__4CRef4TAGK(&ref, 0x24);
    p = (char *)args[0].u.pv;
    CRef_CoerceF32(&refT, &args[1]);
    g = refT.u.g;
    Decref__4CRef(&refT);
    *(float *)(p + 0x30) = g_clock.time + g;
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinDeferObjectUpdate__FiP4CRefP6CFrame

// 0x123EB0 BuiltinObjectOption__FiP4CRefP6CFrame4BIFK
namespace M_BuiltinObjectOption__FiP4CRefP6CFrame4BIFK {

CRef AddBasicOption__FP5BASICiP4CRef(BASIC *pbasic, int optid, CRef *pref) __asm__("AddBasicOption__FP5BASICiP4CRef");
CRef EnsureBasicOption__FP5BASICiP4CRef(BASIC *pbasic, int optid, CRef *pref) __asm__("EnsureBasicOption__FP5BASICiP4CRef");
CRef SetBasicOption__FP5BASICiP4CRef(BASIC *pbasic, int optid, CRef *pref) __asm__("SetBasicOption__FP5BASICiP4CRef");
CRef GetBasicOption__FP5BASICi(BASIC *pbasic, int optid) __asm__("GetBasicOption__FP5BASICi");
CRef BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(int cref, CRef *args, CFrame *frame, int op) __asm__("BuiltinObjectOption__FiP4CRefP6CFrame4BIFK");

// Shared body of addo/ensureo/seto!/geto: switch on BIFK 0x6C..0x6F ->
// AddBasicOption/EnsureBasicOption/SetBasicOption/GetBasicOption.
CRef BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(int cref, CRef *args, CFrame *frame, int op)
{
    CRef ref;
    BASIC *a = args[0].u.pbasic;
    int b = args[1].u.n;
    switch (op) {
    case 0x6C:
        ref = AddBasicOption__FP5BASICiP4CRef(a, b, &args[2]);
        break;
    case 0x6D:
        ref = EnsureBasicOption__FP5BASICiP4CRef(a, b, &args[2]);
        break;
    case 0x6E:
        ref = SetBasicOption__FP5BASICiP4CRef(a, b, &args[2]);
        break;
    case 0x6F:
        ref = GetBasicOption__FP5BASICi(a, b);
        break;
    }
    return ref;
}
} // namespace M_BuiltinObjectOption__FiP4CRefP6CFrame4BIFK

// 0x123FB8 BuiltinAddOption__FiP4CRefP6CFrame
namespace M_BuiltinAddOption__FiP4CRefP6CFrame {
void BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k) __asm__("BuiltinObjectOption__FiP4CRefP6CFrame4BIFK");
// Splice builtin "addo": BuiltinObjectOption(op 0x6C) -> AddBasicOption.
SLY2_BUILTIN_C(BuiltinAddOption__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinAddOption__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(ret, cref, args, frame, 0x6C);
    return ret;
}
} // namespace M_BuiltinAddOption__FiP4CRefP6CFrame

// 0x123FE8 BuiltinEnsureOption__FiP4CRefP6CFrame
namespace M_BuiltinEnsureOption__FiP4CRefP6CFrame {
void BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k) __asm__("BuiltinObjectOption__FiP4CRefP6CFrame4BIFK");
// Splice builtin "ensureo": op 0x6D EnsureBasicOption.
SLY2_BUILTIN_C(BuiltinEnsureOption__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinEnsureOption__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(ret, cref, args, frame, 0x6D);
    return ret;
}
} // namespace M_BuiltinEnsureOption__FiP4CRefP6CFrame

// 0x124018 BuiltinSetOption__FiP4CRefP6CFrame
namespace M_BuiltinSetOption__FiP4CRefP6CFrame {
void BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k) __asm__("BuiltinObjectOption__FiP4CRefP6CFrame4BIFK");
// Splice builtin "seto!": op 0x6E SetBasicOption.
SLY2_BUILTIN_C(BuiltinSetOption__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinSetOption__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(ret, cref, args, frame, 0x6E);
    return ret;
}
} // namespace M_BuiltinSetOption__FiP4CRefP6CFrame

// 0x124048 BuiltinGetOption__FiP4CRefP6CFrame
namespace M_BuiltinGetOption__FiP4CRefP6CFrame {
void BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(CRefData *ret, int cref, CRefData *args, CFrame *frame, int k) __asm__("BuiltinObjectOption__FiP4CRefP6CFrame4BIFK");
// Splice builtin "geto": op 0x6F GetBasicOption.
SLY2_BUILTIN_C(BuiltinGetOption__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinGetOption__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    BuiltinObjectOption__FiP4CRefP6CFrame4BIFK(ret, cref, args, frame, 0x6F);
    return ret;
}
} // namespace M_BuiltinGetOption__FiP4CRefP6CFrame

// 0x124078 BuildListFromObjects__FiPP2LO
namespace M_BuildListFromObjects__FiPP2LO {

CRef BuildListFromObjects__FiPP2LO(int c, LO **apo) __asm__("BuildListFromObjects__FiPP2LO");

// Builds a Splice list of CRef(BASIC*) from an LO* array.
CRef BuildListFromObjects__FiPP2LO(int c, LO **apo)
{
    if (c == 0)
        return CRef(0);
    CRef ref;
    CPair *pprev = 0;
    int i;
    for (i = 0; i < c; i++) {
        CPair *p = CGcObj<CPair>::PtNew();
        if (pprev)
            pprev->next = p;
        else
            ref = p;
        p->ref.SetBasic((BASIC *)apo[i]);   /* LO is incomplete here */
        pprev = p;
    }
    return ref;
}
} // namespace M_BuildListFromObjects__FiPP2LO

// 0x124150 BuiltinFindObject__FiP4CRefP6CFrame
namespace M_BuiltinFindObject__FiP4CRefP6CFrame {
void *FindSwObjectInProxyScope(SW *pg, int k, int a, LO *b, void *c) __asm__("FindSwObjectInProxyScope");
void CRef_SetBasic(CRefData *pref, void *p) __asm__("CRef_SetBasic");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "find-object": 0x15E958 find by oid (k=1 or 5), optional 3rd arg = search root.
SLY2_BUILTIN_C(BuiltinFindObject__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinFindObject__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    LO *b;
    LO *plo;
    void *pv;
    ref.m_tagk = -1;
    b = (LO *)args[1].u.pv;
    if (args[2].m_tagk) {
        plo = (LO *)((CPair *)args[2].u.pv)->ref.u.pv;
    } else {
        plo = b;
    }
    {
        SW *pg = &g_sw;
        int k = b ? 1 : 5;
        int a = args[0].u.n;
        CRef_SetBasic(&ref, FindSwObjectInProxyScope(pg, k, a, b, plo ? (void *)plo->proxyRecord : (void *)pg->ppxrRoot));
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinFindObject__FiP4CRefP6CFrame

// 0x1241E8 BuiltinFindObjects__FiP4CRefP6CFrame
namespace M_BuiltinFindObjects__FiP4CRefP6CFrame {

int FindSwObjectsInProxyScope(SW *psw, int k, void *a, LO *b, void *c, int n, void **ap) __asm__("FindSwObjectsInProxyScope");
CRef BuildListFromObjects__FiPP2LO(int c, void **ap) __asm__("BuildListFromObjects__FiPP2LO");
CRef BuiltinFindObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFindObjects__FiP4CRefP6CFrame");

// Splice builtin "find-objects": 0x15E678 + BuildListFromObjects.
CRef BuiltinFindObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    void *ap[0x40];
    LO *pq = (LO *)args[1].u.pv;
    LO *pqT;
    if (args[2].m_tagk)
        pqT = (LO *)((CPair *)args[2].u.pv)->ref.u.pv;
    else
        pqT = pq;
    SW *pg = &g_sw;
    int k = pq ? 1 : 5;
    void *p0 = args[0].u.pv;
    void *pv;
    if (pqT)
        pv = pqT->proxyRecord;
    else
        pv = pg->ppxrRoot;
    int c = FindSwObjectsInProxyScope(pg, k, p0, pq, pv, 0x40, ap);
    ref = BuildListFromObjects__FiPP2LO(c, ap);
    return ref;
}
} // namespace M_BuiltinFindObjects__FiP4CRefP6CFrame

// 0x1242B8 BuiltinFindObjectsFromOids
namespace M_BuiltinFindObjectsFromOids {
struct SPN; struct OW;

extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    // Access the held pair.
    CPair *operator->() { return p; }
    int pad[3];
};
struct OW { char pad[0xC]; LO *plo; };   /* the optional context argument's object (+0xC) */
extern LO *FindSwObjectInProxyScope(SW *pg, int kind, int n, void *pv, PXR *ppxr) __asm__("FindSwObjectInProxyScope");
extern CRef BuiltinFindObjectsFromOids(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFindObjectsFromOids");
// Maps a list of OIDs to a list of objects via 0x15E958 (same root/optional-arg handling as find-object).
// Also known as: RefOpFindObjectsOfNames (upstream readable / Sly 1 name; RefOpFindObjectsOfNames__FiP4CRefP6CFrame).
CRef BuiltinFindObjectsFromOids(int cref, CRef *args, CFrame *frame)
{
    CRef result;
    if (args[0].m_tagk == 0)
        return args[0];
    void *root = args[1].u.pv;
    LO *context;
    if (args[2].m_tagk != 0)
        context = ((OW *)args[2].u.pv)->plo;
    else
        context = (LO *)root;
    SPN list;
    CPair *head = CGcObj<CPair>::PtNew();
    list.Set(head);
    CPair *tail;
    CPair *oidPair = args[0].u.ppair;
    while (oidPair) {
        SPN newPair;
        CPair *pair = CGcObj<CPair>::PtNew();
        newPair.Set(pair);
        int oid = oidPair->ref.u.n;
        newPair->ref.SetBasic((BASIC *)FindSwObjectInProxyScope(&g_sw, root ? 1 : 5, oid, root,
                                                              context ? context->proxyRecord : g_sw.ppxrRoot));
        if (list->ref.m_tagk != TAGK_PAIR)
            list->ref = newPair.p;
        else
            tail->next = newPair.p;
        CPair *appended = newPair.p;
        oidPair = oidPair->next.p;
        tail = appended;
    }
    int unused[4];   /* stack frame size (inferred: a dead local of the original) */
    result = CRef((CPair *)list->ref.u.pv);
    return result;
}
} // namespace M_BuiltinFindObjectsFromOids

// 0x1244E8 BuiltinFindNearestObject__FiP4CRefP6CFrame
namespace M_BuiltinFindNearestObject__FiP4CRefP6CFrame {
void *FindSwObjectInProxyScope(SW *pg, int k, int a, LO *b, void *c) __asm__("FindSwObjectInProxyScope");
void CRef_SetBasic(CRefData *pref, void *p) __asm__("CRef_SetBasic");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "find-nearest-object": like find-object with k=4.
SLY2_BUILTIN_C(BuiltinFindNearestObject__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinFindNearestObject__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    LO *b;
    LO *plo;
    void *pv;
    ref.m_tagk = -1;
    b = (LO *)args[1].u.pv;
    if (args[2].m_tagk) {
        plo = (LO *)((CPair *)args[2].u.pv)->ref.u.pv;
    } else {
        plo = b;
    }
    {
        SW *pg = &g_sw;
        int a = args[0].u.n;
        CRef_SetBasic(&ref, FindSwObjectInProxyScope(pg, 4, a, b, plo ? (void *)plo->proxyRecord : (void *)pg->ppxrRoot));
    }
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinFindNearestObject__FiP4CRefP6CFrame

// 0x124578 BuiltinFindNearestObjects__FiP4CRefP6CFrame
namespace M_BuiltinFindNearestObjects__FiP4CRefP6CFrame {

int FindSwObjectsInProxyScope(SW *psw, int k, void *a, LO *b, void *c, int n, void **ap) __asm__("FindSwObjectsInProxyScope");
CRef BuildListFromObjects__FiPP2LO(int c, void **ap) __asm__("BuildListFromObjects__FiPP2LO");
CRef BuiltinFindNearestObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFindNearestObjects__FiP4CRefP6CFrame");

// Splice builtin "find-nearest-objects": 0x15E678 k=4 + BuildListFromObjects.
CRef BuiltinFindNearestObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    void *ap[0x40];
    LO *pq = (LO *)args[1].u.pv;
    LO *pqT;
    if (args[2].m_tagk)
        pqT = (LO *)((CPair *)args[2].u.pv)->ref.u.pv;
    else
        pqT = pq;
    void *p0 = args[0].u.pv;
    SW *pg = &g_sw;
    void *pv;
    if (pqT)
        pv = pqT->proxyRecord;
    else
        pv = pg->ppxrRoot;
    int c = FindSwObjectsInProxyScope(pg, 4, p0, pq, pv, 0x40, ap);
    ref = BuildListFromObjects__FiPP2LO(c, ap);
    return ref;
}
} // namespace M_BuiltinFindNearestObjects__FiP4CRefP6CFrame

// 0x124640 BuiltinFindWorldObject__FiP4CRefP6CFrame
namespace M_BuiltinFindWorldObject__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_SetBasic(CRefData *pref, void *p) __asm__("CRef_SetBasic");
// Splice builtin "find-world-object": CRef(BASIC*) of &g_sw.
SLY2_BUILTIN_C(BuiltinFindWorldObject__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinFindWorldObject__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, &g_sw);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinFindWorldObject__FiP4CRefP6CFrame

// 0x124690 BuiltinFindSoundObject
namespace M_BuiltinFindSoundObject {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_SetBasic(CRefData *pref, void *p) __asm__("CRef_SetBasic");
// CRef(BASIC*) of static object D_003DD040 (the object LoadSwFromBrx passes to 0x1FD0E8 sound-section loader).
// The object is the sound manager g_sndmgr; scripts call StartDialog and its other methods on it.
SLY2_BUILTIN_C(BuiltinFindSoundObject)
extern "C" CRefData *BuiltinFindSoundObject(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, &g_sndmgr);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinFindSoundObject

// 0x1246E0 BuiltinFindCameraObject__FiP4CRefP6CFrame
namespace M_BuiltinFindCameraObject__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_SetBasic(CRefData *pref, void *p) __asm__("CRef_SetBasic");
// Splice builtin "find-camera-object": CRef(BASIC*) of &g_cm.
SLY2_BUILTIN_C(BuiltinFindCameraObject__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinFindCameraObject__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, &g_cm);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinFindCameraObject__FiP4CRefP6CFrame

// 0x124730 BuiltinFindClassObjects__FiP4CRefP6CFrame
namespace M_BuiltinFindClassObjects__FiP4CRefP6CFrame {

int FindSwObjectsByClass(void *pg, int k, int cid, LO *context, int c, void **ap) __asm__("FindSwObjectsByClass");
CRef BuildListFromObjects__FiPP2LO(int c, void **ap) __asm__("BuildListFromObjects__FiPP2LO");
CRef BuiltinFindClassObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFindClassObjects__FiP4CRefP6CFrame");

// Splice builtin "find-class-objects": 0x15E9C8 by class + BuildListFromObjects.
// Also known as: RefOpFindObjects (upstream readable / Sly 1 name).
CRef BuiltinFindClassObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    void *ap[0x180];
    LO *context = (LO *)args[1].u.pv;   /* the object whose scope is searched, or 0: the world (inferred) */
    int c = FindSwObjectsByClass(&g_sw, context ? 0x401 : 0x405, args[0].u.n, context, 0x180, ap);
    ref = BuildListFromObjects__FiPP2LO(c, ap);
    return ref;
}
} // namespace M_BuiltinFindClassObjects__FiP4CRefP6CFrame

// 0x1247D8 BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame
namespace M_BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame {

void IntersectSwBoundingBox(void *pg, qword a, qword b, void *p3, void *p4, int *pc, void **pap) __asm__("IntersectSwBoundingBox");
void BuildUniqueSoList(int c, void *ap, int *pc, void **pap) __asm__("BuildUniqueSoList");
CRef BuildListFromObjects__FiPP2LO(int c, void *ap) __asm__("BuildListFromObjects__FiPP2LO");
CRef BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame");

// Splice builtin "find-objects-in-bounding-box": 0x20FAA0 box query + 0x20FDC8 filter.
CRef BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    STACKSCOPE obj;
    int c0;
    void *ap0;
    int c1;
    void *ap1;
    IntersectSwBoundingBox(&g_sw, *(qword *)args[0].u.pv, *(qword *)args[1].u.pv, 0, 0, &c0, &ap0);
    BuildUniqueSoList(c0, ap0, &c1, &ap1);
    ref = BuildListFromObjects__FiPP2LO(c1, ap1);
    return ref;
}
} // namespace M_BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame

// 0x1248B0 BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame
namespace M_BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame {

void IntersectSwBoundingSphere(void *pg, qword a, void *p3, void *p4, float g, int *pc, void **pap) __asm__("IntersectSwBoundingSphere");
void BuildUniqueSoList(int c, void *ap, int *pc, void **pap) __asm__("BuildUniqueSoList");
CRef BuildListFromObjects__FiPP2LO(int c, void *ap) __asm__("BuildListFromObjects__FiPP2LO");
CRef BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame");

// Splice builtin "find-objects-in-bounding-sphere": 0x20FC60 sphere query + 0x20FDC8.
CRef BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    float g;
    if (args[1].m_tagk == TAGK_S32)
        g = (float)args[1].u.n;
    else
        g = args[1].u.g;
    STACKSCOPE obj;
    int c0;
    void *ap0;
    int c1;
    void *ap1;
    IntersectSwBoundingSphere(&g_sw, *(qword *)args[0].u.pv, 0, 0, g, &c0, &ap0);
    BuildUniqueSoList(c0, ap0, &c1, &ap1);
    ref = BuildListFromObjects__FiPP2LO(c1, ap1);
    return ref;
}
} // namespace M_BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame

// 0x1249A8 BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame
namespace M_BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame {
struct SPN; struct HIT;
/* Script builtin (METHOD pfn(pv,c,aref)): ray-cast aref[1]->aref[2] (ClipEdgeToObject) vs every SO of each LO in list aref[0].
   pv==0x7B returns a list of (lo pos normal) for all hits, else (lo normal) of the first hit (the original stores &hit.normal there). */

struct LO;
extern char D_004520A8[] __asm__("_t9CSlotPool2i20i8192$s_pool");

void FreeSlotheapSlot(void *pheap, void *pv) __asm__("FreeSlotheapSlot");

struct SPN {
    CPair *p;
    void Set(CPair *pn) __asm__("Attach__t7CRefPtr1Z5CPairP5CPair");
    // Hold pn (adds a reference).
    SPN(CPair *pn) { Set(pn); }
    // Drop the held pair's reference, deleting it on the last one.
    ~SPN() { CPair *pn = p; if (pn && --pn->m_cref == 0) delete pn; }
    void operator=(CPair *pn) __asm__("__as__t7CRefPtr1Z5CPairP5CPair");
    // Access the held pair.
    CPair *operator->() { return p; }
    int pad;
};
struct HIT {
    qword pos;
    qword normal;
    int pad[16];
};
extern int ClipEdgeToObject(SOPART *pso, qword pos0, qword pos1, int f, HIT *phit) __asm__("ClipEdgeToObject");

/* Script builtin: ray-cast segment aref[1]->aref[2] against every SO of each LO in list aref[0].
   bifk == 0x7B: return list of (lo pos normal) for all hits; otherwise return (lo normal) of the first hit. */
// Line hit test vs objects (0x14C6A0); BIFK 0x7B returns list of (lo pos normal) for all hits, 0x7C returns first
// hit.
extern "C" CRef BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(int bifk, int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    if (args[0].m_tagk == 0) {
        ref = args[0];
    } else {
        SPN spHead(0);
        SPN spTail(0);
        CPair *pn;

        for (pn = args[0].u.ppair; pn; pn = pn->next.p) {
            CRef *prefTo = &args[2];
            SO *plo = (SO *)pn->ref.u.pv;
            int i;
            if (!plo)
                continue;
            for (i = 0; i < plo->numParts; i++) {
                SOPART *pso = &plo->parts[i];
                CRef *prefFrom = &args[1];
                HIT hit;
                if (pso->isHidden)
                    continue;
                if (ClipEdgeToObject(pso, *(qword *)prefFrom->u.pv, *(qword *)prefTo->u.pv, 1, &hit) <= 0)
                    continue;
                {
                    SPN spLo(CGcObj<CPair>::PtNew());
                    SPN spPos(CGcObj<CPair>::PtNew());
                    spLo->ref.SetBasic((BASIC *)plo);
                    spLo->next = spPos.p;
                    if (bifk == 0x7B) {
                        SPN spNormal(CGcObj<CPair>::PtNew());
                        SPN spItem(CGcObj<CPair>::PtNew());
                        spPos->ref.SetVector((VECTOR *)&hit.pos);
                        spPos->next = spNormal.p;
                        spNormal->ref.SetVector((VECTOR *)&hit.normal);
                        spItem->ref = spLo.p;
                        if (spHead.p)
                            spTail->next = spItem.p;
                        else
                            spHead = spItem.p;
                        spTail = spItem.p;
                    } else {
                        spPos->ref.SetVector((VECTOR *)&hit.normal);
                        ref = spLo.p;
                        return ref;
                    }
                }
            }
        }
        if (spHead.p)
            ref = spHead.p;
        else
            ref.m_tagk = 0;
    }
    return ref;
}
} // namespace M_BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame

// 0x124F00 BuiltinHitTestObjects__FiP4CRefP6CFrame
namespace M_BuiltinHitTestObjects__FiP4CRefP6CFrame {

CRef BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(int bifk, int cref, CRef *args, CFrame *frame) __asm__("BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame");
CRef BuiltinHitTestObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinHitTestObjects__FiP4CRefP6CFrame");

// Splice builtin "hit-test-objects": BuiltinHitTestObjectsImpl(0x7B=own BIFK) -> list of (lo pos normal).
CRef BuiltinHitTestObjects__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    ref = BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(0x7B, cref, args, frame);
    return ref;
}
} // namespace M_BuiltinHitTestObjects__FiP4CRefP6CFrame

// 0x124F80 BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame
namespace M_BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame {

CRef BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(int bifk, int cref, CRef *args, CFrame *frame) __asm__("BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame");
CRef BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame) __asm__("BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame");

// Splice builtin "hit-test-objects-first": Impl(0x7C) -> first hit.
CRef BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    ref = BuiltinHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(0x7C, cref, args, frame);
    return ref;
}
} // namespace M_BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame

// 0x125000 BuiltinConvertObjectPosition__FiP4CRefP6CFrame
namespace M_BuiltinConvertObjectPosition__FiP4CRefP6CFrame {
typedef int TI __attribute__((mode(TI)));
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void CRef_ctor(CRefData *pref, TI vec) __asm__("CRef_ctor");
TI ConvertPosBetweenLos(LO *ploFrom, LO *ploTo, TI v) __asm__("ConvertPosBetweenLos");
// Splice builtin "convert-object-position": ConvertPosBetweenLos 0x1A6F00.
SLY2_BUILTIN_C(BuiltinConvertObjectPosition__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinConvertObjectPosition__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    CRef_ctor(&ref, ConvertPosBetweenLos((LO *)args[0].u.pv, (LO *)args[1].u.pv, *(TI *)args[2].u.pv));
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinConvertObjectPosition__FiP4CRefP6CFrame

// 0x125060 BuiltinConvertObjectVector__FiP4CRefP6CFrame
namespace M_BuiltinConvertObjectVector__FiP4CRefP6CFrame {
typedef int TI __attribute__((mode(TI)));
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
TI ConvertVecBetweenLos(void *a, void *b, TI v) __asm__("ConvertVecBetweenLos");
// Splice builtin "convert-object-vector": ConvertVecBetweenLos 0x1A6FC0.
SLY2_BUILTIN_C(BuiltinConvertObjectVector__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinConvertObjectVector__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    TI *pv;
    SetTag__4CRef4TAGK(&ref, 0x1A);
    pv = (__typeof__(pv))ref.u.pv;
    *pv = ConvertVecBetweenLos(args[0].u.pv, args[1].u.pv, *(TI *)args[2].u.pv);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinConvertObjectVector__FiP4CRefP6CFrame

// 0x1250D8 BuiltinConvertObjectMatrix__FiP4CRefP6CFrame
namespace M_BuiltinConvertObjectMatrix__FiP4CRefP6CFrame {

void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void ConvertMat4BetweenLos(MATRIX4 *pmat, int a, int b, int c) __asm__("ConvertMat4BetweenLos");
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
// Splice builtin "convert-object-matrix": ConvertMatBetweenLos 0x1A71B0.
SLY2_BUILTIN_C(BuiltinConvertObjectMatrix__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinConvertObjectMatrix__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    MATRIX4 mat;
    MATRIX4 *pmat;
    SetTag__4CRef4TAGK(&ref, 0x1B);
    pmat = (MATRIX4 *)ref.u.pv;
    ConvertMat4BetweenLos(&mat, args[0].u.n, args[1].u.n, args[2].u.n);
    *pmat = mat;
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinConvertObjectMatrix__FiP4CRefP6CFrame

// 0x125168 BuiltinNearClipCenter__FiP4CRefP6CFrame
namespace M_BuiltinNearClipCenter__FiP4CRefP6CFrame {

CRefData *CRef_ctor(CRefData *pref, qword v) __asm__("CRef_ctor");
// Set *pv to (x, y, z) with a packed 128-bit store.
extern "C" { static __inline__ void SetVector(VECTOR4 *pv, float x, float y, float z)
{
    register qword t __asm__("$6");
    register int ix __asm__("$6"), iy __asm__("$4"), iz __asm__("$4");
    __asm__ ("mfc1 %1, %3\n\tmfc1 %2, %4\n\tpextlw %0, %2, %1" : "=r"(t), "=&r"(ix), "=&r"(iy) : "f"(x), "f"(y));
    __asm__ __volatile__ ("mfc1 %0, %1" : "=r"(iz) : "f"(z));
    __asm__ ("pcpyld %0, %1, %2" : "=r"(t) : "r"(iz), "r"(t));
    *(qword *)pv = t;
} }
// Splice builtin "near-clip-center": uses g_cm near-clip distance * camera matrix.
SLY2_BUILTIN_C(BuiltinNearClipCenter__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinNearClipCenter__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    VECTOR4 vec;
    CM *pcm = &g_cm;
    qword *pm = (qword *)&pcm->matq;   /* the four matrix rows */
    SetVector(&vec, pcm->nearClip, 0.0f, 0.0f);
    {
        register qword v __asm__("vf5") = *(qword *)&vec;
        register qword m0 __asm__("vf4") = pm[0];
        register qword m1 __asm__("vf2") = pm[1];
        register qword m2 __asm__("vf1") = pm[2];
        register qword m3 __asm__("vf3") = pm[3];
        __asm__ ("vmulax.xyz ACC, %1, %0x\n"
            "vmadday.xyz ACC, %2, %0y\n"
            "vmaddaz.xyz ACC, %3, %0z\n"
            "vmaddw.xyz %0, %4, $vf0w" : "+j"(v) : "j"(m0), "j"(m1), "j"(m2), "j"(m3));
        CRef_ctor(ret, v);
    }
    return ret;
}
} // namespace M_BuiltinNearClipCenter__FiP4CRefP6CFrame

// 0x1251E8 BuiltinStartSound__FiP4CRefP6CFrame
namespace M_BuiltinStartSound__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void *CreateSfxSound(int n) __asm__("CreateSfxSound");
void SetSndSourceLo(void *p, int n) __asm__("SetSndSourceLo");
void CRef_ctor_BASIC(CRefData *pref, void *p) __asm__("CRef_ctor_BASIC");
// Splice builtin "start-sound": 0x1FBB90/0x1FB2F8 sound start, returns CRef(BASIC*) of sound.
SLY2_BUILTIN_C(BuiltinStartSound__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinStartSound__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    int n = args[1].u.n;
    void *p = CreateSfxSound(args[0].u.n);
    SetSndSourceLo(p, n);
    CRef_ctor_BASIC(ret, p);
    return ret;
}
} // namespace M_BuiltinStartSound__FiP4CRefP6CFrame

// 0x125248 BuiltinStartRumble__FiP4CRefP6CFrame
namespace M_BuiltinStartRumble__FiP4CRefP6CFrame {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void CRef_CoerceF32(CRefData *pref, CRefData *prefSrc) __asm__("CRef_CoerceF32");
void TriggerJoyRumbleRumk__FP3JOY4RUMKf(void *p, int n, float g) __asm__("TriggerJoyRumbleRumk__FP3JOY4RUMKf");
// Splice builtin "start-rumble": TriggerJoyRumbleRumk-like 0x187E28.
SLY2_BUILTIN_C(BuiltinStartRumble__FiP4CRefP6CFrame)
extern "C" CRefData *BuiltinStartRumble__FiP4CRefP6CFrame(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    void *p = &g_joy;
    int n = args[0].u.n;
    CRefData ref;
    CRef_CoerceF32(&ref, &args[1]);
    TriggerJoyRumbleRumk__FP3JOY4RUMKf(p, n, ref.u.g);
    Decref__4CRef(&ref);
    SetTag__4CRef4TAGK(&ref, 0x24);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinStartRumble__FiP4CRefP6CFrame

// 0x1252D0 FillAsFromPair
namespace M_FillAsFromPair {
struct AREQ; struct ARR;
typedef struct AREQ { int cb; int cbAlign; } AREQ;
typedef struct ARR { int c; short *a; } ARR;
short *AllocFromStack(AREQ *preq) __asm__("AllocFromStack");
// Counts a list and allocates (PvAllocSlot-style 0x1AE318) a U16 array of its S32/Symid elements (save-var path).
// Also known as: AkeyFromList (upstream readable / Sly 1 name; AkeyFromList__FP4CRefP3ARR).
extern "C" void FillAsFromPair(CPair *pnode, ARR *parr)
{
    CPair *p;
    int i;
    parr->c = 0;
    parr->a = 0;
    for (p = pnode; p; p = p->next.p)
        parr->c++;
    if (parr->c) {
        AREQ req;
        req.cb = parr->c * 2;
        req.cbAlign = 0x10;
        parr->a = AllocFromStack(&req);
        for (p = pnode, i = 0; p; p = p->next.p, i++) {
            if (p->ref.m_tagk == TAGK_S32 || p->ref.m_tagk == TAGK_SYMID)
                { int n = p->ref.u.n; parr->a[i] = n; }
        }
    }
}
} // namespace M_FillAsFromPair

// 0x125398 BuiltinGetSaveVar
namespace M_BuiltinGetSaveVar {
struct X;
struct X { int a[4]; };

void FillAsFromPair(void *pv, X *px) __asm__("FillAsFromPair");
CRef GetSaveVar(void *a, X *px, void *b) __asm__("GetSaveVar");
CRef BuiltinGetSaveVar(int cref, CRef *args, CFrame *frame) __asm__("BuiltinGetSaveVar");

// Converts path list to U16 array (0x1252D0) and returns GetSaveVar(D_003E0EA8,..).
CRef BuiltinGetSaveVar(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    STACKSCOPE obj;
    X x;
    FillAsFromPair(args[0].u.pv, &x);
    ref = GetSaveVar(&g_sw.savevars, &x, &g_gs);
    return ref;
}
} // namespace M_BuiltinGetSaveVar

// 0x125450 BuiltinSetSaveVar
namespace M_BuiltinSetSaveVar {
struct X;
struct X { int a[4]; };

void FillAsFromPair(void *pv, X *px) __asm__("FillAsFromPair");
CRef SetSaveVar(void *a, X *px, CRef *pref, void *b) __asm__("SetSaveVar");
CRef BuiltinSetSaveVar(int cref, CRef *args, CFrame *frame) __asm__("BuiltinSetSaveVar");

// Path list + value -> SetSaveVar 0x1D80C8. Unused by retail scripts.
CRef BuiltinSetSaveVar(int cref, CRef *args, CFrame *frame)
{
    CRef ref;
    STACKSCOPE obj;
    X x;
    FillAsFromPair(args[0].u.pv, &x);
    ref = SetSaveVar(&g_sw.savevars, &x, &args[1], &g_gs);
    return ref;
}
} // namespace M_BuiltinSetSaveVar

// 0x125510 BuiltinFindBindingOrNoop
namespace M_BuiltinFindBindingOrNoop {
enum { BIFK_NOOP = 0x86 };   /* builtin 134, noop~ */
extern CRef BuiltinFindBindingOrNoop(void *pbasic, CRef *args, CFrame *pframe) __asm__("BuiltinFindBindingOrNoop");
// Looks up a symbol in the calling object frame chain (CFrame::FindBindingPtr(sym,1)); if unbound returns Bifk 0x86
// (slot 134 no-op builtin).
CRef BuiltinFindBindingOrNoop(void *pbasic, CRef *args, CFrame *frame)
{
    if (frame && frame->m_cpframeParent == 1) {
        while (!frame->m_fObjectFrame) {
            frame = frame->m_apframeParent[0].m_p;
            if (!frame || frame->m_cpframeParent != 1)
                goto unbound;   /* one shared tail; a second `return CRef(...)` here does not match */
        }
        {
            CRef *binding = frame->m_apframeParent[0].m_p->FindBindingPtr(args[0].u.n, 1);
            if (binding)
                return *binding;
        }
    }
unbound:
    return CRef(BIFK_NOOP, TAGK_BIFK);
}
} // namespace M_BuiltinFindBindingOrNoop

// 0x1255C0 BuiltinNoop
// Varargs builtin that returns Void.
SLY2_BUILTIN_C(BuiltinNoop)
extern "C" CRefData *BuiltinNoop(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    SetTag(pref, TAGK_VOID);
    return pref;
}

// 0x1255F0 BuiltinBitAnd
// Bitwise AND of all S32 list args (init -1).
SLY2_BUILTIN_C(BuiltinBitAnd)
extern "C" CRefData *BuiltinBitAnd(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    int n;
    CPair *ppair;
    n = -1;
    ppair = 0;
    if (args->m_tagk != 0) {
        ppair = args->u.ppair;
    }
    if (ppair != 0) {
        do {
            int nItem = ppair->ref.u.n;
            ppair = ppair->next.p;
            n &= nItem;
        } while (ppair != 0);
    }
    pref->u.n = n;
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x125640 BuiltinBitOr
// Bitwise OR of all S32 list args (init 0).
SLY2_BUILTIN_C(BuiltinBitOr)
extern "C" CRefData *BuiltinBitOr(CRefData *pref, int cref, CRefData *args, CFrame *frame)
{
    int n;
    CPair *ppair;
    n = 0;
    ppair = 0;
    if (args->m_tagk != 0) {
        ppair = args->u.ppair;
    }
    if (ppair != 0) {
        do {
            int nItem = ppair->ref.u.n;
            ppair = ppair->next.p;
            n |= nItem;
        } while (ppair != 0);
    }
    pref->u.n = n;
    pref->m_tagk = TAGK_S32;
    return pref;
}

// 0x125690 BuiltinFindHintboxObject
namespace M_func_00125690 {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_SetBasic(CRefData *pref, void *p) __asm__("CRef_SetBasic");
extern char D_003DA0C0[] __asm__("D_003DA0C0");
// Splice builtin slot 137: returns a reference to the static object D_003DA0C0, which Lilly-derived code treats as a
// HINTBOX (ClearIfSlotId) and the save TU uses; hence the name (inferred).
SLY2_BUILTIN_C(BuiltinFindHintboxObject)
extern "C" CRefData *BuiltinFindHintboxObject(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, D_003DA0C0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_func_00125690

// 0x1256E0 BuiltinIsPowerUpAcquired
namespace M_BuiltinIsPowerUpAcquired {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void CRef_SetBool(CRefData *pref, int f) __asm__("CRef_SetBool");
int IsPowerupAcquired(int n) __asm__("IsPowerupAcquired");
// Bool: save var powerUps.mppukfAcquired[n] via 0x1DAEA8.
SLY2_BUILTIN_C(BuiltinIsPowerUpAcquired)
extern "C" CRefData *BuiltinIsPowerUpAcquired(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    ref.m_tagk = -1;
    CRef_SetBool(&ref, IsPowerupAcquired(args->u.n) != 0);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinIsPowerUpAcquired

// 0x125738 BuiltinSetPowerUpAcquired
namespace M_BuiltinSetPowerUpAcquired {
void Decref__4CRef(CRefData *pref) __asm__("Decref__4CRef");
void SetTag__4CRef4TAGK(CRefData *pref, int tagk) __asm__("SetTag__4CRef4TAGK");
void SetPowerupAcquired(int a, int b) __asm__("SetPowerupAcquired");
// Sets powerUps.mppukfAcquired[n]=f via 0x1DAF38 (also fires event 0x3B on g_game when set).
SLY2_BUILTIN_C(BuiltinSetPowerUpAcquired)
extern "C" CRefData *BuiltinSetPowerUpAcquired(CRefData *ret, int cref, CRefData *args, CFrame *frame)
{
    CRefData ref;
    SetTag__4CRef4TAGK(&ref, 0x24);
    SetPowerupAcquired(args[0].u.n, args[1].u.n);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
} // namespace M_BuiltinSetPowerUpAcquired



// 0x256CE8 / 0x256D08: VU_FLOAT's and VU_VECTOR's constructors, inline members defined after their callers, so g++
// calls them and emits the out-of-line copies (linkonce), as the original did. operator* (0x256D28) is a plain
// definition: its callers reach it only through asm-labelled declarations of other views, and g++ 2.95 emits an
// inline free function only when a call or reference names that function itself (an asm label naming its symbol does
// not count, nor does taking its address inside an unemitted inline); calling it through the real declaration
// changes the Builtin* code. Its declaration gives it what g++ gives an inline function's out-of-line copy: a weak
// symbol in .gnu.linkonce.t.<symbol>.
// 0x256CE8 __8VU_FLOATf
// VU_FLOAT constructor: moves a float into a VU quadword's x (an out-of-line inline copy in the linkonce area).
inline VU_FLOAT::VU_FLOAT(float f) { __asm__("mfc1 %0, %1" : "=r"(q) : "f"(f)); }

// 0x256D08 __9VU_VECTORRC6VECTOR
// VU_VECTOR constructor: loads a 16-byte vector into a VU quadword (out-of-line inline copy).
inline VU_VECTOR::VU_VECTOR(const VECTORQ &vec)
{
    q = *(const qword *)&vec;
}

// 0x256D28 __ml__FG8VU_FLOATG9VU_VECTOR
// Scales a VU vector by a VU float (out-of-line inline copy of the operator).
qword operator*(VU_FLOAT f, VU_VECTOR v) __attribute__((weak, section(".gnu.linkonce.t.__ml__FG8VU_FLOATG9VU_VECTOR")));
// Return v scaled by f.
qword operator*(VU_FLOAT f, VU_VECTOR v)
{
    qword r;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(r) : "j"(f.q), "j"(v.q));
    return r;
}



// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
/* The Splice builtin table (eval looks builtins up in it): 169 records {pfn, cargMin, fRest}, right after
   D_002BBB70 in bif's .data. The builtins are bound to their linker names with asm labels in a namespace. */
namespace M_bif_data {
void BuiltinAdd() __asm__("BuiltinAdd__FiP4CRefP6CFrame");
void BuiltinSubtract() __asm__("BuiltinSubtract__FiP4CRefP6CFrame");
void BuiltinMultiply() __asm__("BuiltinMultiply__FiP4CRefP6CFrame");
void BuiltinDivide() __asm__("BuiltinDivide__FiP4CRefP6CFrame");
void BuiltinPrint() __asm__("BuiltinPrint__FiP4CRefP6CFrame");
void BuiltinPrintFrame() __asm__("BuiltinPrintFrame__FiP4CRefP6CFrame");
void BuiltinPrintSidebag() __asm__("BuiltinPrintSidebag__FiP4CRefP6CFrame");
void BuiltinIntEqual() __asm__("BuiltinIntEqual__FiP4CRefP6CFrame");
void BuiltinLess() __asm__("BuiltinLess__FiP4CRefP6CFrame");
void BuiltinLessOrEqual() __asm__("BuiltinLessOrEqual__FiP4CRefP6CFrame");
void BuiltinGreater() __asm__("BuiltinGreater__FiP4CRefP6CFrame");
void BuiltinGreaterOrEqual() __asm__("BuiltinGreaterOrEqual__FiP4CRefP6CFrame");
void BuiltinEqv() __asm__("BuiltinEqv__FiP4CRefP6CFrame");
void BuiltinEqual() __asm__("BuiltinEqual__FiP4CRefP6CFrame");
void BuiltinIsBoolean() __asm__("BuiltinIsBoolean__FiP4CRefP6CFrame");
void BuiltinIsNum() __asm__("BuiltinIsNum__FiP4CRefP6CFrame");
void BuiltinIsInteger() __asm__("BuiltinIsInteger__FiP4CRefP6CFrame");
void BuiltinIsFloat() __asm__("BuiltinIsFloat__FiP4CRefP6CFrame");
void BuiltinIsSymbol() __asm__("BuiltinIsSymbol__FiP4CRefP6CFrame");
void BuiltinIsVector() __asm__("BuiltinIsVector__FiP4CRefP6CFrame");
void BuiltinIsMatrix() __asm__("BuiltinIsMatrix__FiP4CRefP6CFrame");
void BuiltinIsClq() __asm__("BuiltinIsClq__FiP4CRefP6CFrame");
void BuiltinIsLm() __asm__("BuiltinIsLm__FiP4CRefP6CFrame");
void BuiltinIsSmp() __asm__("BuiltinIsSmp__FiP4CRefP6CFrame");
void BuiltinIsList() __asm__("BuiltinIsList__FiP4CRefP6CFrame");
void BuiltinIsNull() __asm__("BuiltinIsNull__FiP4CRefP6CFrame");
void BuiltinIsObject() __asm__("BuiltinIsObject__FiP4CRefP6CFrame");
void BuiltinIsNullObj() __asm__("BuiltinIsNullObj__FiP4CRefP6CFrame");
void BuiltinIsMethod() __asm__("BuiltinIsMethod__FiP4CRefP6CFrame");
void BuiltinIsProcedure() __asm__("BuiltinIsProcedure__FiP4CRefP6CFrame");
void BuiltinAreNear() __asm__("BuiltinAreNear__FiP4CRefP6CFrame");
void BuiltinNot() __asm__("BuiltinNot__FiP4CRefP6CFrame");
void BuiltinCons() __asm__("BuiltinCons__FiP4CRefP6CFrame");
void BuiltinCar() __asm__("BuiltinCar__FiP4CRefP6CFrame");
void BuiltinCdr() __asm__("BuiltinCdr__FiP4CRefP6CFrame");
void BuiltinSetCar() __asm__("BuiltinSetCar__FiP4CRefP6CFrame");
void BuiltinSetCdr() __asm__("BuiltinSetCdr__FiP4CRefP6CFrame");
void BuiltinLength() __asm__("BuiltinLength__FiP4CRefP6CFrame");
void BuiltinNth() __asm__("BuiltinNth__FiP4CRefP6CFrame");
void BuiltinIsMember() __asm__("BuiltinIsMember__FiP4CRefP6CFrame");
void BuiltinList() __asm__("BuiltinList__FiP4CRefP6CFrame");
void BuiltinShuffle() __asm__("BuiltinShuffle");
void BuiltinAppend() __asm__("BuiltinAppend__FiP4CRefP6CFrame");
void BuiltinMap() __asm__("BuiltinMap__FiP4CRefP6CFrame");
void BuiltinFilter() __asm__("BuiltinFilter__FiP4CRefP6CFrame");
void BuiltinForEach() __asm__("BuiltinForEach__FiP4CRefP6CFrame");
void BuiltinEval() __asm__("BuiltinEval__FiP4CRefP6CFrame");
void BuiltinVector() __asm__("BuiltinVector__FiP4CRefP6CFrame");
void BuiltinMatrix() __asm__("BuiltinMatrix__FiP4CRefP6CFrame");
void BuiltinClq() __asm__("BuiltinClq__FiP4CRefP6CFrame");
void BuiltinLm() __asm__("BuiltinLm__FiP4CRefP6CFrame");
void BuiltinSmp() __asm__("BuiltinSmp__FiP4CRefP6CFrame");
void BuiltinGetElement() __asm__("BuiltinGetElement__FiP4CRefP6CFrame");
void BuiltinRandom() __asm__("BuiltinRandom__FiP4CRefP6CFrame");
void BuiltinSqrt() __asm__("BuiltinSqrt__FiP4CRefP6CFrame");
void BuiltinSin() __asm__("BuiltinSin__FiP4CRefP6CFrame");
void BuiltinCos() __asm__("BuiltinCos__FiP4CRefP6CFrame");
void BuiltinTan() __asm__("BuiltinTan__FiP4CRefP6CFrame");
void BuiltinAsin() __asm__("BuiltinAsin__FiP4CRefP6CFrame");
void BuiltinAcos() __asm__("BuiltinAcos__FiP4CRefP6CFrame");
void BuiltinAtan() __asm__("BuiltinAtan__FiP4CRefP6CFrame");
void BuiltinRadNormalize() __asm__("BuiltinRadNormalize__FiP4CRefP6CFrame");
void BuiltinAtan2() __asm__("BuiltinAtan2__FiP4CRefP6CFrame");
void BuiltinVectorDotProduct() __asm__("BuiltinVectorDotProduct__FiP4CRefP6CFrame");
void BuiltinVectorCrossProduct() __asm__("BuiltinVectorCrossProduct__FiP4CRefP6CFrame");
void BuiltinVectorLength() __asm__("BuiltinVectorLength__FiP4CRefP6CFrame");
void BuiltinVectorDistance() __asm__("BuiltinVectorDistance__FiP4CRefP6CFrame");
void BuiltinVectorDistanceSquared() __asm__("BuiltinVectorDistanceSquared__FiP4CRefP6CFrame");
void BuiltinVectorNormalize() __asm__("BuiltinVectorNormalize__FiP4CRefP6CFrame");
void BuiltinVectorProjectNormal() __asm__("BuiltinVectorProjectNormal__FiP4CRefP6CFrame");
void BuiltinVectorProjectTangent() __asm__("BuiltinVectorProjectTangent__FiP4CRefP6CFrame");
void BuiltinVectorBallisticVelocity() __asm__("BuiltinVectorBallisticVelocity__FiP4CRefP6CFrame");
void BuiltinVectorRadianNormal() __asm__("BuiltinVectorRadianNormal__FiP4CRefP6CFrame");
void BuiltinMatrixTranspose() __asm__("BuiltinMatrixTranspose__FiP4CRefP6CFrame");
void BuiltinMatrixInvert() __asm__("BuiltinMatrixInvert__FiP4CRefP6CFrame");
void BuiltinMatrixCalculateDmat() __asm__("BuiltinMatrixCalculateDmat__FiP4CRefP6CFrame");
void BuiltinMatrixInterpolateRotate() __asm__("BuiltinMatrixInterpolateRotate__FiP4CRefP6CFrame");
void BuiltinMatrixDecomposeToTranslate() __asm__("BuiltinMatrixDecomposeToTranslate__FiP4CRefP6CFrame");
void BuiltinMatrixDecomposeToRotate() __asm__("BuiltinMatrixDecomposeToRotate__FiP4CRefP6CFrame");
void BuiltinMatrixDecomposeToEuler() __asm__("BuiltinMatrixDecomposeToEuler__FiP4CRefP6CFrame");
void BuiltinMatrixDecomposeToRadianNormal() __asm__("BuiltinMatrixDecomposeToRadianNormal__FiP4CRefP6CFrame");
void BuiltinMatrixLookAt() __asm__("BuiltinMatrixLookAt__FiP4CRefP6CFrame");
void BuiltinMatrixTiltUpright() __asm__("BuiltinMatrixTiltUpright__FiP4CRefP6CFrame");
void BuiltinClqEvaluate() __asm__("BuiltinClqEvaluate__FiP4CRefP6CFrame");
void BuiltinClqEvaluateLm() __asm__("BuiltinClqEvaluateLm__FiP4CRefP6CFrame");
void BuiltinClqFit() __asm__("BuiltinClqFit__FiP4CRefP6CFrame");
void BuiltinLmLimit() __asm__("BuiltinLmLimit__FiP4CRefP6CFrame");
void BuiltinLmCheck() __asm__("BuiltinLmCheck__FiP4CRefP6CFrame");
void BuiltinFloor() __asm__("BuiltinFloor__FiP4CRefP6CFrame");
void BuiltinCeiling() __asm__("BuiltinCeiling__FiP4CRefP6CFrame");
void BuiltinRound() __asm__("BuiltinRound__FiP4CRefP6CFrame");
void BuiltinTruncate() __asm__("BuiltinTruncate__FiP4CRefP6CFrame");
void BuiltinAbs() __asm__("BuiltinAbs__FiP4CRefP6CFrame");
void BuiltinMaximum() __asm__("BuiltinMaximum__FiP4CRefP6CFrame");
void BuiltinMinimum() __asm__("BuiltinMinimum__FiP4CRefP6CFrame");
void BuiltinModulo() __asm__("BuiltinModulo__FiP4CRefP6CFrame");
void BuiltinCurrentTime() __asm__("BuiltinCurrentTime__FiP4CRefP6CFrame");
void BuiltinCurrentLevel() __asm__("BuiltinCurrentLevel");
void BuiltinCurrentEpisode() __asm__("BuiltinCurrentEpisode");
void BuiltinCurrentWorld() __asm__("BuiltinCurrentWorld");
void BuiltinFindCurrentJob() __asm__("BuiltinFindCurrentJob");
void BuiltinFindCurrentTask() __asm__("BuiltinFindCurrentTask");
void BuiltinFindPendingTask() __asm__("BuiltinFindPendingTask");
void BuiltinGetDifficulty() __asm__("BuiltinGetDifficulty");
void BuiltinGetDevFlag() __asm__("BuiltinGetDevFlag");
void BuiltinScheduleCallback() __asm__("BuiltinScheduleCallback__FiP4CRefP6CFrame");
void BuiltinCancelCallback() __asm__("BuiltinCancelCallback");
void BuiltinDeferObjectUpdate() __asm__("BuiltinDeferObjectUpdate__FiP4CRefP6CFrame");
void BuiltinAddOption() __asm__("BuiltinAddOption__FiP4CRefP6CFrame");
void BuiltinEnsureOption() __asm__("BuiltinEnsureOption__FiP4CRefP6CFrame");
void BuiltinSetOption() __asm__("BuiltinSetOption__FiP4CRefP6CFrame");
void BuiltinGetOption() __asm__("BuiltinGetOption__FiP4CRefP6CFrame");
void BuiltinFindObject() __asm__("BuiltinFindObject__FiP4CRefP6CFrame");
void BuiltinFindObjects() __asm__("BuiltinFindObjects__FiP4CRefP6CFrame");
void BuiltinFindObjectsFromOids() __asm__("BuiltinFindObjectsFromOids");
void BuiltinFindNearestObject() __asm__("BuiltinFindNearestObject__FiP4CRefP6CFrame");
void BuiltinFindNearestObjects() __asm__("BuiltinFindNearestObjects__FiP4CRefP6CFrame");
void BuiltinFindWorldObject() __asm__("BuiltinFindWorldObject__FiP4CRefP6CFrame");
void BuiltinFindSoundObject() __asm__("BuiltinFindSoundObject");
void BuiltinFindCameraObject() __asm__("BuiltinFindCameraObject__FiP4CRefP6CFrame");
void BuiltinFindClassObjects() __asm__("BuiltinFindClassObjects__FiP4CRefP6CFrame");
void BuiltinFindObjectsInBoundingBox() __asm__("BuiltinFindObjectsInBoundingBox__FiP4CRefP6CFrame");
void BuiltinFindObjectsInBoundingSphere() __asm__("BuiltinFindObjectsInBoundingSphere__FiP4CRefP6CFrame");
void BuiltinHitTestObjects() __asm__("BuiltinHitTestObjects__FiP4CRefP6CFrame");
void BuiltinHitTestObjectsFirst() __asm__("BuiltinHitTestObjectsFirst__FiP4CRefP6CFrame");
void BuiltinConvertObjectPosition() __asm__("BuiltinConvertObjectPosition__FiP4CRefP6CFrame");
void BuiltinConvertObjectVector() __asm__("BuiltinConvertObjectVector__FiP4CRefP6CFrame");
void BuiltinConvertObjectMatrix() __asm__("BuiltinConvertObjectMatrix__FiP4CRefP6CFrame");
void BuiltinNearClipCenter() __asm__("BuiltinNearClipCenter__FiP4CRefP6CFrame");
void BuiltinStartSound() __asm__("BuiltinStartSound__FiP4CRefP6CFrame");
void BuiltinStartRumble() __asm__("BuiltinStartRumble__FiP4CRefP6CFrame");
void BuiltinGetSaveVar() __asm__("BuiltinGetSaveVar");
void BuiltinSetSaveVar() __asm__("BuiltinSetSaveVar");
void BuiltinFindBindingOrNoop() __asm__("BuiltinFindBindingOrNoop");
void BuiltinNoop() __asm__("BuiltinNoop");
void BuiltinBitAnd() __asm__("BuiltinBitAnd");
void BuiltinBitOr() __asm__("BuiltinBitOr");
void BuiltinFindHintboxObject() __asm__("BuiltinFindHintboxObject");
void BuiltinIsPowerUpAcquired() __asm__("BuiltinIsPowerUpAcquired");
void BuiltinSetPowerUpAcquired() __asm__("BuiltinSetPowerUpAcquired");
void BuiltinStub140() __asm__("BuiltinStub140");
void BuiltinStub141() __asm__("BuiltinStub141");
void BuiltinStub142() __asm__("BuiltinStub142");
void BuiltinStub143() __asm__("BuiltinStub143");
void BuiltinStub144() __asm__("BuiltinStub144");
void BuiltinStub145() __asm__("BuiltinStub145");
void BuiltinStub146() __asm__("BuiltinStub146");
void BuiltinStub147() __asm__("BuiltinStub147");
void BuiltinStub148() __asm__("BuiltinStub148");
void BuiltinStub149() __asm__("BuiltinStub149");
void BuiltinStub150() __asm__("BuiltinStub150");
void BuiltinStub151() __asm__("BuiltinStub151");
void BuiltinStub152() __asm__("BuiltinStub152");
void BuiltinStub153() __asm__("BuiltinStub153");
void BuiltinStub154() __asm__("BuiltinStub154");
void BuiltinStub155() __asm__("BuiltinStub155");
void BuiltinStub156() __asm__("BuiltinStub156");
void BuiltinStub157() __asm__("BuiltinStub157");
void BuiltinStub158() __asm__("BuiltinStub158");
void BuiltinStub159() __asm__("BuiltinStub159");
void BuiltinStub160() __asm__("BuiltinStub160");
void BuiltinStub161() __asm__("BuiltinStub161");
void BuiltinStub162() __asm__("BuiltinStub162");
void BuiltinStub163() __asm__("BuiltinStub163");
void BuiltinStub164() __asm__("BuiltinStub164");
void BuiltinStub165() __asm__("BuiltinStub165");
void BuiltinStub166() __asm__("BuiltinStub166");
void BuiltinStub167() __asm__("BuiltinStub167");
void BuiltinStub168() __asm__("BuiltinStub168");
// ---- (Defined before g_mpbifkbif: the ELF has D_002BBB70 first.) Pointer constants and records, by address name (docs/guides/tu-merging.md "Data"): words as the ELF has
// them, pointers through asm-labelled declarations. Non-virtual pointer-to-member constants are 8 bytes
// {delta 0, index -1, pfn}; the definitions sit in their own namespace so no declaration above has to agree.
namespace ANPTR_bif {
void p_RadNormalize__Ff() __asm__("NormalizeAngle__Ff");
void p_acosf() __asm__("acosf");
void p_asinf() __asm__("asinf");
void p_atanf() __asm__("atanf");
void p_cosf() __asm__("cosf");
void p_sinf() __asm__("sinf");
void p_sqrtf() __asm__("sqrtf");
void p_tanf() __asm__("tanf");

extern unsigned int D_002BBB70[8] __asm__("D_002BBB70") = {(unsigned int)p_sqrtf, (unsigned int)p_sinf, (unsigned int)p_cosf, (unsigned int)p_tanf, (unsigned int)p_asinf, (unsigned int)p_acosf, (unsigned int)p_atanf, (unsigned int)p_RadNormalize__Ff};

} // namespace ANPTR_bif
struct BIF { void (*pfn)(); int cargMin; int fRest; };   /* pfn: CRef (*)(int, CRef *, CFrame *) */
BIF g_mpbifkbif[169] __asm__("g_mpbifkbif") = {
    {BuiltinAdd, 1, 1}, {BuiltinSubtract, 1, 1}, {BuiltinMultiply, 1, 1}, {BuiltinDivide, 1, 1}, {BuiltinPrint, 1, 0},
    {BuiltinPrintFrame, 0, 1}, {BuiltinPrintSidebag, 1, 0}, {BuiltinIntEqual, 2, 0}, {BuiltinLess, 2, 0}, {BuiltinLessOrEqual, 2, 0},
    {BuiltinGreater, 2, 0}, {BuiltinGreaterOrEqual, 2, 0}, {BuiltinEqv, 2, 0}, {BuiltinEqual, 2, 0}, {BuiltinIsBoolean, 1, 0},
    {BuiltinIsNum, 1, 0}, {BuiltinIsInteger, 1, 0}, {BuiltinIsFloat, 1, 0}, {BuiltinIsSymbol, 1, 0}, {BuiltinIsVector, 1, 0},
    {BuiltinIsMatrix, 1, 0}, {BuiltinIsClq, 1, 0}, {BuiltinIsLm, 1, 0}, {BuiltinIsSmp, 1, 0}, {BuiltinIsList, 1, 0},
    {BuiltinIsNull, 1, 0}, {BuiltinIsObject, 1, 0}, {BuiltinIsNullObj, 1, 0}, {BuiltinIsMethod, 1, 0},
    {BuiltinIsProcedure, 1, 0}, {BuiltinAreNear, 3, 0}, {BuiltinNot, 1, 0}, {BuiltinCons, 2, 0}, {BuiltinCar, 1, 0},
    {BuiltinCdr, 1, 0}, {BuiltinSetCar, 2, 0}, {BuiltinSetCdr, 2, 0}, {BuiltinLength, 1, 0}, {BuiltinNth, 2, 0},
    {BuiltinIsMember, 2, 0}, {BuiltinList, 0, 1}, {BuiltinShuffle, 1, 0}, {BuiltinAppend, 0, 1}, {BuiltinMap, 2, 0},
    {BuiltinFilter, 2, 0}, {BuiltinForEach, 2, 0}, {BuiltinEval, 1, 0}, {BuiltinVector, 0, 1}, {BuiltinMatrix, 0, 1},
    {BuiltinClq, 3, 0}, {BuiltinLm, 2, 0}, {BuiltinSmp, 3, 0}, {BuiltinGetElement, 1, 1}, {BuiltinRandom, 2, 0},
    {BuiltinSqrt, 1, 0}, {BuiltinSin, 1, 0}, {BuiltinCos, 1, 0}, {BuiltinTan, 1, 0}, {BuiltinAsin, 1, 0}, {BuiltinAcos, 1, 0},
    {BuiltinAtan, 1, 0}, {BuiltinRadNormalize, 1, 0}, {BuiltinAtan2, 2, 0}, {BuiltinVectorDotProduct, 2, 0},
    {BuiltinVectorCrossProduct, 2, 0}, {BuiltinVectorLength, 1, 0}, {BuiltinVectorDistance, 1, 1},
    {BuiltinVectorDistanceSquared, 1, 1}, {BuiltinVectorNormalize, 1, 1}, {BuiltinVectorProjectNormal, 2, 0},
    {BuiltinVectorProjectTangent, 2, 0}, {BuiltinVectorBallisticVelocity, 3, 1}, {BuiltinVectorRadianNormal, 2, 0},
    {BuiltinMatrixTranspose, 1, 0}, {BuiltinMatrixInvert, 1, 0}, {BuiltinMatrixCalculateDmat, 2, 0},
    {BuiltinMatrixInterpolateRotate, 3, 0}, {BuiltinMatrixDecomposeToTranslate, 1, 0},
    {BuiltinMatrixDecomposeToRotate, 1, 0}, {BuiltinMatrixDecomposeToEuler, 1, 0},
    {BuiltinMatrixDecomposeToRadianNormal, 1, 0}, {BuiltinMatrixLookAt, 2, 1}, {BuiltinMatrixTiltUpright, 1, 0},
    {BuiltinClqEvaluate, 2, 0}, {BuiltinClqEvaluateLm, 3, 0}, {BuiltinClqFit, 4, 0}, {BuiltinLmLimit, 2, 0},
    {BuiltinLmCheck, 2, 0}, {BuiltinFloor, 1, 0}, {BuiltinCeiling, 1, 0}, {BuiltinRound, 1, 0}, {BuiltinTruncate, 1, 0},
    {BuiltinAbs, 1, 0}, {BuiltinMaximum, 1, 1}, {BuiltinMinimum, 1, 1}, {BuiltinModulo, 2, 0}, {BuiltinCurrentTime, 0, 0},
    {BuiltinCurrentLevel, 0, 0}, {BuiltinCurrentEpisode, 0, 0}, {BuiltinCurrentWorld, 0, 0}, {BuiltinFindCurrentJob, 0, 0},
    {BuiltinFindCurrentTask, 0, 0}, {BuiltinFindPendingTask, 0, 0}, {BuiltinGetDifficulty, 0, 0}, {BuiltinGetDevFlag, 0, 0},
    {BuiltinScheduleCallback, 2, 1}, {BuiltinCancelCallback, 1, 0}, {BuiltinDeferObjectUpdate, 2, 0}, {BuiltinAddOption, 3, 0},
    {BuiltinEnsureOption, 3, 0}, {BuiltinSetOption, 3, 0}, {BuiltinGetOption, 2, 0}, {BuiltinFindObject, 2, 1}, {BuiltinFindObjects, 2, 1},
    {BuiltinFindObjectsFromOids, 2, 1}, {BuiltinFindNearestObject, 2, 1}, {BuiltinFindNearestObjects, 2, 1},
    {BuiltinFindWorldObject, 0, 0}, {BuiltinFindSoundObject, 0, 0}, {BuiltinFindCameraObject, 0, 0},
    {BuiltinFindClassObjects, 2, 0}, {BuiltinFindObjectsInBoundingBox, 2, 0}, {BuiltinFindObjectsInBoundingSphere, 2, 0},
    {BuiltinHitTestObjects, 3, 0}, {BuiltinHitTestObjectsFirst, 3, 0}, {BuiltinConvertObjectPosition, 3, 0},
    {BuiltinConvertObjectVector, 3, 0}, {BuiltinConvertObjectMatrix, 3, 0}, {BuiltinNearClipCenter, 0, 0},
    {BuiltinStartSound, 2, 0}, {BuiltinStartRumble, 2, 0}, {BuiltinGetSaveVar, 1, 0}, {BuiltinSetSaveVar, 2, 0},
    {BuiltinFindBindingOrNoop, 1, 0}, {BuiltinNoop, 0, 1}, {BuiltinBitAnd, 0, 1}, {BuiltinBitOr, 0, 1},
    {BuiltinFindHintboxObject, 0, 0}, {BuiltinIsPowerUpAcquired, 1, 0}, {BuiltinSetPowerUpAcquired, 2, 0}, {BuiltinStub140, 0, 0},
    {BuiltinStub141, 0, 0}, {BuiltinStub142, 0, 0}, {BuiltinStub143, 0, 0}, {BuiltinStub144, 0, 0},
    {BuiltinStub145, 0, 0}, {BuiltinStub146, 0, 0}, {BuiltinStub147, 0, 0}, {BuiltinStub148, 0, 0},
    {BuiltinStub149, 0, 0}, {BuiltinStub150, 0, 0}, {BuiltinStub151, 1, 0}, {BuiltinStub152, 1, 0},
    {BuiltinStub153, 1, 0}, {BuiltinStub154, 0, 0}, {BuiltinStub155, 0, 0}, {BuiltinStub156, 3, 0},
    {BuiltinStub157, 1, 0}, {BuiltinStub158, 0, 0}, {BuiltinStub159, 1, 0}, {BuiltinStub160, 0, 0},
    {BuiltinStub161, 0, 0}, {BuiltinStub162, 3, 0}, {BuiltinStub163, 2, 0}, {BuiltinStub164, 1, 0},
    {BuiltinStub165, 0, 0}, {BuiltinStub166, 2, 0}, {BuiltinStub167, 0, 0}, {BuiltinStub168, 0, 0}
};
}
