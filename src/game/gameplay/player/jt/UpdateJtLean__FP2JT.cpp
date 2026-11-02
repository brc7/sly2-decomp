// UpdateJtLean: ported from Lilly's personal Sly 2 decomp (src/P2/jtlean.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF2D0 (D_002BF2D0), D_002BF2D4 (D_002BF2D4), D_002BF2D8 (D_002BF2D8), D_002BF2DC (D_002BF2DC), D_002BF2E0 (D_002BF2E0), D_002BF300 (D_002BF300), D_002BF310 (D_002BF310), D_002BF320 (D_002BF320), D_002BF330 (D_002BF330), D_002DDED0 (g_clock), D_002BBB60 (g_lmZeroOne), D_002A2DC0 (g_normalZ), D_002A2DD0 (g_qwZero)
// 0x00189618 UpdateJtLean__FP2JT  (our name; was UpdateJtLean)
// The last step of the hero's per-frame update (JT::VUnk15). The three actuators at JT+0x19E4 / 0x19E8 / 0x19EC hold the lean offset of his body.
//  - nothing happens before JT::VUnk14 made them, or while they are not in their owner's chain; in the look mode they go back to zero;
//  - standing (jts 0) on an object (pxpFloor's part: the thing under his feet) or holding on to a curve (jts 7, grab record kind 0: state 3 a SHAPE he holds on at u, state 4 an object):
//    the velocity of that object at his position (GetXfVelocity) is seen from his own frame (matTarget); its horizontal speed picks how far he leans (a quadratic of speed / 1000,
//    D_002BF310, limited to 0..1: k) -> the lean vector (vx, vy) * 20 * k / |v|, z = -7.5 * sqrt(1 - k^2), z kept below what each foot's offset actuator (19F0 / 19F4) allows.
//    (re/notes/hero-movement-matched.md, T1448, has the whole reading with the constants.)
//    The first actuator (19E4) gets that; the other two (19E8 / 19EC) half of it while no animation state is set (jtas -1), else zero;
//  - holding a SHAPE the curve also bends under him: his speed along the curve's tangent (ahead = the tangent 0.1 s later) gives a sideways acceleration; the part of it across the
//    tangent and the vertical (SProjectVector) drives the three-clip lean blend of the grab record (ablC, weights +0x7B0 / +0x7BC / +0x7C8, pasegblC +0x7D4), smoothed with GSmoothA
//    (+0x7D8 the value, +0x7DC its rate), flipped when the record's flip (+0x4E4) is set.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
qword PosLoWorld(LO *plo);
static inline float GEvaluateClq(CLQ *pclq, float x)
{
    return pclq->g0 + x * (pclq->g1 + x * pclq->g2);
}
extern LM D_002BBB60;
float GSmoothA(float gCur, float dgCur, float gTarget, float dt, SMPA *psmpa, float *pdgNext);
float GModPositive(float gDividend, float gDivisor);
struct ACT
{
      int ack;
      SO *pso;
      XF *pxf;
      int nPriority;
      ACT *pactNext;
      int nRole;
    ACT() {}
    ACT(int ack) : ack(ack) {}
};
struct ACT1 : public ACT
{
      int nPhase;
      qword qwRest;
    ACT1() { ack = 1; nPhase = 2; }
};
struct ACT5 : public ACT
{
      ACT *pactCur;
      qword qwOffset;
    ACT5() { ack = 5; }
};
extern qword D_002A2DD0;
int FFindInOwnerChain(ACT *pact);
void SetAct1ValueKeepOffset(ACT1 *pact1, qword qwNew);
extern CLOCK D_002DDED0;
struct EDGE
{
    ushort aipos[2];
    ushort aisurf[2];
    EDGE *pedgeNext;
    EDGE *pedgeOtherNext;
};
struct SURF
{
    VECTOR normal;
    float gDot;
    EDGE *pedge;
    EDGE *pedgeOther;
    uint grf;
    int ipos;
};
struct BSP
{
    BSP *pbspNeg;
    BSP *pbspPos;
    SURF *psurf;
};
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
    virtual void GetPos(float sParam, JT *pjt, VECTOR *ppos, float *pradTarget);
};
struct XPLIST
{
      XP *pxpFirst;
      XPLIST *pxplistNext;
      XP **ppxpHead;
      SO *psoOther;
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
void ReblendAsegbl(ASEGBL *pasegbl);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
bool FJtModeActive(STEPPLAYER *pstepplayer);
struct XPD
{
      SOPART *ppart;
      XF *pxf;
      SO *psoRoot;
      SO *psoLeaf;
      qword pos;
      qword posLeaf;
      qword v;
};
struct XP
{
      XPD axpd[2];
      union { qword qwNormal; VECTOR4 normal; };
      float gDist;
      float mu;
      float elas;
      int xpk;
      union { void *pv0; float sfMin; };
      union { void *pv1; float sfMax; };
      undefined4 unk_0xa8;
      XP *pxpNext;
};
extern "C" {
float PeekFirstB(SHAPE *pshape);
}
extern "C" {
float PeekLastB(SHAPE *pshape);
}
void GetShapePosDirAtU(SHAPE *pshape, float u, qword *ppos, qword *pdir);
void GetShapePosDirAtS(SHAPE *pshape, float s, qword *ppos, qword *pdir);
struct CID12 : public SHAPE, public GRABSPOT
{
      DLE dle;
      int nUnk1bc;
};
struct CID19 : public TARGET, public GRABSPOT
{
      int nHangKind;
      undefined4 unk_0x128;
      int sfxidHangStart;
      int sfxidHangEnd;
      float gUnk134;
      LO *ploUnk138;
      undefined4 unk_0x13c;
};
struct CID100 : public SO
{
      int nState;
      undefined4 unk_0x154;
      int nUnk158;
      int idGadget;
      undefined4 unk_0x160[3];
      LO *ploUnk16c;
      undefined4 unk_0x170[16];
};
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
static inline VU_FLOAT VuFloatFromMemWord(int n)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "rV"(n));
    return v;
}
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
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline qword QwLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLengthXY(qword v)
{
    return GFromVf(QwLengthXY(v));
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline qword QwLengthDot(qword v)
{
    qword qw = QwDot(v, v);
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline qword QwNormalizeDot(qword v)
{
    qword qw = QwLengthDot(v);
    qword qwNormal;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwNormal) : "j"(qw), "j"(v));
    return qwNormal;
}
static inline qword QwNormalize(qword v)
{
    qword qw = QwLength(v);
    qword qwNormal;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qwNormal) : "j"(qw), "j"(v));
    return qwNormal;
}
static inline qword QwCross(qword a, qword b)
{
    qword qw;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(qw) : "j"(a), "j"(b));
    return qw;
}
qword GetXfVelocity(XF *pxfFrom, XF *pxfTo, qword qwPos);
extern "C" float SProjectVector(qword qwVec, qword qwOnto, qword *pqwAlong, qword *pqwAcross);
extern qword D_002A2DC0;
extern float D_002BF2D0;
extern float D_002BF2D4;
extern float D_002BF2D8;
extern float D_002BF2DC;
extern float D_002BF2E0;
extern float D_002BF300;
extern CLQ D_002BF310;
extern SMPA D_002BF320;
extern CLQ D_002BF330;
static inline float GMaxOf(float a, float b)
{
    return (a > b) ? a : b;
}
static inline float GMinOf(float a, float b)
{
    return (a < b) ? a : b;
}
static inline float GLimitG(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
static inline qword QwScale(qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(gScale.qw), "j"(qwVec));
    return qw;
}
static inline qword QwDotRowsMatP(MATRIX3 *pmat, qword qwV)
{
    qword qwLocal;
    qword qwTmp0, qwTmp1, qwTmp2, qwOnes;
    __asm__("vmul.xyzw %2, %5, %8\n\tvmaxw.xyzw %1, $vf0, $vf0w\n\tvmul.xyzw %3, %6, %8\n\tvmul.xyzw %4, %7, %8\n\tvadday.x $ACC, %2, %2y\n\tvmaddz.x %0, %1, %2z\n\tvaddax.y $ACC, %3, %3x\n\tvmaddz.y %0, %1, %3z\n\tvaddax.z $ACC, %4, %4x\n\tvmaddy.z %0, %1, %4y"
            : "=j"(qwLocal), "=&j"(qwOnes), "=&j"(qwTmp0), "=&j"(qwTmp1), "=&j"(qwTmp2)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(qwV));
    return qwLocal;
}
static inline qword QwFromXyzV(float gX, float gY, float gZ)
{
    qword qwXYZ;
    int iTmp;
    __asm__("mfc1 %0, %2\n\tmfc1 %1, %3\n\tpextlw %0, %1, %0\n\tmfc1 %1, %4\n\tpcpyld %0, %1, %0"
            : "=&r"(qwXYZ), "=&r"(iTmp) : "f"(gX), "f"(gY), "f"(gZ));
    VECTOR4 vecXyz;
    *(qword *)&vecXyz = qwXYZ;
    return *(qword *)&vecXyz;
}
void UpdateJtLean(JT *pjt) __asm__("UpdateJtLean__FP2JT");
/** @sly2-annot addr=0x189618
 * Purpose: Body lean actuators x19E4/E8/EC from velocity of the floor object / curve he stands on; last step
 *   of UpdateJt
 * Info: proposed name UpdateJtLean (T1); TU jt
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void UpdateJtLean(JT *pjt)
{
    VECTOR4 vecDv;
    VECTOR4 vecPos;
    VECTOR4 vecDir;
    VECTOR4 vecLean;
    VECTOR4 vecLocal;
    *(qword *)&vecDv = D_002A2DD0;
    LO *plo = 0;
    if (!pjt->pact5Unk19e4)
        return;
    if (!FFindInOwnerChain(pjt->pact5Unk19e4))
        return;
    if (FJtModeActive(pjt))
    {
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19e4, D_002A2DD0);
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19e8, D_002A2DD0);
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19ec, D_002A2DD0);
        return;
    }
    switch (pjt->jts)
    {
    case JTS_Stand:
        if (pjt->pxpFloor)
            plo = pjt->pxpFloor->axpd[1].psoLeaf;
        break;
    case JTS_Grab:
        if (pjt->jtgrab.nKind == 0)
        {
            switch (pjt->jtgrab.nState)
            {
            case 4:
                plo = pjt->jtgrab.plo;
                *(qword *)&vecPos = PosLoWorld(plo);
                break;
            case 3:
            {
                SHAPE *pshape = (SHAPE *)pjt->jtgrab.plo;
                GetShapePosDirAtU(pshape, pjt->jtgrab.uParam, (qword *)&vecPos, (qword *)&vecDir);
                plo = pshape;
                if (((CID12 *)plo)->nUnk1bc)
                {
                    *(qword *)&vecDir = QwNormalizeDot(*(qword *)&vecDir);
                    qword qwVel = pjt->GetVelocity();
                    float gSpeed = GFromVf(QwDot(qwVel, *(qword *)&vecDir));
                    *(qword *)&vecLean = QwScale(*(qword *)&vecDir, VuFloat(gSpeed));
                    float sAhead = pjt->jtgrab.sParam + gSpeed * D_002BF2D0;
                    float sMax = PeekLastB((SHAPE *)plo);
                    if (((SHAPE *)plo)->fClosed)
                    {
                        sAhead = GModPositive(sAhead, sMax);
                    }
                    else
                    {
                        float sMin = PeekFirstB((SHAPE *)plo);
                        if (sAhead < sMin)
                            sAhead = sMin;
                        else if (sMax < sAhead)
                            sAhead = sMax;
                    }
                    GetShapePosDirAtS(pshape, sAhead, 0, (qword *)&vecDir);
                    *(qword *)&vecLocal = QwScale(QwNormalize(*(qword *)&vecDir), VuFloat(gSpeed));
                    *(qword *)&vecDv = QwSub(*(qword *)&vecLocal, *(qword *)&vecLean);
                    if (pjt->jtgrab.pasegblC)
                    {
                        VECTOR4 vecAcross;
                        *(qword *)&vecAcross = QwNormalizeDot(QwCross(D_002A2DC0, *(qword *)&vecDir));
                        float gAcross = SProjectVector(*(qword *)&vecDv, *(qword *)&vecAcross, 0, (qword *)&vecDv);
                        float g = GLimitG(gAcross / D_002BF2D4, -1.0f, 1.0f);
                        if (pjt->jtgrab.fFlip)
                            g = -g;
                        float gLean = GSmoothA(pjt->jtgrab.gLean, pjt->jtgrab.dgLean, g * (2.0f - fabsf(g)), D_002DDED0.dt, &D_002BF320, &pjt->jtgrab.dgLean);
                        pjt->jtgrab.gLean = gLean;
                        float gZero = 0.0f;
                        float gOne = 1.0f;
                        pjt->jtgrab.ablC[0].gWeight = GMaxOf(gZero, gLean);
                        pjt->jtgrab.ablC[2].gWeight = GMaxOf(gZero, -gLean);
                        pjt->jtgrab.ablC[1].gWeight = (gOne - pjt->jtgrab.ablC[0].gWeight) - pjt->jtgrab.ablC[2].gWeight;
                        ReblendAsegbl(pjt->jtgrab.pasegblC);
                    }
                    goto lComputeLean;
                }
                break;
            }
            }
        }
        break;
    }
    if (!plo)
        return;
lComputeLean:
    *(qword *)&vecDir = GetXfVelocity(plo->PxfLocal(0), 0, *(qword *)&vecPos);
    *(qword *)&vecLocal = QwDotRowsMatP(&pjt->matTarget, *(qword *)&vecDir);
    float gLen = GLengthXY(*(qword *)&vecLocal);
    if (gLen > 0.0001f)
    {
        float k = GLimitG(GEvaluateClq(&D_002BF310, GMinOf(gLen / D_002BF300, 1.0f)), D_002BBB60.gMin, D_002BBB60.gMax);
        float gX = vecLocal.x * D_002BF2D8 * k / gLen;
        float gY = vecLocal.y * D_002BF2D8 * k / gLen;
        float gR = ({ float __result; __asm__ volatile("sqrt.s %0, %1" : "=&f"(__result) : "f"( 1.0f - k * k ) : "memory"); __result; }) ;
        float gZ = D_002BF2DC * gR;
        vecLean.x = gX;
        vecLean.y = gY;
        vecLean.z = gZ;
    }
    else
    {
        vecLean.z = 0.0f;
        vecLean.y = 0.0f;
        vecLean.x = 0.0f;
    }
    float gFootL = GEvaluateClq(&D_002BF330, ((float *)pjt->pact5Unk19f0)[10]);
    float gFootR = GEvaluateClq(&D_002BF330, ((float *)pjt->pact5Unk19f4)[10]);
    float gZ = vecLean.z;
    if (gZ < gFootL && gZ < gFootR)
        gFootR = gZ;
    else if (gFootL < gFootR)
        gFootR = gFootL;
    SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19e4, QwFromXyzV(vecLean.x, vecLean.y, gFootR));
    qword qwHalf = QwScale(*(qword *)&vecLean, VuFloatFromMemWord(*(int *)&D_002BF2E0));
    if (pjt->jtas == -1)
    {
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19ec, qwHalf);
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19e8, qwHalf);
    }
    else
    {
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19ec, D_002A2DD0);
        SetAct1ValueKeepOffset((ACT1 *)pjt->pact5Unk19e8, D_002A2DD0);
    }
}
