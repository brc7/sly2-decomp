struct M { int a, b; int n8; unsigned int nC; };
struct S { char pad[0x160]; char x160[0x10]; unsigned int c170; };
void func_0013D7D0(void *p, unsigned int a, int b);
/** @sly2-annot addr=0x1B3388
 * Purpose: sceMpeg video stream callback: CbFill video queue
 * Info: proposed name FMpegAcceptVideo (T1); TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130+verdicts_A2
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
int func_001B3388(void *a, struct M *pm, struct S *p)
{
    if (p->c170 < pm->nC) return 0;
    func_0013D7D0(p->x160, pm->nC, pm->n8);
    return 1;
}
