// NextJsgCommand: ported from Lilly's personal Sly 2 decomp (src/P2/jsg.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002A7AD0 (D_002A7AD0), D_002DE570 (D_002DE570), D_003DA530 (D_003DA530), D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002DEB30 (g_game), D_003DA0C0 (g_hintbox), D_002DFC00 (g_joy), D_002A2DC0 (g_normalZ), D_002A2DD0 (g_qwZero), D_003DD040 (g_sndmgr), D_004435F0 (g_subtitle), D_003DD490 (g_sw)
// 0x001E1E30 NextJsgCommand__FP3JSG  (T1200; was DispatchAsegEvent: it is Sly 1's NextJsgJsge)
// Steps the script to the next command of its block and executes it. First the command's actor is chosen (the oid the command carries, else the
// SetActor default, the block's object or the relay's firer), then the command runs by kind (the table in re/notes/relay-commands-t1289.md), and
// the time it started is noted (the relay's busy bit 0x100 picks the real-time clock). A reference is held on the script meanwhile: a command can
// end it (StopBlock on itself).
#include "sly2/basic.h"
#include "sly2/lo.h"
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/po.h"
#include "sly2/step.h"
#include "sly2/aseg.h"
#include "sly2/stepplayer.h"
#include "sly2/jt.h"
#include "sly2/players.h"
#include "sly2/cm.h"
typedef unsigned  size_t;
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
extern "C" {
void* memset(void* ptr, int value, size_t num);
}
inline int GrfUpdateLo(LO *plo)
{
    int grf = plo->grfUnk40;
    if (!(grf & 1))
    {
        return *(int *)grf;
    }
    return grf;
}
qword PosLoWorld(LO *plo);
SMA *PsmaFindOnAlo(ALO *paloRoot, OID oidSm);
int FIsSmaRetracted(SMA *psma);
OID GetSmaCur(SMA *psma);
struct AMB
{
    SFXID sfxid;
    int iSerial;
    void *bnk;
    AMB **ppamb;
    float tStarted;
    float uVolAtSource;
    uint sfxh;
    undefined4 unk_1;
    VECTOR pos;
    ALO *palo;
    float sStart;
    float sFull;
    int fContinuous;
    int fOutOfRange;
    int fAttached;
    int fStopped;
    float volAttenuated;
    float pan;
    float frq;
    float rDoppler;
    int dnDoppler;
    int fPitchOrig;
    int ptchOrig;
    float tNext;
    LM lmRepeat;
    LM lmRepDist;
    undefined4 unk_2;
    undefined4 unk_3;
    undefined4 unk_4;
};
struct EXC
{
    int iexc;
    DLE dle;
};
struct GAME
{
      float tIdleEvent;
      float tCmTurned;
      int nTaskStateDenied;
      int idGadgetLast;
      int oidJobCur;
      int oidTaskCur;
      int oidTaskPending;
};
extern GAME D_002DEB30;
struct TASKPAIR
{
    OID oidTask;
    int nState;
};
void SetTaskStates(GAME *pgame, int ctaskpair, TASKPAIR *ataskpair, LO *ploContext);
extern CLOCK D_002DDED0;
void SetClockRate(CLOCK *pclock, CRK crk, float rt);
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
ASEGA *PasegaApplyAseg(ASEG *paseg, ALO *paloAsegRoot, float tLocal, float svtLocal, GRFAPL grfapl);
extern JOY D_002DFC00;
void TriggerJoyRumbleRumk(JOY *pjoy, RUMK rumk, float dt);
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
};
struct WARP : XFM
{
      undefined4 unk_0xe0[(0x16c - 0xe0) / 4];
      int coidAlias;
      OID aoidAlias[4];
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
void JoltCm(CM *pcm, int nUnk61c);
void CutCm(CM *pcm);
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
};
extern SW D_003DD490;
void ClearSoXps(SO *pso);
struct BHV166 : public BHV
{
      void *pvUnk30;
      float gUnk34;
      int nUnk38;
      int fHasEnd;
      int nUnk40;
      undefined4 unk_0x44;
      float gUnk48;
      int nMode;
      float tMode;
      void *pshapeUnk54;
      int nUnk58;
      float uUnk5c;
      float uUnk60;
      float sUnk64;
      float sUnk68;
      float sUnk6c;
      undefined4 unk_0x70[4];
      qword posStart;
      undefined4 unk_0x90[4];
      int nUnkA0;
      undefined4 unk_0xa4[3];
      qword posEnd;
};
struct BHV168 : public BHV166
{
      struct POCTL *pctlLeader;
      int nUnkc4;
      float gUnkc8;
      int nUnkcc;
      float gUnkd0;
};
struct BHV193 : public BHV
{
      undefined4 nUnk30;
      float gUnk34;
      int nUnk38;
      float gUnk3c;
      qword qwUnk40;
};
void SetBhv193Goal(BHV193 *pbhv, qword qwPos);
void SetBhv166Path(BHV166 *pbhv, void *pshape, int nUnk38);
void SetStepRadTarget(STEP *pstep, float radTarget);
void SetStepRadTowardDir(STEP *pstep, qword dpos);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
void StartJtMode(JTMODE *pjtmode, int nKind, STEP *pstepOther, LO *ploHudOverride);
void SetJtAirJump(JTAIR *pjtair, int nKind, int n2, LO *plo);
void StepCmdHoldOn(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdPutDown(STEPPLAYER *pstepplayer, LO *ploObj, LO *ploWhere);
void StepCmdHoldOn9(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdCarry(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdPlaceHeld(STEPPLAYER *pstepplayer, LO *plo);
void StepCmdGrab0xC(STEPPLAYER *pstepplayer, ASEG *paseg, LO *plo, int n);
void PlayStepClip(STEPPLAYER *pstepplayer, OID oid, int grfapl, int nState);
void FindShapeClosestPointAll(SHAPE *pshape, qword posFrom, CONSTR *pconstr, qword *pposClosest, VECTOR4 *pnormTangentClosest, float *pu, float *ps);
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
struct JSG : public BASIC
{
      int cref;
      struct RELAY *prelay;
      LO *ploContext;
      LO *ploBlock;
      LO *ploActorDefault;
      LO *ploActor;
      struct RELAYCMD *prelaycmdFirst;
      struct RELAYCMD *prelaycmdLast;
      struct RELAYCMD *prelaycmdCur;
      float tCommand;
      int fHandsOff;
      int grfHandsOff;
      struct RELAYCMD *prelaycmdJt;
      LO *pjt;
      int fHoldBtn10;
      ASEGA *pasegaCur;
      struct RELAY *prelayWait;
      struct STEP *pstepAseg;
      struct SMA *psmaCur;
      int fSmaReached;
      LO *ploFade;
      LO *ploWidget;
      struct RELAYCMD *prelaycmdLine;
      TN *ptnCur;
      int fHideTn;
      undefined4 unk_0x74;
      float sGoTo;
      float sPipe;
      float tBlendEnd;
      undefined4 unk_0x84[(0x90 - 0x84) / 4];
      int nUnk90;
      undefined4 unk_0x94[(0x9c - 0x94) / 4];
      int fUnk9c;
      struct STEPPLAYER *pstepplayerMode;
      int cstepHold;
      struct STEP *apstepHold[4];
      int anHold5[4];
      int anHold6[4];
      struct STEP *pstepSsgGoal;
      struct SSG *pssgGoal;
      int fRun;
      DLE dleRelay;
      CRefPtr<SND> psnd;
};
struct JSGREF
{
    JSG *pjsg;
    JSGREF(JSG *pjsgHold)
    {
        pjsg = pjsgHold;
        pjsg->VUnk00();
    }
    ~JSGREF()
    {
        pjsg->VUnk01();
    }
};
void SetJsgHandsOff(JSG *pjsg, int fHandsOff, int grfHandsOff);
void GetJsgLocation(JSG *pjsg, LO *plo, qword *ppos, float *prad);
void SetJsgJt(JSG *pjsg, struct RELAYCMD *prelaycmdJt, LO *pjt);
void EndJsgJtMode(JSG *pjsg);
void SetJsgFocus(JSG *pjsg, LO *plo);
void SetJsgTn(JSG *pjsg, TN *ptn);
void UpdateJsg(JSG *pjsg);
LO *PloFindJsgObject(JSG *pjsg, OID oid);
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
static inline qword QwAdd(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
struct ALARM : public SO
{
      ALARMS alarms;
    float tAlarms;
    SM *psm;
    SMA *psma;
    float dtReset;
      int calbrks;
      OID aoidAlbrks[4];
      int coidSensors;
      OID aoidSensors[16];
      int cpsensors;
      SENSOR *apsensors[16];
      int coidStepguards;
      OID aoidStepguards[6];
      int calbrksDisabled;
    AMB *pambSiren;
    EXC *pexc;
    int fSilent;
      int crsmg;
      RSMG arsmg[8];
      int ichkDisabled;
};
struct RELAYCMD
{
      RELAYCK relayck;
      int fAsync;
      undefined4 unk_0x8[2];
      union
    {
        undefined4 an[16];
    struct
    {
        OID oidLabel;
    } blockBegin;
    struct
    {
        OID oidLabel;
    } stopBlock;
    struct
    {
        int coid;
        int aoid[4];
    } waitBlocks;
    struct
    {
        int idAlarm;
        int fForget;
    } waitAlarm;
    struct
    {
        int idAlarm;
        OID oidRelay;
    } raiseAlarm;
    struct
    {
        OID oidRelay;
    } stopRelay;
    struct
    {
        OID oidObj;
    } setCtlState;
    struct
    {
        OID oidActor;
    } setActor;
    struct
    {
        int idSignal;
    } signal;
    struct
    {
        OID oidAseg;
        OID oidRoot;
        int grfapl;
    } playAseg;
    struct
    {
        OID oidRelay;
        OID oidFirer;
    } fireRelay;
    struct
    {
        OID oidContext;
    } setContext;
    struct
    {
        OID oidSm;
        OID oidUnk5;
        OID oidState;
        int fUnk7;
    } applySm;
    struct
    {
        OID oidWidget;
        int idLabel;
    } widgetShow;
    struct
    {
        OID oidWidget;
    } widgetHide;
    struct
    {
        float rate;
    } setClockRate;
    struct
    {
        OID oidTask;
        int nState;
    } setTaskState;
    struct
    {
        int fHandsOff;
        int grfHandsOff;
    } handsOff;
    struct
    {
        OID oidLabel;
    } label;
    struct
    {
        OID oidLabel;
    } gotoLabel;
    struct
    {
        OID oidLabel;
        int nValue;
    } gotoLabelIf;
    struct
    {
        OID oidWhere;
        int fAir;
        OID oidActor;
    } teleportTo;
    struct
    {
        OID oidTunnel;
        OID oidActor;
    } setTunnel;
    struct
    {
        OID oidFocus;
        OID oidActor;
    } setFocus;
    struct
    {
        OID oidTarget;
        float gTolerance;
        OID oidActor;
    } pipeTo;
    struct
    {
        OID oidActor;
    } unk1B;
    struct
    {
        int nUnk4;
        int nState;
        OID oidActor;
    } heroState;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd1D;
    struct
    {
        OID oidA;
        OID oidB;
        OID oidActor;
    } stepCmd1E;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd1F;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd20;
    struct
    {
        OID oidGoal;
        OID oidActor;
    } stepCmd21;
    struct
    {
        OID oidTarget;
        int nMode;
        int nUnk6;
        OID oidActor;
    } stepCmd22;
    struct
    {
        float dtWait;
        OID oidFace;
        OID oidActor;
    } face;
    struct
    {
        OID oidTarget;
        float uRun;
        float gArrive;
        OID oidActor;
        int idClip;
        int fUnk9;
    } goTo;
    struct
    {
        OID oidPath;
        int nPathMode;
        int nUnk6;
        float gUnk7;
        OID oidActor;
        int idClip;
    } followPath;
    struct
    {
        OID oidTarget;
        float gUnk5;
        int nUnk6;
        float gUnk7;
        OID oidActor;
        int idClip;
    } walkTo;
    struct
    {
        OID oidTarget;
        float gUnk5;
        float gArrive;
        OID oidActor;
    } stepCmd29;
    struct
    {
        OID oidSpot;
        int nUnk5;
        int nUnk6;
        OID oidActor;
    } respawn;
    struct
    {
        OID oidTarget;
        float gUnk5;
        float gUnk6;
        OID oidActor;
    } launchTo;
    struct
    {
        OID oidObj;
        float dtFade;
    } addObject;
    struct
    {
        OID oidObj;
        float dtFade;
    } removeObject;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } setPartFlag;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } clearPartFlag;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } showPart;
    struct
    {
        OID oidAseg;
        OID oidPart;
    } hidePart;
    struct
    {
        int nUnk4;
    } unk32;
    struct
    {
        float dtRumble;
        int rumk;
    } rumble;
    struct
    {
        OID oidA;
        OID oidB;
        OID oidC;
        float dtDelay;
    } explode;
    struct
    {
        OID oidA;
        OID oidB;
        float dtEyes;
    } eyes;
    struct
    {
        int idSnd;
        OID oidA;
        int nUnk6;
        int nUnk7;
        float gUnk8;
        float gUnk9;
        float gUnk10;
    } sound;
    struct
    {
        int idSnd;
    } stopSound;
    struct
    {
        qword qwPos;
        float radX;
        float radY;
        float radZ;
        undefined4 unk_0x2c;
        float gFov;
        undefined4 unk_0x34[3];
        float dtBlend;
        OID oidAseg;
    } camBlendFrom;
    struct
    {
        qword qwPos;
        float radX;
        float radY;
        float radZ;
        undefined4 unk_0x2c;
        float gFov;
        undefined4 unk_0x34[3];
        int nUnk16;
        int nUnk17;
    } camBlendTo;
    struct
    {
        int nUnk4;
    } unk3A;
    struct
    {
        OID oidActor;
        OID oidStep;
        OID oidUnk6;
    } startJtMode;
    struct
    {
        OID oidObj;
    } tweenPosObj;
    struct
    {
        OID oidObj;
    } tweenAimObj;
    struct
    {
        qword qwPos;
        float radX;
        float radY;
        float radZ;
        undefined4 unk_0x2c;
        float gFov;
    } tweenTarget;
    struct
    {
        float gUnk4;
    } tweenTime;
    struct
    {
        int grfbtn;
        OID oidWho;
    } buttonPrompt;
    struct
    {
        int nUnk4;
        OID oidActor;
    } unk43;
    struct
    {
        int idLine;
        int fWait;
    } line;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
    } unk47;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
    } unk48;
    struct
    {
        int nUnk4;
    } unk49;
    struct
    {
        float gUnk4;
        int nUnk5;
        int nUnk6;
        OID oidActor;
    } unk4A;
    struct
    {
        OID oidOther;
        OID oidActor;
    } playAsegDefault;
    struct
    {
        int idMusic;
    } startMusic;
    struct
    {
        int idMusic;
    } stopMusic;
    struct
    {
        float dtWait;
    } waitTimer;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
        int nUnk7;
        int nUnk8;
    } unk4F;
    struct
    {
        OID oidActor;
    } unk50;
    struct
    {
        OID oidActor;
    } unk51;
    struct
    {
        float dtWait;
    } unk52;
    struct
    {
        int idHint;
        float dtHint;
    } hintBox;
    struct
    {
        int idHint;
    } clearHintBox;
    struct
    {
        OID oidStep;
        OID oidSsg;
    } setSsgGoal;
    struct
    {
        OID oidStep;
        OID oidUnk5;
    } resetStepGoal;
    struct
    {
        OID oidActor;
    } unk57;
    struct
    {
        OID oidShip;
        OID oidActor;
    } unk58;
    struct
    {
        OID oidShip;
        OID oidActor;
    } unk59;
    struct
    {
        OID oidShip;
        OID oidActor;
        int nUnk6;
    } unk5A;
    };
};
struct RELAY : public LO
{
      int crelaycmd;
      RELAYCMD *arelaycmd;
      int crelaycmdMax;
      DL dlJsg;
      struct JSG *pjsgSignal;
      LO *ploFirer;
      float tFired;
      int fFiring;
      int fBusy;
      int fAppendEnd;
      int nUnk80;
      int calarm;
      ALARM *apalarm[8];
      undefined4 unk_0xa8[2];
};
LO *PloFindSwObject(SW *psw, GRFFSO grffso, OID oid, LO *ploContext);
void PlayAsegSoundEvent(SW *psw, int sfxid, OID oid, int sNear, int sFar, float gPitch, float gUnk14, float gVolume, LO *plo, LO *ploRoot,
                        ASEG *paseg, CRefPtr<SND> *ppsnd);
