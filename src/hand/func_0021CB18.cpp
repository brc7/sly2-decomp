// func_0021CB18: ported from Lilly's personal Sly 2 decomp (src/P2/chain.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DA0 (D_002A2DA0), D_002DDED0 (g_clock), D_003DD490 (g_sw)
// 0x0021CB18 UpdateChainLink__FP2SOP9CHAINLINK (our name; role 0x10 of UpdateSoSlots 0x22DE88)
// Runs for every link of a chain every frame. The first call of a frame (g_clock.t != tLast) simulates the links from this one to the end:
// the start point is the anchor (link 0: the XF's world position; its velocity is the finite difference over dt, or the XF's own velocity
// (0x22D348) when update flag 0x10 is set) or the tip of the link before (earlier links are re-read from their XFs first). Then cstep steps
// of dt / cstep: per link, the segment is turned back when it bends more than radBendMax against the one before; the tip velocity gets damping
// (gDamp, gDampRel against the point it hangs from), a bend pull (gBend * angle, along normal x segment) and gravity; the tip moves, is put back
// at the segment's rest length from the base (unit direction, or (1,0,0) for a zero segment) and loses the stretch part of its velocity.
// Every call then turns its XF so that its rest segment points at the simulated tip (BuildRotateVectorsMatrix 0x11BA08), in the parent's frame.
// Matching notes: the anchor copy needs the flag read between `posAnchor` and `qwPos = posAnchor` (else cse swaps the copy and the anchor is
// reloaded from the stack); `MATRIX3 mat` at loop-body scope keeps the game's slot for the VU_FLOAT homes (sp+0x50); the default unit vector is a
// plain qword copy in the then-arm (asm can't sit in the bc1t delay slot); gravity is read as `*(qword *)&` so it stays behind the VU_FLOAT homes.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/sw.h"
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
inline qword QwRotateVec(MATRIX3 *pmat, qword qwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(qwVec));
    return qwResult;
}
MATRIX3 BuildRotateVectorsMatrix(qword vec1, qword vec2);
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
extern CLOCK D_002DDED0;
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
qword ConvertXfPos(XF *pxfFrom, XF *pxfTo, qword qwPos);
struct SLOT
{
      SLOT *pslotNext;
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
struct CHAINLINK
{
      MATRIX3 matRest;
      qword qwSeg;
      qword posTip;
      qword velTip;
      CHAIN *pchain;
      XF *pxf;
      undefined4 unk_0x68[2];
};
struct CHAIN
{
      int clink;
      CHAINLINK *alink;
      int cstep;
      float gBend;
      float gDamp;
      float gDampRel;
      float radBendMax;
      float tLast;
      qword qwGravity;
      qword qwSegLast;
      qword posAnchorPrev;
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
static inline qword QwLengthUntied(qword v)
{
    qword qwSq;
    qword vfOne;
    qword qwLen;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qwSq), "=j"(vfOne) : "j"(v));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qwLen) : "j"(qwSq));
    return qwLen;
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline qword QwCross(qword a, qword b)
{
    qword qw;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(qw) : "j"(a), "j"(b));
    return qw;
}
qword func_0022D348(XF *pxfFrom, XF *pxfTo, qword qwPos);
float func_0011E4B0(qword qwVec0, qword qwVec1, qword *pqwNormal);
MATRIX3 LoadRotateMatrixRad(float rad, qword normal);
extern qword D_002A2DA0;
static inline qword QwAddScaledA(qword qwBase, qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw));
    return qw;
}
static inline qword QwDivVf(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qw) : "j"(g.qw), "j"(qwVec));
    return qw;
}
static inline qword QwScaleX(qword qwVec, qword qwScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(qwScale));
    return qw;
}
static inline qword QwAddScaledX(qword qwBase, qword qwVec, qword qwScale)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(qwScale));
    return qw;
}
static inline qword QwChainVel(qword qwVel, VU_FLOAT gA, qword qwVelRel, VU_FLOAT gB, qword qwPerp, VU_FLOAT gK, qword qwGravity, VU_FLOAT gDt)
{
    qword qw;
    __asm__("vmulax.xyzw $ACC, %1, %2x\n\tvmaddax.xyzw $ACC, %3, %4x\n\tvmaddax.xyzw $ACC, %5, %6x\n\tvmaddx.xyzw %0, %7, %8x"
            : "=j"(qw) : "j"(qwVel), "j"(gA.qw), "j"(qwVelRel), "j"(gB.qw), "j"(qwPerp), "j"(gK.qw), "j"(qwGravity), "j"(gDt.qw));
    return qw;
}
static inline float GMaxOf(float a, float b)
{
    return (a > b) ? a : b;
}
void UpdateChainLink(SO *pso, CHAINLINK *plink) __asm__("func_0021CB18");
void UpdateChainLink(SO *pso, CHAINLINK *plink)
{
    CHAIN *pchain = plink->pchain;
    XF *pxf = plink->pxf;
    XF *pxfParent = pxf->pxfParent;
    VECTOR4 vecVel;
    if (D_002DDED0.t != pchain->tLast)
    {
        qword qwPos;
        qword qwDirPrev;
        int i;
        CHAINLINK *plinkCur;
        if (plink == pchain->alink)
        {
            qword posAnchor = *(qword *)pxf->matWorld.mat[3];
            uint grf = *(uint *)pso->grfUnk40;
            qwPos = posAnchor;
            if (grf & 0x10)
            {
                *(qword *)&vecVel = func_0022D348(pxfParent, 0, posAnchor);
            }
            else
            {
                *(qword *)&vecVel = QwDivVf(QwSub(qwPos, pchain->posAnchorPrev), VuFloatFromMemWord(*(int *)&D_002DDED0.dt));
            }
            pchain->posAnchorPrev = posAnchor;
            MATRIX3 matWorld3 = *(MATRIX3 *)&pxfParent->matWorld * plink->matRest;
            qwDirPrev = QwRotateVec(&matWorld3, plink->qwSeg);
        }
        else
        {
            for (i = 0; i < plink - pchain->alink; i++)
            {
                plinkCur = &pchain->alink[i];
                plinkCur->posTip = ConvertXfPos(plinkCur->pxf, 0, plinkCur->qwSeg);
                plinkCur->velTip = func_0022D348(plinkCur->pxf, 0, plinkCur->posTip);
            }
            *(qword *)&vecVel = plink[-1].velTip;
            qword *pqwPosPrev = &plink[-1].posTip;
            qwPos = *pqwPosPrev;
            if (i >= 2)
            {
                qword *pqwPosPrev2 = &plink[-2].posTip;
                qwDirPrev = QwSub(qwPos, *pqwPosPrev2);
            }
            else
            {
                qwDirPrev = QwSub(qwPos, *(qword *)pxfParent->matWorld.mat[3]);
            }
        }
        float dtStep = D_002DDED0.dt / pchain->cstep;
        int istep;
        for (istep = 0; istep < pchain->cstep; istep++)
        {
            int j = plink - pchain->alink;
            float tOff = (istep - pchain->cstep + 1) * dtStep;
            qword posBase = QwAddScaledA(qwPos, *(qword *)&vecVel, VuFloat(tOff));
            qword velBase = *(qword *)&vecVel;
            qword qwDPrev = qwDirPrev;
            for (; j < pchain->clink; j++)
            {
                VECTOR4 vecDir;
                MATRIX3 mat;
                plinkCur = &pchain->alink[j];
                qword qwTip = plinkCur->posTip;
                qword qwVelTip = plinkCur->velTip;
                qword qwD = QwSub(qwTip, posBase);
                qword qwVelRel = QwSub(qwVelTip, velBase);
                float rad = func_0011E4B0(qwD, qwDPrev, (qword *)&vecDir);
                if (pchain->radBendMax < rad)
                {
                    mat = LoadRotateMatrixRad(rad - pchain->radBendMax, *(qword *)&vecDir);
                    qwD = QwRotateVec(&mat, qwD);
                    qwTip = QwAdd(posBase, qwD);
                }
                qword qwDefault = D_002A2DA0;
                float gA = GMaxOf(-pchain->gDamp * dtStep, -1.0f) + 1.0f;
                float gB = GMaxOf(-pchain->gDampRel * dtStep, -1.0f);
                float gK = pchain->gBend * rad * dtStep;
                VU_FLOAT vuA = VuFloat(gA);
                VU_FLOAT vuB = VuFloat(gB);
                VU_FLOAT vuK = VuFloat(gK);
                VU_FLOAT vuDt = VuFloat(dtStep);
                qword qwGravity = *(qword *)&pchain->qwGravity;
                qword qwPerp = *(qword *)&vecDir;
                qwPerp = QwCross(qwPerp, qwD);
                qwVelTip = QwChainVel(qwVelTip, vuA, qwVelRel, vuB, qwPerp, vuK, qwGravity, vuDt);
                qwTip = QwAddScaledA(qwTip, qwVelTip, vuDt);
                qword qwD2 = QwSub(qwTip, posBase);
                qword qwLen = QwLengthUntied(qwD2);
                qword qwV;
                if (GFromVf(qwLen) < 0.0001f)
                {
                    qwV = qwDefault;
                }
                else
                {
                    __asm__("vdiv $Q, $vf0w, %0x\n\tvwaitq" : : "j"(qwLen));
                    __asm__("vmulq.xyzw %0, %1, $Q" : "=j"(qwV) : "j"(qwD2));
                }
                *(qword *)&vecDir = qwV;
                qword qwAlong = QwDot(qwVelTip, qwV);
                qword qwAlongBase = QwDot(velBase, qwV);
                qwVelTip = QwAdd(QwScaleX(qwV, qwAlongBase), QwSub(qwVelTip, QwScaleX(qwV, qwAlong)));
                plinkCur->velTip = qwVelTip;
                qword qwSegLen = QwLength(plinkCur->qwSeg);
                qwTip = QwAddScaledX(posBase, qwV, qwSegLen);
                velBase = qwVelTip;
                plinkCur->posTip = qwTip;
                posBase = qwTip;
                qwDPrev = qwD;
            }
        }
        pchain->tLast = D_002DDED0.t;
    }
    qword qwPosParent = ConvertXfPos(0, pxfParent, plink->posTip);
    qword qwTo = QwSub(qwPosParent, *(qword *)pxf->mat.mat[3]);
    qword qwFrom = QwRotateVec(&plink->matRest, plink->qwSeg);
    MATRIX3 matRot;
    matRot = BuildRotateVectorsMatrix(qwFrom, qwTo);
    *(MATRIX3 *)&pxf->mat = matRot * plink->matRest;
    pxf->mat.mat[1][3] = 0.0f;
    pxf->mat.mat[2][3] = 0.0f;
    pxf->mat.mat[0][3] = 0.0f;
    int nStamp = ++D_003DD490.nStampXf;
    pxf->nStampWorld = nStamp;
}
