// FCanUseGadget__2JTi: ported from Lilly's personal Sly 2 decomp (src/P2/jtgadget.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF3EC (g_SheetMiscVariablesStatesThiefReflexesClock), D_002BF3F0 (g_SheetMiscVariablesStatesTimeRushClock), D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002DFBC0 (g_grfjoyt)
// 0x0018D760 FCanUseGadget__2JTi  (T1329)  Virtual 98: may Sly use the gadget in the state he is in? Nothing while the look mode runs (FJtModeActive). Thief reflexes (0x1D) and time rush (0x2A)
// work in every state but jts 4 (hurt), and only while the clock's power-up rate is 1 (nothing runs) or is this gadget's own rate (the press that ends it). Else the state decides: the
// air / grab / pipe records say (FJtairCanUseGadget, FJtgrabCanUseGadget, FJtpipeCanUseGadget), hang (3) allows only the look mode, the ground states (0, 1, 5, 10, 0xC) allow the
// gadget tests below, the throw state (0x17, aiming) allows only the pocket item he carries; every other state allows nothing. The gadget tests: 1, 2, 0x19, 0x1E, 0x1F, 0x21, 0x22,
// 0x23, 0x27 always; 3 needs the pad flag and the camera policy on top; 0x17 needs its proxy to have a spare; 0x18 only in jts 5 (fight stance), 0x1B only in jts 2 (air); 0x28 only on the ground
// with no animation state, no interaction (nInteractMode 4) and nUnk1d30 clear; the pocket items (0x1A, 0x25, 0x26, 0x29): an item in use must be carried, else the row's proxy needs a spare.
// (jump2: the failing call tests, the throw state and the pocket arm share the return-0 block because `return 1` is the LAST arm and the pocket arm falls into `default:`.)
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/cm.h"
extern CLOCK D_002DDED0;
struct SUBGLOBX
{
      VECTOR posCenter;
      float sRadius;
      float gUnk10;
      uint grfUnk14;
};
extern int D_002DFBC0;
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
int FJtairCanUseGadget(JTAIR *pjtair, int idGadget);
int FJtgrabCanUseGadget(JTGRAB *pjtgrab, int idGadget);
int FJtpipeCanUseGadget(JTPIPE *pjtpipe, int idGadget);
bool FJtModeActive(STEPPLAYER *pstepplayer);
struct PROXYSPARE
{
      struct PROXY *pproxy;
      PROXYSPARE *pspareNext;
      int fLocked;
      undefined4 unk_0xc;
      MATRIX4 mat;
};
PROXYSPARE *PspareChoose(PROXY *pproxy, int rankMin);
int FCanAcquireProxy(PROXY *pproxy);
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
extern float D_002BF3EC;
extern float D_002BF3F0;
int FPocketItemCarried(CID100 *pcid100);
JTPOCKET *FindJtPocket(JT *pjt, int idGadget);
/** @sly2-annot addr=0x18D760
 * Purpose: Can Sly use gadget id now (per-state allow lists; aiming state 0x17 allows only carried pocket
 *   item)
 * Info: proposed name JT_FCanUseGadget (T1); class JT slot99; TU jt
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
int JT::FCanUseGadget(int idGadget)
{
    if (FJtModeActive(this))
        return 0;
    if (idGadget == 0x1D || idGadget == 0x2A)
    {
        if (jts == JTS_Unk04)
            return 0;
        if (D_002DDED0.art[CRK_PowerUp] == 1.0f)
            return 1;
        return D_002DDED0.art[CRK_PowerUp] == (idGadget == 0x1D ? D_002BF3EC : D_002BF3F0);
    }
    switch (jts)
    {
        case JTS_Air:
            if (!FJtairCanUseGadget(&jtair, idGadget))
                return 0;
            break;
        case JTS_Grab:
            if (!FJtgrabCanUseGadget(&jtgrab, idGadget))
                return 0;
            break;
        case 8:
            if (!FJtpipeCanUseGadget(&jtpipe, idGadget))
                return 0;
            break;
        case 3:
            if (idGadget != 3)
                return 0;
            break;
        case 0:
        case 1:
        case 5:
        case 10:
        case 12:
            break;
        case JTS_Throw:
            if (jtthrow.nState - 4U < 3)
            {
                switch (idGadget)
                {
                    case 0x1A:
                    case 0x25:
                    case 0x26:
                    case 0x29:
                        if (((CID100 *)jtthrow.ploCarried)->idGadget == idGadget)
                            return 1;
                }
            }
            return 0;
        default:
            return 0;
    }
    switch (idGadget)
    {
        case 0x17:
            return pproxyUnk2010 != 0 && FCanAcquireProxy(pproxyUnk2010);
        case 0x18:
            return jts == 5;
        case 0x1B:
            return jts == 2;
        case 0x1A:
        case 0x25:
        case 0x26:
        case 0x29:
        {
            JTPOCKET *ppocket = FindJtPocket(this, idGadget);
            if (ppocket->pcid100)
            {
                if (FPocketItemCarried(ppocket->pcid100))
                    return 1;
            }
            else
            {
                if (ppocket->pproxy == 0)
                    return 0;
                if (PspareChoose(ppocket->pproxy, 2))
                    return 1;
            }
        }
        default:
            return 0;
        case 3:
            return nUnk12f0 != 0 && (D_002DFBC0 & 2) && !D_002DDF40.part3a0.FIsTop();
        case 0x28:
            if (nUnk1d30 != 0)
                return 0;
            if (nInteractMode == 4)
                return 0;
            if (jtas != -1)
                return 0;
            switch (jts)
            {
                case 0:
                case 1:
                case 10:
                case 12:
                    return 1;
            }
            return 0;
        case 1:
        case 2:
        case 0x19:
        case 0x1E:
        case 0x1F:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x27:
            return 1;
    }
}
