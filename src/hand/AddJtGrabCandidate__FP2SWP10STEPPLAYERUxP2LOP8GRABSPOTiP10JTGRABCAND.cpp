// AddJtGrabCandidate: ported from Lilly's personal Sly 2 decomp (src/P2/jtgrab.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF664 (D_002BF664), D_002E1E4C (D_002E1E4C), D_002E1E40 (g_SheetGangSly), D_002DDED0 (g_clock)
// 0x00194688 AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND  (MATCHES Sly 2; our name)
// Tests one grab spot against the hero (qwPosJt is the hero's position) and, when the hero may take hold of it, puts it into the
// sorted array of ccandMax candidates: by priority (higher first), then by distance, one entry per group (the nearer spot of a
// group wins). Objects the hero already holds / just let go of, spots whose mask the hero's flags do not share and objects
// farther than their own sBusy or than 1000 are skipped.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/sw.h"
typedef unsigned  size_t;
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
qword PosLoWorld(LO *plo);
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
struct BND
{
      OID oid;
      LO *plo;
      int ixf;
      XF *pxf;
      MATRIX4 matInv;
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
    virtual int FGetWrapRange(float *pasRange);
    virtual void GetClosestParam(JT *pjt, float *psParam);
    virtual void GetPos(float sParam, JT *pjt, VECTOR *ppos, float *pradTarget);
    virtual void AdjustDist(float *pgDist);
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
struct SWALLOC
{
    int cb;
    int cbAlign;
};
void *PvAllocSwClearImpl(SWALLOC *pswalloc);
void CopyAb(void *pvDst, void *pvSrc, uint cb);
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
extern "C" void *memmove(void *pvDst, const void *pvSrc, size_t cb);
extern JT *D_002E1E40;
extern int D_002E1E4C;
extern float D_002BF664;
static inline int FBeyondBusy(LO *plo, qword qwPos)
{
    qword qwDiff = QwSub(PosLoWorld(plo), qwPos);
    return plo->sBusy * plo->sBusy < GFromVf(QwDot(qwDiff, qwDiff));
}
static inline void ZeroJtGrabCand(JTGRABCAND *pcand)
{
    memset(pcand, 0, sizeof(JTGRABCAND));
}
static inline void MoveJtGrabCands(JTGRABCAND *pcandDst, JTGRABCAND *pcandSrc, size_t cb)
{
    memmove(pcandDst, pcandSrc, cb);
}
void AddJtGrabCandidate(SW *psw, STEPPLAYER *pstepplayer, qword qwPosJt, LO *plo, GRABSPOT *pspot, int ccandMax, JTGRABCAND *ajtgrabcand) __asm__("AddJtGrabCandidate__FP2SWP10STEPPLAYERUxP2LOP8GRABSPOTiP10JTGRABCAND");
void AddJtGrabCandidate(SW *psw, STEPPLAYER *pstepplayer, qword qwPosJt, LO *plo, GRABSPOT *pspot, int ccandMax, JTGRABCAND *ajtgrabcand)
{
    JTGRABCAND cand;
    if (plo->psoParent && FBeyondBusy(plo, qwPosJt))
        return;
    if (plo == pstepplayer->jtgrab.plo)
        return;
    if (plo == pstepplayer->jtair.plo)
        return;
    if (pspot->nGroup && pstepplayer->jtgrab.pspot && pspot->nGroup == pstepplayer->jtgrab.pspot->nGroup)
        return;
    if (plo == pstepplayer->jtair.ploUnk37c)
        return;
    if (plo == (LO *)pstepplayer->jtpipe.pshape)
        return;
    if (pstepplayer == (STEPPLAYER *)D_002E1E40)
    {
        if (plo == ((JT *)pstepplayer)->phndHang)
            return;
        if (plo == ((JT *)pstepplayer)->ploUnk1f18)
            return;
    }
    if (!(pstepplayer->grfUnk1000 & pspot->grfUnk1c))
        return;
    if (D_002E1E4C)
    {
        if (pstepplayer->jtgrab.plo)
            return;
        if (pstepplayer->jtpipe.pshape)
            return;
        if (pstepplayer == (STEPPLAYER *)D_002E1E40)
        {
            if (((JT *)pstepplayer)->phndHang)
                return;
            if (((JT *)pstepplayer)->ploUnk1f18)
                return;
        }
    }
    pspot->GetClosestParam((JT *)pstepplayer, &cand.sParam);
    pspot->GetPos(cand.sParam, (JT *)pstepplayer, (VECTOR *)&cand.qwPos, 0);
    cand.nPriority = pspot->nPriority;
    cand.sDist = GLength(QwSub(qwPosJt, cand.qwPos));
    pspot->AdjustDist(&cand.sDist);
    if (cand.sDist > D_002BF664)
        return;
    int i;
    if (pspot->nGroup)
    {
        for (i = 0; i < ccandMax; i++)
        {
            JTGRABCAND *pcand = &ajtgrabcand[i];
            if (!pcand->pspot)
                break;
            if (pcand->pspot->nGroup == pspot->nGroup)
            {
                if (cand.sDist < ajtgrabcand[i].sDist)
                {
                    CopyAb(&ajtgrabcand[i], &ajtgrabcand[i + 1], (ccandMax - 1 - i) * sizeof(JTGRABCAND));
                    ZeroJtGrabCand(&ajtgrabcand[ccandMax] - 1);
                    ajtgrabcand[ccandMax - 1].sDist = D_002BF664;
                }
                else
                    return;
                break;
            }
        }
    }
    i = ccandMax;
    do
    {
        JTGRABCAND *pcandPrev = &ajtgrabcand[--i];
        if (pcandPrev->nPriority > cand.nPriority || (pcandPrev->nPriority == cand.nPriority && pcandPrev->sDist < cand.sDist))
        {
            i++;
            break;
        }
    } while (i != 0);
    if (i == ccandMax)
        return;
    MoveJtGrabCands(&ajtgrabcand[i + 1], &ajtgrabcand[i], (ccandMax - i) * sizeof(JTGRABCAND) - sizeof(JTGRABCAND));
    cand.plo = plo;
    cand.pspot = pspot;
    cand.tUnk14 = D_002DDED0.t;
    cand.ploUnk10 = 0;
    cand.dsParam = 0.0f;
    ajtgrabcand[i] = cand;
}
