/* include/sly2/task.h -- mission graph objects: JOB (CID 0x84, cb 0x100) and TASK (CID 0x85, cb 0x110), both direct LO
 * subclasses with no new virtuals (vtable 48 = LO). Names are the devtools' (ram_inspector / slygame), not Sucker Punch's.
 * Offsets from runtime6 FINDINGS and devtools/pine_toolkit/slygame.py tasks(). Tags: [R] runtime, [S] static, [H] hypothesis.
 * The current JOB/TASK/pending TASK are SW::currentJob/ploTaskCur/ploTaskPending (SW+0x3B0C..). */
#ifndef SLY2_TASK_H
#define SLY2_TASK_H
#include "sly2/basic.h"
#include "sly2/lo.h"

struct TASK;

/* JOB: a job (mission) of the episode graph: its first and final task, timer, counter and save-variable name. */
struct JOB : public LO
{
      int isDisabled;       /* +0x50 [R] */
      undefined4 unk_0x54[(0x64 - 0x54) / 4];
      TASK *firstTask;      /* +0x64 [R] first task of the job */
      TASK *finalTask;      /* +0x68 [R] final task (game.cpp IsJobComplete tests it) */
      int labelNameId;      /* +0x6C [S] name-plate string id, -1 none (waypoint.cpp) */
      int unknown70;        /* +0x70 [S] (game.cpp) */
      int survivesSafehouse; /* +0x74 [S] non-zero: entering the safehouse does not fail the job (tu_1D7238 SetCharSelectState; inferred) */
      undefined4 unk_0x78;
      int counter;           /* +0x7C [R] counter shown by the job (ram_inspector "count") */
      DL tasks;             /* +0x80 [S] the job's tasks (game.cpp) */
      undefined4 unk_0x8c[(0x9c - 0x8c) / 4];
      float easeStartTime;  /* +0x9C [S] difficulty-ease thresholds (game.cpp) */
      float easeEndTime;    /* +0xA0 [S] */
      float elapsedTime;    /* +0xA4 [R] elapsed time in the job */
      float *difficulty;    /* +0xA8 [S] per-job difficulty override (HYPOTHESIS, difficulty.cpp) */
      int completionWidgetOid; /* +0xAC [S] (SetJobCompletionWidget, game.cpp) */
      int introLabel;       /* +0xB0 [S] */
      int outroLabel;       /* +0xB4 [S] */
      void *completionWidget; /* +0xB8 [S] the job has a completion sequence when set */
      undefined4 unk_0xbc[(0xc4 - 0xbc) / 4];
      int phase;            /* +0xC4 [R] completion phase */
      undefined4 unknownC8[2];
      int ksvName;          /* +0xD0 [R] save-name reference {kind, prec} (job save-variable name) */
      void *psvName;        /* +0xD4 [R] */
      undefined4 unk_0xd8[(0x100 - 0xd8) / 4];
    virtual void Init() __asm__("InitJob");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetJobClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadJobFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostJobLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateJob");   /* slot 16 */
};
typedef char assert_JOB_size[sizeof(JOB) == 0x100 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_JOB_firstTask[SLY2_OFFSETOF(JOB, firstTask) == 0x64 ? 1 : -1];     /* CONFIRMED runtime */
typedef char assert_JOB_tasks[SLY2_OFFSETOF(JOB, tasks) == 0x80 ? 1 : -1];
typedef char assert_JOB_completionWidget[SLY2_OFFSETOF(JOB, completionWidget) == 0xB8 ? 1 : -1];
typedef char assert_JOB_phase[SLY2_OFFSETOF(JOB, phase) == 0xC4 ? 1 : -1];     /* CONFIRMED runtime */

/* TASK: a task (step of a job): its state, owning job, prerequisite and successor tasks and the character it is
   played with. */
struct TASK : public LO
{
      undefined4 unknown50;
      int state;            /* +0x54 [R] -1 unset, 0 locked, 1 available, 2 done (in job), 3 done */
      float stateStartTime; /* +0x58 [R] clock.time when state was entered */
      undefined4 unk_0x5c[(0x68 - 0x5c) / 4];
      int completionCount;  /* +0x68 [S] (difficulty.cpp) */
      undefined4 unk_0x6c[(0x7c - 0x6c) / 4];
      JOB *pjob;            /* +0x7C [R] owning JOB */
      undefined4 unk_0x80[(0x94 - 0x80) / 4];
      int numPrerequisites; /* +0x94 [R] prerequisite tasks */
      TASK **prerequisites; /* +0x98 [R] */
      undefined4 unknown9C;
      int numSuccessors;    /* +0xA0 [R] successor tasks */
      TASK **successors;    /* +0xA4 [R] */
      float *rating;        /* +0xA8 [S] (difficulty.cpp) */
      undefined4 unk_0xac[(0xb8 - 0xac) / 4];
      int warpOid;          /* +0xB8 [S] a task with a warp becomes the pending task (game.cpp) */
      undefined4 unk_0xbc;
      int character;        /* +0xC0 [R] character the task is played with */
      undefined4 unk_0xc4[(0xf4 - 0xc4) / 4];   /* +0xC8..+0xE8: {kind, prec} save-name references (one of them names the task) */
      int nMode;            /* +0xF4 [R] */
      undefined4 unk_0xf8[(0x110 - 0xf8) / 4];
    virtual void Init() __asm__("InitTask");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetTaskClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadTaskFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostTaskLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateTask");   /* slot 16 */
};
typedef char assert_TASK_size[sizeof(TASK) == 0x110 ? 1 : -1];   /* class descriptor cb (CONFIRMED runtime) */
typedef char assert_TASK_state[SLY2_OFFSETOF(TASK, state) == 0x54 ? 1 : -1];     /* CONFIRMED runtime */
typedef char assert_TASK_pjob[SLY2_OFFSETOF(TASK, pjob) == 0x7C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_completionCount[SLY2_OFFSETOF(TASK, completionCount) == 0x68 ? 1 : -1];
typedef char assert_TASK_warpOid[SLY2_OFFSETOF(TASK, warpOid) == 0xB8 ? 1 : -1];
typedef char assert_TASK_character[SLY2_OFFSETOF(TASK, character) == 0xC0 ? 1 : -1];
typedef char assert_TASK_successors[SLY2_OFFSETOF(TASK, successors) == 0xA4 ? 1 : -1];   /* CONFIRMED runtime */

#endif /* SLY2_TASK_H */
