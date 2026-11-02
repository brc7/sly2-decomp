struct ThreadParam {
    int status; void *entry; void *stack; int stackSize; void *gpReg;
    int initPriority; int currentPriority; unsigned attr; unsigned option; int waitType; int waitId; int wakeupCount;
};
extern int D_004425D0[2];
extern char D_004425F0[];
extern char D_002E5270[];
void FrameRenderLoop__FPv(void *);
int func_00234910(void);
void func_002348B0(int tid, int prio);
void *func_002333B4(void *p, int c, int n);
int func_00234820(struct ThreadParam *tp);

/** @sly2-annot addr=0x21F100
 * Purpose: creates the worker thread (stack 0x1000, prio 2, entry 0x160B70) after raising main prio to 5
 * Info: proposed name StartupThread (T1); TU thread
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r8_21BB88+verdicts_A2
 * See: docs/engine/core-library.md#7-key-functions, docs/engine/main-loop-startup.md#open-questions,
 *   docs/engine/overview.md#1-boot
 * @end-sly2-annot */
void func_0021F100(void)
{
    struct ThreadParam tp;
    int *ptid = D_004425D0;
    *ptid = func_00234910();
    func_002348B0(*ptid, 5);
    func_002333B4(&tp, 0, sizeof(tp));
    tp.stackSize = 0x1000;
    tp.stack = D_004425F0;
    tp.initPriority = 2;
    tp.gpReg = D_002E5270;
    tp.entry = FrameRenderLoop__FPv;
    ptid[1] = func_00234820(&tp);
}
