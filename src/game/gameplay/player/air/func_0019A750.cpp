// func_0019A750: ported from Lilly's personal Sly 2 decomp (src/P2/jtair.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF8B0 (D_002BF8B0), D_002BF8C0 (D_002BF8C0), D_002BF8C8 (D_002BF8C8), D_002BF8CC (D_002BF8CC), D_002BF8D0 (D_002BF8D0), D_002BF8D4 (D_002BF8D4), D_002BF8D8 (D_002BF8D8), D_002BF8DC (D_002BF8DC), D_002BF73C (g_SheetMovementMiscDoubleJumpHeight), D_002BF738 (g_SheetMovementMiscJumpHeight)
// 0x0019A750 JtPlanLaunchVz (MATCH, T1312): the velocity the hero gets when a flight of this kind starts. It is the current velocity with its z raised:
//   z = max(z, vz of the kind), vz from g = the gravity (negative, SOPHYS::dvGravity.z, -1750) and, per kind
//   1, 7, 0xF   sqrt(2 g (zLand - zFoot - 215))      the X jump: the apex is 215 above the take-off (zLand - zFoot = *pqwUnk218.z - qwUnk230.z is 0 on flat ground, the radicand is floored at 0)
//   9, 0xE, 0xD the same with the apex 1200 / 250 / 500
//   2, 0x12, 3  the constants 700 / 900 / 800 (a launch is never slower than that upwards)
//   4           700 + 0.5 z limited to 700 (a bounce: a fall is turned into a launch of 700 at the most)
//   6           the double jump: it aims for the same landing time as a jump followed by a jump from the apex (CSolveQuadratic: the flight from the first
//               apex 215 with the speed that climbs 100 more) and gets the vz that does it from the current height; still rising: the vz that reaches 315
//   0, 5, 8, 0xA - 0xC, 0x10, 0x11: nothing (z stays)
// Returns the velocity (the callers SetVelocity it).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
qword PosLoWorld(LO *plo);
static inline float GEvaluateClq(CLQ *pclq, float x)
{
    return pclq->g0 + x * (pclq->g1 + x * pclq->g2);
}
int CSolveQuadratic(float a, float b, float c, float *ax);
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
struct KEY
{
    static void *operator new(uint cb)
    {
        SWALLOC swalloc;
        swalloc.cb = cb;
        SWALLOC *pswalloc = &swalloc;
        pswalloc->cbAlign = 0x10;
        return PvAllocSwClearImpl(pswalloc);
    }
};
struct CHN
{
      int oid : 20;
      int ixf : 10;
      uint fStripped : 1;
      uint fScalar : 1;
      KEY *pkey;
};
struct OVR
{
      int id;
      OID oid;
      KEY *pkey;
};
struct EASND
{
      int sfxid;
      OID oid;
      int sNear;
      int sFar;
      float gPitch;
      float gUnk14;
      float gUnk18;
};
struct EA17
{
      int n0;
      int n4;
      int n8;
      float gC;
};
struct EA20
{
      int n0;
      int n4;
      float g8;
};
struct EA24
{
      RAW8 a[3];
};
struct MRSGC
{
      int ib;
      int cmrsg;
      MRSG *amrsg;
};
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
static inline float GMaxOf(float a, float b)
{
    return (a > b) ? a : b;
}
static inline float GMinOf(float a, float b)
{
    return (a < b) ? a : b;
}
static inline float GLimitRange(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = GEvaluateClq(pclq, x);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    return GLimitRange(g, gMin, gMax);
}
extern float D_002BF8C8;
extern float D_002BF8CC;
extern float D_002BF8D0;
extern float D_002BF8D4;
extern float D_002BF8D8;
extern float D_002BF8DC;
extern float D_002BF738;
extern float D_002BF73C;
extern CLQ D_002BF8B0;
extern LM D_002BF8C0;
static inline float SqrtfEarly(float g)
{
    float r;
    __asm__ volatile("sqrt.s %0, %1" : "=&f"(r) : "f"(g));
    return r;
}
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind) __asm__("func_0019A750");
/** @sly2-annot addr=0x19A750
 * Purpose: Launch velocity for a flight kind: z raised to the per-kind value (215/1200/250/500 apex,
 *   700/900/800 constants, bounce, double jump solved with CSolveQuadratic)
 * Info: proposed name JtPlanLaunchVz (T1); TU air
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind)
{
    VECTOR4 pos;
    VECTOR4 vel;
    float g;
    float vzNew;
    *(qword *)&pos = PosLoWorld(pjtair->pstepplayer);
    *(qword *)&vel = pjtair->pstepplayer->GetVelocity();
    g = pjtair->pstepplayer->psophys->dvGravity.z;
    switch (nKind)
    {
        case 2:
            vzNew = D_002BF8C8;
            break;
        case 0x12:
            vzNew = D_002BF8CC;
            break;
        case 3:
            vzNew = D_002BF8D0;
            break;
        case 1:
        case 7:
        case 9:
        case 0xD:
        case 0xE:
        case 0xF:
        {
            float sApex;
            switch (nKind)
            {
                case 9:
                    sApex = D_002BF8D4;
                    break;
                case 0xE:
                    sApex = D_002BF8D8;
                    break;
                case 0xD:
                    sApex = D_002BF8DC;
                    break;
                default:
                    sApex = D_002BF738;
                    break;
            }
            vzNew = SqrtfEarly(GMaxOf((g + g) * (((VECTOR4 *)pjtair->pstepplayer->pqwUnk218)->z - ((VECTOR4 *)&pjtair->pstepplayer->qwUnk230)->z - sApex), 0.0f));
            break;
        }
        case 4:
            vzNew = GEvaluateClqLm(&D_002BF8B0, &D_002BF8C0, vel.z);
            break;
        case 6:
        {
            float vJump = SqrtfEarly(g * -2.0f * D_002BF738);
            float tApex = -vJump / g;
            float vzMin = GZOfQw(pjtair->pstepplayer->GetVelocity());
            vzMin = GMinOf(vzMin, vJump);
            float tElapsed = (vzMin - vJump) / g;
            float h = tElapsed * (vJump + g * 0.5f * tElapsed);
            float ax[4];
            float bq = SqrtfEarly(g * -2.0f * (D_002BF73C - D_002BF738));
            CSolveQuadratic(g * 0.5f, bq, D_002BF738, ax);
            float tFall = ax[1] - tApex;
            if (0.0f < vzMin)
                vzNew = SqrtfEarly(g * -2.0f * (D_002BF73C - h));
            else
            {
                float T;
                if (vzMin < -vJump)
                    h = 0.0f;
                T = tApex + tApex + tFall - tElapsed;
                vzNew = -h / T - g * 0.5f * T;
            }
            break;
        }
        default:
            vzNew = vel.z;
            break;
    }
    vel.z = GMaxOf(vel.z, vzNew);
    return *(qword *)&vel;
}
