// func_00195A00: ported from Lilly's personal Sly 2 decomp (src/P2/jtair.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF750 (D_002BF750), D_002BF754 (D_002BF754), D_002BF758 (D_002BF758), D_002BF75C (D_002BF75C), D_002BF760 (D_002BF760), D_002BF764 (D_002BF764), D_002BF76C (D_002BF76C), D_002BF770 (D_002BF770), D_002BF730 (g_SheetMovementMiscJumpHorizontalSpeed), D_002BF768 (g_SheetMovementMiscTurnbuckleLaunchHorizontalSpeed), D_002DDED0 (g_clock), D_002A2DC0 (g_normalZ)
// 0x00195A00 SetJtAirJump (curated name): starts a flight of the given kind. n2 picks the directional clip; plo is the object the sub-state works on.
// Every kind but the double jump (6) restarts the record (its timers, zStart, the JT's landing clue). The launch velocity comes from JtPlanLaunchVz; kinds
// 3 / 0xE add a push along the facing when the stick is at rest, 0x11 (wall run) flattens it and adds 400 up. Then the sub-state (SetJtAirState) and the
// per-kind ratios.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/cm.h"
extern "C" qword MakeVecFromAngle(float rad, float sXY, float sZ);
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
qword PosLoWorld(LO *plo);
extern CLOCK D_002DDED0;
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
void SetStepGravityRatio(STEP *pstep, float rGravity);
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
void SetJtAirState(JTAIR *pjtair, int nState, LO *plo);
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
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
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
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
}
qword JtPlanLaunchVz(JTAIR *pjtair, int nKind);
void SetPlayerTn(STEPPLAYER *pstepplayer, TN *ptn);
void DecomposeCylind(qword v, float *prad, float *psXY, float *pz);
extern qword D_002A2DC0;
extern qword D_002BF770;
extern float D_002BF750;
extern float D_002BF754;
extern float D_002BF758;
extern float D_002BF75C;
extern float D_002BF760;
extern float D_002BF764;
extern float D_002BF76C;
extern float D_002BF730;
extern float D_002BF768;
static inline qword QwProjectOut(qword v, qword n)
{
    qword qwDot = QwDot(v, n);
    qword qw;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(qw) : "j"(qwDot), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(v), "j"(qw));
    return qw;
}
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo) __asm__("func_00195A00");
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo)
{
    qword vel;
    qword velWall;
    float t;
    if (nKind != 6)
        pjtair->nKind = nKind;
    pjtair->n2 = n2;
    pjtair->pstepplayer->VUnk87(1);
    if (nKind != 6)
    {
        pjtair->nUnk40 = 0;
        t = D_002DDED0.t;
        pjtair->tUnk4c = t;
        pjtair->tUnk48 = t;
        pjtair->tUnk44 = t;
    }
    if (nKind == 9)
    {
        pjtair->rGravitySaved = pjtair->pstepplayer->rUnk260;
        SetStepGravityRatio(pjtair->pstepplayer, D_002BF750);
    }
    vel = JtPlanLaunchVz(pjtair, nKind);
    pjtair->pstepplayer->SetVelocity(vel);
    t = D_002DDED0.t;
    pjtair->pstepplayer->tUnk131c = t;
    if (nKind != 6)
        pjtair->tStart = D_002DDED0.t;
    switch (nKind)
    {
        case 0:
            SetJtAirState(pjtair, 4, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0xB:
            SetJtAirState(pjtair, 4, plo);
            pjtair->fUnk24 = 1;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 3:
            if (pjtair->pstepplayer->pctl->sLeft < 0.2f)
            {
                qword dvel;
                pjtair->nUnk40 = 3;
                dvel = MakeVecFromAngle(pjtair->pstepplayer->radTarget, D_002BF754, 0.0f);
                pjtair->pstepplayer->SetVelocity(QwAdd(vel, dvel));
            }
            SetJtAirState(pjtair, 0, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 0.6f;
            pjtair->fUnk24 = 0;
            break;
        case 1:
        case 2:
        case 0x12:
            SetJtAirState(pjtair, 0, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 9:
            SetJtAirState(pjtair, 0, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 1;
            SetPlayerTn(pjtair->pstepplayer, pjtair->ptnLaunch);
            pjtair->gSpeedRatio = D_002BF758;
            pjtair->gUnk34 = D_002BF75C;
            pjtair->fGravityChanged = 1;
            break;
        case 7:
            SetJtAirState(pjtair, 7, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gSpeedRatio = 1.0f;
            pjtair->gUnk34 = 1.0f;
            break;
        case 8:
            SetJtAirState(pjtair, 5, plo);
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            pjtair->fCanDoubleJump = 1;
            DecomposeCylind(vel, &pjtair->radSmash, &pjtair->sSmash, 0);
            pjtair->gUnkb4 = 1.0f;
            break;
        case 6:
        {
            int nState = 2;
            if (pjtair->nKind < 0xF)
            {
                if (pjtair->nKind > 0xC)
                    nState = 3;
            }
            SetJtAirState(pjtair, nState, plo);
            break;
        }
        case 0xE:
        {
            float gRatio;
            SetJtAirState(pjtair, 1, plo);
            gRatio = D_002BF760;
            pjtair->fCanDoubleJump = 1;
            pjtair->gUnk34 = D_002BF764;
            pjtair->fUnk24 = 0;
            pjtair->gSpeedRatio = gRatio;
            if (pjtair->pstepplayer->pctl->sLeft < 0.2f)
            {
                qword dvel;
                pjtair->nUnk40 = 3;
                dvel = MakeVecFromAngle(pjtair->pstepplayer->pctl->radLeft, D_002BF730 * gRatio, 0.0f);
                pjtair->pstepplayer->SetVelocity(QwAdd(vel, dvel));
            }
            break;
        }
        case 0xD:
            SetJtAirState(pjtair, 1, plo);
            pjtair->fCanDoubleJump = 1;
            pjtair->gSpeedRatio = D_002BF768;
            pjtair->gUnk34 = D_002BF76C;
            pjtair->fUnk24 = 0;
            break;
        case 0xA:
            pjtair->nUnk40 = -1;
            SetJtAirState(pjtair, 0x15, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 0.0f;
            pjtair->gSpeedRatio = 0.0f;
            break;
        case 0xC:
            SetJtAirState(pjtair, 1, plo);
            pjtair->nUnk40 = 3;
            pjtair->fCanDoubleJump = 1;
            pjtair->fUnk24 = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0x10:
            SetJtAirState(pjtair, 4, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0xF:
            SetJtAirState(pjtair, 0, plo);
            pjtair->fUnk24 = 0;
            pjtair->fCanDoubleJump = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
        case 0x11:
            if (pjtair->nState == -1)
            {
                pjtair->pstepplayer->SetVelocity(QwAdd(QwProjectOut(velWall, D_002A2DC0), D_002BF770));
                pjtair->fCanDoubleJump = 1;
            }
            SetJtAirState(pjtair, 0x16, 0);
            pjtair->fUnk24 = 0;
            pjtair->fUnk2c = 0;
            pjtair->gUnk34 = 1.0f;
            pjtair->gSpeedRatio = 1.0f;
            break;
    }
    if (nKind != 6)
    {
        if (nKind != 0x11)
            pjtair->fUnk2c = pjtair->fCanDoubleJump;
        if (FIsBasicDerivedFrom(pjtair->pstepplayer, (CID)0x35))
            ((JT *)pjtair->pstepplayer)->janim.nUnk10 = -1;
        pjtair->zStart = GZOfQw(PosLoWorld(pjtair->pstepplayer));
    }
}
