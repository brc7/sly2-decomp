/* include/sly2/task.h -- mission graph objects: JOB (CID 0x84, cb 0x100) and TASK (CID 0x85, cb 0x110), both direct LO
 * subclasses with no new virtuals (vtable 48 = LO). Names are the devtools' (ram_inspector / slygame), not Sucker Punch's.
 * Offsets from runtime6 FINDINGS and devtools/pine_toolkit/slygame.py tasks(). Tags: [R] runtime, [S] static, [H] hypothesis.
 * The current JOB/TASK/pending TASK are SW::ploJobCur/ploTaskCur/ploTaskPending (SW+0x3B0C..). */
#ifndef SLY2_TASK_H
#define SLY2_TASK_H
#include "sly2/basic.h"
#include "sly2/lo.h"

struct TASK;

/* JOB: a job (mission) of the episode graph: its first and final task, timer, counter and save-variable name. */
struct JOB : public LO
{
      int fDisabled;        /* +0x50 [R] */
      undefined4 unk_0x54[(0x64 - 0x54) / 4];
      TASK *ptaskStart;     /* +0x64 [R] first task of the job */
      TASK *ptaskEnd;       /* +0x68 [R] final task */
      undefined4 unk_0x6c[(0x7c - 0x6c) / 4];
      int cCount;           /* +0x7C [R] counter shown by the job (ram_inspector "count") */
      undefined4 unk_0x80[(0xa4 - 0x80) / 4];
      float dtElapsed;      /* +0xA4 [R] elapsed time in the job */
      undefined4 unk_0xa8[(0xc4 - 0xa8) / 4];
      int nPhase;           /* +0xC4 [R] completion phase */
      undefined4 unk_0xc8[2];
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
typedef char assert_JOB_ptaskStart[SLY2_OFFSETOF(JOB, ptaskStart) == 0x64 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_JOB_nPhase[SLY2_OFFSETOF(JOB, nPhase) == 0xC4 ? 1 : -1];   /* CONFIRMED runtime */

/* TASK: a task (step of a job): its state, owning job, prerequisite and successor tasks and the character it is
   played with. */
struct TASK : public LO
{
      undefined4 unk_0x50;
      int nState;           /* +0x54 [R] -1 unset, 0 locked, 1 available, 2 done (in job), 3 done */
      float tState;         /* +0x58 [R] clock.t when nState was entered */
      undefined4 unk_0x5c[(0x7c - 0x5c) / 4];
      JOB *pjob;            /* +0x7C [R] owning JOB */
      undefined4 unk_0x80[(0x94 - 0x80) / 4];
      int cptaskPrereq;     /* +0x94 [R] prerequisite tasks */
      TASK **aptaskPrereq;  /* +0x98 [R] */
      undefined4 unk_0x9c;
      int cptaskSucc;       /* +0xA0 [R] successor tasks */
      TASK **aptaskSucc;    /* +0xA4 [R] */
      undefined4 unk_0xa8[(0xc0 - 0xa8) / 4];
      int nCharacter;       /* +0xC0 [R] character the task is played with */
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
typedef char assert_TASK_nState[SLY2_OFFSETOF(TASK, nState) == 0x54 ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_pjob[SLY2_OFFSETOF(TASK, pjob) == 0x7C ? 1 : -1];   /* CONFIRMED runtime */
typedef char assert_TASK_aptaskSucc[SLY2_OFFSETOF(TASK, aptaskSucc) == 0xA4 ? 1 : -1];   /* CONFIRMED runtime */

#endif /* SLY2_TASK_H */
