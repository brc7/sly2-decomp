/* include/sly2/task.h -- mission graph objects: JOB (CID 0x84, cb 0x100) and TASK (CID 0x85, cb 0x110), both direct LO
 * subclasses with no new virtuals (vtable 48 = LO). Names are the devtools' (ram_inspector / slygame), not Sucker Punch's.
 * Offsets from runtime6 FINDINGS and devtools/pine_toolkit/slygame.py tasks() [R], the rest from game.cpp's code [S].
 * The current JOB/TASK/pending TASK are SW::currentJob/ploTaskCur/ploTaskPending (SW+0x3B0C..). */
#ifndef SLY2_TASK_H
#define SLY2_TASK_H
#include "sly2/basic.h"
#include "sly2/lo.h"

struct TASK;

/* SVKEY: a save-variable key (a path of symbol ids into the save schema): what GetGlobalSaveVar / SetGlobalSaveVar /
   GetSaveVarAddress take. A JOB / TASK embeds the keys of the save variables that mirror its fields; a zero count
   means the field is not saved. (save.cpp keeps its own copy for now.) */
struct SVKEY
{
      int c;        /* number of symbols, 0 none */
      short *ash;   /* the symbol ids */
};
typedef char assert_SVKEY_size[sizeof(SVKEY) == 8 ? 1 : -1];

/* STATEVIS: a visibility record of a JOB / TASK (ApplyStateLoVisibility): the object (found in context, -1 none) is
   added to the world while the owner's state is in stateMask and removed otherwise, on level (-1 any). */
struct STATEVIS
{
      int oidObject;
      int oidContext;
      int grfState;   /* bit mask of states (GetStateBit) */
      int level;
};
typedef char assert_STATEVIS_size[sizeof(STATEVIS) == 16 ? 1 : -1];

/* STATERSMG: a state-machine record of a JOB / TASK (TriggerStateRsmg): on entering state (-1 any) the state machine of
   root seeks / is set to goal (-1: the default goal 0x612..0x615 of the state); restoreOnly: only for restores. */
struct STATERSMG
{
      int nState;
      int oidRoot;
      int oidSm;
      int oidGoal;
      int fRestoreOnly;
      int level;
};
typedef char assert_STATERSMG_size[sizeof(STATERSMG) == 24 ? 1 : -1];

/* JOB: a job (mission) of the episode graph: its first and final task, completion sequence, game-event lists,
   visibility / state-machine records, counters and save-variable keys. A JOB's stateSource / stateRequester sit 4 bytes
   below a TASK's (a scope that saves them must know which it holds: game.cpp StateSourceScope). */
