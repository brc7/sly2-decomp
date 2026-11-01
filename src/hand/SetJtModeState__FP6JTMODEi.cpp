// SetJtModeState: ported from Lilly's personal Sly 2 decomp (src/P2/jtmode.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002A2DD0 (g_qwZero), D_003DD040 (g_sndmgr), D_003DD490 (g_sw)
// 0x0019CD38 SetJtModeState__FP6JTMODEi  (our name)
// Leaving a state: from off (-1) the mode takes hold of the hero's own animation (held at speed 0; kind 1 also counts a no-pause request in the world); from running (1) the
// camera policy is taken off the camera's list (kind 1 also lets the held object go), the camera cuts, the sound buses return to full volume and the hero
// is let go (virtual 86). Entering a state: starting (0) plays the start clip, running (1) pushes the camera policy (CPLOOK priority 7 for kinds 0 and 2, the recon
// camera priority 8 for kind 1), turns the hero to its look direction, stops him and the held object and lowers sound buses 0 and 1 to 0.6; ending (2) plays the end clip and
// off (-1) gives the hero's animation back. The overlay (hud) follows the same states.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/cm.h"
extern qword D_002A2DD0;
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
struct CAMEVENT
{
      uint32_t   _pad_376    [ 3 ] ;
};
struct CAMPOSE
{
      VECTOR4 pos;
      VECTOR4 eul;
      float radFov;
};
struct CAMMOVE
{
      CAMPOSE *pposeFrom;
      CAMPOSE *pposeTo;
      float dt;
      int fCutOnLeave;
      int iCurve;
      CAMEVENT *pevent;
      int nUnk18;
};
extern CM D_002DDF40;
void CutCm(CM *pcm);
void PushHeroCameraMode(CM *pcm, int nPriority, void *pvMode, int nUnk, int nUnk2);
extern "C" void PopHeroCameraMode(CM *pcm, int grf, int nPriority, void *pvMode, void *pvHero, void *pvOwner);
extern SW D_003DD490;
void IncrementSwNoPause(SW *psw);
void DecrementSwNoPause(SW *psw);
void SetStepRadTarget(STEP *pstep, float radTarget);
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
void PlayJtAseg(ASEG *paseg, ALO *paloRoot, float tLocal, float svtLocal, GRFAPL grfapl, ASEGA **ppasega);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
void SetAsegaSpeed(ASEGA *pasega, float svt);
void SetSoundBusVolume(SNDMGR *psndmgr, int iBus, float gVolume, int iSlot);
extern "C" {
void func_0020F198(SW *psw);
}
extern "C" {
void func_0020F1A8(SW *psw);
}
extern "C" {
void SetSoCnstrForce(SO *pso, int fOn);
}
extern SNDMGR D_003DD040;
static inline void ClearXyz(VECTOR *pvec)
{
    pvec->z = 0.0f;
    pvec->y = 0.0f;
    pvec->x = 0.0f;
}
void SetJtModeState(JTMODE *pjtmode, int nState) __asm__("SetJtModeState__FP6JTMODEi");
/** @sly2-annot addr=0x19CD38
 * Purpose: Leave/enter mode states -1 off, 0 starting, 1 running (push CPLOOK prio 7 for kinds 0/2, recon
 *   camera prio 8 for kind 1, stop hero, duck sound buses 0/1 to 0.6), 2 ending; HUD follows
 * Info: proposed name SetJtModeState (T1); TU mode
 * Notes: runtime6 C7/C8: kind 1 (recon) pushes CPZOOM at CM+0x520 prio 8 + IncrementSwNoPause (S57)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r4b_rest+runtime
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
void SetJtModeState(JTMODE *pjtmode, int nState)
{
    ASEG *paseg;
    if (nState == pjtmode->nState)
        return;
    switch (pjtmode->nState)
    {
        case 1:
            switch (pjtmode->nKind)
            {
                case 0:
                    PopHeroCameraMode(&D_002DDF40, 0xF, 7, &D_002DDF40.cplook, pjtmode->pstepplayer, 0);
                    break;
                case 1:
                    PopHeroCameraMode(&D_002DDF40, 0xF, 8, &D_002DDF40.cpzoom, pjtmode->pstepplayer, 0);
                    {
                        STEP *pstep = pjtmode->pstepOther;
                        if (pstep)
                        {
                            pstep->VUnk83(0x3F, 1);
                            SetSoCnstrForce(pstep, 0);
                        }
                    }
                    pjtmode->pstepplayer->VUnk83(0x3F, 0);
                    break;
                case 2:
                    PopHeroCameraMode(&D_002DDF40, 0xF, 7, &D_002DDF40.cplook, pjtmode->pstepOther, 0);
                    break;
            }
            CutCm(&D_002DDF40);
            SetSoundBusVolume(&D_003DD040, 0, 1.0f, 3);
            SetSoundBusVolume(&D_003DD040, 1, 1.0f, 3);
            if (pjtmode->nKind != 2)
                pjtmode->pstepplayer->VUnk86(0);
            pjtmode->nState = nState;
            break;
        case -1:
        {
            if (pjtmode->nKind == 1)
            {
                func_0020F198(&D_003DD490);
                IncrementSwNoPause(&D_003DD490);
            }
            STEPPLAYER *pstepplayer = pjtmode->pstepplayer;
            if (pstepplayer->pasegaUnk1030)
                pjtmode->pasegaHold = pstepplayer->pasegaUnk1030;
            else if (pstepplayer->psmaUnk1034)
                pjtmode->pasegaHold = pstepplayer->psmaUnk1034->pasegaCur;
            if (pjtmode->pasegaHold)
            {
                pjtmode->pasegaHold->VUnk00();
                SetAsegaSpeed(pjtmode->pasegaHold, 0.0f);
            }
            pjtmode->nState = nState;
            break;
        }
        default:
            pjtmode->nState = nState;
            break;
    }
    pjtmode->tEnter = D_002DDED0.t;
    paseg = 0;
    switch (nState)
    {
        case 0:
            paseg = pjtmode->pasegStart;
            break;
        case 1:
            switch (pjtmode->nKind)
            {
                case 0:
                    PushHeroCameraMode(&D_002DDF40, 7, &D_002DDF40.cplook, (int)pjtmode->pstepplayer, 0);
                    SetStepRadTarget(pjtmode->pstepplayer, *(float *)((char *)&D_002DDF40 + 0x460));
                    pjtmode->hud.SetState(0);
                    break;
                case 1:
                    PushHeroCameraMode(&D_002DDF40, 8, &D_002DDF40.cpzoom, (int)pjtmode->pstepplayer, 0);
                    SetStepRadTarget(pjtmode->pstepplayer, *(float *)((char *)&D_002DDF40 + 0x5C4));
                    {
                        STEP *pstep = pjtmode->pstepOther;
                        if (pstep)
                        {
                            pstep->VUnk82(0x3F, 1);
                            SetSoCnstrForce(pstep, 1);
                            ClearXyz(&pstep->vUnk190);
                            pstep->SetVelocity(D_002A2DD0);
                            pstep->SetAngularVelocity(D_002A2DD0);
                        }
                    }
                    pjtmode->hud.SetWidgetCur(pjtmode->ploHudOverride);
                    pjtmode->hud.SetState(0);
                    pjtmode->pstepplayer->VUnk82(0x3F, 0);
                    break;
                case 2:
                    PushHeroCameraMode(&D_002DDF40, 7, &D_002DDF40.cplook, (int)pjtmode->pstepOther, 0);
                    pjtmode->hud.SetState(0);
                    break;
            }
            ClearXyz(&pjtmode->pstepplayer->vUnk190);
            pjtmode->pstepplayer->SetVelocity(D_002A2DD0);
            pjtmode->pstepplayer->SetAngularVelocity(D_002A2DD0);
            CutCm(&D_002DDF40);
            SetSoundBusVolume(&D_003DD040, 0, 0.6f, 3);
            SetSoundBusVolume(&D_003DD040, 1, 0.6f, 3);
            if (pjtmode->nKind != 2)
                pjtmode->pstepplayer->VUnk86(1);
            break;
        case 2:
            pjtmode->hud.SetState(2);
            paseg = pjtmode->pasegEnd;
            break;
        case -1:
            switch (pjtmode->nKind)
            {
                case 0:
                case 2:
                    pjtmode->hud.SetState(-1);
                    break;
                case 1:
                    pjtmode->hud.SetState(-1);
                    func_0020F1A8(&D_003DD490);
                    DecrementSwNoPause(&D_003DD490);
                    break;
            }
            if (pjtmode->pasegaHold)
            {
                SetAsegaSpeed(pjtmode->pasegaHold, 1.0f);
                pjtmode->pasegaHold->VUnk01();
                pjtmode->pasegaHold = 0;
            }
            pjtmode->nKind = -1;
            break;
    }
    if (paseg && !pjtmode->pstepplayer->VUnk93())
        paseg = 0;
    PlayJtAseg(paseg, (ALO *)pjtmode->pstepplayer, 0.0f, 1.0f, 1, &pjtmode->pasega);
}
