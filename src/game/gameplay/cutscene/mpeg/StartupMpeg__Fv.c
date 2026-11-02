struct ThreadParam { int status; void (*entry)(void *); void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned attr; unsigned option; int waitType; int waitId; int wakeupCount; };
struct G { char pad[0x114]; int n114; char pad1[0x23C - 0x118]; int n23C; };
extern struct G D_002F66B0;
extern char D_002F6900[];
extern char D_002E5270[];
extern int D_004425DC;
int SemaCreate__Fii(int a, int b);
void *func_002333B4(void *pv, int c, int cb);
int func_00234820(struct ThreadParam *ptp);
int func_002355E0(int id, void *arg);
void MpegDisplayThread(void *);
/** @sly2-annot addr=0x1B2940
 * Purpose: Create mpeg semaphore and start the display thread
 * Info: proposed name StartupMpeg (T1); TU mpeg
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/engine/main-loop-startup.md#open-questions,
 *   docs/engine/overview.md#1-boot
 * @end-sly2-annot */
void StartupMpeg__Fv(void)
{
    struct ThreadParam tp;
    D_002F66B0.n23C = SemaCreate__Fii(0, 1);
    D_002F66B0.n114 = 0;
    func_002333B4(&tp, 0, sizeof(tp));
    tp.entry = MpegDisplayThread;
    tp.stack = D_002F6900;
    tp.gpReg = D_002E5270;
    tp.stackSize = 0x1000;
    tp.initPriority = 2;
    D_004425DC = func_00234820(&tp);
    func_002355E0(D_004425DC, 0);
}
