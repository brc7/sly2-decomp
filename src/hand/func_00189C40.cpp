// func_00189C40: ported from Lilly's personal Sly 2 decomp (src/P2/jt.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF340 (D_002BF340), D_002BF344 (D_002BF344), D_002A2DC0 (g_normalZ)
// 0x00189C40 RebuildJtXmg__FP2JTP3ALOfT1P4ACT5P3XMG  (T1460 MATCH; T0983 had it at 77 %)  Sly 1's RebuildJtXmg (JT.c): Sly's foot placement for one foot. When the clip his feet follow
// changes (pxmg->paseg) the foot's XMG is rebuilt. The clip's label 0x16A (a time) is asked three times where the XFs stand then (PredictAsegEffect 0x1300B8, relative to his root XF):
// paloExtrem = the foot's ball XF, paloHip = the leg's root XF (its slot +0xA8 holds the leg's IK record, IKREC: the limb's goal XF with the IK handle's point, the two bone lengths) and
// the record's goal XF (the heel's IK handle). posExtrem = the ball lowered by sRadius (5: ball to sole) along g_normalZ; the sphere the foot can reach is centred on posSphere = the
// hip + (posExtrem - the handle's goal point) with the radius sSphere = both bones of the leg + 10; the vertical line posExtrem + g_normalZ * 50 .. posExtrem - g_normalZ * 50 is clipped to
// that sphere (0x14C748, ClsgClipEdgeToSphere's Sly 2 form: the first piece goes to posUp / posDown, fValid = it hit) and posSphereLow = the sphere's lowest point (Sly 2 only).
// UpdateJtStand (every frame, the two feet) and JT::VUnk15 call it; FMatchJtXmg then probes the ground along those lines. pact5 (the foot's offset actuator) is not touched here.
// Match notes (decomp/learnings.md "T1460"): the two pool constants are `extern int` words read with VuFloatFromMemWord(D) (by-value param); the source order that keeps the game's
// schedule is "sphere centre store, radius store, then the two constants, then the two QwAddScaled into locals, then the call", and the posExtrem store before the two clip stores.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]),
              "j"(qwPos));
    return qwResult;
}
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
struct IKREC
{
      XF *pxfGoal;
      XF *pxfS;
      XF *pxfE;
      XF *pxfEnd;
      MATRIX4 matSBind;
      MATRIX4 matEBind;
      qword qwAxisElbow;
      qword qwPosIkh;
      qword qwPosWrist;
      float sUpper;
      float sLower;
      float radOrig;
      float radTwist;
      undefined4 unk_0xd0[2];
      undefined4 unk_0xd8[2];
};
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
struct LSG
{
      qword apos[2];
      qword anormal[2];
      float au[2];
      LSGK lsgk;
      SURF *apsurf[2];
      EDGE *pedge;
      int iiposSwap;
      LSG *plsgNext;
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
struct OVRA
{
      KEY *pkey;
      void *pv;
      LO *plo;
      int id;
      OVRA *povraNext;
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
EA *PeaFindAsegLabel(ASEG *paseg, OID oidLabel);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
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
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
int ClsgClipEdgeToSphere(qword posSphere, float sRadius, qword pos0, qword pos1, int clsgMax, LSG *alsg);
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw));
    return qw;
}
extern qword D_002A2DC0;
extern int D_002BF340;
extern int D_002BF344;
extern "C" void PredictAsegEffect(ASEG *paseg, SO *psoRoot, float tLocal, float svt, XF *pxfEffect, XF *pxfRef, MATRIX4 *pmatOut, int f);
void RebuildJtXmg(JT *pjt, ALO *paloExtrem, float sRadius, ALO *paloHip, ACT5 *pact5, XMG *pxmg) __asm__("func_00189C40");
void RebuildJtXmg(JT *pjt, ALO *paloExtrem, float sRadius, ALO *paloHip, ACT5 *pact5, XMG *pxmg)
{
    ASEGA *pasega;
    if (pjt->jts == 0)
    {
        switch (pjt->jtas)
        {
        case 6: case 7: case 8: case 9: case 12: case 13:
            pasega = pjt->janim.pasega;
            break;
        default:
            pasega = pjt->psmaUnk1034->pasegaCur;
            break;
        }
    }
    else
    {
        if (pjt->psmaUnk1034 != 0)
            pasega = pjt->psmaUnk1034->pasegaCur;
        else
            pasega = pjt->pasegaUnk1030;
    }
    if (pasega->paseg != pxmg->paseg)
    {
        pxmg->paseg = pasega->paseg;
        float tLocal = 0.0f;
        EA *pea = PeaFindAsegLabel(pasega->paseg, (OID)0x16A);
        if (pea != 0)
        {
            tLocal = pea->tLocal;
        }
        MATRIX4 matExtrem;
        PredictAsegEffect(pasega->paseg, pjt, tLocal, 0.0f, (XF *)paloExtrem, pjt->pxfRoot, &matExtrem, 0);
        qword posExtrem = QwAddScaled(*(qword *)matExtrem.mat[3], D_002A2DC0, VuFloat(-sRadius));
        MATRIX4 matHip;
        PredictAsegEffect(pasega->paseg, pjt, tLocal, 0.0f, (XF *)paloHip, pjt->pxfRoot, &matHip, 0);
        MATRIX4 matIkh;
        PredictAsegEffect(pasega->paseg, pjt, tLocal, 0.0f, ((IKREC *)((XF *)( paloHip ))->pxfRef) ->pxfGoal, pjt->pxfRoot, &matIkh, 0);
        qword posIkh = QwTransformPos(&matIkh, ((IKREC *)((XF *)( paloHip ))->pxfRef) ->qwPosIkh);
        qword posSphereCenter = QwAdd(*(qword *)matHip.mat[3], QwSub(posExtrem, posIkh));
        pxmg->posSphere = posSphereCenter;
        pxmg->sSphere = ((IKREC *)((XF *)( paloHip ))->pxfRef) ->sUpper + ((IKREC *)((XF *)( paloHip ))->pxfRef) ->sLower + 10.0f;
        VU_FLOAT vfUp = VuFloatFromMemWord(D_002BF340);
        VU_FLOAT vfDown = VuFloatFromMemWord(D_002BF344);
        LSG lsgClip;
        qword posLineUp = QwAddScaled(posExtrem, D_002A2DC0, vfUp);
        qword posLineDown = QwAddScaled(posExtrem, D_002A2DC0, vfDown);
        pxmg->fValid = ClsgClipEdgeToSphere(posSphereCenter, pxmg->sSphere, posLineUp, posLineDown, 1, &lsgClip) > 0;
        pxmg->posExtrem = posExtrem;
        pxmg->posUp = lsgClip.apos[0];
        pxmg->posDown = lsgClip.apos[1];
        pxmg->posSphereLow = QwAddScaled(pxmg->posSphere, D_002A2DC0, VuFloat(-pxmg->sSphere));
    }
}
