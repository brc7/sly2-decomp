// func_0018A8C8: ported from Lilly's personal Sly 2 decomp (src/P2/jtground.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A2DA0 (D_002A2DA0), D_002BF354 (D_002BF354), D_002BF360 (D_002BF360), D_002BF364 (D_002BF364), D_002BF370 (D_002BF370), D_002DDED0 (g_clock), D_002A2DC0 (g_normalZ)
// 0x0018A8C8 JtChooseGroundState (curated name)
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/cm.h"
inline qword QwTransformPos(MATRIX4 *pmat, qword qwPos)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(qwResult)
            : "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]), "j"(*(qword *)pmat->mat[3]),
              "j"(qwPos));
    return qwResult;
}
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
qword PosLoWorld(LO *plo);
qword ConvertLoPos(LO *ploFrom, LO *ploTo, qword qwPos);
MATRIX4 *PmatLoWorld(LO *plo);
int FIsSmaRetracted(SMA *psma);
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
struct ACT5 : public ACT
{
      ACT *pactCur;
      qword qwOffset;
    ACT5() { ack = 5; }
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
    virtual void GetClosestParam(JT *pjt, float *psParam);
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
float TFindAsegLabel(ASEG *paseg, OID oidLabel);
struct JTREC
{
      JT *pjt;
      int nState;
      float tStart;
      int nSide;
      ASEG *apaseg[2];
};
void JtInteractPre(JTREC *prec);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
struct STUNHOLD
{
      int fValid;
      float gSide;
      float gUp;
      undefined4 unk_0x0c;
      qword qwPos;
      qword qwVel;
};
void StartJtMode(JTMODE *pjtmode, int nKind, STEP *pstepOther, LO *ploHudOverride);
void HeroHurtPre(JTHURT *pjthurt);
void UpdateJtThrow(JTTHROW *pjtthrow);
void UpdateJtPipe(JTPIPE *pjtpipe);
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo);
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
float PeekLastB(SHAPE *pshape);
}
void GetShapePosDirAtS(SHAPE *pshape, float s, qword *ppos, qword *pdir);
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
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs);
int FAsegaStopped(ASEGA *pasega);
void SeekAsega(ASEGA *pasega, SEEK seek, float dtLocal, float svtLocal);
struct VU_FLOAT
{
    qword qw;
};
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
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline float GXOfQw(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
extern "C" {
void func_00197500(JTAIR *pjtair);
}
extern "C" {
void UpdateJtGrabPre(JTGRAB *pjtgrab);
}
extern "C" {
int GetHeroMp(STEPPLAYER *pstepplayer);
}
extern "C" {
uint GrfMatchingSlots(STEPPLAYER *pstepplayer, int n);
}
extern "C" {
int FFightStanceInRange(STEPPLAYER *pstepplayer, int fHysteresis);
}
extern "C" {
qword CalculateLaunchVelocity(float gArc, qword qwFrom, qword qwTo, qword dvGravity);
}
int ChooseJtGrabTarget(JTGRAB *pjtgrab, uint grf, int fUnk);
MATRIX3 LoadRotateMatrixRad(float rad, qword normal);
extern float D_002BF354;
extern float D_002BF360;
extern float D_002BF364;
extern qword D_002BF370;
extern qword D_002A2DA0;
extern qword D_002A2DC0;
static inline qword QwProjectOut(qword v, qword n)
{
    qword qwDot = QwDot(v, n);
    qword qwAlong;
    qword qwAcross;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qwAlong) : "j"(qwDot), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qwAcross) : "j"(v), "j"(qwAlong));
    return qwAcross;
}
static inline qword QwScaleX(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline qword QwRotateVecAt(MATRIX3 *pmat, qword *pqwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qwResult)
            : "j"(*pqwVec), "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]));
    return qwResult;
}
extern "C" void JtChooseGroundState(JT *pjt) __asm__("func_0018A8C8");
extern "C" void JtChooseGroundState(JT *pjt)
{
    JTS jtsOld;
    JTBS jtbsOld;
    do
    {
        jtsOld = pjt->jts;
        jtbsOld = pjt->jtbs;
        if (jtsOld >= 0 && jtsOld < JTS_Max)
        {
            switch (jtsOld)
            {
                case JTS_Grab:
                {
                    int nState = pjt->jtgrab.nState;
                    if (nState < 5)
                    {
                        if (nState >= 3 || nState == 1)
                            break;
                        goto fell;
                    }
                    if (nState != 0xC)
                        goto fell;
                    break;
                }
                case JTS_Stand:
                case JTS_Run:
                case JTS_Unk05:
                case JTS_Unk0A:
                case JTS_Pickpocket:
                case JTS_Gait2:
                case JTS_Throw:
                case JTS_Unk18:
                    if (pjt->jtas != 8 && pjt->jtas != 9 || pjt->janim.fInputOpen)
                    {
    fell:
                        if (D_002DDED0.t - pjt->tUnk20c > 0.1f)
                            SetJtAirJump(&pjt->jtair, 0, 0, 0);
                    }
                    break;
            }
            switch (pjt->jts)
            {
                case JTS_Air:
                    func_00197500(&pjt->jtair);
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if (!pjt->FCanUseGadget(2))
                        break;
                    if (pjt->pctl->GrfConsumePressed(0x80, 0.0f))
                        SetJtJts(pjt, JTS_Unk0D, JTBS_Nil);
                    else if (pjt->pctl->GrfConsumePressed(0x10, 0.0f))
                        SetJtJts(pjt, JTS_Unk0E, JTBS_Nil);
                    break;
                case JTS_Grab:
                    UpdateJtGrabPre(&pjt->jtgrab);
                    break;
                case JTS_Unk04:
                    HeroHurtPre(&pjt->jthurt);
                    break;
                case JTS_Throw:
                    UpdateJtThrow(&pjt->jtthrow);
                    break;
                case JTS_Interact:
                    JtInteractPre((JTREC *)&pjt->unk_0x1ff0);
                    break;
                case JTS_Unk08:
                    UpdateJtPipe(&pjt->jtpipe);
                    break;
                case JTS_Stand:
                case JTS_Run:
                case JTS_Unk0A:
                case JTS_Gait2:
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if (pjt->jtas == 8 || pjt->jtas == 9)
                    {
                        if (!pjt->janim.fInputOpen)
                            break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.1f))
                    {
                        SetJtAirJump(&pjt->jtair, 1, 0, 0);
                        break;
                    }
                    if (!(pjt->pctl->grfbtn & 8))
                    {
                        if (FFightStanceInRange(pjt, 0))
                        {
                            SetJtJts(pjt, JTS_Unk05, JTBS_4);
                            break;
                        }
                    }
                    if (pjt->pctl->GrfPressedWithin(0x400, 0.15f) && pjt->FCanUseGadget(3))
                    {
                        pjt->pctl->ClearJoyPressed(0x400);
                        StartJtMode(&pjt->jtmode, 0, 0, 0);
                        break;
                    }
                    if (pjt->nUnk10e4 == 3)
                    {
                        if (pjt->pctl->GrfConsumePressed(0x20, 0.15f))
                        {
                            SetJtJts(pjt, JTS_Pickpocket, JTBS_Nil);
                            break;
                        }
                    }
                    if (pjt->pctl->GrfConsumePressed(0x20, 0.2f))
                    {
                        ChooseJtGrabTarget(&pjt->jtgrab, 0xFFFF, 0);
                        break;
                    }
                    {
                        float sLeft = pjt->pctl->sLeft;
                        if (pjt->jts == 0 ? sLeft > 0.1f : sLeft > 0.0f)
                        {
                            if (pjt->pctl->grfbtn & 8)
                                SetJtJts(pjt, JTS_Run, JTBS_Nil);
                            else if (pjt->nUnk10e4 == 3)
                                SetJtJts(pjt, JTS_Unk0A, JTBS_Nil);
                            else
                                SetJtJts(pjt, JTS_Gait2, JTBS_Nil);
                        }
                        else
                            SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    }
                    break;
                case JTS_Unk18:
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if (!(0.0f < GetHeroMp(pjt)))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.1f))
                    {
                        SetJtAirJump(&pjt->jtair, 1, 0, 0);
                        break;
                    }
                    if (!(pjt->pctl->grfbtn & GrfMatchingSlots(pjt, 0x19)))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->jtbs == JTBS_13)
                    {
                        qword qwVel = pjt->GetVelocity();
                        int fStill = GXOfQw(QwDot(qwVel, *(qword *)PmatLoWorld(pjt)->mat[0])) < D_002BF354;
                        if (fStill)
                            SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    }
                    break;
                case JTS_Unk05:
                    if (pjt->nUnk1d30)
                    {
                        SetJtJts(pjt, JTS_Unk11, JTBS_Nil);
                        break;
                    }
                    if ((uint)(pjt->jtbs - 6) < 4)
                        break;
                    if (pjt->pctl->grfbtn & 8)
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (!FFightStanceInRange(pjt, 1))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.1f))
                    {
                        SetJtAirJump(&pjt->jtair, 1, 0, 0);
                        break;
                    }
                    if (pjt->pctl->GrfPressedWithin(0x400, 0.15f) && pjt->FCanUseGadget(3))
                    {
                        pjt->pctl->ClearJoyPressed(0x400);
                        StartJtMode(&pjt->jtmode, 0, 0, 0);
                        break;
                    }
                    if (pjt->pctl->GrfConsumePressed(0x20, 0.2f))
                    {
                        ChooseJtGrabTarget(&pjt->jtgrab, 0xFFFF, 0);
                        break;
                    }
                    if (0.0f < pjt->pctl->sLeft)
                        SetJtJts(pjt, JTS_Unk05, JTBS_5);
                    else
                        SetJtJts(pjt, JTS_Unk05, JTBS_4);
                    break;
                case JTS_Hang:
                    if (D_002DDED0.t - pjt->tUnk18cc > 0.25f)
                    {
                        if (pjt->pctl->GrfConsumePressed(0x40, 0.25f))
                            SetJtAirJump(&pjt->jtair, 2, 0, 0);
                    }
                    break;
                case JTS_Unk0D:
                    if (FAsegaStopped(pjt->pasegaUnk1030))
                    {
                        SetJtAirJump(&pjt->jtair, 0xB, 0, 0);
                        break;
                    }
                    if (pjt->pxpFloor)
                    {
                        if (0.7f < pjt->pxpFloor->normal.z)
                        {
                            if (D_002DDED0.t - pjt->tUnk18cc > 0.4f)
                                SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        }
                    }
                    break;
                case JTS_Unk0E:
                    if (FAsegaStopped(pjt->pasegaUnk1030))
                    {
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                        break;
                    }
                    if (pjt->pxpFloor)
                    {
                        if (0.7f < pjt->pxpFloor->normal.z)
                        {
                            if (D_002DDED0.t - pjt->tUnk18cc > 0.4f)
                            {
                                float tLabel = TFindAsegLabel(pjt->pasegUnk1a84, (OID)0x16D);
                                ASEGA *pasega = pjt->pasegaUnk1030;
                                if (pasega->tLocal < tLabel)
                                    SeekAsega(pasega, SEEK_Start, tLabel, pasega->svt);
                            }
                        }
                    }
                    break;
                case JTS_Pounce:
                    if (!pjt->pasegaUnk1030 || FAsegaStopped(pjt->pasegaUnk1030))
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    break;
                case JTS_Unk13:
                {
                    if (!pjt->pasegaUnk1030 || !FAsegaStopped(pjt->pasegaUnk1030))
                    {
                        if (!pjt->psmaUnk1034)
                            break;
                        if (!FIsSmaRetracted(pjt->psmaUnk1034))
                            break;
                    }
                    SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    break;
                }
                case JTS_Unk11:
                    if (!pjt->nUnk1d30 || D_002DDED0.t - pjt->tUnk18cc > D_002BF360)
                        SetJtJts(pjt, JTS_Stand, JTBS_Nil);
                    break;
                case JTS_SurfaceClimb:
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.25f))
                        SetJtAirJump(&pjt->jtair, 0x12, 0, 0);
                    break;
                case JTS_HangFollow:
                    if (pjt->pctl->GrfConsumePressed(0x40, 0.25f))
                    {
                        VECTOR4 vTarget;
                        LO *ploTarget = pjt->phndUnk18c4->ploUnk138;
                        if (ploTarget && FIsBasicDerivedFrom(ploTarget, (CID)10))
                        {
                            SHAPE *pshape = (SHAPE *)pjt->phndUnk18c4->ploUnk138;
                            GetShapePosDirAtS(pshape, PeekLastB(pshape) * (0.5f - pjt->gUnk1f94 * 0.5f), (qword *)&vTarget, 0);
                        }
                        else
                        {
                            VECTOR4 vLocal;
                            if (pjt->phndUnk18c4->ploUnk138)
                                *(qword *)&vLocal = ConvertLoPos(0, pjt->phndUnk18c4, PosLoWorld(pjt->phndUnk18c4->ploUnk138));
                            else
                                *(qword *)&vLocal = D_002BF370;
                            MATRIX3 mat;
                            mat = LoadRotateMatrixRad(pjt->gUnk1f94 * D_002BF364, D_002A2DA0);
                            *(qword *)&vTarget = QwTransformPos(pjt->phndUnk18c4->PmatWorld(), QwRotateVecAt(&mat, (qword *)&vLocal));
                        }
                        qword qwVel = CalculateLaunchVelocity(0.5f, PosLoWorld(pjt), QwSub(*(qword *)&vTarget, pjt->qwUnk240), *(qword *)&pjt->psophys->dvGravity);
                        VU_FLOAT gScale = VuFloatFromMemWord(*(int *)&pjt->gUnk1f98);
                        pjt->SetVelocity(QwScaleX(qwVel, gScale));
                        SetJtAirJump(&pjt->jtair, 0xC, 0, 0);
                        pjt->jtair.qwUnkA0 = QwProjectOut(qwVel, D_002A2DC0);
                    }
                    break;
            }
        }
    } while (pjt->jts != jtsOld || pjt->jtbs != jtbsOld);
}
