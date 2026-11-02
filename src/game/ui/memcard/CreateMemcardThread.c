struct PMF { short delta; short pad; int (*pfn)(void *); };
struct S { int x0; int x4; struct PMF pmf; int x10; };
struct ThreadParam { int attr; void *entry; void *stack; int stackSize; void *gpReg; int initPriority; int currentPriority; unsigned int attr2; int option; int waitType; int waitId; int wakeupCount; };
extern char D_002E5270[], D_002E2080[];
extern int D_004425D8[];
void MemcardThreadMain(struct S *s);
void SetMemcardThreadHighPrio__FP7MEMCARD(struct S *s);
void *func_002333B4(void *p, int c, unsigned int n);
int func_00234820(struct ThreadParam *tp);
void func_002355E0(int id, void *arg);
/** @sly2-annot addr=0x1AAF90
 * Purpose: Wait for previous job, store pointer-to-member job {0xFFFF0000,pfn}, _iDisableIntc+_iEnableDmac
 * Info: proposed name CreateMemcardThread (T1); TU memcard
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6+verdicts_A2
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void CreateMemcardThread(struct S *s, struct PMF pmf)
{
    struct ThreadParam tp;
    SetMemcardThreadHighPrio__FP7MEMCARD(s);
    s->pmf = pmf;
    s->x10 = 0x21;
    func_002333B4(&tp, 0, sizeof(tp));
    tp.entry = MemcardThreadMain;
    tp.stack = D_002E2080;
    tp.gpReg = D_002E5270;
    tp.stackSize = 0x4000;
    tp.initPriority = 3;
    s->x4 = func_00234820(&tp);
    D_004425D8[0] = s->x4;
    func_002355E0(s->x4, s);
}
