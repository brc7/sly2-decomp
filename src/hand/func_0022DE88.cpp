// func_0022DE88: ported from Lilly's personal Sly 2 decomp (src/P2/xf.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DA0 (D_002A2DA0), D_002DDED0 (g_clock), D_002A2DD0 (g_qwZero), D_003DD490 (g_sw)
// 0x0022DE88 UpdateSoSlots__FP2SO (our name, was FUN_0022DE88; SO::Update 0x1F3980 calls it right after the SO's own update). THE per-frame step of an SO's XF slots:
// for every XF it runs SlotRefresh and then, by the XF's role (nRole, see SlotRefresh / SlotSetRole), moves the XF's local matrix `mat` and stamps it (nStampWorld = ++g_sw.nStampXf):
//   roles 1 2   constant acceleration qwUnkF0 on the velocity vUnkB0: xfk 1 moves the position row (trapezoid: pos += (v_old + v_new) * dt / 2), xfk 6 turns the rotation by the
//               same angular step (LoadRotateMatrix, renormalized); when the new and the old velocity are both ~0 the velocity is cleared and role 1 -> 3 (role 2 stays awake)
//   role 0x14   xfk 6: continuous rotation, angle in qwRestUnkE0.x += rate (qwUnkD0.x) * dt about the axis qwRest
//   role 4      the position row = minus the position row of the reference XF (pxfRef), same stamp
//   roles 5 6   the actuator's value (QwSlotValue: phase 0 rest value, 1 Hermite segment, 2 3 4 qwRest, 5 the 3 rows of a rotation in qwRest/D0/E0) is stored (SlotStoreValue); role 6 + phase 2 -> role 10
//   roles 7 8 9 0xB  CHASE the actuator's value: delta = value - current, the distance is eased with GSmooth under an SMP built from the XF's speed limit nUnkAc (+ the target
//               speed along the direction); role 9 must arrive by the time stored in pxfRef (as a float); role 0xB also limits the change of velocity (nUnkAc * gain per second);
//               xfk 6 / 7 do the same with rotations (CalculateDmat -> axis + angle, LoadRotateMatrixRad about the axis) and angular velocities (QwRotRateFromEulerRate);
//               on arrival role 7/9 stay, 0xB phase 2 -> 0xC, phase 1 -> 6, phase 2 -> 10; the velocity becomes the target's rate
//   roles 0xD 0xF 0x10  call the class loaders' update functions 0x186A50 / 0x1604E8 / 0x21CB18 with (pso, pxfRef)
//   role 0x11   position row: Hermite from the current value + velocity to qwRest at the time pxfRef (as float), then qwRest
//   role 0x12   spring-damper to the rest value (xfk 1: kp = nUnkAc * 0.015, kd = nUnkAe * 0.0025; xfk 6 the same on the angle); settled (phase 2, small speed + distance) -> role 0x13
//   role 0x15   only keeps the SO awake
// After the switch the XF's world matrix is parent->matWorld * mat (VU0 microprogram 0x60, the T1025 recipe; the stamp is lifted to the parent's) or a copy of mat without a parent.
// The SO's flag 4 (grfUnk40) is cleared when no XF kept it awake (and the parent is not awake), flag 0x10 (snap: no smoothing this frame) is cleared, then func_0022FA20.
// HOW IT IS WRITTEN (T1113, each point verified on the diff; scratch decomp/build/scratch/t1113/, cmp.py = aligned diff + sp-offset score):
//  - every 128-bit read through a cast (`*(qword *)&pxf->vUnkB0`, `*(qword *)&pxf->qwUnkF0`, the position row) is an alias-everything load that cannot move
//    across the VU_FLOAT home stores: the game reads vUnkB0 / the position row BEFORE the VuFloat and qwUnkF0 after it (hence the qwV0 / qwPos0 locals);
//  - the value / rate switches are open-coded (macros below, as in SlotCurrentMatrix) with the T1074 join-copy trick (`qword qw0 = qw` + `qwVel = qw = g_qwZero`);
//    their Hermite arms call tiny inline helpers (QwHermitePos/Vel) so the VECTOR4 lives in an inline frame whose address is a pseudo - otherwise gcse CSEs it
//    with the SlotStoreValue temporary at the same offset (s0 = sp+0xD0 in the 0x12 rotation arm);
//  - the chase arms (roles 7 8 9 0xB) leave the limited case of role 0xB with `goto LChaseDone` (no flag in the game; inline functions with `return` match
//    the code too but their frames do not share stack slots like the game's);
//  - locals of the rotation arms are declared at the top of the arm in the game's frame order (matOld 0x00, matDmat/matTarget 0x30, matNew 0x60/0x90, vecAxis);
//  - xfk 11/12 tests are nested ifs with the shared else duplicated (two slti), SMP fields assigned svFast, svSlow, dtFast; gMin is an if/else and gOut is set
//    before dgNext; ClearXyz stores z, y, x; case 1's "else" is `if (nRole != 2) SlotSetRole(..3) else fIdle = 0` (case 6 too); fIdle = 0 after SlotSetRole(10).
//  (T1137, each verified on the diff; scratch decomp/build/scratch/t1137/ - var.py/opperm.py/rtlsum.py/pri.py, notes re/notes/decomp-compiler-quirks.md section 62)
//  - case 5/6 FALLS THROUGH into case 0x15 (`fIdle = 0; break;`): one `fIdle = 0` fewer = n_refs 21 instead of 23 = global.c priority 1157 < the g_qwZero hi copy's 1200,
//    which gives the copy $s7 and fIdle $fp like the game (the "knife-edge" of T1113 was a missing fall-through);
//  - the asm OPERAND ORDER decides which input local-alloc ties the output to (the first local input that dies): the scalar first for the rotation chase's QwMulAddX
//    (QwSMulAddX), (S, A, B, T) for the 0x12 rotation arm's QwMulXMulX; an output that is also an input variable (`qwDir = qwDir / g`, `qwDV = QwSub(qwDV, ..)`, the limited
//    `qwDV = QwMulAddX(qwV0, qwDV, ..)`) is ONE pseudo set twice = global, gets a vf register from global alloc instead of being tied (pos chase);
//  - the position chase reads vUnkB0 before its VU_FLOAT only in the limited path (qwV0), the rotation chase's gMax is computed first; `MATRIX3 matRot; matRot = LoadRotateMatrix(..);`
//    then `matTurn = matRot * matOld` gives the game's frame slots and the copy of the rows to 0xB0; pointers `pmatTarget` / `pmatStored` make the rows 1 and 2 load through a0 / s0;
//  - `int ixf;` declared before fSnap (frame slots 0x190 / 0x194), qwRate before qwOmega0 in the 0x12 rotation arm, qwTurn as a separate statement there, ClearXyz in role 0x11,
//    the tail keeps `pxfParent` but reads `pxf->pxfParent->nStampWorld` again, gDist = FloatOfVf(QwLength(..)) (the bits are not CSE'd: the game has a fresh mfc1 before the divide).
// MATCH (T1641, Lilly's team decomp). The personal draft (xf.c, #if 0) was 14 lines off; with the team toolchain it took three changes to close it:
//  - Near()'s Y word is ONE float-out asm (`prot3w %0,%1` "=r"(float)) like the X/Z words, not a TI temp + asm mtc1: that alone moved the z `pextuw` pairs to $a0/$a1;
//  - the position chase's distance goes through GXOfQwAct(QwLength(..)) (compiler qmfc2 + asm mtc1): the divide gets a fresh `mfc1 v1,f21` and a plain bc1f;
//  - the position chase's rate switch fills a short-lived local (qwR) and copies it into qwTargetVel through PassQ (empty tied asm, an opaque copy): the arms are plain
//    $v0 loads, the spill `sq v0,0x1C0(sp)` is shared, and the goal reload becomes $vf4; a bare `asm("")` after it keeps the `sq` ahead of the argument moves of the next call.
//  - the world-matrix tail is written with asm text (loads + vcallms in one output-less asm, the result rows as cfc2.i outputs) and ONE register pin instead of
//    the personal draft's eight bound vf variables.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/sw.h"
MATRIX3 LoadRotateMatrix(qword vec);
struct VU_VECTOR4
{
    qword qw;
};
inline MATRIX3 operator*(const MATRIX3 &matL, const MATRIX3 &matR) return mat
{
    qword qwL0 = *(qword *)matL.mat[0];
    qword qwL1 = *(qword *)matL.mat[1];
    qword qwL2 = *(qword *)matL.mat[2];
    VU_VECTOR4 v0;
    VU_VECTOR4 v1;
    VU_VECTOR4 v2;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v0.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[0])->qw));
    *(VU_VECTOR4 *)mat.mat[0] = v0;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v1.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[1])->qw));
    *(VU_VECTOR4 *)mat.mat[1] = v1;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(v2.qw) : "j"(qwL0), "j"(qwL1), "j"(qwL2), "j"(((VU_VECTOR4 *)matR.mat[2])->qw));
    *(VU_VECTOR4 *)mat.mat[2] = v2;
}
void DecomposeRotateMatrixRad(MATRIX3 *pmat, float *prad, VECTOR *pnormal);
qword DecomposeRotateMatrixEuler(MATRIX3 *pmat);
void NormalizeRotateMatrix3(MATRIX3 *pmat);
MATRIX3 CalculateDmat(MATRIX3 *pmat0, MATRIX3 *pmat1);
float RadNormalize(float rad);
float GSmooth(float gCur, float gTarget, float dt, SMP *psmp, float *pdgNext);
int FFloatsNear(float g1, float g2, float gEpsilon);
struct SLOT
{
      SLOT *pslotNext;
};
extern CLOCK D_002DDED0;
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct GRABSPOTDATA
{
      TN *ptn;
      undefined4 unk_0x04;
      float sUnk08;
      undefined4 unk_0x0c;
      int nPriority;
      int nUnk14;
      int nGroup;
      uint grfUnk1c;
};
struct GRABSPOT : public GRABSPOTDATA
{
};
struct CELL
{
      qword plane;
      CELL *pcellNeg;
      CELL *pcellPos;
      int ileafNeg;
      int ileafPos;
};
struct LANGSTR
{
      int id;
      char *pchz;
};
struct SVN
{
      int nHash;
      char *pchzName;
      int id;
      SVN *psvnNext;
};
struct SVPATH
{
      int cid;
      short *aid;
};
struct LOUPD
{
      int grf;
      LO *plo;
};
extern SW D_003DD490;
void HermiteVec4(float dt, float t, float sRate, qword p0, qword m0, qword p1, qword m1, VECTOR4 *ppos, VECTOR4 *pvel, VECTOR4 *pacc);
struct VU_FLOAT
{
    qword qw;
};
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLength(qword v)
{
    return GFromVf(QwLength(v));
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
static inline float GXOfQwAct(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
static inline float GYOfQwAct(qword qw)
{
    float y;
    __asm__("prot3w %0, %1" : "=r"(y) : "r"(qw));
    return y;
}
static inline int FQwNearXyz(qword qwA, qword qwB)
{
    return FFloatsNear(GXOfQwAct(qwA), GXOfQwAct(qwB), 0.0001f) && FFloatsNear(GYOfQwAct(qwA), GYOfQwAct(qwB), 0.0001f) &&
           FFloatsNear(GZOfQw(qwA), GZOfQw(qwB), 0.0001f);
}
extern qword D_002A2DD0;
extern qword D_002A2DA0;
qword SlotRestValue(XF *pxf, MATRIX4 *pmat);
void SlotRefresh(SO *pso, XF *pxf);
void SlotSetRole(SO *pso, XF *pxf, int nRole);
MATRIX4 SlotStoreValue(XF *pxf, qword qwVal);
qword QwRotRateFromEulerRate(qword qwEuler, qword qwEulerRate);
extern "C" void func_0022FA20(SO *pso);
extern "C" void func_00186A50(SO *pso, void *pv);
void UpdateXflock(SO *pso, struct XFLOCK *pxflock);
void UpdateChainLink(SO *pso, struct CHAINLINK *plink);
extern "C" float *func_00256D18(float *pg, qword qw);
struct GFTUNE
{
    float g;
};
static const SMP s_smpChaseRot = {0.015000001f, 0.0f, 0.5f};
static const GFTUNE s_gAccelRot = {0.06000001f};
static const SMP s_smpChasePos = {1.5000001f, 0.0f, 0.5f};
static const SMP s_smpChaseScale = {0.005000001f, 0.0f, 0.5f};
static const GFTUNE s_gAccelPos = {6.0000005f};
static const GFTUNE s_gAccelScale = {0.020000001f};
static const GFTUNE s_gKpPos = {0.015000001f};
static const GFTUNE s_gKdPos = {0.0025000001f};
static const GFTUNE s_gKpRot = {0.015000001f};
static const GFTUNE s_gKdRot = {0.0025000001f};
static inline qword QwMulAddX(qword qwA, qword qwB, qword qwS)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwA), "j"(qwB), "j"(qwS));
    return qw;
}
static inline qword QwSMulAddX(qword qwS, qword qwB, qword qwA)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %3, $vf0\n\tvmaddx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(qwS), "j"(qwB), "j"(qwA));
    return qw;
}
static inline qword QwMulXMulX(qword qwA, qword qwS, qword qwB, qword qwT)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(qw) : "j"(qwA), "j"(qwS), "j"(qwB), "j"(qwT));
    return qw;
}
static inline qword QwAddMulXMulX(qword qwA, qword qwB, qword qwC, qword qwS)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %4x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(qw) : "j"(qwA), "j"(qwB), "j"(qwC), "j"(qwS));
    return qw;
}
static inline qword QwMulXAddMulX(qword qwA, qword qwB, qword qwS)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %3x\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwA), "j"(qwB), "j"(qwS));
    return qw;
}
static inline qword QwDivX(qword qwV, qword qwS)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(qw) : "j"(qwV), "j"(qwS));
    return qw;
}
static inline qword QwZero()
{
    qword qw;
    __asm__("vsub.xyzw %0, $vf0, $vf0" : "=j"(qw));
    return qw;
}
static inline qword QwMulX(qword qwA, qword qwS)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwA), "j"(qwS));
    return qw;
}
static inline qword QwXMul(qword qwS, qword qwA)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(qwS), "j"(qwA));
    return qw;
}
static inline qword QwMulXMulX_P2134(qword q1, qword q2, qword q3, qword q4)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %3, %4x" : "=j"(qw) : "j"(q2), "j"(q1), "j"(q3), "j"(q4));
    return qw;
}
static inline qword QwSlotValue(XF *pxf)
{
    qword qw;
    switch (pxf->nPhase)
    {
        case 0:
            qw = SlotRestValue(pxf, &pxf->mat);
            break;
        case 1:
        {
            VECTOR4 vec;
            HermiteVec4(pxf->gUnk84 - pxf->gUnk80, *pxf->pgUnk88 - pxf->gUnk80, *pxf->pgUnk8c, pxf->qwRest, pxf->qwUnkD0, pxf->qwRestUnkE0, pxf->qwUnkF0, &vec, 0, 0);
            qw = *(qword *)&vec;
            break;
        }
        case 2:
        case 3:
        case 4:
            qw = pxf->qwRest;
            break;
        case 5:
            qw = DecomposeRotateMatrixEuler((MATRIX3 *)&pxf->qwRest);
            break;
        default:
            qw = D_002A2DD0;
            break;
    }
    return qw;
}
static inline qword QwHermitePos(XF *pxf)
{
    VECTOR4 vec;
    HermiteVec4(pxf->gUnk84 - pxf->gUnk80, *pxf->pgUnk88 - pxf->gUnk80, *pxf->pgUnk8c, pxf->qwRest, pxf->qwUnkD0, pxf->qwRestUnkE0, pxf->qwUnkF0, &vec, 0, 0);
    return *(qword *)&vec;
}
static inline qword QwHermiteVel(XF *pxf)
{
    VECTOR4 vec;
    HermiteVec4(pxf->gUnk84 - pxf->gUnk80, *pxf->pgUnk88 - pxf->gUnk80, *pxf->pgUnk8c, pxf->qwRest, pxf->qwUnkD0, pxf->qwRestUnkE0, pxf->qwUnkF0, 0, &vec, 0);
    return *(qword *)&vec;
}
// The value / rate switches of an XF's actuator, open-coded (the game does not use a helper; see SlotCurrentMatrix in the personal xf.c).
#define SLOT_VALUE_SWITCH(pxf, qw) \
    switch ((pxf)->nPhase) \
    { \
        case 0: \
            qw = SlotRestValue(pxf, &(pxf)->mat); \
            break; \
        case 1: \
            qw = QwHermitePos(pxf); \
            break; \
        case 2: \
        case 3: \
        case 4: \
            qw = (pxf)->qwRest; \
            break; \
        case 5: \
            qw = DecomposeRotateMatrixEuler((MATRIX3 *)&(pxf)->qwRest); \
            break; \
        default: \
            qw = D_002A2DD0; \
            break; \
    }
