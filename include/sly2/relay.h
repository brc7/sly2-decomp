/* include/sly2/relay.h -- the relay script records: RELAYCMD (one command of a relay's list, 0x50 bytes; the
 * argument block is a union selected by relayck, filled by the AddRelay* Splice builders) and RELAYJSG (a relay's
 * running job-sequence instance, 0xF0 bytes). Layouts from src/game/gameplay/cutscene/relay.cpp (the former JSGV view
 * and its RELAYCMD); field names from their use there.
 *
 * Include after sly2/classes.h in relay.cpp: that TU emits RELAYJSG's vtable (key function AddJsgRef) and the ELF
 * has it after RELAY's. */
#ifndef SLY2_RELAY_H
#define SLY2_RELAY_H
#include "sly2/basic.h"
#include "sly2/cm.h"   /* CAMMOVE */

struct SSG;

/* One relay command (AddRelayCommand appends it to RELAY's arelaycmd). */
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
    } waitAndReleaseGrab;
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
    } joltCamera;
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
    } popCamBlend;
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
        int state;
        OID oidActor;
    } waitPlayerState;
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
    } stepHold;
    struct
    {
        OID oidActor;
        int nUnk5;
        int nUnk6;
    } stepRelease;
    struct
    {
        int nUnk4;
    } rhythmSequence;
    struct
    {
        float gUnk4;
        int nUnk5;
        int nUnk6;
        OID oidActor;
    } rhythmCue;
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
    } murrayApplySma;
    struct
    {
        OID oidActor;
    } waitBentleyGadget;
    struct
    {
        OID oidActor;
    } waitBentleyGadgetDone;
    struct
    {
        float dtWait;
    } waitLine;
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
    } murrayThrowAim;
    struct
    {
        OID oidShip;
        OID oidActor;
    } bentleyJumpOnMurray;
    struct
    {
        OID oidShip;
        OID oidActor;
    } murrayMoveTo;
    struct
    {
        OID oidShip;
        OID oidActor;
        int nUnk6;
    } murrayMoveToAndControl;
    };
};
typedef char assert_RELAYCMD_size[sizeof(RELAYCMD) == 0x50 ? 1 : -1];

/* A relay's running job-sequence instance (0xF0, slot heap g_sw.slotheapJsg; NewRelayJsg builds it by hand): a
   BASIC with its own reference count and message handler, vtable 0x2D8C08 (BASIC's class descriptor in slot 6, so
   not the registry's JSG). DRAFT name RELAYJSG. STEPPLAYER::pjsgScript points at one while a relay drives the
   player. */
struct RELAYJSG : BASIC
{
      int cref;
      RELAY *relay;
      LO *ploContext;
      LO *ploBlock;
      LO *ploActorDefault;
      LO *ploActor;
      RELAYCMD *prelaycmdFirst;
      RELAYCMD *prelaycmdLast;
      RELAYCMD *prelaycmdCur;
      float tCommand;
      int fHandsOff;
      int grfHandsOff;
      RELAYCMD *prelaycmdJt;
      LO *pjt;
      int fHoldBtn10;
      ASEGA *pasegaCur;
      RELAY *prelayWait;
      STEP *pstepAseg;
      SMA *psmaCur;
      int fSmaReached;
      LO *ploFade;
      LO *ploWidget;
      RELAYCMD *prelaycmdLine;
      TN *ptnCur;
      int fHideTn;
      void *pvFocus;   /* +0x74 what SetRelayScriptFocus aims the actor at (an LO or SHAPE; GetJsgTargetPosHeading) (inferred) */
      float sGoTo;
      float sPipe;
      float tBlendEnd;
      CAMMOVE cammove;   /* +0x84 the script's camera blend (StartRelayScriptCamBlend; SetCmPolicy's move, its pevent the script) */
      STEPPLAYER *pstepplayerMode;
      int cstepHold;
      STEP *apstepHold[4];
      int anHold5[4];
      int anHold6[4];
      STEP *pstepSsgGoal;
      SSG *pssgGoal;
      int fRun;
      DLE dleRelay;
      CRefPtr<SND> psnd;
    virtual void AddRef() __asm__("AddJsgRef");   /* slot 1; key function (relay.cpp) */
    virtual void Release() __asm__("ReleaseJsg");   /* slot 2 */
    virtual void HandleMessage(MSGID msgid, void *pv) __asm__("HandleJsgMessage");   /* slot 5 */
};
typedef char assert_RELAYJSG_prelaycmdJt[SLY2_OFFSETOF(RELAYJSG, prelaycmdJt) == 0x40 ? 1 : -1];
typedef char assert_RELAYJSG_dleRelay[SLY2_OFFSETOF(RELAYJSG, dleRelay) == 0xE4 ? 1 : -1];
typedef char assert_RELAYJSG_size[sizeof(RELAYJSG) == 0xF0 ? 1 : -1];

#endif
