// GatherJtGrabCandidates: ported from Lilly's personal Sly 2 decomp (src/P2/jtgrab.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BF660 (D_002BF660), D_002BF664 (D_002BF664), D_002BF70C (D_002BF70C), D_003DA0C0 (g_hintbox), D_003DD490 (g_sw)
// 0x00194A40 GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi  (MATCHES Sly 2; our name for the record)
// Every frame (UpdateJtPre 0x18B7E8 asks with 0xFFFF; the others with a mask the current state allows): collects the hero's grab
// candidates. grf says which of the world's grab lists to walk: 1 the HNDs (found in the TARGET list), 2 PIPEs, 4 class 27, 8 HSHAPEs,
// 0x10 class 34, 0x20 class 38, 0x80 class 35, 0x200 RAILs, 0x400 class 39. 0x195368 and 0x194F70 then refine the four best, and the
// hint box (the popup at the top of the screen) shows the hint text of the best one's spot (nUnk14) while it is within 250 - or the
// text 0x125 for an object of oid 0x624 that has been the best for more than a second.
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/stepplayer.h"
#include "sly2/sw.h"
typedef unsigned  size_t;
struct SLOT
{
      SLOT *pslotNext;
};
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
qword PosLoWorld(LO *plo);
struct TASKPAIR
{
    OID oidTask;
    int nState;
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
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
};
struct REGNODE
{
      VECTOR4 *pplane;
      unsigned short aiChild[2];
};
struct REGTREE
{
      int cnode;
      REGNODE *anode;
      int cplane;
      VECTOR4 *aplane;
      int ccid17;
      OID *aoid;
      CID17 **apcid17;
      REGLINKS *alinks;
      int ccid124;
      CID124 **apcid124;
      int nVersion;
      int fReady;
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
void UpdateJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand);
void EmitJtGrabSparkles(SW *psw, STEPPLAYER *pstepplayer, int ccand, JTGRABCAND *ajtgrabcand);
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
struct CID34 : public XFM, public GRABSPOT
{
      DLE dle;
      float sUnk10c;
};
struct CID38 : public XFM, public GRABSPOT
{
      DLE dle;
      int nGrabState;
};
struct CID11 : public SHAPE, public GRABSPOT
{
      DLE dle;
      undefined4 unk_0x1bc;
      qword qwUnk1c0;
      int nUnk1d0;
      int nUnk1d4;
      undefined4 unk_0x1d8[2];
};
struct CID12 : public SHAPE, public GRABSPOT
{
      DLE dle;
      int nUnk1bc;
};
struct CID13 : public SHAPE, public GRABSPOT
{
      DLE dle;
      undefined4 unk_0x1bc;
      float gUnk1c0;
      float gUnk1c4;
      undefined4 unk_0x1c8[2];
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
struct CID27 : public VOL, public GRABSPOT
{
      DLE dle;
      LO *ploUnk15c;
};
struct CID39 : public XFM, public GRABSPOT
{
      DLE dle;
      undefined4 unk_0x10c;
};
struct CID35 : public XFM, public GRABSPOT
{
      DLE dle;
      LO *ploUser;
      float sUnk110;
      LO *ploRelay;
      int crsmg;
      RSMG arsmg[4];
      int ctaskpair;
      TASKPAIR ataskpair[4];
};
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
struct PROXYSPARE
{
      struct PROXY *pproxy;
      PROXYSPARE *pspareNext;
      int fLocked;
      undefined4 unk_0xc;
      MATRIX4 mat;
};
struct PROXYINIT
{
      int kind;
      CID cid;
      CFrame *pframe;
      int cb;
      void *pvOptions;
      LO *plo;
};
PROXY *PproxyFromLo(LO *plo);
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
extern "C" {
int FIsSwHero(STEPPLAYER *pstepplayer);
}
extern "C" {
int GetHintBoxTextId(HINTBOX *phintbox);
}
extern "C" {
void SetHintBoxText(HINTBOX *phintbox, int idText, float dt);
}
extern "C" {
void ClearIfSlotId(HINTBOX *phintbox, int idText);
}
extern float D_002BF660;
extern float D_002BF664;
extern float D_002BF70C;
extern HINTBOX D_003DA0C0;
void AddJtGrabCandidate(SW *psw, STEPPLAYER *pstepplayer, qword qwPosJt, LO *plo, GRABSPOT *pspot, int ccandMax, JTGRABCAND *ajtgrabcand);
void GatherJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, uint grf) __asm__("GatherJtGrabCandidates__FP2SWP10STEPPLAYERUi");
void GatherJtGrabCandidates(SW *psw, STEPPLAYER *pstepplayer, uint grf)
{
    JTGRABCAND ajtgrabcand[4];
    if (!FIsSwHero(pstepplayer))
        return;
    memset(ajtgrabcand, 0, sizeof(ajtgrabcand));
    uint i = 0;
    do
    {
        ajtgrabcand[i].nPriority = -1;
        ajtgrabcand[i].sDist = D_002BF664;
        i++;
    } while (i < 4);
    qword qwPosJt = PosLoWorld(pstepplayer);
    if (grf & 8)
    {
        DL *pdlpcid12 = &D_003DD490.dlCid12;
        for (CID12 *pcid12 = (CID12 *)pdlpcid12->head; pcid12; pcid12 = (CID12 *)pcid12->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid12, pcid12, 4, ajtgrabcand);
    }
    if (grf & 0x10)
    {
        DL *pdlpcid34 = &D_003DD490.dlCid34;
        for (CID34 *pcid34 = (CID34 *)pdlpcid34->head; pcid34; pcid34 = (CID34 *)pcid34->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid34, pcid34, 4, ajtgrabcand);
    }
    if (grf & 0x80)
    {
        DL *pdlpcid35 = &D_003DD490.dlCid35;
        for (CID35 *pcid35 = (CID35 *)pdlpcid35->head; pcid35; pcid35 = (CID35 *)pcid35->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid35, pcid35, 4, ajtgrabcand);
    }
    if (grf & 4)
    {
        DL *pdlpcid27 = &D_003DD490.dlCid27;
        for (CID27 *pcid27 = (CID27 *)pdlpcid27->head; pcid27; pcid27 = (CID27 *)pcid27->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid27, pcid27, 4, ajtgrabcand);
    }
    if (grf & 0x400)
    {
        DL *pdlpcid39 = &D_003DD490.dlCid39;
        for (CID39 *pcid39 = (CID39 *)pdlpcid39->head; pcid39; pcid39 = (CID39 *)pcid39->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid39, pcid39, 4, ajtgrabcand);
    }
    if (grf & 0x200)
    {
        DL *pdlpcid13 = &D_003DD490.dlCid13;
        for (CID13 *pcid13 = (CID13 *)pdlpcid13->head; pcid13; pcid13 = (CID13 *)pcid13->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid13, pcid13, 4, ajtgrabcand);
    }
    if (grf & 2)
    {
        DL *pdlpcid11 = &D_003DD490.dlCid11;
        for (CID11 *pcid11 = (CID11 *)pdlpcid11->head; pcid11; pcid11 = (CID11 *)pcid11->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid11, pcid11, 4, ajtgrabcand);
    }
    if (grf & 1)
    {
        DL *pdlTarget = &D_003DD490.dlTarget;
        for (TARGET *ptarget = (TARGET *)pdlTarget->head; ptarget; ptarget = (TARGET *)ptarget->dleTarget.next)
        {
            if ((ptarget->grftak & 1) && FIsBasicDerivedFrom(ptarget, (CID)19))
                AddJtGrabCandidate(psw, pstepplayer, qwPosJt, ptarget, (CID19 *)ptarget, 4, ajtgrabcand);
        }
    }
    if (grf & 0x20)
    {
        DL *pdlpcid38 = &D_003DD490.dlCid38;
        for (CID38 *pcid38 = (CID38 *)pdlpcid38->head; pcid38; pcid38 = (CID38 *)pcid38->dle.next)
            AddJtGrabCandidate(psw, pstepplayer, qwPosJt, pcid38, pcid38, 4, ajtgrabcand);
    }
    int ccand;
    for (ccand = 4; ccand > 0 && !ajtgrabcand[ccand - 1].plo; ccand--)
        ;
    SW *pswG = &D_003DD490;
    EmitJtGrabSparkles(pswG, pstepplayer, ccand, ajtgrabcand);
    UpdateJtGrabCandidates(pswG, pstepplayer, ccand, ajtgrabcand);
    int idHintOld = GetHintBoxTextId(&D_003DA0C0);
    if (idHintOld != -1 && idHintOld != pswG->idHintGrab)
        return;
    int idHint;
    int fFar;
    if (pswG->cjtgrabcand == 0)
        idHint = -1;
    else if (fFar = D_002BF660 < GLength(QwSub(qwPosJt, pswG->ajtgrabcand[0].qwPos)), fFar)
        idHint = -1;
    else
    {
        PROXY *pproxy = pswG->ajtgrabcand[0].ploUnk10 ? PproxyFromLo(pswG->ajtgrabcand[0].ploUnk10) : 0;
        if (pproxy && pproxy->oid == 0x624 && D_002BF70C < pswG->ajtgrabcand[0].tUnk14)
            idHint = 0x125;
        else
            idHint = D_003DD490.ajtgrabcand[0].pspot->nUnk14;
    }
    if (idHint != idHintOld)
    {
        if (idHint == -1)
            ClearIfSlotId(&D_003DA0C0, D_003DD490.idHintGrab);
        else
            SetHintBoxText(&D_003DA0C0, idHint, -1.0f);
        D_003DD490.idHintGrab = idHint;
    }
}