#define SLOT_RATE_SWITCH(pxf, qw, qwVel) \
    switch ((pxf)->nPhase) \
    { \
        case 3: \
        case 4: \
            qwVel = (pxf)->qwUnkD0; \
            break; \
        case 1: \
            qwVel = QwHermiteVel(pxf); \
            break; \
        case 0: \
        case 2: \
        case 5: \
        default: \
            qwVel = qw = D_002A2DD0; \
            break; \
    }
// An opaque copy (empty asm, "=r" tied to its input): cse cannot merge the two pseudos. The default arm of the position chase needs it so the arms of its rate
// switch are plain $v0 loads into a short-lived local with ONE shared spill store (T1641; without it each arm spills its own and the Hermite arm picks $v1).
static inline qword PassQ(qword x){qword r;__asm__("" : "=r"(r) : "0"(x));return r;}
static inline void ClearXyz(VECTOR4 *pvec)
{
    pvec->z = 0.0f;
    pvec->y = 0.0f;
    pvec->x = 0.0f;
}
static inline void TurnSlotMat(XF *pxf, MATRIX3 *pmatFrom, qword qwTurn)
{
    MATRIX3 matNew;
    matNew = LoadRotateMatrix(qwTurn) * *pmatFrom;
    NormalizeRotateMatrix3(&matNew);
    *(MATRIX3 *)&pxf->mat = matNew;
    pxf->mat.mat[2][3] = 0.0f;
    pxf->mat.mat[1][3] = 0.0f;
    pxf->mat.mat[0][3] = 0.0f;
}
MATRIX3 LoadRotateMatrixRad(float rad, qword qwAxis);
void UpdateSoSlots(SO *pso) __asm__("func_0022DE88");
void UpdateSoSlots(SO *pso)
{
    int fMoving;
    int ixf;
    int fIdle = (pso->psoParent == 0 || !(*(uint *)pso->psoParent->grfUnk40 & 4));
    int fSnap = *(uint *)pso->grfUnk40 & 0x10;
    for (ixf = 0; ixf < pso->cxf; ixf++)
    {
        XF *pxf = &pso->axf[ixf];
        SlotRefresh(pso, pxf);
        switch (pxf->nRole)
        {
            case 0:
                break;
            case 1:
            case 2:
            {
                switch (pxf->xfk)
                {
                case 1:
                {
                    float dt = D_002DDED0.dt;
                    qword qwV0 = *(qword *)&pxf->vUnkB0;
                    VU_FLOAT vfDt = VuFloat(dt);
                    qword qwF0 = *(qword *)&pxf->qwUnkF0;
                    qword qwVel = QwMulAddX(qwV0, qwF0, vfDt.qw);
                    fMoving = 0;
                    if (!(FQwNearXyz(D_002A2DD0, qwVel) && FQwNearXyz(D_002A2DD0, *(qword *)&pxf->vUnkB0)))
                        fMoving = 1;
                    if (fMoving)
                    {
                        fIdle = 0;
                        qword qwPos0 = *(qword *)pxf->mat.mat[3];
                        VU_FLOAT vfHalf = VuFloat(dt * 0.5f);
                        qword qwSum = QwAdd(qwVel, *(qword *)&pxf->vUnkB0);
                        qword qwPos = QwMulAddX(qwPos0, qwSum, vfHalf.qw);
                        func_00256D18(pxf->mat.mat[3], qwPos);
                        *(qword *)&pxf->vUnkB0 = qwVel;
                        pxf->mat.mat[3][3] = 1.0f;
                        pxf->nStampWorld = ++D_003DD490.nStampXf;
                    }
                    else
                    {
                        ClearXyz(&pxf->vUnkB0);
                        if (pxf->nRole != 2)
                            SlotSetRole(pso, pxf, 3);
                        else
                            fIdle = 0;
                    }
                    break;
                }
                case 6:
                {
                    float dt = D_002DDED0.dt;
                    qword qwV0 = *(qword *)&pxf->vUnkB0;
                    VU_FLOAT vfDt = VuFloat(dt);
                    qword qwF0 = *(qword *)&pxf->qwUnkF0;
                    qword qwVel = QwMulAddX(qwV0, qwF0, vfDt.qw);
                    fMoving = 0;
                    if (!(FQwNearXyz(D_002A2DD0, qwVel) && FQwNearXyz(D_002A2DD0, *(qword *)&pxf->vUnkB0)))
                        fMoving = 1;
                    if (fMoving)
                    {
                        fIdle = 0;
                        VU_FLOAT vfHalf = VuFloat(dt * 0.5f);
                        qword qwSum = QwAdd(*(qword *)&pxf->vUnkB0, qwVel);
                        TurnSlotMat(pxf, (MATRIX3 *)&pxf->mat, QwMulX(qwSum, vfHalf.qw));
                        *(qword *)&pxf->vUnkB0 = qwVel;
                        pxf->nStampWorld = ++D_003DD490.nStampXf;
                    }
                    else
                    {
                        ClearXyz(&pxf->vUnkB0);
                        if (pxf->nRole != 2)
                            SlotSetRole(pso, pxf, 3);
                        else
                            fIdle = 0;
                    }
                    break;
                }
                }
                break;
            }
            case 0x14:
                if (pxf->xfk == 6)
                {
                    fIdle = 0;
                    float gAngle = RadNormalize(*(float *)&pxf->qwRestUnkE0 + D_002DDED0.dt * *(float *)&pxf->qwUnkD0);
                    *(float *)&pxf->qwRestUnkE0 = gAngle;
                    MATRIX3 matRot = LoadRotateMatrixRad(gAngle, pxf->qwRest);
                    *(MATRIX3 *)&pxf->mat = matRot;
                    pxf->mat.mat[1][3] = 0.0f;
                    pxf->mat.mat[2][3] = 0.0f;
                    pxf->mat.mat[0][3] = 0.0f;
                    pxf->nStampWorld = ++D_003DD490.nStampXf;
                }
                break;
            case 4:
                pxf->mat.mat[3][0] = -pxf->pxfRef->mat.mat[3][0];
                pxf->mat.mat[3][1] = -pxf->pxfRef->mat.mat[3][1];
                pxf->mat.mat[3][2] = -pxf->pxfRef->mat.mat[3][2];
                pxf->nStampWorld = pxf->pxfRef->nStampWorld;
                break;
            case 5:
            case 6:
            {
                if (pxf->nPhase == 5)
                {
                    MATRIX3 *pmatTarget = (MATRIX3 *)&pxf->qwRest;
                    *(MATRIX3 *)&pxf->mat = *pmatTarget;
                    pxf->mat.mat[1][3] = 0.0f;
                    pxf->mat.mat[2][3] = 0.0f;
                    pxf->mat.mat[0][3] = 0.0f;
                }
                else
                {
                    qword qwVal = QwSlotValue(pxf);
                    pxf->mat = SlotStoreValue(pxf, qwVal);
                }
                pxf->nStampWorld = ++D_003DD490.nStampXf;
                if (pxf->nRole == 6 && pxf->nPhase == 2)
                    SlotSetRole(pso, pxf, 10);
            }
            case 0x15:
                fIdle = 0;
                break;
            case 7:
            case 8:
            case 9:
            case 0xb:
            {
                switch (pxf->xfk)
                {
                case 6:
                case 7:
                {
                    MATRIX3 matOld = *(MATRIX3 *)&pxf->mat;
                    qword qwRotRate;
                    MATRIX3 matDmat;
                    MATRIX3 matNew;
                    float gAngle;
                    VECTOR4 vecAxis;
                    if (pxf->nPhase == 5)
                    {
                        MATRIX3 *pmatTarget = (MATRIX3 *)&pxf->qwRest;
                        *(MATRIX3 *)&pxf->mat = *pmatTarget;
                        pxf->mat.mat[1][3] = 0.0f;
                        pxf->mat.mat[2][3] = 0.0f;
                        pxf->mat.mat[0][3] = 0.0f;
                        qwRotRate = D_002A2DD0;
                    }
                    else
                    {
                        qword qw;
                        SLOT_VALUE_SWITCH(pxf, qw)
                        qword qwEuler = qw;
                        qword qwEulerRate;
                        SLOT_RATE_SWITCH(pxf, qw, qwEulerRate)
                        pxf->mat = SlotStoreValue(pxf, qwEuler);
                        qwRotRate = QwRotRateFromEulerRate(qwEuler, qwEulerRate);
                    }
                    matDmat = CalculateDmat(&matOld, (MATRIX3 *)&pxf->mat);
                    DecomposeRotateMatrixRad(&matDmat, &gAngle, (VECTOR *)&vecAxis);
                    float gRateAlong = GFromVf(QwDot(qwRotRate, *(qword *)&vecAxis));
                    SMP smp;
                    if (pxf->nPhase == 3)
                    {
                        smp.svFast = (float)(ushort)pxf->nUnkAc * s_smpChaseRot.svFast;
                        smp.svSlow = (float)(ushort)pxf->nUnkAc * s_smpChaseRot.svSlow;
                        smp.dtFast = s_smpChaseRot.dtFast;
                    }
                    else
                    {
                        smp.svFast = (float)(ushort)pxf->nUnkAc * s_smpChaseRot.svFast + fabsf(gRateAlong);
                        smp.svSlow = (float)(ushort)pxf->nUnkAc * s_smpChaseRot.svSlow + fabsf(gRateAlong);
                        smp.dtFast = s_smpChaseRot.dtFast;
                    }
                    float gOut;
                    float dgNext;
                    if (fSnap)
                    {
                        gOut = gAngle;
                    }
                    else
                    {
                        gOut = GSmooth(0.0f, gAngle, D_002DDED0.dt, &smp, &dgNext);
                        switch (pxf->nRole)
                        {
                            case 0xb:
                            {
                                float gMax = (float)(ushort)pxf->nUnkAc * s_gAccelRot.g;
                                qword qwNewOmega = QwSMulAddX(VuFloat(dgNext - gRateAlong).qw, *(qword *)&vecAxis, qwRotRate);
                                qword qwV0 = *(qword *)&pxf->vUnkB0;
                                qword qwDOmega = QwSub(qwNewOmega, qwV0);
                                float gNeed = GLength(qwDOmega) / D_002DDED0.dt;
                                if (gMax < gNeed)
                                {
                                    qword qwLimited = QwMulAddX(qwV0, qwDOmega, VuFloat(gMax / gNeed).qw);
                                    MATRIX3 matRot;
                                    MATRIX3 matTurn;
                                    matRot = LoadRotateMatrix(QwXMul(VuFloat(D_002DDED0.dt * 0.5f).qw, QwAdd(*(qword *)&pxf->vUnkB0, qwLimited)));
                                    matTurn = matRot * matOld;
                                    NormalizeRotateMatrix3(&matTurn);
                                    *(qword *)&pxf->vUnkB0 = qwLimited;
                                    *(MATRIX3 *)&pxf->mat = matTurn;
                                    pxf->mat.mat[2][3] = 0.0f;
                                    pxf->mat.mat[1][3] = 0.0f;
                                    pxf->mat.mat[0][3] = 0.0f;
                                    goto LChaseDone;
                                }
                                break;
                            }
                            case 9:
                            {
                                float gMin;
                                if (D_002DDED0.t < *(float *)&pxf->pxfRef)
                                    gMin = gAngle * D_002DDED0.dt / ((*(float *)&pxf->pxfRef - D_002DDED0.t) + D_002DDED0.dt);
                                else
                                    gMin = gAngle;
                                if (gOut < gMin)
                                {
                                    gOut = gMin;
                                    dgNext = gOut / D_002DDED0.dt;
                                }
                                break;
                            }
                        }
                    }
                    {
                        if (gAngle <= gOut)
                        {
                            if (pxf->nRole != 8)
                            {
                                if (pxf->nRole == 0xb)
                                {
                                    if (pxf->nPhase == 2)
                                        SlotSetRole(pso, pxf, 0xc);
                                }
                                else if (pxf->nPhase == 1)
                                    SlotSetRole(pso, pxf, 6);
                                else if (pxf->nPhase == 2)
                                    SlotSetRole(pso, pxf, 10);
                            }
                            *(qword *)&pxf->vUnkB0 = qwRotRate;
                        }
                        else
                        {
                            matNew = LoadRotateMatrixRad(gOut, *(qword *)&vecAxis) * matOld;
                            NormalizeRotateMatrix3(&matNew);
                            *(MATRIX3 *)&pxf->mat = matNew;
                            pxf->mat.mat[1][3] = 0.0f;
                            pxf->mat.mat[2][3] = 0.0f;
                            pxf->mat.mat[0][3] = 0.0f;
                            VU_FLOAT vfRate = VuFloat(dgNext - gRateAlong);
                            *(qword *)&pxf->vUnkB0 = QwMulAddX(qwRotRate, *(qword *)&vecAxis, vfRate.qw);
                        }
                    }
                    break;
                }
                default:
                {
                    qword qw;
                    SLOT_VALUE_SWITCH(pxf, qw)
                    qword qwTarget = qw;
                    qword qwTargetVel;
                    qword qwR;
                    SLOT_RATE_SWITCH(pxf, qw, qwR)
                    qwTargetVel = PassQ(qwR);
                    __asm__("" : : ); // schedule only: puts the spill store ahead of the call's argument moves (no instruction)
                    qword qwCur = SlotRestValue(pxf, &pxf->mat);
                    qword qwDir = QwSub(qwTarget, qwCur);
                    float gDist = GXOfQwAct(QwLength(qwDir));
                    if (gDist > 0.0001f)
                    {
                        qwDir = QwDivX(qwDir, VuFloat(gDist).qw);
                    }
                    else
                    {
                        qwDir = D_002A2DA0;
                    }
                    float gVelAlong = GFromVf(QwDot(qwTargetVel, qwDir));
                    SMP smp;
                    if (pxf->xfk < 13)
                    {
                        if (pxf->xfk >= 11)
                        {
                            smp.svFast = (float)(ushort)pxf->nUnkAc * s_smpChaseScale.svFast + fabsf(gVelAlong);
                            smp.svSlow = (float)(ushort)pxf->nUnkAc * s_smpChaseScale.svSlow + fabsf(gVelAlong);
                            smp.dtFast = s_smpChaseScale.dtFast;
                        }
                        else
                        {
                            smp.svFast = (float)(ushort)pxf->nUnkAc * s_smpChasePos.svFast + fabsf(gVelAlong);
                            smp.svSlow = (float)(ushort)pxf->nUnkAc * s_smpChasePos.svSlow + fabsf(gVelAlong);
                            smp.dtFast = s_smpChasePos.dtFast;
                        }
                    }
                    else
                    {
                        smp.svFast = (float)(ushort)pxf->nUnkAc * s_smpChasePos.svFast + fabsf(gVelAlong);
                        smp.svSlow = (float)(ushort)pxf->nUnkAc * s_smpChasePos.svSlow + fabsf(gVelAlong);
                        smp.dtFast = s_smpChasePos.dtFast;
                    }
                    float gOut;
                    float dgNext;
                    if (fSnap)
                    {
                        gOut = gDist;
                    }
                    else
                    {
                        gOut = GSmooth(0.0f, gDist, D_002DDED0.dt, &smp, &dgNext);
                        switch (pxf->nRole)
                        {
                            case 0xb:
                            {
                                VU_FLOAT vfRate = VuFloat(dgNext - gVelAlong);
                                qword qwDV = QwMulAddX(qwTargetVel, qwDir, vfRate.qw);
                                qwDV = QwSub(qwDV, *(qword *)&pxf->vUnkB0);
                                float gNeed = GLength(qwDV) / D_002DDED0.dt;
                                float gMax;
                                if (pxf->xfk < 13)
                                {
                                    if (pxf->xfk >= 11)
                                        gMax = (float)(ushort)pxf->nUnkAc * s_gAccelScale.g;
                                    else
                                        gMax = (float)(ushort)pxf->nUnkAc * s_gAccelPos.g;
                                }
                                else
                                    gMax = (float)(ushort)pxf->nUnkAc * s_gAccelPos.g;
                                if (gMax < gNeed)
                                {
                                    qword qwV0 = *(qword *)&pxf->vUnkB0;
                                    VU_FLOAT vfLimit = VuFloat(gMax / gNeed);
                                    VU_FLOAT vfHalf = VuFloat(D_002DDED0.dt * 0.5f);
                                    qwDV = QwMulAddX(qwV0, qwDV, vfLimit.qw);
                                    qword qwSum = QwAdd(*(qword *)&pxf->vUnkB0, qwDV);
                                    *(qword *)&pxf->vUnkB0 = qwDV;
                                    pxf->mat = SlotStoreValue(pxf, QwMulAddX(qwCur, qwSum, vfHalf.qw));
                                    goto LChaseDone;
                                }
                                break;
                            }
                            case 9:
                            {
                                float gMin;
                                if (D_002DDED0.t < *(float *)&pxf->pxfRef)
                                    gMin = gDist * D_002DDED0.dt / ((*(float *)&pxf->pxfRef - D_002DDED0.t) + D_002DDED0.dt);
                                else
                                    gMin = gDist;
                                if (gOut < gMin)
                                {
                                    gOut = gMin;
                                    dgNext = gOut / D_002DDED0.dt;
                                }
                                break;
                            }
                        }
                    }
                    {
                        VU_FLOAT vfOut = VuFloat(gOut);
                        pxf->mat = SlotStoreValue(pxf, QwMulAddX(qwCur, qwDir, vfOut.qw));
                        if (gDist <= gOut)
                        {
                            if (pxf->nRole != 8)
                            {
                                if (pxf->nPhase == 1)
                                    SlotSetRole(pso, pxf, 6);
                                else if (pxf->nPhase == 2)
                                    SlotSetRole(pso, pxf, 10);
                            }
                            *(qword *)&pxf->vUnkB0 = qwTargetVel;
                        }
                        else
                        {
                            VU_FLOAT vfRate = VuFloat(dgNext - gVelAlong);
                            *(qword *)&pxf->vUnkB0 = QwMulAddX(qwTargetVel, qwDir, vfRate.qw);
                        }
                    }
                    break;
                }
                }
            LChaseDone:
                fIdle = 0;
                pxf->nStampWorld = ++D_003DD490.nStampXf;
                break;
            }
            case 0xd:
                func_00186A50(pso, pxf->pxfRef);
                break;
            case 0xf:
                UpdateXflock(pso, (struct XFLOCK *)pxf->pxfRef);
                break;
            case 0x10:
                UpdateChainLink(pso, (struct CHAINLINK *)pxf->pxfRef);
                break;
            case 0x11:
            {
                VECTOR4 vecPos;
                VECTOR4 vecVel;
                if (*(float *)&pxf->pxfRef <= D_002DDED0.t)
                {
                    *(qword *)&vecPos = pxf->qwRest;
                    ClearXyz(&vecVel);
                }
                else
                {
                    HermiteVec4((*(float *)&pxf->pxfRef - D_002DDED0.t) + D_002DDED0.dt, D_002DDED0.dt, 1.0f, *(qword *)pxf->mat.mat[3], *(qword *)&pxf->vUnkB0, pxf->qwRest, pxf->qwUnkD0, &vecPos, &vecVel, 0);
                }
                fIdle = 0;
                func_00256D18(pxf->mat.mat[3], *(qword *)&vecPos);
                pxf->mat.mat[3][3] = 1.0f;
                *(qword *)&pxf->vUnkB0 = *(qword *)&vecVel;
                pxf->nStampWorld = ++D_003DD490.nStampXf;
                break;
            }
            case 0x12:
            {
                switch (pxf->xfk)
                {
                case 1:
                {
                    qword qw;
                    SLOT_VALUE_SWITCH(pxf, qw)
                    qword qwTarget = qw;
                    qword qwTargetVel;
                    SLOT_RATE_SWITCH(pxf, qw, qwTargetVel)
                    float gKp = (float)(ushort)pxf->nUnkAc * s_gKpPos.g;
                    float gKd = (float)(ushort)pxf->nUnkAe * s_gKdPos.g;
                    float dt = D_002DDED0.dt;
                    qword qwV0 = *(qword *)&pxf->vUnkB0;
                    qword qwDV = QwSub(qwTargetVel, qwV0);
                    qword qwDPos = QwSub(qwTarget, *(qword *)pxf->mat.mat[3]);
                    qword qwAccel = QwMulXMulX(qwDPos, VuFloat(gKp).qw, qwDV, VuFloat(gKd).qw);
                    qword qwV1 = QwMulAddX(qwV0, qwAccel, VuFloat(dt).qw);
                    int fSettled = 0;
                    if (pxf->nPhase == 2)
                    {
                        if (GFromVf(QwDot(qwV1, qwV1)) < 25.0f)
                        {
                            if (GFromVf(QwDot(qwDPos, qwDPos)) < 4.0f)
                                fSettled = 1;
                        }
                    }
                    if (fSettled)
                    {
                        qwV1 = QwZero();
                        SlotSetRole(pso, pxf, 0x13);
                    }
                    qword qwPos0 = *(qword *)pxf->mat.mat[3];
                    func_00256D18(pxf->mat.mat[3], QwAddMulXMulX(qwPos0, qwV0, qwV1, VuFloat(dt * 0.5f).qw));
                    *(qword *)&pxf->vUnkB0 = qwV1;
                    pxf->mat.mat[3][3] = 1.0f;
                    break;
                }
                case 6:
                {
                    float dt = D_002DDED0.dt;
                    MATRIX3 matOld = *(MATRIX3 *)&pxf->mat;
                    MATRIX3 matTarget;
                    MATRIX3 matDmat;
                    MATRIX3 matNew;
                    VECTOR4 vecAxis;
                    float gAngle;
                    qword qwRate;
                    qword qwOmega0 = *(qword *)&pxf->vUnkB0;
                    if (pxf->nPhase == 5)
                    {
                        MATRIX3 *pmatTarget = (MATRIX3 *)&pxf->qwRest;
                        matTarget = *pmatTarget;
                        qwRate = D_002A2DD0;
                    }
                    else
                    {
                        qword qw;
                        SLOT_VALUE_SWITCH(pxf, qw)
                        qword qwEuler = qw;
                        qword qwEulerRate;
                        SLOT_RATE_SWITCH(pxf, qw, qwEulerRate)
                        MATRIX4 matStored = SlotStoreValue(pxf, qwEuler);
                        MATRIX3 *pmatTarget = (MATRIX3 *)&matStored;
                        matTarget = *pmatTarget;
                        qwRate = QwRotRateFromEulerRate(qwEuler, qwEulerRate);
                    }
                    matDmat = CalculateDmat((MATRIX3 *)&pxf->mat, &matTarget);
                    DecomposeRotateMatrixRad(&matDmat, &gAngle, (VECTOR *)&vecAxis);
                    float gKp = (float)(ushort)pxf->nUnkAc * s_gKpRot.g;
                    float gKd = (float)(ushort)pxf->nUnkAe * s_gKdRot.g;
                    qword qwDV = QwSub(qwRate, qwOmega0);
                    qword qwAccel = QwMulXMulX_P2134(*(qword *)&vecAxis, VuFloat(gKp * gAngle).qw, qwDV, VuFloat(gKd).qw);
                    qword qwOmega1 = QwMulAddX(qwOmega0, qwAccel, VuFloat(dt).qw);
                    int fSettled = 0;
                    if (pxf->nPhase == 2)
                    {
                        if (GFromVf(QwDot(qwOmega1, qwOmega1)) < 0.0025000001f && gAngle < 0.01f)
                            fSettled = 1;
                    }
                    if (fSettled)
                    {
                        qwOmega1 = QwZero();
                        SlotSetRole(pso, pxf, 0x13);
                    }
                    qword qwTurn = QwMulXAddMulX(qwOmega0, qwOmega1, VuFloat(dt * 0.5f).qw);
                    matNew = LoadRotateMatrix(qwTurn) * matOld;
                    NormalizeRotateMatrix3(&matNew);
                    *(MATRIX3 *)&pxf->mat = matNew;
                    pxf->mat.mat[1][3] = 0.0f;
                    pxf->mat.mat[2][3] = 0.0f;
                    pxf->mat.mat[0][3] = 0.0f;
                    *(qword *)&pxf->vUnkB0 = qwOmega1;
                    break;
                }
                }
                fIdle = 0;
                pxf->nStampWorld = ++D_003DD490.nStampXf;
                break;
            }
        }
        XF *pxfParent = pxf->pxfParent;
        if (pxfParent != 0)
        {
            // matWorld = parent->matWorld * mat through VU0 microprogram 0x60 (VU0 mem: parent rows in vf1-vf4, ours in vf5-vf8, result back in vf5-vf8).
            // The loads and vcallms are one asm WITHOUT outputs: a volatile asm with no outputs makes cse forget everything, so the stamp
            // compare below reads pxf->pxfParent again and gcse keeps the game's copy of the parent pointer ($a0).
            MATRIX4 *pmatParent = &pxfParent->matWorld;
            __asm__ __volatile__("lqc2 vf1, 0x40(%0)\n\tlqc2 vf4, 0x30(%1)\n\tlqc2 vf2, 0x10(%1)\n\tlqc2 vf3, 0x20(%1)\n\t"
                                 "lqc2 vf5, 0x0(%2)\n\tlqc2 vf6, 0x10(%2)\n\tlqc2 vf7, 0x20(%2)\n\tlqc2 vf8, 0x30(%2)\n\tvcallms 0x60"
                                 : : "r"(pxfParent), "r"(pmatParent), "r"(pxf));
            int nStamp = pxf->nStampWorld;
            if (nStamp <= pxf->pxfParent->nStampWorld)
                nStamp = pxf->pxfParent->nStampWorld;
            pxf->nStampWorld = nStamp;
            // cfc2.i waits for the microprogram; its four "=j" outputs are the result rows still sitting in vf5-vf8 (vf1-vf4 are clobbered,
            // so the allocator hands out vf5 and up). Row 0 is the one pin: without it the row stored twice (stack temp and matWorld) lands in vf8.
            int nWait;
            register qword qwRow0 __asm__("vf5"); // pin: result row 0 is the microprogram's vf5
            qword qwRow1, qwRow2, qwRow3;
            __asm__ __volatile__("cfc2.i %0, $vi0" : "=r"(nWait), "=j"(qwRow0), "=j"(qwRow1), "=j"(qwRow2), "=j"(qwRow3) : : "vf1", "vf2", "vf3", "vf4");
            MATRIX4 matWorldNew;
            *(qword *)matWorldNew.mat[0] = qwRow0;
            *(qword *)matWorldNew.mat[1] = qwRow1;
            *(qword *)matWorldNew.mat[2] = qwRow2;
            *(qword *)matWorldNew.mat[3] = qwRow3;
            pxf->matWorld = matWorldNew;
        }
        else
        {
            pxf->matWorld = pxf->mat;
        }
    }
    if (fIdle)
    {
        if (!(pso->grfUnk40 & 1))
        {
            *(int *)pso->grfUnk40 &= ~4;
        }
        else
        {
            pso->grfUnk40 &= ~4;
        }
    }
    if (fSnap)
    {
        if (!(pso->grfUnk40 & 1))
        {
            *(int *)pso->grfUnk40 &= ~0x10;
        }
        else
        {
            pso->grfUnk40 &= ~0x10;
        }
    }
    func_0022FA20(pso);
}
