// FIsJsgCommandComplete: ported from Lilly's personal Sly 2 decomp (src/P2/jsg.c), reduced to the declarations it needs.
// Every global is named by its address; the function keeps its readable name and gets the team symbol through __asm__.
// Globals: D_002BD850 (D_002BD850), D_002C3620 (D_002C3620), D_002C3624 (D_002C3624), D_002C3628 (D_002C3628), D_002C362C (D_002C362C), D_003DA530 (D_003DA530), D_002DDED0 (g_clock), D_002DDF40 (g_cm), D_002DFC00 (g_joy), D_003DD040 (g_sndmgr), D_004435F0 (g_subtitle), D_003DD490 (g_sw)
// 0x001E3890 FIsJsgCommandComplete__FP3JSGP8RELAYCMD  (T1306, our name; MATCH)
// Whether the command the script is on has finished, by kind; UpdateJsg asks it of the current command (a command that is fAsync is not asked) and
// ReleaseJsgJtIfComplete of the one that took the character. A kind with nothing to wait for (and every kind past the table) is complete at once.
// The waits (the table is in re/notes/relay-commands-t1289.md, "T1306"): WaitBlocks / WaitAlarm / FireRelay / ApplySm / WidgetShow / PlayAseg wait for the thing
// the runner started and let go of it when it is done; the character commands (PipeTo, GoTo, FollowPath, WalkTo, StepCmd1D..22 / 29, Respawn, LaunchTo, Unk1B,
// HeroState, Unk50 / 51 / 58 / 59 / 5A) wait for the character (pjt / ploActor) to get there or reach a state; Face / Unk32 / Rumble / HintBox wait their time
// since tCommand; Line / StopLine / Unk52 / WaitTimer / ButtonPrompt wait for the dialogue; the camera ones (CamBlendTo, Tween*) for the camera.
// The result is one variable (the game's s2) assigned in every arm; a command that has several "done at once" exits writes them as `else fComplete = 1`.
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
#include "sly2/sw.h"
int FIsBasicDerivedFrom(BASIC *pbasic, CID cid);
inline int FIsLoInWorld(LO *plo)
{
    return plo->fInWorld != 0;
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
int FIsSmaRetracted(SMA *psma);
OID GetSmaCur(SMA *psma);
int FFloatsNear(float g1, float g2, float gEpsilon);
float GLimitLm(LM *plm, float g);
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
extern CLOCK D_002DDED0;
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
struct SLOT
{
      SLOT *pslotNext;
};
struct RUMINS
{
      byte fHighSpeedMotor;
      byte bLowSpeedMotor;
      byte unk1;
      byte unk2;
      float dt;
};
struct RUMPAT
{
      int crumins;
      RUMINS arumins[32];
};
struct RUMBLE
{
      int nPort;
      int nSlot;
      RUMS rums;
      RUMPAT *prumpat;
      int irumins;
      float dtRumble;
      float dtRumins;
};
struct JOY
{
      byte abPadDma[0x1000];
      int nPort;
      int nSlot;
      int term;
      JOYS joys;
      JOYK joyk;
      float tJoys;
      float tRead;
      float tActive;
      int dtIdle;
      undefined4 unk_0x1024;
      undefined4 unk_0x1028;
      undefined4 unk_0x102c[4];
      float dxLeft;
      float dyLeft;
      float uDeflectLeft;
      byte bXLeft;
      byte bYLeft;
      ushort unk_0x104a;
      int fStickMovedLeft;
      LM almDeflect[4];
      float dxRight;
      float dyRight;
      float sRight;
      byte bXRight;
      byte bYRight;
      ushort unk_0x107e;
      int fStickMovedRight;
      LM almDeflect2[4];
      float dxdtLeft;
      float dydtLeft;
      float dxdtRight;
      float dydtRight;
      uint grfbtn;
      uint grfbtnPressed;
      uint grfbtnLatched;
      byte mpbtnpb[12];
      RUMBLE rumble;
      int fRumbleEnabled;
      float uDpad;
      undefined4 unk_0x10f0[4];
};
extern JOY D_002DFC00;
void LatchButtonEdges(JOY *pjoy, uint grfbtn);
struct RSMG
{
    OID oidRoot;
    OID oidSM;
    OID oidTriggerGoal;
    OID oidUntriggerGoal;
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
int FAsegaStopped(ASEGA *pasega);
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
struct BHV193 : public BHV
{
      undefined4 nUnk30;
      float gUnk34;
      int nUnk38;
      float gUnk3c;
      qword qwUnk40;
};
int FBhv193AtGoal(BHV193 *pbhv);
int FBhv166NearEnd(BHV166 *pbhv);
struct LOREF
{
      LO *plo;
      undefined4 unk_0x04;
};
void GetJtPipeLimits(JTPIPE *pjtpipe, float *pagLimit);
int FStepHoldArrived(STEPPLAYER *pstepplayer);
int FStepPutDownDone(STEPPLAYER *pstepplayer);
int FStepHoldOn9Done(STEPPLAYER *pstepplayer);
int FStepGrab0xCDone(STEPPLAYER *pstepplayer);
int FStepClipPlaying(STEPPLAYER *pstepplayer);
void StopStepClip(STEPPLAYER *pstepplayer);
void SetJtGoalPos(JTGOAL *pjtgoal, qword qwPos);
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
void GetJsgLocation(JSG *pjsg, LO *plo, qword *ppos, float *prad);
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
static inline qword QwLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline float GLengthXY(qword v)
{
    return GFromVf(QwLengthXY(v));
}
static inline float GYOfQw(qword qw)
{
    float y;
    __asm__("prot3w %0, %1" : "=r"(y) : "r"(qw));
    return y;
}
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(qw));
    return z;
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
void RemoveRelayAlarm(RELAY *prelay, ALARM *palarm);
int FRelayHasAlarm(RELAY *prelay, ALARM *palarm);
struct SUBTITLE
{
      int idString;
      RGBA rgba;
      int grfbtnPrompt;
};
extern SUBTITLE D_004435F0;
int FStepGoalIs(STEP *pstep, SSG *pssg);
struct PARTLINK
{
      undefined4 unk_0x00;
      void *precA;
      void *precB;
      void *precC;
      undefined4 unk_0x10[0xD0 / 4];
};
static inline float GXOfQwAct(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
JSG *PjsgFindRelayBlock(RELAY *prelay, OID oidLabel, JSG *pjsgExcept);
void PopJsgCamBlend(JSG *pjsg, int n);
void ReleaseJsgSma(JSG *pjsg);
extern SNDMGR D_003DD040;
extern char D_003DA530[];
LO *PloFindJsgObject(JSG *pjsg, OID oid);
extern "C" BHV *PbhvFindStep(STEP *pstep, int cid);
extern "C" int FObjHasTarget(LO *plo, OID oid);
extern "C" int func_002121C8(LO *plo);
extern "C" void func_001E5468(JSG *pjsg);
extern "C" int GrfHandleJoyBtn(JOY *pjoy, uint grfbtn);
extern "C" int FScreenShown(void *pv);
extern "C" int FCurrentIdMatches(SNDMGR *psndmgr, int idLine);
extern "C" float func_001FE888(SNDMGR *psndmgr);
extern "C" float GetFirstTimerSeconds(SNDMGR *psndmgr);
extern "C" void ClearBhvstk(BHVSTK *pbhvstk);
extern float D_002BD850;
extern float D_002C3620;
extern float D_002C3624;
extern float D_002C3628;
extern float D_002C362C;
static inline float TJsgNow(JSG *pjsg)
{
    if (GrfUpdateLo(pjsg->prelay) & 0x100)
    {
        return D_002DDED0.tReal;
    }
    return D_002DDED0.t;
}
int FIsJsgCommandComplete(JSG *pjsg, RELAYCMD *prelaycmd) __asm__("FIsJsgCommandComplete__FP3JSGP8RELAYCMD");
/** @sly2-annot addr=0x1E3890
 * Purpose: Whether the given command has finished, by kind
 * Info: proposed name FIsJsgCommandComplete (T1); TU relay
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
int FIsJsgCommandComplete(JSG *pjsg, RELAYCMD *prelaycmd)
{
    int fComplete;
    MURRAY *pmurray;
    switch (prelaycmd->relayck)
    {
    case RELAYCK_WaitBlocks:
    {
        int i;
        fComplete = 1;
        for (i = 0; i < prelaycmd->waitBlocks.coid; i++)
        {
            if (PjsgFindRelayBlock(pjsg->prelay, (OID)prelaycmd->waitBlocks.aoid[i], pjsg))
            {
                fComplete = 0;
                break;
            }
        }
        break;
    }
    case RELAYCK_WaitAlarm:
        fComplete = FRelayHasAlarm(pjsg->prelay, (ALARM *)prelaycmd->waitAlarm.idAlarm);
        if (fComplete && prelaycmd->waitAlarm.fForget)
        {
            RemoveRelayAlarm(pjsg->prelay, (ALARM *)prelaycmd->waitAlarm.idAlarm);
        }
        break;
    case RELAYCK_PlayAseg:
        fComplete = 0;
        if (pjsg->pstepAseg)
        {
            if (!FStepClipPlaying((STEPPLAYER *)pjsg->pstepAseg))
            {
                pjsg->pstepAseg = 0;
                fComplete = 1;
            }
        }
        else
        {
            if (pjsg->pasegaCur && FAsegaStopped(pjsg->pasegaCur))
            {
                pjsg->pasegaCur->VUnk01();
                pjsg->pasegaCur = 0;
            }
            fComplete = pjsg->pasegaCur == 0;
        }
        break;
    case RELAYCK_FireRelay:
        if (pjsg->prelayWait && !pjsg->prelayWait->fFiring)
        {
            pjsg->prelayWait = 0;
        }
        fComplete = pjsg->prelayWait == 0;
        break;
    case RELAYCK_ApplySm:
        if (pjsg->psmaCur && (FIsSmaRetracted(pjsg->psmaCur) || pjsg->fSmaReached))
        {
            ReleaseJsgSma(pjsg);
        }
        fComplete = pjsg->psmaCur == 0;
        break;
    case RELAYCK_WidgetShow:
        if (pjsg->ploWidget && func_002121C8(pjsg->ploWidget))
        {
            func_001E5468(pjsg);
        }
        fComplete = pjsg->ploWidget == 0;
        break;
    case RELAYCK_PipeTo:
        if (pjsg->pjt)
        {
            VECTOR4 vLimits;
            GetJtPipeLimits(&((STEPPLAYER *)pjsg->pjt)->jtpipe, (float *)&vLimits);
            float s = GLimitLm((LM *)&vLimits, pjsg->sPipe);
            fComplete = fabsf(((STEPPLAYER *)pjsg->pjt)->jtpipe.s - s) < prelaycmd->pipeTo.gTolerance;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk1B:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->ploActor;
            if (pstepplayer->jtair.nState >= 0xB && pstepplayer->jtair.nState <= 0xD)
            {
                fComplete = 0;
            }
            else if (pstepplayer->jtgrab.nKind != -1 && (pstepplayer->jtgrab.nState == 0 || pstepplayer->jtgrab.nState == 0xD))
            {
                pstepplayer->VUnk87(0);
                fComplete = 1;
            }
            else
            {
                fComplete = 1;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_HeroState:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x35))
        {
            JT *pjt = (JT *)pjsg->ploActor;
            switch (prelaycmd->heroState.nState)
            {
            case 2:
                fComplete = pjt->jtas == -1;
                break;
            case 4:
                fComplete = pjt->jts != JTS_Air;
                break;
            case 8:
                fComplete = pjt->jts != 9;
                break;
            default:
                fComplete = 1;
                break;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd1D:
    case RELAYCK_StepCmd21:
        if (pjsg->pjt)
        {
            fComplete = FStepHoldArrived((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd1E:
        if (pjsg->pjt)
        {
            fComplete = FStepPutDownDone((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd1F:
        if (pjsg->pjt)
        {
            fComplete = FStepHoldOn9Done((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd22:
        if (pjsg->pjt)
        {
            fComplete = FStepGrab0xCDone((STEPPLAYER *)pjsg->pjt);
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Face:
        fComplete = prelaycmd->face.dtWait < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_GoTo:
    {
        LO *pjt = pjsg->pjt;
        if (pjt)
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->goTo.oidTarget);
            if (plo)
            {
                qword pos;
                GetJsgLocation(pjsg, plo, &pos, 0);
                fComplete = GLengthXY(QwSub(PosLoWorld(pjt), pos)) < prelaycmd->goTo.gArrive;
                SetJtGoalPos(&((STEPPLAYER *)pjt)->jtgoal, pos);
            }
            else
            {
                fComplete = 1;
            }
            if (fComplete && prelaycmd->goTo.idClip != -1)
            {
                StopStepClip((STEPPLAYER *)pjt);
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    }
    case RELAYCK_FollowPath:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            BHV166 *pbhv = (BHV166 *)PbhvFindStep(pstep, 0xA8);
            fComplete = 0;
            if (!pbhv || FBhv166NearEnd(pbhv))
            {
                fComplete = 1;
            }
            if (fComplete)
            {
                ClearBhvstk(&pstep->bhvstk);
                if (prelaycmd->followPath.idClip != -1)
                {
                    StopStepClip((STEPPLAYER *)pstep);
                }
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_WalkTo:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            STEP *pstep = (STEP *)pjsg->ploActor;
            BHV *pbhv = PbhvFindStep(pstep, 0xC1);
            fComplete = 0;
            if (!pbhv || FBhv193AtGoal((BHV193 *)pbhv))
            {
                fComplete = 1;
            }
            if (fComplete)
            {
                ClearBhvstk(&pstep->bhvstk);
                if (prelaycmd->walkTo.idClip != -1)
                {
                    StopStepClip((STEPPLAYER *)pstep);
                }
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StepCmd29:
    {
        STEPPLAYER *pstepplayer = (STEPPLAYER *)pjsg->pjt;
        if (pstepplayer && pstepplayer->jtgrab.nKind != -1 && pstepplayer->jtgrab.nState == 8)
        {
            LO *plo = PloFindJsgObject(pjsg, prelaycmd->stepCmd29.oidTarget);
            if (plo)
            {
                VECTOR4 pos;
                GetJsgLocation(pjsg, plo, (qword *)&pos, 0);
                fComplete = GLengthXY(QwSub(PosLoWorld(pstepplayer), *(qword *)&pos)) < prelaycmd->stepCmd29.gArrive;
            }
            else
            {
                fComplete = 1;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    }
    case RELAYCK_Respawn:
        if (pjsg->pjt)
        {
            fComplete = ((STEPPLAYER *)pjsg->pjt)->jtair.nState == -1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_LaunchTo:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            fComplete = ((STEPPLAYER *)pjsg->ploActor)->jtair.nState == -1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_AddObject:
    case RELAYCK_RemoveObject:
        if (pjsg->ploFade)
        {
            if (prelaycmd->relayck == RELAYCK_AddObject)
            {
                if (FIsLoInWorld(pjsg->ploFade) && ((SO *)pjsg->ploFade)->uAlpha == 1.0f)
                {
                    pjsg->ploFade = 0;
                }
            }
            else if (!FIsLoInWorld(pjsg->ploFade))
            {
                pjsg->ploFade = 0;
            }
        }
        fComplete = pjsg->ploFade == 0;
        break;
    case RELAYCK_Unk32:
        fComplete = D_002BD850 < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_Rumble:
        fComplete = prelaycmd->rumble.dtRumble < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_CamBlendTo:
        fComplete = pjsg->tBlendEnd <= D_002DDED0.t;
        if (fComplete)
        {
            if (prelaycmd->camBlendTo.nUnk16 != 0)
            {
                PopJsgCamBlend(pjsg, prelaycmd->camBlendTo.nUnk17);
            }
            else
            {
                pjsg->fUnk9c = 0;
            }
        }
        break;
    case RELAYCK_StartJtMode:
        if (pjsg->pstepplayerMode)
        {
            fComplete = pjsg->pstepplayerMode->jtmode.hud.nState == 1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_EndJtMode:
        if (pjsg->pstepplayerMode)
        {
            fComplete = pjsg->pstepplayerMode->jtmode.hud.nState == -1;
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_TweenPosObj:
        fComplete = D_002DDF40.cpzoom.FAtTargetFocus();
        break;
    case RELAYCK_TweenAimObj:
        fComplete = D_002DDF40.cpzoom.FAtTargetEye();
        break;
    case RELAYCK_TweenTime:
        fComplete = D_002DDF40.cpzoom.FAtTargetZoom();
        break;
    case RELAYCK_ButtonPrompt:
        if (prelaycmd->buttonPrompt.oidWho == -1)
        {
            fComplete = GrfHandleJoyBtn(&D_002DFC00, prelaycmd->buttonPrompt.grfbtn);
        }
        else
        {
            POCTL *ppoctl;
            for (ppoctl = (POCTL *)D_003DD490.slPoctl.head; ppoctl; ppoctl = (POCTL *)ppoctl->pvUnk84)
            {
                if (FObjHasTarget(ppoctl->ppo, prelaycmd->buttonPrompt.oidWho))
                {
                    break;
                }
            }
            fComplete = 0;
            if (!ppoctl || ppoctl->FConsumePressed(prelaycmd->buttonPrompt.grfbtn))
            {
                fComplete = 1;
            }
            if (fComplete)
            {
                LatchButtonEdges(&D_002DFC00, prelaycmd->buttonPrompt.grfbtn);
            }
        }
        if (fComplete)
        {
            D_004435F0.grfbtnPrompt = 0;
        }
        break;
    case RELAYCK_Unk43:
        if (pjsg->ploActor && FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x34))
        {
            fComplete = 0;
            if (((STEPPLAYER *)pjsg->ploActor)->VUnk88() == prelaycmd->unk43.nUnk4)
            {
                fComplete = 1;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_StopLine:
        fComplete = FScreenShown(D_003DA530) == 0;
        break;
    case RELAYCK_Line:
        fComplete = FCurrentIdMatches(&D_003DD040, prelaycmd->line.idLine) == 0;
        break;
    case RELAYCK_Unk52:
    {
        int idLine = -1;
        if (pjsg->prelaycmdLine)
        {
            idLine = pjsg->prelaycmdLine->line.idLine;
        }
        if (!FCurrentIdMatches(&D_003DD040, idLine))
        {
            fComplete = 1;
        }
        else if (prelaycmd->unk52.dtWait < 0.0f)
        {
            fComplete = 0;
        }
        else
        {
            fComplete = 0.0f <= func_001FE888(&D_003DD040) - prelaycmd->unk52.dtWait;
        }
        break;
    }
    case RELAYCK_Unk4A:
        fComplete = prelaycmd->unk4A.gUnk4 <= GetFirstTimerSeconds(&D_003DD040);
        break;
    case RELAYCK_WaitTimer:
        fComplete = prelaycmd->waitTimer.dtWait <= GetFirstTimerSeconds(&D_003DD040);
        break;
    case RELAYCK_Unk4F:
        fComplete = 0;
        if (FIsBasicDerivedFrom(pjsg->ploActor, (CID)0x36))
        {
            MURRAY *pmurrayActor = (MURRAY *)pjsg->ploActor;
            if (pmurrayActor->nState != 6)
            {
                fComplete = 1;
            }
            else if (prelaycmd->unk4F.nUnk7 != -1)
            {
                fComplete = GetSmaCur(pmurrayActor->psmaUnk1034) == prelaycmd->unk4F.nUnk7;
            }
        }
        break;
    case RELAYCK_Unk50:
        fComplete = 0;
        if (!pjsg->pjt || ((BENTLEY *)pjsg->pjt)->nState == 6)
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk51:
        fComplete = 0;
        if (!pjsg->pjt || ((BENTLEY *)pjsg->pjt)->nState != 6)
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_HintBox:
        fComplete = prelaycmd->hintBox.dtHint < TJsgNow(pjsg) - pjsg->tCommand;
        break;
    case RELAYCK_SetSsgGoal:
        if (pjsg->pssgGoal && !FStepGoalIs(pjsg->pstepSsgGoal, pjsg->pssgGoal))
        {
            pjsg->pstepSsgGoal = 0;
            pjsg->pssgGoal = 0;
        }
        fComplete = pjsg->pssgGoal == 0;
        break;
    case RELAYCK_Unk58:
        if (pjsg->pjt)
        {
            MURRAY *pmurrayShip = (MURRAY *)PloFindJsgObject(pjsg, prelaycmd->unk58.oidShip);
            if (TJsgNow(pjsg) - pjsg->tCommand > D_002C3620)
            {
                fComplete = 1;
            }
            else
            {
                fComplete = 0;
                if (pmurrayShip->moverec.nState < 7)
                {
                    fComplete = pmurrayShip->moverec.nState >= 4;
                }
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk59:
        pmurray = (MURRAY *)pjsg->pjt;
        if (pmurray)
        {
            if (TJsgNow(pjsg) - pjsg->tCommand > D_002C3624)
            {
                fComplete = 1;
            }
            else
            {
                qword qwNew = pmurray->moverec.qwPosNew;
                qword qwOld = pmurray->moverec.qwPosOld;
                float gEps = D_002C3628;
                fComplete = FFloatsNear(GXOfQwAct(qwNew), GXOfQwAct(qwOld), gEps) && FFloatsNear(GYOfQw(qwNew), GYOfQw(qwOld), gEps) &&
                            FFloatsNear(GZOfQw(qwNew), GZOfQw(qwOld), gEps);
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    case RELAYCK_Unk5A:
    {
        pmurray = (MURRAY *)pjsg->pjt;
        if (pmurray)
        {
            if (TJsgNow(pjsg) - pjsg->tCommand > D_002C362C)
            {
                fComplete = 1;
            }
            else if (pmurray->moverec.nState == -1 || pmurray->moverec.nState == 0)
            {
                if (pmurray->moverec.nState == 0)
                {
                    pmurray->VUnk87(0);
                }
                fComplete = ((STEPPLAYER *)prelaycmd->unk5A.nUnk6)->jtair.nState == -1;
            }
            else
            {
                fComplete = 0;
            }
        }
        else
        {
            fComplete = 1;
        }
        break;
    }
    default:
        fComplete = 1;
        break;
    }
    return fComplete;
}
