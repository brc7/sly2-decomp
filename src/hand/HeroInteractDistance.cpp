// HeroInteractDistance: ported from Lilly's personal Sly 2 decomp (src/P2/stepplayer_raw.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BD280 (D_002BD280), D_002BD290 (D_002BD290), D_002BD2A0 (D_002BD2A0), D_002BD2B0 (D_002BD2B0), D_002BBB60 (g_lmZeroOne)
// 0x001428D8 HeroInteractDistance  (T1414, our name)
// How far an object is from the hero as the interact pickers rank it: *psDist = the xy distance |hero - plo| plus a bias, *pdz = the height difference (hero - plo)
// plus a bias (both optional). Callers: GuardEvalInteractMode 0x208E70 (plo = a guard, *pploCur = &hero->ploInteract, *pdz is tested against the guard's height window;
// HeroPickInteractGuard minimises *psDist) and the interact-target pick 0x142AF8 (three calls, *pploCur = &hero->ploLean).
// The biases are CLQs (c0 + c1 d + c2 d^2) from 0x2BD280: (1) for the current target: xy D_002BD280(d) = -200 - 0.2 d, height D_002BD290(dz) = -0.2 dz (hysteresis: the
// target already chosen looks nearer). (2) while the hero is steered (pctl) and d > 0.0001: rAway = (stick . (hero - plo)) / d with the stick vector = MakeVecFromAngle
// (radLeft, sLeft, 0) (|stick| = sLeft); xy += clamp01(D_002BD2A0(rAway) = 0.5 - 0.5 rAway) * D_002BD2B0(d) (= -500 - 0.5 d): pushing the stick toward the object (rAway
// = -sLeft) takes up to 500 + d/2 off its distance, away from it nothing, no stick half.
// NOTE as compiled the hysteresis test compares the HERO (a0) with the current target (`lw v0,0(s1); bnel s2,v0`, s2 = a0), not the object (a1): both callers pass a
// field of the hero (+0x10DC / +0x10F8) that never holds the hero himself, so bias (1) is never added by the game (it looks like a typo for plo in the original source).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
struct VU_VECTOR4
{
    qword qw;
};
qword PosLoWorld(LO *plo);
static inline float GEvaluateClq(CLQ *pclq, float x)
{
    return pclq->g0 + x * (pclq->g1 + x * pclq->g2);
}
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
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
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
extern CLQ D_002BD280;
extern CLQ D_002BD290;
extern CLQ D_002BD2A0;
extern CLQ D_002BD2B0;
extern LM D_002BBB60;
static inline VU_VECTOR4 SubPos(qword a, qword b)
{
    VU_VECTOR4 r;
    r.qw = QwSub(a, b);
    return r;
}
static inline qword QwLengthXYTwice(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %2\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v), "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GXOfQwVf(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw));
    return x;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
extern "C" qword MakeVecFromAngle(float rad, float sXY, float sZ);
extern "C" void HeroInteractDistance(STEPPLAYER *pstepplayer, LO *plo, LO **pploCur, float *psDist, float *pdz) __asm__("HeroInteractDistance");
extern "C" void HeroInteractDistance(STEPPLAYER *pstepplayer, LO *plo, LO **pploCur, float *psDist, float *pdz)
{
    float sBias = 0.0f;
    float zBias = 0.0f;
    qword posHero = PosLoWorld(pstepplayer);
    VU_VECTOR4 vecS = SubPos(posHero, PosLoWorld(plo));
    qword vec = vecS.qw;
    float dz = GZOfQw(vec);
    float sXY = GFromVf(QwLengthXYTwice(vec));
    if ((LO *)pstepplayer == *pploCur)
    {
        sBias = GEvaluateClq(&D_002BD280, sXY);
        zBias = GEvaluateClq(&D_002BD290, dz);
    }
    POCTL *pctl = pstepplayer->pctl;
    if (pctl != 0 && sXY > 0.0001f)
    {
        qword qwStick = MakeVecFromAngle(pctl->radLeft, pctl->sLeft, 0.0f);
        float rAway = GXOfQwVf(QwDot(qwStick, vec)) / sXY;
        sBias += GEvaluateClqLm(&D_002BD2A0, &D_002BBB60, rAway) * GEvaluateClq(&D_002BD2B0, sXY);
    }
    sXY += sBias;
    dz += zBias;
    if (psDist != 0)
    {
        *psDist = sXY;
    }
    if (pdz != 0)
    {
        *pdz = dz;
    }
}
