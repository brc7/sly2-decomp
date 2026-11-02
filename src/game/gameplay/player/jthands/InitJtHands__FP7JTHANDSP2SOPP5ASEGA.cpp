// InitJtHands: ported from Lilly's personal Sly 2 decomp (src/P2/jthands.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002C1ED0 (D_002C1ED0)
// 0x001CC5E8 InitJtHands__FP7JTHANDSP2SOPP5ASEGA
// Builds the hands record (the hero's VUnk14 and InitJtGrabRecord call it once ploHand is set): the matrix of the hand object's XF chain up to the first
// rotation XF (xfk 6) - the hand's frame in the hero's - and, for each hand that has a hold clip, two curves (class 10 SHAPEs of D_002C1ED0 points, removed
// from the world again): the path the hand object's XF (A) and the chain's top XF (B) take through the clip, sampled with PredictAsegEffect and carried to
// the hand's frame. With both clips the blend of the two (weights 1 and 0 to start with) is made.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
typedef unsigned  size_t;
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
inline void SetMatrix4Identity(MATRIX4 *pmat)
{
    memset(pmat, 0, sizeof(MATRIX4));
    pmat->mat[3][3] = 1.0f;
    pmat->mat[2][2] = 1.0f;
    pmat->mat[1][1] = 1.0f;
    pmat->mat[0][0] = 1.0f;
}
MATRIX4 operator*(const MATRIX4 &matL, const MATRIX4 &matR);
inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]),
              "j"(qwPos));
    return qwResult;
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
void *PvAllocSwImpl(SWALLOC *pswalloc);
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
void EnsureAsegBlendDynamic(ALO *palo, int cbBl, int cbl, void *abl, int cmrsgc, int *mpimrsgcib, int *mpimrsgccmrsg, ASEGBL **ppasegbl);
LO *PloNew(CID cid, SO *psoParent, OID oid, int isplice);
extern "C" void PredictAsegEffect(float tLocal, ASEG *paseg, SO *psoRoot, float svt, XF *pxfEffect, XF *pxfRef, MATRIX4 *pmatOut, int f);
extern "C" void MeasureShape(SHAPE *pshape);
extern int D_002C1ED0;
inline void *PvAllocSw(int cb, int cbAlign)
{
    SWALLOC swalloc;
    swalloc.cb = cb;
    SWALLOC *pswalloc = &swalloc;
    pswalloc->cbAlign = cbAlign;
    return PvAllocSwImpl(pswalloc);
}
void InitJtHands(JTHANDS *phands, SO *pso, ASEGA **ppasega) __asm__("InitJtHands__FP7JTHANDSP2SOPP5ASEGA");
/** @sly2-annot addr=0x1CC5E8
 * Purpose: Build hands record: hand-frame matrix, sampled A/B curves per hold clip (PredictAsegEffect), blend
 * Info: proposed name InitJtHands (T1); TU jthands
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void InitJtHands(JTHANDS *phands, SO *pso, ASEGA **ppasega)
{
    phands->pso = pso;
    phands->ppasega = ppasega;
    if (!phands->ploHand)
        return;
    MATRIX4 matHand;
    XF *pxfHand = phands->ploHand->PxfLocal(&matHand);
    phands->matChain = matHand;
    phands->pxf = pxfHand;
    while (phands->pxf->xfk != 6)
    {
        phands->matChain = phands->pxf->mat * phands->matChain;
        phands->pxf = phands->pxf->pxfParent;
    }
    int cHands = 0;
    for (int i = 0; i < 2; i++)
    {
        ASEG *paseg = phands->abl[i].paseg;
        if (!paseg)
            continue;
        cHands++;
        for (int j = 0; j < 2; j++)
        {
            SHAPE *pshape = (SHAPE *)PloNew((CID)10, phands->pso, (OID)0xB2, -1);
            pshape->crvk = (CRVK)0;
            pshape->ccv = D_002C1ED0;
            pshape->mpicvu = (float *)PvAllocSw(D_002C1ED0 << 2, 16);
            pshape->mpicvs = (float *)PvAllocSw(D_002C1ED0 << 2, 16);
            pshape->mpicvpos = (VECTOR4 *)PvAllocSw(D_002C1ED0 << 4, 16);
            MATRIX4 matSample;
            XF *pxfEffect;
            if (j == 0)
            {
                matSample = matHand;
                pxfEffect = pxfHand;
            }
            else
            {
                pxfEffect = phands->pxf;
                SetMatrix4Identity(&matSample);
            }
            for (int k = 0; k < D_002C1ED0; k++)
            {
                MATRIX4 matOut;
                float u = (float)k / (float)(D_002C1ED0 - 1);
                PredictAsegEffect(u * paseg->tMax, paseg, phands->pso, 0.0f, pxfEffect, phands->pso->pxfRoot, &matOut, 0);
                pshape->mpicvu[k] = u;
                qword qwPos = QwTransformPos(&matOut, *(qword *)matSample.mat[3]);
                pshape->mpicvpos[k] = *(VECTOR4 *)&qwPos;
            }
            pshape->Remove();
            MeasureShape(pshape);
            if (j == 0)
                phands->abl[i].pshapeA = pshape;
            else
                phands->abl[i].pshapeB = pshape;
        }
    }
    if (cHands == 2)
    {
        phands->abl[0].uWeight = 1.0f;
        EnsureAsegBlendDynamic((ALO *)phands->pso, 0x14, 2, phands->abl, 0, 0, 0, &phands->pasegbl);
    }
}
