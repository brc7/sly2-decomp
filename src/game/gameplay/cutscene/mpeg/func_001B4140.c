struct P { char pad[0x10C]; int n10C; int pad1; int n114; int a118[26]; int a180[48]; volatile int n240; };
extern int D_002DEDBC;
void func_001B3130(int *pn);
void func_001B3190(int *pn);
int func_0024B590(int *pn);
int func_0024B530(int *pn);
/** @sly2-annot addr=0x1B4140
 * Purpose: audio reset/close, sceMpegReset/Delete
 * Info: proposed name CMpeg_Finish (T1); class CMpeg; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void func_001B4140(struct P *p)
{
    p->n114 = 0;
    p->n10C = 0;
    func_001B3130(p->a180);
    func_001B3190(p->a180);
    func_0024B590(p->a118);
    func_0024B530(p->a118);
    while (p->n240)
        __asm__ __volatile__("");
    D_002DEDBC = 3;
}
