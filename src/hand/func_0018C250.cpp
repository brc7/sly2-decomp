// func_0018C250: ported from Lilly's personal Sly 2 decomp (src/P2/jtstate.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF358 (D_002BF358), D_002BF35C (D_002BF35C), D_002BF3E0 (D_002BF3E0), D_002BF3E4 (D_002BF3E4), D_002BF3E8 (D_002BF3E8), D_002DDED0 (g_clock), D_002DEB30 (g_game), D_002A2DD0 (g_qwZero), D_003DD490 (g_sw)
// 0x0018C250 SetJtJts
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
extern GAME D_002DEB30;
extern "C" qword MakeVecFromAngle(float rad, float sXY, float sZ);
qword PosLoWorld(LO *plo);
SMA *PsmaApplySm(SM *psm, ALO *paloRoot, OID oidInitialState, GRFAPL grfapl, int grfRoute);
int IsmsFindSmOptional(SM *psm, OID oid);
void RetractSma(SMA *psma);
float RadNormalize(float rad);
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
void InsertNodeBySlot(SO *pso, XF *pxf, ACT *pact);
void RemoveNodeBySlot(SO *pso, XF *pxf, ACT *pact);
extern qword D_002A2DD0;
SND *MakeSnd(int sfxid);
void SetSndOwner(SND *psnd, LO *ploOwner);
void SetSndVolume(SND *psnd, float gVolume);
void SetSndNearFar(SND *psnd, float sNear, float sFar);
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
struct EXPL : public XFM
{
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
};
extern SW D_003DD490;
void FreeSwXaList(SW *psw, XA *pxaFirst);
void ShowSoPartRec(SO *pso, SOPART *ppart);
void RemoveSoXa(SO *pso, XA *pxaRemove);
void SetStepRadTarget(STEP *pstep, float radTarget);
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
void ApplyAseg(ASEG *paseg, ALO *paloAsegRoot, float tLocal, float svtLocal, GRFAPL grfapl, ASEGA **ppasega);
float TFindAsegLabel(ASEG *paseg, OID oidLabel);
void ReblendAsegbl(ASEGBL *pasegbl);
struct JTREC
{
      JT *pjt;
      int nState;
      float tStart;
      int nSide;
      ASEG *apaseg[2];
};
int NJtRecSide(JTREC *prec);
void SetJtRecState(JTREC *prec, int nState);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
void SetJtModeState(JTMODE *pjtmode, int nState);
void SetJtPipeState(JTPIPE *pjtpipe, int nState);
void SetJtAirState(JTAIR *pjtair, int nState, LO *plo);
struct TRI3
{
    int a, b, c;
};
struct SURFLOC
{
    int itri;
    float u;
    float v;
};
struct CLIMBSURF : public XFM
{
      int cvtx;
      VECTOR4 *avtx;
      int ctri;
      TRI3 *atri;
      TRI3 *atriB;
      undefined4 unk_0xf4[3];
    void FindClosestPoint(qword pos, SURFLOC *plocHint, SURFLOC *ploc, qword *ppos, qword *pnormal);
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
int FAsegaStopped(ASEGA *pasega);
void RetractAsega(ASEGA *pasega);
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
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
void ExplodeExpl(EXPL *pexpl);
void OnGameEvent(GAME *pgame, int nEvent, LO *plo);
void CalculateJtHangAccel(JT *pjt);
extern "C" {
void SetJtAnimState(JT *pjt, int jtas);
}
extern "C" {
void SetHeroHurtState(JTHURT *pjthurt, int nState);
}
extern "C" {
void SetJtThrowState(JTTHROW *pjtthrow, int nState);
}
extern "C" {
void SetJtGrabPhase(JTGRAB *pjtgrab, int nPhase);
}
extern "C" {
void UpdateJtPounceArc(JT *pjt);
}
extern "C" {
void JtHangFrame(JT *pjt, float *prad);
}
extern "C" {
int GrfJtLeanMask(STEPPLAYER *pstepplayer);
}
extern "C" {
void RollHeroIdleTimer(STEPPLAYER *pstepplayer);
}
extern "C" {
int FPowerUpAcquired(int ipowerup);
}
extern "C" {
void ReleaseRefNode(void *pvTarget, void *pvRef);
}
extern "C" {
void AddRefOidRef(void *pvTarget, void *pvRef);
}
extern "C" {
void UpdateJtHangPlace(JT *pjt);
}
extern "C" {
qword func_00190C58(JT *pjt);
}
extern "C" {
void func_001FB3F0(SND *psnd, qword qwPos);
}
extern "C" {
void GuardCapVelocityForPounce(STEPGUARD *pguard);
}
extern "C" {
void ClearHeroIdleTimer(STEPPLAYER *pstepplayer, int nIdleKind);
}
extern "C" {
void SetJtUnk1d30(JT *pjt, int n);
}
extern "C" {
void ClearGaitFootDown(GAIT *pgait);
}
extern "C" {
void PredictAsegEffect(float t, float svt, ASEG *paseg, ALO *palo, XF *pxfPart, XF *pxfRoot, MATRIX4 *pmat, int n);
}
extern "C" {
void func_001F5720(SO *pso, int fOn);
}
extern "C" {
void UpdateHeadingSmooth(STEP *pstep);
}
void SlotSetRole(SO *pso, XF *pxf, int nRole);
void SetPlayerTn(STEPPLAYER *pstepplayer, TN *ptn);
extern float D_002BF3E0;
extern float D_002BF3E4;
extern float D_002BF3E8;
extern float D_002BF358;
extern float D_002BF35C;
static inline void ClearXyz(VECTOR *pvec)
{
    pvec->z = 0.0f;
    pvec->y = 0.0f;
    pvec->x = 0.0f;
}
static inline qword QwScale(qword qwVec, VU_FLOAT gScale)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(qwVec), "j"(gScale.qw));
    return qw;
}
static inline qword QwDivide(qword qwVec, VU_FLOAT g)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(qw) : "j"(qwVec), "j"(g.qw));
    return qw;
}
static inline VU_FLOAT VuFloatBits(float g)
{
    VU_FLOAT v;
    __asm__("mfc1 %0, %1" : "=r"(v.qw) : "f"(g));
    return v;
}
static inline qword QwAddVelocity(VU_FLOAT t, qword pos, qword vel)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(qw) : "j"(pos), "j"(t.qw), "j"(vel));
    return qw;
}
static inline qword QwRotateVecRows(qword qwRow0, qword qwRow1, qword qwRow2, qword qwVec)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(qwResult)
            : "j"(qwRow0), "j"(qwRow1), "j"(qwRow2), "j"(qwVec));
    return qwResult;
}
static inline qword QwRotateVecV(qword qwVec, MATRIX3 *pmat)
{
    qword qwResult;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qwResult)
            : "j"(qwVec), "j"(*(qword *)pmat->mat[0]), "j"(*(qword *)pmat->mat[1]), "j"(*(qword *)pmat->mat[2]));
    return qwResult;
}
void ChooseJtRushTarget(JT *pjt);
void ChooseJtSmashTarget(JT *pjt);
void SetJtChannelMask(JT *pjt, uint grf);
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs) __asm__("func_0018C250");
void SetJtJts(JT *pjt, JTS jts, JTBS jtbs)
{
    ASEG *paseg = 0;
    uint grf = 0;
    int sfxid = -1;
    OID oid = OID_Nil;
    int grfapl = 1;
    float tStart = 0.0f;
    float svt = 1.0f;
    SM *psm = 0;
    int grfRoute = 0xFFFC;
    int fKeep = 0;
    if (pjt->jts == jts && pjt->jtbs == jtbs)
        return;
    if (pjt->jts != jts)
    {
        if (jts == JTS_Air)
        {
            switch (pjt->jts)
            {
                case JTS_Hang:
                case JTS_HangFollow:
                    pjt->jtair.ploUnk37c = (LO *)pjt->phndHang;
                    break;
                case JTS_Unk08:
                    pjt->jtair.ploUnk37c = (LO *)pjt->jtpipe.pshape;
                    break;
                case JTS_SurfaceClimb:
                    pjt->jtair.ploUnk37c = pjt->ploUnk1f18;
                    break;
                default:
                    pjt->jtair.ploUnk37c = 0;
                    break;
            }
        }
        switch (pjt->jts)
        {
            case JTS_Air:
                switch (jts)
                {
                    case JTS_Hang:
                    case JTS_Unk08:
                    case JTS_Unk12:
                        break;
                    case JTS_Grab:
                        if (pjt->jtgrab.nState == 1)
                            break;
                    default:
                    {
                        SND *psnd = MakeSnd(0x3D);
                        SetSndOwner(psnd, pjt);
                        SetSndVolume(psnd, 0.7f);
                        SetSndNearFar(psnd, 500.0f, 1000.0f);
                    }
                    break;
                }
                if (pjt->jtas == 0)
                {
                    if (jts != JTS_Unk08 && jts != JTS_Hang && jts != JTS_HangFollow)
                        SetJtAnimState(pjt, -1);
                }
                if (pjt->nUnk108c == 0)
                {
                    EXPL *pexpl = 0;
                    switch (jts)
                    {
                        case JTS_Grab:
                            switch (pjt->jtgrab.nState)
                            {
                                case 1:
                                    break;
                                case 3:
                                case 4:
                                    pexpl = pjt->pexplDustGrab;
                                    break;
                                default:
                                    pexpl = pjt->pexplDustLand;
                                    break;
                            }
                            break;
                        case JTS_Stand:
                        case JTS_Run:
                        case JTS_Unk05:
                        case JTS_Unk09:
                        case JTS_Unk18:
                            pexpl = pjt->pexplDustLand;
                            break;
                    }
                    if (pexpl != 0)
                    {
                        pexpl->Add();
                        ExplodeExpl(pexpl);
                        pexpl->Remove();
                    }
                }
                SetJtAirState(&pjt->jtair, -1, 0);
                break;
            case JTS_Hang:
                pjt->phndHang->HandleSpliceEvent(6, 0, 0, 0, 0);
                ReleaseRefNode(pjt->phndHang, pjt);
                SetJtAnimState(pjt, -1);
                RemoveNodeBySlot(pjt, (XF *)pjt->precUnk19c0, (ACT *)pjt->nUnk1a00);
                if (pjt->janim.pxaHang != 0)
                {
                    RemoveSoXa(pjt->janim.pxaHang->pso, pjt->janim.pxaHang);
                    FreeSwXaList(&D_003DD490, pjt->janim.pxaHang);
                    pjt->janim.pxaHang = 0;
                }
                SetPlayerTn(pjt, 0);
                break;
            case JTS_Unk08:
            {
                SetJtPipeState(&pjt->jtpipe, -1);
                int jtas = -1;
                if (jts == JTS_Air && pjt->pasegUnk14d8 != 0)
                    jtas = 0xB;
                SetJtAnimState(pjt, jtas);
            }
            break;
            case JTS_Grab:
                SetJtGrabPhase(&pjt->jtgrab, -1);
                break;
            case JTS_Unk04:
                SetHeroHurtState(&pjt->jthurt, -1);
                break;
            case JTS_Throw:
                SetJtThrowState(&pjt->jtthrow, -1);
                break;
            case JTS_Interact:
                SetJtRecState((JTREC *)&pjt->unk_0x1ff0, -1);
                break;
            case JTS_Unk09:
                pjt->ploAttached = 0;
                ShowSoPartRec(pjt, pjt->precUnk1a90);
                break;
            case JTS_Stand:
                SlotSetRole(pjt, pjt->pxfUnk58, 2);
                SlotSetRole(pjt, pjt->pxfUnk5c, 2);
                switch (pjt->jtas)
                {
                    case 6:
                        SetJtAnimState(pjt, 4);
                        break;
                    case 7:
                        SetJtAnimState(pjt, 5);
                        break;
                    case 8:
                    case 9:
                        SetJtAnimState(pjt, -1);
                        break;
                    case 0xC:
                        if (jtbs != JTBS_6)
                            SetJtAnimState(pjt, 0xE);
                        break;
                    case 0xD:
                        if (jtbs != JTBS_7)
                            SetJtAnimState(pjt, 0xF);
                        break;
                }
                RollHeroIdleTimer(pjt);
                break;
            case JTS_Pickpocket:
                pjt->grfUnk10e0 &= ~2;
                break;
            case JTS_Run:
            case JTS_Unk0A:
            case JTS_Gait2:
                pjt->uGait = pjt->pasegaUnk1030->tLocal / pjt->pasegaUnk1030->paseg->tMax;
                break;
            case JTS_Pounce:
                if (pjt->pasegaUnk1030 != 0 && !FAsegaStopped(pjt->pasegaUnk1030))
                    GuardCapVelocityForPounce((STEPGUARD *)pjt->ploInteract);
                pjt->nUnk1d2c = 0;
                pjt->grfUnk10e0 &= ~4;
                break;
            case JTS_Unk12:
            case JTS_Unk13:
            case JTS_Unk14:
                SlotSetRole(pjt, pjt->pxfUnk58, 2);
                SlotSetRole(pjt, pjt->pxfUnk5c, 2);
                break;
            case JTS_SurfaceClimb:
                func_001F5720(pjt, 0);
                pjt->ploUnk1f18->HandleSpliceEvent(6, 0, 0, 0, 0);
                if (pjt->ptnUnk1024 == *(TN **)((char *)pjt->ploUnk1f18 + 0x110))
                    SetPlayerTn(pjt, 0);
                pjt->ploUnk1f18 = 0;
                break;
            case JTS_HangFollow:
                func_001F5720(pjt, 0);
                pjt->phndHang->HandleSpliceEvent(6, 0, 0, 0, 0);
                ReleaseRefNode(pjt->phndHang, pjt);
                pjt->phndHang = 0;
                SetPlayerTn(pjt, 0);
                break;
            case JTS_Unk11:
                pjt->nUnk1d30 = 0;
                OnGameEvent(&D_002DEB30, 0x38, pjt);
                break;
            case JTS_Unk18:
                pjt->rUnk200 = 1.0f;
                pjt->pctl->radLeft = pjt->radTarget;
                break;
            case JTS_Unk0D:
                pjt->ploAttached = 0;
                break;
        }
        switch (jts)
        {
            case JTS_Stand:
            {
                psm = pjt->psmUnk1028;
                SlotSetRole(pjt, pjt->pxfUnk58, 1);
                SlotSetRole(pjt, pjt->pxfUnk5c, 1);
                pjt->nUnk1514 = 0;
                switch (pjt->jts)
                {
                    case JTS_Unk0A:
                        oid = (OID)0x177;
                        break;
                    case JTS_Gait2:
                        oid = (OID)0x176;
                        break;
                    case JTS_Air:
                    {
                        int fHigh = pjt->janim.nUnk10;
                        if (fHigh == -1)
                            fHigh = D_002BF3E0 < pjt->jtair.zStart - GZOfQw(PosLoWorld(pjt));
                        oid = (fHigh == 1) ? (OID)0x179 : (OID)0x178;
                    }
                    break;
                    default:
                        oid = (OID)0x175;
                        break;
                }
                if (IsmsFindSmOptional(psm, oid) < 0)
                    oid = OID_Nil;
                grfRoute = GrfJtLeanMask(pjt);
                ClearXyz(&pjt->vUnk190);
                if (pjt->pasegblUnk1a54 != 0 && pjt->pasegblUnk1a70 != 0)
                {
                    switch (pjt->jtas)
                    {
                        case 4:
                            SetJtAnimState(pjt, 6);
                            break;
                        case 5:
                            SetJtAnimState(pjt, 7);
                            break;
                    }
                }
                grf = 0x1F;
                ClearHeroIdleTimer(pjt, 1);
            }
            break;
            case JTS_Run:
                paseg = ((GAIT *)pjt->unk_0x1320)->pasegbl;
                grfapl = 9;
                tStart = pjt->uGait * paseg->tMax;
                ClearGaitFootDown((GAIT *)pjt->unk_0x1320);
                break;
            case JTS_Interact:
                fKeep = 1;
                SetJtRecState((JTREC *)&pjt->unk_0x1ff0, NJtRecSide((JTREC *)&pjt->unk_0x1ff0));
                break;
            case JTS_Hang:
            {
                pjt->janim.nUnk18 = -1;
                pjt->gUnk199c = 1.0f;
                pjt->gUnk1998 = 1.0f;
                ACT1 *pact1 = (ACT1 *)pjt->nUnk1a00;
                svt = 0.0f;
                pjt->janim.tHang = D_002DDED0.t;
                paseg = pjt->pasegHang;
                ClearXyz((VECTOR *)&pact1->qwRest);
                tStart = 0.5f;
                InsertNodeBySlot(pjt, (XF *)pjt->precUnk19c0, (ACT *)pjt->nUnk1a00);
                qword posHand = PosLoWorld(pjt->psoUnk19a4);
                pjt->janim.dposHang = QwSub(posHand, func_00190C58(pjt));
                UpdateJtHangPlace(pjt);
                pjt->phndHang->HandleSpliceEvent(5, 0, 0, 0, 0);
                AddRefOidRef(pjt->phndHang, pjt);
                float radFrame;
                JtHangFrame(pjt, &radFrame);
                float radBack = RadNormalize(radFrame + 3.1415927f);
                int nKind = 1;
                if (pjt->phndHang != 0)
                    nKind = pjt->phndHang->nHangKind;
                switch (nKind)
                {
                    case 1:
                        SetStepRadTarget(pjt, radFrame);
                        pjt->janim.fHangBack = 0;
                        break;
                    case 2:
                        SetStepRadTarget(pjt, radBack);
                        pjt->janim.fHangBack = 1;
                        break;
                    case 0:
                        if (fabsf(RadNormalize(radFrame - pjt->pctl->radLeft)) < 1.5707964f)
                        {
                            SetStepRadTarget(pjt, radFrame);
                            pjt->janim.fHangBack = 0;
                        }
                        else
                        {
                            SetStepRadTarget(pjt, radBack);
                            pjt->janim.fHangBack = 1;
                        }
                        break;
                }
                sfxid = pjt->phndHang->sfxidHangStart;
                SetJtAnimState(pjt, 1);
                ClearXyz(&pjt->vUnk190);
                CalculateJtHangAccel(pjt);
                SetPlayerTn(pjt, pjt->phndHang->ptn);
            }
            break;
            case JTS_Unk08:
                SetJtPipeState(&pjt->jtpipe, 0);
                fKeep = 1;
                break;
            case JTS_Unk04:
                SetJtModeState(&pjt->jtmode, -1);
                SetJtUnk1d30(pjt, 0);
                break;
            case JTS_Grab:
                if (pjt->jtgrab.nState < 5)
                    grf = (pjt->jtgrab.nState < 3) ? 0 : 7;
                break;
            case JTS_Throw:
                fKeep = 1;
                break;
            case JTS_Unk09:
                SetJtAnimState(pjt, -1);
                ChooseJtRushTarget(pjt);
                paseg = pjt->pasegUnk1a7c;
                break;
            case JTS_Unk0D:
                SetJtAnimState(pjt, -1);
                ClearXyz(&pjt->vUnk190);
                ChooseJtSmashTarget(pjt);
                paseg = pjt->pasegUnk1a80;
                pjt->tUnk19b0 = D_002DDED0.t + D_002BF3E4;
                break;
            case JTS_Unk0E:
                SetJtAnimState(pjt, -1);
                ClearXyz(&pjt->vUnk190);
                paseg = pjt->pasegUnk1a84;
                break;
            case JTS_Unk0A:
                paseg = pjt->pasegblUnk1bc0;
                grfapl = 9;
                tStart = pjt->uGait * paseg->tMax;
                pjt->nUnk1038 = -1;
                break;
            case JTS_Pickpocket:
                paseg = pjt->pasegPickpocket;
                *(qword *)&pjt->vUnk190 = D_002A2DD0;
                pjt->grfUnk10e0 |= 2;
                grf = 100;
                pjt->nUnk1bd4 = 0;
                break;
            case JTS_Gait2:
                paseg = ((GAIT *)pjt->unk_0x1c30)->pasegbl;
                grfapl = 9;
                tStart = pjt->uGait * paseg->tMax;
                ClearGaitFootDown((GAIT *)pjt->unk_0x1c30);
                break;
            case JTS_Pounce:
                pjt->grfUnk10e0 |= 4;
                if (FPowerUpAcquired(0x1C))
                {
                    paseg = pjt->pasegPouncePowerUp;
                    UpdateJtPounceArc(pjt);
                    MATRIX3 *pmat = &pjt->matTarget;
                    qword qwVel = QwRotateVecRows(*(qword *)pmat->mat[0], *(qword *)pmat->mat[1], *(qword *)pmat->mat[2], QwScale(*(qword *)&pjt->vUnk190, VuFloat(-1.0f)));
                    pjt->SetVelocity(qwVel);
                    UpdateHeadingSmooth(pjt);
                }
                else
                {
                    paseg = pjt->pasegUnk1d1c;
                    qword posTarget = PosLoWorld(pjt->ploInteract);
                    float gZero = 0.0f;
                    qword dpos = QwSub(posTarget, PosLoWorld(pjt));
                    SetStepRadTowardDir(pjt, QwScale(dpos, VuFloat(-1.0f)));
                    UpdateHeadingSmooth(pjt);
                    float tLabel = TFindAsegLabel(paseg, (OID)0x173);
                    MATRIX4 matHand;
                    PredictAsegEffect(tLabel, gZero, paseg, (ALO *)pjt, pjt->psoUnk19a4->pxfRoot, pjt->pxfRoot, &matHand, 0);
                    tLabel /= paseg->svtMaster;
                    qword posHold = PosLoWorld(pjt->ploInteract);
                    qword posAim = QwSub(QwAddVelocity(VuFloatBits(tLabel), posHold, pjt->ploInteract->GetVelocity()), QwRotateVecV(*(qword *)&matHand.mat[3], &pjt->matTarget));
                    *(qword *)&pjt->vUnk190 = QwDivide(QwSub(posAim, PosLoWorld(pjt)), VuFloatBits(tLabel));
                    pjt->vUnk190.z = gZero;
                    pjt->SetVelocity(*(qword *)&pjt->vUnk190);
                }
                break;
            case JTS_Unk12:
                psm = pjt->psmUnk102c;
                SlotSetRole(pjt, pjt->pxfUnk58, 1);
                SlotSetRole(pjt, pjt->pxfUnk5c, 1);
                ClearXyz(&pjt->vUnk190);
                SetJtAnimState(pjt, -1);
            case JTS_Unk13:
                SlotSetRole(pjt, pjt->pxfUnk58, 1);
                SlotSetRole(pjt, pjt->pxfUnk5c, 1);
                ClearXyz(&pjt->vUnk190);
                SetJtAnimState(pjt, -1);
                break;
            case JTS_Unk14:
                SlotSetRole(pjt, pjt->pxfUnk58, 2);
                SlotSetRole(pjt, pjt->pxfUnk5c, 2);
                ClearXyz(&pjt->vUnk190);
                SetJtAnimState(pjt, -1);
                break;
            case JTS_Unk11:
                paseg = pjt->pasegUnk1d34;
                grfapl = 9;
                OnGameEvent(&D_002DEB30, 0x37, pjt);
                break;
            case JTS_SurfaceClimb:
                ((CLIMBSURF *)pjt->ploUnk1f18)->FindClosestPoint(PosLoWorld(pjt), 0, (SURFLOC *)&pjt->unk_0x1f1c, 0, 0);
                func_001F5720(pjt, 1);
                pjt->SetVelocity(D_002A2DD0);
                pjt->SetAngularVelocity(D_002A2DD0);
                ClearXyz(&pjt->vUnk190);
                pjt->ploUnk1f18->HandleSpliceEvent(5, 0, 0, 0, 0);
                if (*(TN **)((char *)pjt->ploUnk1f18 + 0x110) != 0)
                    SetPlayerTn(pjt, *(TN **)((char *)pjt->ploUnk1f18 + 0x110));
                break;
            case JTS_HangFollow:
                paseg = pjt->pasegblUnk1f90;
                grfapl = 9;
                SetJtAnimState(pjt, -1);
                func_001F5720(pjt, 1);
                pjt->SetVelocity(D_002A2DD0);
                pjt->SetAngularVelocity(D_002A2DD0);
                ClearXyz(&pjt->vUnk190);
                pjt->phndHang->HandleSpliceEvent(5, 0, 0, 0, 0);
                AddRefOidRef(pjt->phndHang, pjt);
                SetPlayerTn(pjt, pjt->phndHang->ptn);
                break;
            case JTS_Unk18:
            {
                float gZero = 0.0f;
                grfapl = 9;
                grf = 0x18;
                pjt->ablLean[2].gWeight = gZero;
                pjt->ablLean[0].gWeight = gZero;
                pjt->ablLean[1].gWeight = 1.0f;
                ReblendAsegbl(pjt->pasegblUnk2038);
                paseg = pjt->pasegblUnk2038;
                SetStepRadTarget(pjt, pjt->pctl->radLeft);
                qword qwVel = MakeVecFromAngle(pjt->radTarget, D_002BF3E8, gZero);
                pjt->SetVelocity(qwVel);
                ClearXyz(&pjt->vUnk190);
            }
            break;
        }
    }
    if (pjt->jtbs != jtbs)
    {
        switch (jtbs)
        {
            case JTBS_Nil:
                break;
            case JTBS_0:
            case JTBS_1:
            case JTBS_2:
            {
                ASEG *pasegHang = pjt->pasegHang;
                if (pjt->pasegaUnk1030 != 0 && pjt->pasegaUnk1030->paseg == pasegHang)
                {
                    fKeep = 1;
                }
                else
                {
                    paseg = pasegHang;
                    svt = 0.0f;
                    tStart = paseg->tMax * 0.5f;
                }
            }
            break;
            case JTBS_3:
                psm = pjt->janim.psmHang;
                break;
            case JTBS_4:
                psm = pjt->psmUnk1bc4;
                ClearXyz(&pjt->vUnk190);
                break;
            case JTBS_5:
                pjt->nUnk1038 = -1;
                paseg = pjt->pasegblUnk14d0;
                ClearXyz(&pjt->vUnk190);
                grfapl = 9;
                break;
            case JTBS_6:
                SetJtAnimState(pjt, -1);
                paseg = pjt->apasegSwing[10];
                ClearXyz(&pjt->vUnk190);
                pjt->janim.fLandAlt = 0;
                pjt->janim.fInputOpen = 0;
                break;
            case JTBS_7:
                SetJtAnimState(pjt, -1);
                paseg = pjt->apasegSwing[11];
                ClearXyz(&pjt->vUnk190);
                pjt->janim.fLandAlt = 0;
                pjt->janim.fInputOpen = 0;
                break;
            case JTBS_8:
            case JTBS_9:
                if (jtbs == JTBS_8)
                    paseg = pjt->apasegSwing[12];
                else
                    paseg = pjt->pasegUnk1fe8;
                pjt->janim.fInputOpen = 0;
                ClearXyz(&pjt->vUnk190);
                break;
            case JTBS_10:
                psm = pjt->psmUnk1f14;
                break;
            case JTBS_11:
                paseg = pjt->pasegblUnk1f10;
                grfapl = 9;
                break;
            case JTBS_12:
                pjt->rUnk200 = D_002BF358;
                grf = 0x18;
                break;
            case JTBS_13:
                fKeep = 1;
                pjt->rUnk200 = D_002BF35C;
                grf = 0x18;
                break;
        }
    }
    if (jts != pjt->jts)
    {
        pjt->jts = jts;
        pjt->tJts = D_002DDED0.t;
    }
    if (jtbs != pjt->jtbs)
    {
        pjt->jtbs = jtbs;
        pjt->tJtbs = D_002DDED0.t;
    }
    pjt->VUnk89();
    SetJtChannelMask(pjt, grf);
    if (sfxid != -1)
    {
        SND *psnd = MakeSnd(sfxid);
        SetSndOwner(psnd, 0);
        func_001FB3F0(psnd, PosLoWorld(pjt));
    }
    if (!fKeep)
    {
        if (pjt->pasegaUnk1030 != 0)
        {
            RetractAsega(pjt->pasegaUnk1030);
            pjt->pasegaUnk1030->VUnk01();
            pjt->pasegaUnk1030 = 0;
        }
        else if (pjt->psmaUnk1034 != 0)
        {
            RetractSma(pjt->psmaUnk1034);
            pjt->psmaUnk1034->VUnk01();
            pjt->psmaUnk1034 = 0;
        }
        if (paseg != 0)
        {
            ApplyAseg(paseg, (ALO *)pjt, tStart, svt, grfapl, &pjt->pasegaUnk1030);
            pjt->pasegaUnk1030->VUnk00();
        }
        else if (psm != 0)
        {
            pjt->psmaUnk1034 = PsmaApplySm(psm, (ALO *)pjt, oid, grfapl, grfRoute);
            pjt->psmaUnk1034->VUnk00();
        }
    }
}