void StopAsegSoundEvent(SW *psw, int sfxid, ASEG *paseg, CRefPtr<SND> *ppsnd);
LO *FadeAsegObjectEvent(SW *psw, int fIn, OID oid, float dtFade, LO *ploRoot, ASEG *paseg);
void SetAsegPartFlagEvent(SW *psw, int fClear, OID oidSo, OID oidPart, LO *paloRoot, LO *ploRoot, ASEG *paseg);
void ShowAsegPartEvent(SW *psw, int fShow, OID oidSo, OID oidPart, LO *paloRoot, LO *ploRoot, ASEG *paseg);
void ExplodeAsegEvent(SW *psw, OID oidExpl, OID oidRef, OID oidUnk30, LO *ploRoot, ASEG *paseg, float gScale);
void SetAsegEyesEvent(SW *psw, OID oidSo, OID oidEyes, LO *ploRoot, ASEG *paseg, float g);
struct WIDGET : public LO
{
      undefined4 unk_0x50[2];
      int nState;
      float tState;
      int cframeState;
      int nStateStamped;
      float tStateStamped;
      undefined4 unk_0x6c;
      int idLabelHold;
      int idLabelIn;
      int idLabelOut;
      int idLabelPlaying;
      ushort cclip;
      WIDGETCLIP *aclip;
      undefined4 unk_0x88;
      undefined4 unk_0x8c[6];
      int nPriority;
      int fHidden;
      int unk_0xac;
      int fFreezeClock;
      DLE dleDraw;
      undefined4 unk_0xbc[0x15];
    virtual void Show(int idLabel);
    virtual void Hide();
};
struct SUBTITLE
{
      int idString;
      RGBA rgba;
      int grfbtnPrompt;
};
extern SUBTITLE D_004435F0;
void ClearSubtitle(SUBTITLE *psubtitle);
void SetStepSsgGoal(STEP *pstep, SSG *pssg);
void ResetStepGoal(STEP *pstep, SSG *pssg);
void EnsureSggAlarm(SGG *psgg, ALARM *palarm);
struct PARTLINK
{
      undefined4 unk_0x00;
      void *precA;
      void *precB;
      void *precC;
      undefined4 unk_0x10[0xD0 / 4];
};
JSG *PjsgStartBlock(RELAY *prelay, RELAYCMD *prelaycmdBegin, JSG *pjsgParent);
JSG *PjsgFindRelayBlock(RELAY *prelay, OID oidLabel, JSG *pjsgExcept);
void EndJsg(JSG *pjsg);
extern "C" void AddJsgStepHold(JSG *pjsg, LO *plo, int n5, int n6);
extern "C" void RemoveJsgStepHold(JSG *pjsg, LO *plo, int n5, int n6);
void StartJsgCamBlend(JSG *pjsg, RELAYCMD *prelaycmdFrom, RELAYCMD *prelaycmdTo, float dt);
void PopJsgCamBlend(JSG *pjsg, int n);
float DtJsgCommand(JSG *pjsg, RELAYCMD *prelaycmd);
extern "C" void func_0020FFF0(SW *psw, OID *aoid, LO *ploContext, int grf, JSG *pjsg, SMA **ppsma);
extern "C" int func_00210B68();
extern "C" void func_001DD820(void *pv);
extern "C" void func_001531A8(LO *plo, int n, int cent, void *aent);
extern "C" void func_001532D8(LO *plo, LO *ploActor, int n5, int n6, float g);
extern "C" void func_001B9C00(MURRAY *pmurray, SMA *psma, int n7, int n6, int n8);
extern "C" void func_001B9880(MURRAY *pmurray);
void ReleaseJsgSma(JSG *pjsg);
extern "C" void func_00220508(MOVEREC *pmr, LO *plo);
void PutSwHeroAtWarp(WARP *pwarp);
MATRIX3 LoadRotateMatrixRad(float rad, qword normal);
GRABSPOT *PspotFromLo(LO *plo);
extern "C" int GuardSpawnerGroundSnap(qword *ppos, int n, void *pfnFilter, float gUp, float gDown);
void SetJtAnimState(JT *pjt, int jtas);
int ChooseJtGrabTarget(JTGRAB *pjtgrab, uint grf, int fUnk);
struct JSGENT
{
    float g;
    int n;
    undefined4 unk_0x08[(0x30 - 0x08) / 4];
};
extern "C" void RelayFire(RELAY *prelay, LO *ploFirer);
extern "C" void StopRelay(RELAY *prelay);
extern "C" BHV *PbhvPushStep(STEP *pstep, int cid);
extern "C" void EnterJtGrabState(JTGRAB *pjtgrab, int nState, LO *plo, GRABSPOT *pspot);
extern "C" void SetJtGrabPhase(JTGRAB *pjtgrab, int nPhase);
extern "C" void LaunchJtToPos(STEP *pstep, qword posTarget, float radTilt, float uSteer);
extern "C" void RespawnAtSafeSpot(STEPPLAYER *pstepplayer, JTPLACE *pplace);
extern "C" void PlayJtAsegDefault(LO *plo, LO *ploOther, int n);
extern "C" int StartDialogueLine(SNDMGR *psndmgr, int idLine, int fWait);
extern "C" void StartMusic(SNDMGR *psndmgr, int idMusic);
extern "C" void StopMusic(SNDMGR *psndmgr, int idMusic);
extern "C" void SetHintBoxText(HINTBOX *phintbox, int idText, float dt);
extern "C" void ClearIfSlotId(HINTBOX *phintbox, int idText);
extern SNDMGR D_003DD040;
extern HINTBOX D_003DA0C0;
extern char D_003DA530[];
extern LO *D_002DE570;
extern float D_002A7AD0;
extern qword D_002A2DD0;
extern qword D_002A2DC0;
static inline LO *PloJsgActorDefault(JSG *pjsg)
{
    if (pjsg->ploActorDefault)
    {
        return pjsg->ploActorDefault;
    }
    if (pjsg->ploBlock)
    {
        return pjsg->ploBlock;
    }
    return pjsg->prelay->ploFirer;
}
static inline qword QwAddScaledX(qword qw, qword qwDir, VU_FLOAT vuScale)
{
    qword qwSum;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qwSum) : "j"(qw), "j"(qwDir), "j"(vuScale.qw));
    return qwSum;
}
void NextJsgCommand(JSG *pjsg) __asm__("NextJsgCommand__FP3JSG");
/** @sly2-annot addr=0x1E1E30
 * Purpose: Advance the script to the next command and execute it by kind
 * Info: proposed name NextJsgCommand (T1); TU relay
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void NextJsgCommand(JSG *pjsg)
{
    pjsg->prelaycmdCur++;
    if (pjsg->prelaycmdCur > pjsg->prelaycmdLast)
    {
        return;
    }
    JSGREF jsgref(pjsg);
    RELAYCMD *prelaycmd = pjsg->prelaycmdCur;
    OID oidActor;
    switch (prelaycmd->relayck)
    {
    case RELAYCK_TeleportTo:
    case RELAYCK_PipeTo:
    case RELAYCK_HeroState:
    case RELAYCK_StepCmd1E:
    case RELAYCK_Face:
        oidActor = (OID)prelaycmd->an[2];
        break;
    case RELAYCK_FollowPath:
    case RELAYCK_WalkTo:
        oidActor = (OID)prelaycmd->an[4];
        break;
    case RELAYCK_StepCmd22:
    case RELAYCK_GoTo:
    case RELAYCK_StepCmd29:
    case RELAYCK_Respawn:
    case RELAYCK_LaunchTo:
    case RELAYCK_Unk4A:
        oidActor = (OID)prelaycmd->an[3];
        break;
    case RELAYCK_Unk1B:
    case RELAYCK_Unk47:
    case RELAYCK_Unk48:
    case RELAYCK_Unk4F:
    case RELAYCK_Unk50:
    case RELAYCK_Unk51:
    case RELAYCK_Unk57:
        oidActor = (OID)prelaycmd->an[0];
        break;
    case RELAYCK_FireRelay:
    case RELAYCK_SetTunnel:
    case RELAYCK_SetFocus:
    case RELAYCK_StepCmd1D:
    case RELAYCK_StepCmd1F:
    case RELAYCK_StepCmd20:
    case RELAYCK_StepCmd21:
    case RELAYCK_Unk43:
    case RELAYCK_PlayAsegDefault:
    case RELAYCK_Unk58:
    case RELAYCK_Unk59:
    case RELAYCK_Unk5A:
        oidActor = (OID)prelaycmd->an[1];
        break;
    default:
        oidActor = OID_Nil;
        break;
    }
    LO *ploActor;
    if (oidActor == OID_Nil)
    {
        ploActor = PloJsgActorDefault(pjsg);
    }
    else
    {
        ploActor = PloFindJsgObject(pjsg, oidActor);
    }
    pjsg->ploActor = ploActor;
    switch (prelaycmd->relayck)
    {
    case RELAYCK_BlockBegin:
    {
        JSG *pjsgBlock = PjsgStartBlock(pjsg->prelay, prelaycmd, pjsg);
        pjsg->prelaycmdCur = pjsgBlock->prelaycmdLast;
        UpdateJsg(pjsgBlock);
        break;
    }
    case RELAYCK_StopBlock:
    {
        JSG *pjsgStop;
        if (prelaycmd->stopBlock.oidLabel == OID_Nil)
        {
            pjsgStop = pjsg;
        }
        else
        {
            pjsgStop = PjsgFindRelayBlock(pjsg->prelay, prelaycmd->stopBlock.oidLabel, 0);
        }
        if (pjsgStop)
        {
            EndJsg(pjsgStop);
        }
        break;
    }
    case RELAYCK_RaiseAlarm:
    {
        RELAY *prelayAlarm;
        if (prelaycmd->raiseAlarm.oidRelay == OID_Nil)
        {
            prelayAlarm = pjsg->prelay;
        }
        else
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->raiseAlarm.oidRelay);
            prelayAlarm = (plo && FIsBasicDerivedFrom(plo, (CID)114)) ? (RELAY *)plo : 0;
        }
        if (prelayAlarm)
        {
            EnsureSggAlarm((SGG *)prelayAlarm, (ALARM *)prelaycmd->raiseAlarm.idAlarm);
        }
        break;
    }
    case RELAYCK_StopRelay:
    {
        RELAY *prelayStop;
        if (prelaycmd->stopRelay.oidRelay == OID_Nil)
        {
            prelayStop = pjsg->prelay;
        }
        else
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->stopRelay.oidRelay);
            prelayStop = (plo && FIsBasicDerivedFrom(plo, (CID)114)) ? (RELAY *)plo : 0;
        }
        if (prelayStop)
        {
            StopRelay(prelayStop);
        }
        break;
    }
    case RELAYCK_SetCtlState:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->setCtlState.oidObj);
        if (plo && FIsBasicDerivedFrom(plo, (CID)50))
        {
            ((PO *)plo)->SetCtlState(1);
        }
        break;
    }
    case RELAYCK_SetActor:
        pjsg->ploActorDefault = PloFindJsgObject(pjsg, prelaycmd->setActor.oidActor);
        break;
    case RELAYCK_Signal:
    {
        void *apv[1];
        apv[0] = &prelaycmd->signal.idSignal;
        pjsg->prelay->pjsgSignal = pjsg;
        pjsg->prelay->HandleSpliceEvent(4, 0, 1, apv, 0);
        pjsg->prelay->pjsgSignal = 0;
        break;
    }
    case RELAYCK_PlayAseg:
    {
        LO *ploRoot = PloFindJsgObject(pjsg, prelaycmd->playAseg.oidRoot);
        if (ploRoot)
        {
            if (FIsBasicDerivedFrom(ploRoot, (CID)52))
            {
                PlayStepClip((STEPPLAYER *)ploRoot, prelaycmd->playAseg.oidAseg, prelaycmd->playAseg.grfapl | 1, 5);
                if (!prelaycmd->fAsync)
                {
                    pjsg->pstepAseg = (STEP *)ploRoot;
                }
                break;
            }
            if (!FIsBasicDerivedFrom(ploRoot, (CID)48))
            {
                ploRoot = 0;
            }
        }
        LO *ploAseg = PloFindJsgObject(pjsg, prelaycmd->playAseg.oidAseg);
        if (ploAseg && FIsBasicDerivedFrom(ploAseg, (CID)109))
        {
            ASEGA *pasega = PasegaApplyAseg((ASEG *)ploAseg, (ALO *)ploRoot, 0.0f, 1.0f, prelaycmd->playAseg.grfapl);
            if (!prelaycmd->fAsync)
            {
                pjsg->pasegaCur = pasega;
                pasega->VUnk00();
            }
        }
        break;
    }
    case RELAYCK_FireRelay:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->fireRelay.oidRelay);
        if (plo && FIsBasicDerivedFrom(plo, (CID)114))
        {
            RelayFire((RELAY *)plo, ploActor);
            if (!prelaycmd->fAsync)
            {
                pjsg->prelayWait = (RELAY *)plo;
            }
        }
        break;
    }
    case RELAYCK_SetContext:
        pjsg->ploContext = PloFindJsgObject(pjsg, prelaycmd->setContext.oidContext);
        if (!pjsg->ploContext)
        {
            pjsg->ploContext = pjsg->prelay;
        }
        break;
    case RELAYCK_ApplySm:
    {
        OID aoid[4];
        aoid[0] = prelaycmd->applySm.oidSm;
        aoid[1] = prelaycmd->applySm.oidUnk5;
        aoid[2] = prelaycmd->applySm.oidState;
        aoid[3] = OID_Nil;
        int grfSma = prelaycmd->applySm.fUnk7 ? 3 : 1;
        func_0020FFF0(&D_003DD490, aoid, pjsg->ploContext, grfSma, pjsg, &pjsg->psmaCur);
        if (prelaycmd->fAsync)
        {
            ReleaseJsgSma(pjsg);
        }
        else if (pjsg->psmaCur && !pjsg->fSmaReached && !FIsSmaRetracted(pjsg->psmaCur) && GetSmaCur(pjsg->psmaCur) == prelaycmd->applySm.oidState)
        {
            pjsg->fSmaReached = 1;
        }
        break;
    }
    case RELAYCK_WidgetShow:
    case RELAYCK_WidgetHide:
    {
        LO *plo = PloFindSwObject(&D_003DD490, (GRFFSO)0x405, prelaycmd->widgetShow.oidWidget, 0);
        if (plo && FIsBasicDerivedFrom(plo, (CID)126))
        {
            if (prelaycmd->relayck == RELAYCK_WidgetShow)
            {
                ((WIDGET *)plo)->Show(prelaycmd->widgetShow.idLabel);
                if (!prelaycmd->fAsync)
                {
                    pjsg->ploWidget = plo;
                }
            }
            else
            {
                ((WIDGET *)plo)->Hide();
            }
        }
        break;
    }
    case RELAYCK_SetClockRate:
        SetClockRate(&D_002DDED0, (CRK)6, prelaycmd->setClockRate.rate);
        if (prelaycmd->setClockRate.rate == 0.0f)
        {
            RELAY *prelay = pjsg->prelay;
            int grf = prelay->grfUnk40;
            if ((grf ^ 1) & 1)
            {
                *(int *)grf |= 0x100;
            }
            else
            {
                prelay->grfUnk40 = grf | 0x100;
            }
        }
        else
        {
            RELAY *prelay = pjsg->prelay;
            int grf = prelay->grfUnk40;
            if ((grf ^ 1) & 1)
            {
                *(int *)grf &= ~0x100;
            }
            else
            {
                prelay->grfUnk40 = grf & ~0x100;
            }
        }
        break;
    case RELAYCK_SetTaskState:
        SetTaskStates(&D_002DEB30, 1, (TASKPAIR *)&prelaycmd->setTaskState, pjsg->ploContext);
        break;
    case RELAYCK_HandsOff:
        SetJsgHandsOff(pjsg, prelaycmd->handsOff.fHandsOff, prelaycmd->handsOff.grfHandsOff);
        break;
    case RELAYCK_GotoLabel:
    case RELAYCK_GotoLabelIf:
    {
        if (prelaycmd->relayck == RELAYCK_GotoLabelIf && func_00210B68() != prelaycmd->gotoLabelIf.nValue)
        {
            break;
        }
        RELAYCMD *prelaycmdGoto = prelaycmd;
        RELAYCMD *prelaycmdLabel = 0;
        int cBlock = 0;
        for (prelaycmd = pjsg->prelaycmdFirst + 1; prelaycmd < pjsg->prelaycmdLast; prelaycmd++)
        {
            switch (prelaycmd->relayck)
            {
            case RELAYCK_BlockBegin:
                cBlock++;
                break;
            case RELAYCK_BlockEnd:
                cBlock--;
                break;
            case RELAYCK_Label:
                if (cBlock == 0 && prelaycmd->label.oidLabel == prelaycmdGoto->gotoLabel.oidLabel)
                {
                    prelaycmdLabel = prelaycmd;
                    prelaycmd = pjsg->prelaycmdLast;
                }
                break;
            }
        }
        if (prelaycmdLabel)
        {
            pjsg->prelaycmdCur = prelaycmdLabel;
        }
        break;
    }
    case RELAYCK_TeleportTo:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->teleportTo.oidWhere);
        if (plo && FIsBasicDerivedFrom(plo, (CID)41))
        {
            PutSwHeroAtWarp((WARP *)plo);
            break;
        }
        if (!pjsg->ploActor)
        {
            break;
        }
        if (plo && FIsBasicDerivedFrom(plo, (CID)27))
        {
            if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
            {
                STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
                EnterJtGrabState(&pstepplayer->jtgrab, 8, plo, 0);
                SetJtGrabPhase(&pstepplayer->jtgrab, 9);
                SetJsgJt(pjsg, 0, 0);
            }
            break;
        }
        VECTOR4 pos;
        MATRIX3 mat;
        float rad;
        GetJsgLocation(pjsg, plo, (qword *)&pos, &rad);
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)51))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            pstep->TranslateToPos(QwSub(*(qword *)&pos, pstep->qwUnk240));
            SetStepRadTarget(pstep, rad);
        }
        else
        {
            pjsg->ploActor->TranslateToPos(*(qword *)&pos);
        }
        mat = LoadRotateMatrixRad(rad, D_002A2DC0);
        pjsg->ploActor->RotateToMat((MATRIX4 *)&mat);
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            pstepplayer->qwUnk1060 = QwAddScaledX(*(qword *)&pos, D_002A2DC0, VuFloatFromMemWord(pstepplayer->nUnk1048));
            if (!prelaycmd->teleportTo.fAir && pstepplayer->cUnk12f4 == 0)
            {
                SetJtAirJump(&pstepplayer->jtair, 0xB, 0, 0);
                ClearSoXps(pstepplayer);
            }
            SetJsgJt(pjsg, 0, 0);
        }
        pjsg->ploActor->SetVelocity(D_002A2DD0);
        pjsg->ploActor->SetAngularVelocity(D_002A2DD0);
        break;
    }
    case RELAYCK_CutCamera:
        CutCm(&D_002DDF40);
        break;
    case RELAYCK_SetTunnel:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->setTunnel.oidTunnel);
        if (plo && FIsBasicDerivedFrom(plo, (CID)15))
        {
            SetJsgTn(pjsg, (TN *)plo);
        }
        break;
    }
    case RELAYCK_SetFocus:
        SetJsgFocus(pjsg, PloFindJsgObject(pjsg, prelaycmd->setFocus.oidFocus));
        break;
    case RELAYCK_PipeTo:
    {
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (pstepplayer->jtpipe.nState == -1)
            {
                break;
            }
            VECTOR4 pos;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->pipeTo.oidTarget);
            GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
            FindShapeClosestPointAll(pstepplayer->jtpipe.pshape, *(qword *)&pos, 0, 0, 0, 0, &pjsg->sPipe);
            SetJsgJt(pjsg, prelaycmd, pstepplayer);
        }
        break;
    }
    case RELAYCK_HeroState:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)53))
        {
            JT *pjt = (JT *)pjsg->ploActor;
            switch (prelaycmd->heroState.nState)
            {
            case 2:
                SetJtAnimState(pjt, 3);
                break;
            case 4:
                SetJtJts(pjt, (JTS)0xD, (JTBS)-1);
                break;
            case 8:
                SetJtJts(pjt, (JTS)9, (JTBS)-1);
                break;
            }
        }
        break;
    case RELAYCK_StepCmd1D:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdHoldOn((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd1D.oidGoal));
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd1E:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            LO *ploA = PloFindJsgObject(pjsg, prelaycmd->stepCmd1E.oidA);
            LO *ploB = PloFindJsgObject(pjsg, prelaycmd->stepCmd1E.oidB);
            StepCmdPutDown((STEPPLAYER *)pstep, ploA, ploB);
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd1F:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdHoldOn9((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd1F.oidGoal));
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd20:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdCarry((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd20.oidGoal));
        }
        break;
    case RELAYCK_StepCmd21:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            StepCmdPlaceHeld((STEPPLAYER *)pstep, PloFindJsgObject(pjsg, prelaycmd->stepCmd21.oidGoal));
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_StepCmd22:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            LO *ploTarget = PloFindSwObject(&D_003DD490, (GRFFSO)1, prelaycmd->stepCmd22.oidTarget, pstep);
            LO *ploDir;
            if (prelaycmd->stepCmd22.nMode == 3)
            {
                CM *pcm = &D_002DDF40;
                qword qwCm = *(qword *)&pcm->pos;
                SetStepRadTowardDir(pstep, QwSub(qwCm, PosLoWorld(pstep)));
                ploDir = 0;
            }
            else
            {
                ploDir = PloFindJsgObject(pjsg, (OID)prelaycmd->stepCmd22.nMode);
            }
            StepCmdGrab0xC((STEPPLAYER *)pstep, (ASEG *)ploTarget, ploDir, prelaycmd->stepCmd22.nUnk6);
            SetJsgJt(pjsg, prelaycmd, pstep);
        }
        break;
    case RELAYCK_Face:
        if (!PloFindJsgObject(pjsg, prelaycmd->face.oidFace))
        {
            break;
        }
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            SetJsgJt(pjsg, prelaycmd, pjsg->ploActor);
        }
        break;
    case RELAYCK_GoTo:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (prelaycmd->goTo.idClip != -1)
            {
                PlayStepClip(pstepplayer, (OID)prelaycmd->goTo.idClip, 9, 6);
            }
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->goTo.oidTarget);
            if (plo)
            {
                if (FIsBasicDerivedFrom(plo, (CID)34) || FIsBasicDerivedFrom(plo, (CID)12) || FIsBasicDerivedFrom(plo, (CID)38) ||
                    FIsBasicDerivedFrom(plo, (CID)88) || FIsBasicDerivedFrom(plo, (CID)11) || FIsBasicDerivedFrom(plo, (CID)19))
                {
                    uint grf;
                    if (FIsBasicDerivedFrom(pstepplayer, (CID)53))
                    {
                        grf = 0xFFFF;
                    }
                    else if (FIsBasicDerivedFrom(pstepplayer, (CID)55))
                    {
                        grf = 0xF8FE;
                    }
                    else
                    {
                        grf = 0xF8FA;
                    }
                    ChooseJtGrabTarget(&pstepplayer->jtgrab, grf, (int)plo);
                }
                if (pstepplayer->jtgrab.nKind != -1 && pstepplayer->jtgrab.nState == 3)
                {
                    VECTOR4 pos;
                    SHAPE *pshape = (SHAPE *)pstepplayer->jtgrab.plo;
                    GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
                    FindShapeClosestPointAll(pshape, *(qword *)&pos, 0, 0, 0, &pjsg->sGoTo, 0);
                }
            }
            SetJsgJt(pjsg, prelaycmd, pstepplayer);
        }
        break;
    case RELAYCK_RunOn:
        pjsg->fRun = 1;
        break;
    case RELAYCK_RunOff:
        pjsg->fRun = 0;
        break;
    case RELAYCK_FollowPath:
    {
        if (!pjsg->ploActor)
        {
            break;
        }
        if (!FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            break;
        }
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->followPath.oidPath);
        if (plo && FIsBasicDerivedFrom(plo, (CID)10))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            if (prelaycmd->followPath.idClip != -1)
            {
                PlayStepClip((STEPPLAYER *)pstep, (OID)prelaycmd->followPath.idClip, 9, 6);
            }
            BHV168 *pbhv = (BHV168 *)PbhvPushStep(pstep, 0xA8);
            pbhv->nUnk40 = pjsg->fRun;
            pbhv->nUnkcc = prelaycmd->followPath.nUnk6;
            pbhv->gUnkd0 = prelaycmd->followPath.gUnk7;
            SetBhv166Path(pbhv, plo, prelaycmd->followPath.nPathMode == 0);
            pbhv->Start();
        }
        break;
    }
    case RELAYCK_WalkTo:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            VECTOR4 pos;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->walkTo.oidTarget);
            GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
            if (prelaycmd->walkTo.idClip != -1)
            {
                PlayStepClip((STEPPLAYER *)pstep, (OID)prelaycmd->walkTo.idClip, 9, 6);
            }
            BHV193 *pbhv = (BHV193 *)PbhvPushStep(pstep, 0xC1);
            pbhv->nUnk30 = pjsg->fRun;
            pbhv->gUnk34 = prelaycmd->walkTo.gUnk5;
            pbhv->nUnk38 = prelaycmd->walkTo.nUnk6;
            pbhv->gUnk3c = prelaycmd->walkTo.gUnk7;
            SetBhv193Goal(pbhv, *(qword *)&pos);
            pbhv->Start();
        }
        break;
    case RELAYCK_StepCmd29:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (pstepplayer->jtgrab.nKind != -1)
            {
                if (pstepplayer->jtgrab.nState != 8)
                {
                    break;
                }
                SetJsgJt(pjsg, prelaycmd, pstepplayer);
            }
        }
        break;
    case RELAYCK_Respawn:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->respawn.oidSpot);
            JTPLACE place;
            memset(&place, 0, sizeof(JTPLACE));
            if (!plo)
            {
                place.qwPos = QwAdd(PosLoWorld(pstepplayer), pstepplayer->qwUnk240);
            }
            else
            {
                GRABSPOT *pspot = PspotFromLo(plo);
                place.pspot = pspot;
                if (pspot)
                {
                    if (pstepplayer->jtpipe.nState != -1 && (LO *)pstepplayer->jtpipe.pshape == plo)
                    {
                        break;
                    }
                    pspot->GetClosestParam((JT *)pstepplayer, (float *)&place.qwPos);
                    place.plo = plo;
                }
                else
                {
                    GetJsgLocation(pjsg, plo, &place.qwPos, 0);
                    GuardSpawnerGroundSnap(&place.qwPos, 0, 0, 100.0f, -150.0f);
                }
            }
            RespawnAtSafeSpot(pstepplayer, &place);
            if (prelaycmd->respawn.nUnk5 == 1)
            {
                pstepplayer->jtair.fCanDoubleJump = prelaycmd->respawn.nUnk5;
            }
            if (FIsBasicDerivedFrom(pstepplayer, (CID)53))
            {
                ((JT *)pstepplayer)->janim.nUnk10 = prelaycmd->respawn.nUnk6;
            }
            SetJsgJt(pjsg, prelaycmd, pstepplayer);
        }
        break;
    case RELAYCK_LaunchTo:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)52))
        {
            VECTOR4 pos;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->launchTo.oidTarget);
            STEP *pstep = (STEP *)pjsg->ploActor;
            GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
            LaunchJtToPos(pstep, *(qword *)&pos, prelaycmd->launchTo.gUnk5, prelaycmd->launchTo.gUnk6);
        }
        break;
    case RELAYCK_AddObject:
    case RELAYCK_RemoveObject:
    {
        LO *plo = FadeAsegObjectEvent(&D_003DD490, prelaycmd->relayck == RELAYCK_AddObject, prelaycmd->addObject.oidObj, prelaycmd->addObject.dtFade, pjsg->ploContext,
                                      (ASEG *)pjsg->prelay);
        if (plo && !prelaycmd->fAsync && FIsBasicDerivedFrom(plo, (CID)48))
        {
            pjsg->ploFade = plo;
        }
        break;
    }
    case RELAYCK_SetPartFlag:
    case RELAYCK_ClearPartFlag:
        SetAsegPartFlagEvent(&D_003DD490, prelaycmd->relayck == RELAYCK_SetPartFlag, prelaycmd->setPartFlag.oidAseg, prelaycmd->setPartFlag.oidPart, 0, pjsg->ploContext,
                             (ASEG *)pjsg->prelay);
        break;
    case RELAYCK_ShowPart:
    case RELAYCK_HidePart:
        ShowAsegPartEvent(&D_003DD490, prelaycmd->relayck == RELAYCK_ShowPart, prelaycmd->showPart.oidAseg, prelaycmd->showPart.oidPart, 0, pjsg->ploContext,
                          (ASEG *)pjsg->prelay);
        break;
    case RELAYCK_Unk32:
        JoltCm(&D_002DDF40, prelaycmd->unk32.nUnk4);
        break;
    case RELAYCK_Rumble:
        TriggerJoyRumbleRumk(&D_002DFC00, (RUMK)prelaycmd->rumble.rumk, prelaycmd->rumble.dtRumble);
        break;
    case RELAYCK_Explode:
        ExplodeAsegEvent(&D_003DD490, prelaycmd->explode.oidA, prelaycmd->explode.oidB, prelaycmd->explode.oidC, pjsg->ploContext, (ASEG *)pjsg->prelay,
                         prelaycmd->explode.dtDelay);
        break;
    case RELAYCK_Eyes:
        SetAsegEyesEvent(&D_003DD490, prelaycmd->eyes.oidA, prelaycmd->eyes.oidB, pjsg->ploContext, (ASEG *)pjsg->prelay, prelaycmd->eyes.dtEyes);
        break;
    case RELAYCK_Sound:
        PlayAsegSoundEvent(&D_003DD490, prelaycmd->sound.idSnd, prelaycmd->sound.oidA, prelaycmd->sound.nUnk6, prelaycmd->sound.nUnk7, prelaycmd->sound.gUnk8,
                           prelaycmd->sound.gUnk9, prelaycmd->sound.gUnk10, 0, pjsg->ploContext, (ASEG *)pjsg->prelay, &pjsg->psnd);
        break;
    case RELAYCK_StopSound:
        StopAsegSoundEvent(&D_003DD490, prelaycmd->stopSound.idSnd, (ASEG *)pjsg->prelay, &pjsg->psnd);
        break;
    case RELAYCK_CamBlendFrom:
    {
        float dtBlend;
        int fSum;
        if (prelaycmd->camBlendFrom.dtBlend == 0.0f)
        {
            dtBlend = 0.0f;
            fSum = 1;
        }
        else
        {
            dtBlend = prelaycmd->camBlendFrom.dtBlend;
            fSum = 0;
        }
        RELAYCMD *prelaycmdTo = 0;
        int cBlock = 0;
        for (RELAYCMD *prelaycmdT = prelaycmd + 1; prelaycmdT < pjsg->prelaycmdLast && !prelaycmdTo; prelaycmdT++)
        {
            switch (prelaycmdT->relayck)
            {
            case RELAYCK_BlockBegin:
                cBlock++;
                break;
            case RELAYCK_BlockEnd:
                cBlock--;
                break;
            case RELAYCK_CamBlendTo:
                if (cBlock == 0)
                {
                    prelaycmdTo = prelaycmdT;
                }
                break;
            default:
                if (cBlock == 0 && fSum)
                {
                    dtBlend += DtJsgCommand(pjsg, prelaycmdT);
                }
                break;
            }
        }
        if (prelaycmdTo)
        {
            StartJsgCamBlend(pjsg, prelaycmd, prelaycmdTo, dtBlend);
        }
        break;
    }
    case RELAYCK_CamBlendTo:
        if (pjsg->tBlendEnd != D_002A7AD0)
        {
            break;
        }
        if (prelaycmd->camBlendTo.nUnk16 != 0)
        {
            PopJsgCamBlend(pjsg, prelaycmd->camBlendTo.nUnk17);
        }
        else
        {
            pjsg->fUnk9c = 0;
        }
        break;
    case RELAYCK_Unk3A:
        PopJsgCamBlend(pjsg, prelaycmd->unk3A.nUnk4);
        break;
    case RELAYCK_StartJtMode:
    {
        LO *ploStep = PloFindJsgObject(pjsg, prelaycmd->startJtMode.oidStep);
        if (ploStep)
        {
            FIsBasicDerivedFrom(ploStep, (CID)51);
        }
        LO *ploHud = PloFindSwObject(&D_003DD490, (GRFFSO)0x405, prelaycmd->startJtMode.oidUnk6, 0);
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->startJtMode.oidActor);
        if (plo && FIsBasicDerivedFrom(plo, (CID)52))
        {
            pjsg->pstepplayerMode = (STEPPLAYER *)plo;
            StartJtMode(&((STEPPLAYER *)plo)->jtmode, 1, (STEP *)ploStep, ploHud);
        }
        break;
    }
    case RELAYCK_EndJtMode:
        EndJsgJtMode(pjsg);
        break;
    case RELAYCK_TweenPosObj:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->tweenPosObj.oidObj);
        D_002DDF40.cpzoom.SetOwner(plo);
        break;
    }
    case RELAYCK_TweenAimObj:
    {
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->tweenAimObj.oidObj);
        D_002DDF40.cpzoom.SetFocus(plo);
        break;
    }
    case RELAYCK_TweenTarget:
        D_002DDF40.cpzoom.SetPose(prelaycmd->tweenTarget.qwPos, *(qword *)&prelaycmd->tweenTarget.radX, prelaycmd->tweenTarget.gFov);
        break;
    case RELAYCK_TweenTime:
        D_002DDF40.cpzoom.SetZoom(prelaycmd->tweenTime.gUnk4);
        break;
    case RELAYCK_TweenStart:
        D_002DDF40.cpzoom.Reset();
        break;
    case RELAYCK_ButtonPrompt:
        D_004435F0.grfbtnPrompt = prelaycmd->buttonPrompt.grfbtn & 0xF0;
        break;
    case RELAYCK_StopLine:
        func_001DD820(D_003DA530);
        break;
    case RELAYCK_Line:
        StartDialogueLine(&D_003DD040, prelaycmd->line.idLine, prelaycmd->line.fWait);
        pjsg->prelaycmdLine = prelaycmd;
        break;
    case RELAYCK_ClearSubtitle:
        ClearSubtitle(&D_004435F0);
        break;
    case RELAYCK_Unk47:
        if (ploActor)
        {
            AddJsgStepHold(pjsg, ploActor, prelaycmd->unk47.nUnk5, prelaycmd->unk47.nUnk6);
        }
        break;
    case RELAYCK_Unk48:
        if (ploActor)
        {
            RemoveJsgStepHold(pjsg, ploActor, prelaycmd->unk48.nUnk5, prelaycmd->unk48.nUnk6);
        }
        break;
    case RELAYCK_Unk49:
        if (D_002DE570)
        {
            JSGENT aent[6];
            uint cent = 0;
            int cBlock = 0;
            for (RELAYCMD *prelaycmdT = prelaycmd + 1; prelaycmdT < pjsg->prelaycmdLast; prelaycmdT++)
            {
                switch (prelaycmdT->relayck)
                {
                case RELAYCK_BlockBegin:
                    cBlock++;
                    break;
                case RELAYCK_BlockEnd:
                    cBlock--;
                    break;
                case RELAYCK_Unk4A:
                    if (cBlock == 0 && cent < 6)
                    {
                        JSGENT *pent = &aent[cent++];
                        pent->g = prelaycmdT->unk4A.gUnk4;
                        pent->n = prelaycmdT->unk4A.nUnk5;
                    }
                    break;
                case RELAYCK_Unk49:
                    if (cBlock == 0)
                    {
                        prelaycmdT = pjsg->prelaycmdLast;
                    }
                    break;
                }
            }
            func_001531A8(D_002DE570, prelaycmd->unk49.nUnk4, cent, aent);
        }
        break;
    case RELAYCK_Unk4A:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)68))
        {
            if (D_002DE570)
            {
                func_001532D8(D_002DE570, pjsg->ploActor, prelaycmd->unk4A.nUnk5, prelaycmd->unk4A.nUnk6, prelaycmd->unk4A.gUnk4);
            }
        }
        break;
    case RELAYCK_PlayAsegDefault:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)68))
        {
            LO *plo = pjsg->ploActor;
            LO *ploOther;
            if (prelaycmd->playAsegDefault.oidOther == OID_Nil)
            {
                ploOther = 0;
            }
            else
            {
                ploOther = PloFindSwObject(&D_003DD490, (GRFFSO)4, prelaycmd->playAsegDefault.oidOther, plo);
            }
            PlayJtAsegDefault(plo, ploOther, 0);
        }
        break;
    case RELAYCK_StartMusic:
        StartMusic(&D_003DD040, prelaycmd->startMusic.idMusic);
        break;
    case RELAYCK_StopMusic:
        StopMusic(&D_003DD040, prelaycmd->stopMusic.idMusic);
        break;
    case RELAYCK_Unk4F:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            MURRAY *pmurray = (MURRAY *)pjsg->ploActor;
            LO *plo = PloFindJsgObject(pjsg, (OID)prelaycmd->unk4F.nUnk5);
            if (plo)
            {
                SMA *psma = PsmaFindOnAlo((ALO *)plo, OID_Nil);
                if (psma)
                {
                    func_001B9C00(pmurray, psma, prelaycmd->unk4F.nUnk7, prelaycmd->unk4F.nUnk6, prelaycmd->unk4F.nUnk8);
                }
            }
        }
        break;
    case RELAYCK_Unk50:
    case RELAYCK_Unk51:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)55))
        {
            SetJsgJt(pjsg, prelaycmd, pjsg->ploActor);
        }
        break;
    case RELAYCK_HintBox:
        SetHintBoxText(&D_003DA0C0, prelaycmd->hintBox.idHint, prelaycmd->hintBox.dtHint);
        break;
    case RELAYCK_ClearHintBox:
        ClearIfSlotId(&D_003DA0C0, prelaycmd->clearHintBox.idHint);
        break;
    case RELAYCK_SetSsgGoal:
    {
        STEP *pstep = (STEP *)PloFindJsgObject(pjsg, prelaycmd->setSsgGoal.oidStep);
        SSG *pssg = (SSG *)PloFindJsgObject(pjsg, prelaycmd->setSsgGoal.oidSsg);
        SetStepSsgGoal(pstep, pssg);
        if (!prelaycmd->fAsync)
        {
            pjsg->pstepSsgGoal = pstep;
            pjsg->pssgGoal = pssg;
        }
        break;
    }
    case RELAYCK_ResetStepGoal:
    {
        STEP *pstep = (STEP *)PloFindJsgObject(pjsg, prelaycmd->resetStepGoal.oidStep);
        LO *plo = PloFindJsgObject(pjsg, prelaycmd->resetStepGoal.oidUnk5);
        ResetStepGoal(pstep, (SSG *)plo);
        break;
    }
    case RELAYCK_Unk57:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            func_001B9880((MURRAY *)pjsg->ploActor);
        }
        break;
    case RELAYCK_Unk58:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)55))
        {
            BENTLEY *pbentley = (BENTLEY *)pjsg->ploActor;
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->unk58.oidShip);
            if (plo && FIsBasicDerivedFrom(plo, (CID)54))
            {
                SetJtAirJump(&pbentley->jtair, (pbentley->jtair.nState == 0x11) ? 3 : 1, 0, 0);
                SetJsgJt(pjsg, prelaycmd, pbentley);
            }
        }
        break;
    case RELAYCK_Unk59:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (!FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            break;
        }
        {
            MURRAY *pmurray = (MURRAY *)pjsg->ploActor;
            if (pmurray->moverec.nState < 7)
            {
                if (pmurray->moverec.nState < 4)
                {
                    break;
                }
                LO *plo = PloFindJsgObject(pjsg, prelaycmd->unk59.oidShip);
                if (plo)
                {
                    func_00220508(&pmurray->moverec, plo);
                }
            }
        }
        break;
    case RELAYCK_Unk5A:
        if (!pjsg->ploActor)
        {
            break;
        }
        if (!FIsBasicDerivedFrom(pjsg->ploActor, (CID)54))
        {
            break;
        }
        {
            MURRAY *pmurray = (MURRAY *)pjsg->ploActor;
            if (pmurray->moverec.nState < 7)
            {
                if (pmurray->moverec.nState < 4)
                {
                    break;
                }
                prelaycmd->unk5A.nUnk6 = pmurray->moverec.nUnk00;
                LO *plo = PloFindJsgObject(pjsg, prelaycmd->unk5A.oidShip);
                if (plo)
                {
                    func_00220508(&pmurray->moverec, plo);
                    SetJsgJt(pjsg, prelaycmd, pmurray);
                }
            }
        }
        break;
    }
    if (GrfUpdateLo(pjsg->prelay) & 0x100)
    {
        pjsg->tCommand = D_002DDED0.tReal;
    }
    else
    {
        pjsg->tCommand = D_002DDED0.t;
    }
}
