// func_00197500: ported from Lilly's personal Sly 2 decomp (src/P2/jtair_update.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF818 (D_002BF818), D_002BF81C (D_002BF81C), D_002BF820 (D_002BF820), D_002BF82C (D_002BF82C), D_002F7900 (g_SheetGangMurray), D_002E1E40 (g_SheetGangSly), D_002BF830 (g_SheetMovementMiscHoverPackDuration), D_002DDED0 (g_clock), D_002A2DD0 (g_qwZero)
// 0x00197500 UpdateJtAirInput (our name; KB FUN_00197500)
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/players.h"
#include "sly2/cm.h"
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
qword PosLoWorld(LO *plo);
qword PosSmooth(qword posCur, qword posTarget, float dt, SMP *psmp, VECTOR *pv);
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
    virtual void Deactivate();
};
struct ACT8 : public ACT
{
      float tEnd;
      qword qwGoalPos;
      qword qwGoalVel;
    ACT8() { ack = 8; }
    void SetGoal(qword qwPos, qword qwVel);
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
};
void SetStepRadTowardDir(STEP *pstep, qword dpos);
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
void PlayJtAseg(ASEG *paseg, ALO *paloRoot, float tLocal, float svtLocal, GRFAPL grfapl, ASEGA **ppasega);
float TFindAsegLabel(ASEG *paseg, OID oidLabel);
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
extern "C" int GetHeroMp(STEPPLAYER *pstepplayer);
struct JTEDGE
{
      struct XF *pxf;
      LO *plo;
};
void PayGadgetCost(STEPPLAYER *pstepplayer, int idGadget);
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
struct CID88 : public SO, public GRABSPOT
{
      DLE dle;
      int nUnk17c;
      float tUnk180;
      undefined4 unk_0x184[2];
      int nUnk18c;
      STEPPLAYER *pstepplayerUnk190;
      undefined4 unk_0x194[3];
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
struct PARTLINK
{
      undefined4 unk_0x00;
      void *precA;
      void *precB;
      void *precC;
      undefined4 unk_0x10[0xD0 / 4];
};
int FAsegaStopped(ASEGA *pasega);
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
extern STEPPLAYER *D_002E1E40;
extern MURRAY *D_002F7900;
extern qword D_002A2DD0;
extern float D_002BF818;
extern float D_002BF81C;
extern SMP D_002BF820;
extern float D_002BF82C;
extern float D_002BF830;
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo);
void SetJtAirState(JTAIR *pjtair, int nState, LO *plo);
void SteerJtAirClip(JTAIR *pjtair);
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind);
void ReleaseJtAirObject(JTAIR *pjtair);
int FJtLandsWithin(float dt, STEP *pstep);
XP *PxpFirstOfTree(SOTREE *ptree);
void TryJtAirGrab(JTAIR *pjtair, LO *plo);
int FJtAirWedged(float tState, STEPPLAYER *pstepplayer, float *ptCheck);
extern "C" XP *FindJtEdgeGrabContact(JTAIR *pjtair, int *pixpd, VECTOR4 *pnormal);
extern "C" int FCheckJtEdgeGrab(JTAIR *pjtair, SOPART *ppart, qword posHold, int xpk, void *pv, qword *pnormal);
extern "C" void StartJtGrabForAirKind(JTAIR *pjtair);
extern "C" void RespawnAtSafeSpot(STEPPLAYER *pstepplayer, JTPLACE *pplace);
extern "C" void SetJtThrowState(JTTHROW *pjtthrow, int nState);
extern "C" int FPowerUpAcquired(int ipowerup);
extern "C" uint GrfMatchingSlots(STEPPLAYER *pstepplayer, int n);
extern "C" void EnterJtGrabState(JTGRAB *pjtgrab, int nState, LO *plo, GRABSPOT *pspot);
extern "C" qword QwPosOfJtPlace(float dt, STEPPLAYER *pstepplayer, JTPLACE *pplace);
static inline float GMinOf(float a, float b)
{
    return (a < b) ? a : b;
}
static inline qword QwScale(qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(gScale.qw), "j"(qwVec));
    return qw;
}
void UpdateJtAirInput(JTAIR *pjtair) __asm__("func_00197500");
void UpdateJtAirInput(JTAIR *pjtair)
{
    VECTOR4 posHero;
    VECTOR4 vel;
    VECTOR4 vec;
    int ixpd;
    int nStatePrev;
    int fFloor = 0;
    XP *pxpFloor;
    POCTL *pctl;
    *(qword *)&posHero = PosLoWorld(pjtair->pstepplayer);
    *(qword *)&vel = pjtair->pstepplayer->GetVelocity();
    pctl = pjtair->pstepplayer->pctl;
    pxpFloor = pjtair->pstepplayer->pxpFloor;
    if (pxpFloor != 0 && pxpFloor->normal.z > 0.7f)
        fFloor = 1;
    do
    {
        nStatePrev = pjtair->nState;
        if (pjtair->plo != 0)
        {
            ulong fWorld = pjtair->plo->fInWorld;
            if (!fWorld)
                ReleaseJtAirObject(pjtair);
        }
        switch (pjtair->nState)
        {
            case 9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
                break;
            default:
            if (pjtair->fCanDoubleJump)
            {
                if (pctl->GrfPressedWithin(0x40, 0.0f))
                {
                    if (!FJtLandsWithin(0.075f, pjtair->pstepplayer))
                    {
                        pctl->ClearJoyPressed(0x40);
                        SetJtAirJump(pjtair, 6, pjtair->n2, 0);
                    }
                }
            }
            if (pctl->GrfPressedWithin(0x20, 0.0f))
            {
                int nState = pjtair->nState;
                TryJtAirGrab(pjtair, 0);
                if (pjtair->nState != nState)
                    pctl->ClearJoyPressed(0x20);
            }
            if (pctl->GrfPressedWithin(0x8, 0.0f))
            {
                if (pjtair->pstepplayer == D_002E1E40 && FPowerUpAcquired(0x1B) && !FJtLandsWithin(0.3f, pjtair->pstepplayer))
                {
                    PayGadgetCost(pjtair->pstepplayer, 0x1B);
                    pctl->ClearJoyPressed(0x8);
                    D_002E1E40->StartGadget(0x1B);
                }
            }
            break;
        }
        switch (pjtair->nState)
        {
            case 0:
                if (D_002DDED0.t - pjtair->tState > D_002BF818)
                {
                    if (!(pctl->grfbtn & 0x40) && pjtair->nUnk40 != 3)
                        pjtair->fUnk24 = 1;
                    SetJtAirState(pjtair, 1, pjtair->plo);
                }
                else if (!pjtair->fUnk30 && pctl->sLeft > 0.5f)
                    SteerJtAirClip(pjtair);
                break;
            case 2:
                if (D_002DDED0.t - pjtair->tState > D_002BF81C)
                    SetJtAirState(pjtair, 3, pjtair->plo);
                else if (!pjtair->fUnk30 && pctl->sLeft > 0.5f)
                    SteerJtAirClip(pjtair);
                break;
            case 1:
            case 7:
                if (vel.z < 0.0f)
                    SetJtAirState(pjtair, 4, pjtair->plo);
                break;
            case 6:
                if (((CID88 *)pjtair->plo)->nUnk17c == 3)
                    SetJtAirState(pjtair, 4, 0);
                else if (*(float *)&pjtair->pstepplayer->pxfUnk58->pxfRef < D_002DDED0.t)
                    StartJtGrabForAirKind(pjtair);
                break;
            case 9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0xF:
            case 0x10:
                if (*(float *)&pjtair->pstepplayer->pxfUnk58->pxfRef < D_002DDED0.t)
                    StartJtGrabForAirKind(pjtair);
                break;
            case 0x12:
            {
                float tLabel = TFindAsegLabel(D_002F7900->moverec.pasegUnk64, (OID)0x1C7);
                float t = *(float *)&pjtair->pstepplayer->pxfUnk58->pxfRef;
                t -= tLabel;
                if (t <= D_002DDED0.t && D_002F7900->moverec.nState != 3)
                {
                    SetJtThrowState((JTTHROW *)&D_002F7900->moverec, 3);
                    break;
                }
                if (*(float *)&pjtair->pstepplayer->pxfUnk58->pxfRef < D_002DDED0.t)
                    StartJtGrabForAirKind(pjtair);
                break;
            }
            case 8:
                if (*(float *)&pjtair->pstepplayer->pxfUnk58->pxfRef < D_002DDED0.t)
                    SetJtAirState(pjtair, 4, pjtair->plo);
                break;
            case 5:
                if (D_002DDED0.t - pjtair->tState > 0.25f && fFloor)
                {
                    pjtair->pstepplayer->VUnk87(0);
                    break;
                }
                if (FJtAirWedged(pjtair->tState, pjtair->pstepplayer, &pjtair->pstepplayer->tUnk131c))
                    RespawnAtSafeSpot(pjtair->pstepplayer, 0);
                break;
            case 3:
            case 4:
                if (fFloor)
                {
                    if (pjtair->nKind == 0xE)
                        EnterJtGrabState(&pjtair->pstepplayer->jtgrab, 0xA, 0, 0);
                    else
                        pjtair->pstepplayer->VUnk87(0);
                    break;
                }
                else
                {
                    XP *pxp = FindJtEdgeGrabContact(pjtair, &ixpd, &vec);
                    if (pxp != 0)
                    {
                        XPD *pxpd = &pxp->axpd[1 - ixpd];
                        pjtair->pedge = (JTEDGE *)pxpd->ppart;
                        pjtair->posEdge = ConvertXfPos(0, pjtair->pedge->pxf, pxpd->pos);
                        pjtair->nUnk100 = pxp->xpk;
                        pjtair->nUnk104 = (ixpd == 0) ? (int)pxp->pv1 : (int)pxp->pv0;
                        pjtair->normalEdge = *(qword *)&vec;
                        SetJtAirState(pjtair, 0x11, 0);
                        break;
                    }
                }
                if (FJtAirWedged(pjtair->tState, pjtair->pstepplayer, &pjtair->pstepplayer->tUnk131c))
                    RespawnAtSafeSpot(pjtair->pstepplayer, 0);
                break;
            case 0x11:
                if (D_002DDED0.t - pjtair->tState > 0.25f)
                {
                    if (pctl->GrfConsumePressed(0x40, 0.25f))
                    {
                        SetJtAirJump(pjtair, 3, 0, 0);
                        break;
                    }
                }
                if (((SO *)pjtair->pedge->plo)->ptree == 0)
                    SetJtAirState(pjtair, 4, 0);
                else
                {
                    STEPPLAYER *pstepplayer = pjtair->pstepplayer;
                    pstepplayer->dposUnk1010 = PosSmooth(pstepplayer->dposUnk1010, D_002A2DD0, D_002DDED0.dt, &D_002BF820, 0);
                    *(qword *)&vec = ConvertXfPos(pjtair->pedge->pxf, 0, pjtair->posEdge);
                    if (D_002DDED0.t - pjtair->tState > 0.03f)
                    {
                        if (!FCheckJtEdgeGrab(pjtair, (SOPART *)pjtair->pedge, *(qword *)&vec, pjtair->nUnk100, (void *)pjtair->nUnk104, &pjtair->normalEdge))
                        {
                            SetJtAirState(pjtair, 4, 0);
                            break;
                        }
                    }
                    SetStepRadTowardDir(pjtair->pstepplayer, QwScale(pjtair->normalEdge, VuFloat(-1.0f)));
                    if (pctl->GrfPressedWithin(0x400, 0.15f))
                    {
                        if (pjtair->pstepplayer->FCanUseGadget(3))
                        {
                            pctl->ClearJoyPressed(0x400);
                            StartJtMode(&pjtair->pstepplayer->jtmode, 0, 0, 0);
                        }
                    }
                }
                break;
            case 0x13:
            case 0x14:
                if (PxpFirstOfTree(pjtair->pstepplayer->ptree) != 0)
                {
                    SetJtAirState(pjtair, 4, 0);
                    break;
                }
                if (!(pctl->grfbtn & (GrfMatchingSlots(pjtair->pstepplayer, 0x1B) | 8)))
                {
                    SetJtAirState(pjtair, 4, 0);
                    break;
                }
                if (pjtair->nState == 0x13)
                {
                    ASEGA *pasega = pjtair->pstepplayer->pasegaUnk1030;
                    if (pasega == 0 || FAsegaStopped(pasega))
                        SetJtAirState(pjtair, 0x14, 0);
                }
                break;
            case 0x15:
                if (pjtair->pact8->tEnd < D_002DDED0.t)
                {
                    JT *pjt = 0;
                    int nUnk10;
                    if (FIsBasicDerivedFrom(pjtair->pstepplayer, (CID)0x35))
                    {
                        pjt = (JT *)pjtair->pstepplayer;
                        nUnk10 = pjt->janim.nUnk10;
                    }
                    *(qword *)&vec = pjtair->pstepplayer->GetVelocity();
                    vec.x = vec.y = 0.0f;
                    pjtair->pstepplayer->SetVelocity(*(qword *)&vec);
                    SetJtAirJump(pjtair, 0xB, 0, 0);
                    if (pjt != 0)
                        pjt->janim.nUnk10 = nUnk10;
                    break;
                }
                if (pjtair->placeTarget.pspot != 0 && pjtair->pact8->tEnd < D_002DDED0.t + 0.25f)
                {
                    if (FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0x13) || FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0xB) ||
                        FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0xD) || FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0x27) ||
                        FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0xC) || FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0x22) ||
                        FIsBasicDerivedFrom(pjtair->placeTarget.plo, (CID)0x26))
                    {
                        SetJtAirJump(pjtair, 0xB, 0, 0);
                        TryJtAirGrab(pjtair, pjtair->placeTarget.plo);
                        break;
                    }
                }
                if (pjtair->fCanDoubleJump)
                {
                    if (pjtair->pstepplayer->pxfUnk58->vUnkB0.z < 0.0f)
                    {
                        PlayJtAseg(pjtair->apasegDoubleJump[0], (ALO *)pjtair->pstepplayer, 0.0f, 1.0f, 1, &pjtair->pstepplayer->pasegaUnk1030);
                        pjtair->fCanDoubleJump = 0;
                        pjtair->pstepplayer->SetVelocity(JtPlanLaunchVz(pjtair, 6));
                        break;
                    }
                }
                if (pjtair->placeTarget.pspot != 0 || pjtair->placeTarget.ppart != 0)
                {
                    qword posTarget = QwPosOfJtPlace(GMinOf(0.7f, pjtair->pact8->tEnd - D_002DDED0.t), pjtair->pstepplayer, &pjtair->placeTarget);
                    pjtair->pact8->SetGoal(ConvertXfPos(0, pjtair->pstepplayer->pxfUnk58->pxfParent, posTarget), pjtair->pact8->qwGoalVel);
                }
                break;
            case 0x16:
                if (GetHeroMp(pjtair->pstepplayer) <= 0)
                    SetJtAirState(pjtair, 4, 0);
                else if (D_002DDED0.t - pjtair->tState > D_002BF82C && fFloor)
                    pjtair->pstepplayer->VUnk87(0);
                else if (D_002DDED0.t - pjtair->tState > D_002BF82C && !(pctl->grfbtn & GrfMatchingSlots(pjtair->pstepplayer, 0xC)))
                    SetJtAirState(pjtair, 4, 0);
                else if (D_002DDED0.t - pjtair->tState > D_002BF830)
                    SetJtAirState(pjtair, 4, 0);
                break;
        }
        if (pjtair->nState == nStatePrev)
            break;
    } while (pjtair->nState != -1);
}