struct JOB : public LO
{
      int fDisabled;        /* +0x50 [R] */
      int ceventComplete;   /* +0x54 [S] game-event ids that complete the job (AddJobCompleteEvent, at most 32) */
      int *aeventComplete;  /* +0x58 [S] */
      int ceventFail;       /* +0x5C [S] game-event ids that fail the job (at most 32) */
      int *aeventFail;      /* +0x60 [S] */
      TASK *ptaskStart;     /* +0x64 [R] first task of the job */
      TASK *ptaskEnd;       /* +0x68 [R] final task (game.cpp IsJobComplete tests it) */
      int labelNameId;      /* +0x6C [S] name-plate string id, -1 none (waypoint.cpp) */
      int unknown70;        /* +0x70 [S] (game.cpp) */
      int survivesSafehouse; /* +0x74 [S] non-zero: entering the safehouse does not fail the job (tu_1D7238 SetCharSelectState; inferred) */
      int fReplay;          /* +0x78 [S] (inferred) a replay: finishing skips the end task's completion and the autosave */
      int cStart;           /* +0x7C [R] times the job was started (ram_inspector "count") */
      SL sltask;            /* +0x80 [S] the job's tasks (linked through TASK::sleJob) */
      int cvis;             /* +0x8C [S] visibility records */
      STATEVIS *avis;       /* +0x90 [S] */
      int crsmg;            /* +0x94 [S] state-machine records */
      STATERSMG *arsmg;     /* +0x98 [S] */
      float easeStartTime;  /* +0x9C [S] difficulty-ease thresholds (game.cpp) */
      float easeEndTime;    /* +0xA0 [S] */
      float dtElapsed;      /* +0xA4 [R] elapsed time in the job */
      float *difficulty;    /* +0xA8 [S] the job's difficulty rating: the save variable of svkDifficulty (difficulty.cpp) */
      int completionWidgetOid; /* +0xAC [S] (SetJobCompletionWidget, game.cpp) */
      int introLabel;       /* +0xB0 [S] */
      int outroLabel;       /* +0xB4 [S] */
      LO *completionWidget; /* +0xB8 [S] the job has a completion sequence when set */
      LO *widgetShared;     /* +0xBC [S] the shared completion widget (oid 0x379) */
      TASK *ptaskNext;      /* +0xC0 [S] the task the completion sequence continues with */
      int nPhase;           /* +0xC4 [R] completion phase (0 none, 1 intro, 2 outro, 3 shared widget) */
      float tPhase;         /* +0xC8 [S] clock.time when nPhase was entered */
      LO *ploExit;          /* +0xCC [S] the exit taken after the job when there is no next task */
      SVKEY svkStart;       /* +0xD0 [R] save-variable keys: cStart, */
      SVKEY svkComplete;    /* +0xD8 [S] the complete flag, */
      SVKEY svkElapsed;     /* +0xE0 [S] dtElapsed, */
      SVKEY svkDifficulty;  /* +0xE8 [S] the difficulty rating */
      int stateSource;      /* +0xF0 [S] what drives the current state change (game.cpp STATESOURCE) */
      int stateRequester;   /* +0xF4 [S] */
      undefined4 unk_0xf8[(0x100 - 0xf8) / 4];
    virtual void Init() __asm__("InitJob");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetJobClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadJobFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostJobLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateJob");   /* slot 16 */
};
typedef char assert_JOB_size[sizeof(JOB) == 0x100 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_JOB_ceventComplete[SLY2_OFFSETOF(JOB, ceventComplete) == 0x54 ? 1 : -1];
typedef char assert_JOB_ptaskStart[SLY2_OFFSETOF(JOB, ptaskStart) == 0x64 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_JOB_fReplay[SLY2_OFFSETOF(JOB, fReplay) == 0x78 ? 1 : -1];
typedef char assert_JOB_cStart[SLY2_OFFSETOF(JOB, cStart) == 0x7C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_JOB_sltask[SLY2_OFFSETOF(JOB, sltask) == 0x80 ? 1 : -1];
typedef char assert_JOB_cvis[SLY2_OFFSETOF(JOB, cvis) == 0x8C ? 1 : -1];
typedef char assert_JOB_easeStartTime[SLY2_OFFSETOF(JOB, easeStartTime) == 0x9C ? 1 : -1];
typedef char assert_JOB_difficulty[SLY2_OFFSETOF(JOB, difficulty) == 0xA8 ? 1 : -1];
typedef char assert_JOB_completionWidget[SLY2_OFFSETOF(JOB, completionWidget) == 0xB8 ? 1 : -1];
typedef char assert_JOB_nPhase[SLY2_OFFSETOF(JOB, nPhase) == 0xC4 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_JOB_ploExit[SLY2_OFFSETOF(JOB, ploExit) == 0xCC ? 1 : -1];
typedef char assert_JOB_svkStart[SLY2_OFFSETOF(JOB, svkStart) == 0xD0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_JOB_stateSource[SLY2_OFFSETOF(JOB, stateSource) == 0xF0 ? 1 : -1];

/* TASK: a task (step of a job): its state, owning job, prerequisite and successor tasks, records, warp and the
   character it is played with, its save-variable keys and the scratch of the state cascade and topological sort. */
struct TASK : public LO
{
      int fDisabled;        /* +0x50 [S] */
      int nState;           /* +0x54 [R] -1 unset, 0 locked, 1 available, 2 done (in job), 3 done */
      float tState;         /* +0x58 [R] clock.time when nState was entered */
      int objectiveTextId;  /* +0x5C [S] objective notice string id, -1 none */
      int fShowObjective;   /* +0x60 [S] post the objective notice while current */
      int cStart;           /* +0x64 [S] times the task was started */
      int completionCount;  /* +0x68 [S] times it was done in gameplay (difficulty.cpp) */
      int fOptional;        /* +0x6C [S] an available optional prerequisite counts as met */
      int fActivateWhenAvailable; /* +0x70 [S] becomes current when it becomes available */
      int fResumeHere;      /* +0x74 [S] restored available: becomes g_game's current task oid */
      float dtLimit;        /* +0x78 [S] time limit while available (SendTaskTimeout), 0 none */
      JOB *pjob;            /* +0x7C [R] owning JOB */
      LO *ploMarker;        /* +0x80 [S] shown only while the task is available and no job is current */
      SLE sleJob;           /* +0x84 [S] link in its job's sltask */
      DLE dleActive;        /* +0x88 [S] link in SW::dlTaskActive (the available tasks) */
      int *aoidPrereq;      /* +0x90 [S] prerequisite oids as loaded */
      int cptaskPrereq;     /* +0x94 [R] prerequisite tasks */
      TASK **aptaskPrereq;  /* +0x98 [R] */
      int *aoidSucc;        /* +0x9C [S] successor oids as loaded */
      int cptaskSucc;       /* +0xA0 [R] successor tasks */
      TASK **aptaskSucc;    /* +0xA4 [R] */
      int cvis;             /* +0xA8 [S] visibility records */
      STATEVIS *avis;       /* +0xAC [S] */
      int crsmg;            /* +0xB0 [S] state-machine records */
      STATERSMG *arsmg;     /* +0xB4 [S] */
      int warpOid;          /* +0xB8 [S] a task with a warp becomes the pending task (game.cpp) */
      int level;            /* +0xBC [S] level of the warp, -1 the current one */
      int nCharacter;       /* +0xC0 [R] character the task is played with (oidPoCurrent), -1 any */
      LO *ploExit;          /* +0xC4 [S] the exit taken when the task begins */
      SVKEY svkStart;       /* +0xC8 [S] save-variable keys: cStart, */
      SVKEY svkCompletionCount; /* +0xD0 [S] completionCount, */
      SVKEY svkComplete;    /* +0xD8 [S] the complete flag, */
      SVKEY svkState;       /* +0xE0 [S] nState, */
      SVKEY svkInterrupted; /* +0xE8 [S] saved while its job was in progress */
      int nStatePending;    /* +0xF0 [S] the state a running cascade will set, -1 none (CollectTaskCascade) */
      int stateSource;      /* +0xF4 [R] what drives the current state change (game.cpp STATESOURCE; ram_inspector "nMode") */
      int stateRequester;   /* +0xF8 [S] */
      int cVisit;           /* +0xFC [S] topological sort: prerequisites visited (VisitTaskTopo) */
      int depth;            /* +0x100 [S] topological depth */
      int jobDepth;         /* +0x104 [S] number of job ends above it */
      undefined4 unk_0x108[(0x110 - 0x108) / 4];
    virtual void Init() __asm__("InitTask");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetTaskClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadTaskFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostTaskLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateTask");   /* slot 16 */
};
typedef char assert_TASK_size[sizeof(TASK) == 0x110 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_TASK_nState[SLY2_OFFSETOF(TASK, nState) == 0x54 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_completionCount[SLY2_OFFSETOF(TASK, completionCount) == 0x68 ? 1 : -1];
typedef char assert_TASK_pjob[SLY2_OFFSETOF(TASK, pjob) == 0x7C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_dleActive[SLY2_OFFSETOF(TASK, dleActive) == 0x88 ? 1 : -1];   /* InitDl(&SW::dlTaskActive, 0x88) */
typedef char assert_TASK_cptaskPrereq[SLY2_OFFSETOF(TASK, cptaskPrereq) == 0x94 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_aptaskSucc[SLY2_OFFSETOF(TASK, aptaskSucc) == 0xA4 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_cvis[SLY2_OFFSETOF(TASK, cvis) == 0xA8 ? 1 : -1];
typedef char assert_TASK_warpOid[SLY2_OFFSETOF(TASK, warpOid) == 0xB8 ? 1 : -1];
typedef char assert_TASK_nCharacter[SLY2_OFFSETOF(TASK, nCharacter) == 0xC0 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_svkStart[SLY2_OFFSETOF(TASK, svkStart) == 0xC8 ? 1 : -1];
typedef char assert_TASK_nStatePending[SLY2_OFFSETOF(TASK, nStatePending) == 0xF0 ? 1 : -1];
typedef char assert_TASK_stateSource[SLY2_OFFSETOF(TASK, stateSource) == 0xF4 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_cVisit[SLY2_OFFSETOF(TASK, cVisit) == 0xFC ? 1 : -1];
typedef char assert_TASK_jobDepth[SLY2_OFFSETOF(TASK, jobDepth) == 0x104 ? 1 : -1];

#endif /* SLY2_TASK_H */
