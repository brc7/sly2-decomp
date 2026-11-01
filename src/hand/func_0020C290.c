struct M { int a, b, n8; };
void func_0020C2F8(void *p, int n);
int func_00133590(void *p, int msg, struct M *pm);
/** @sly2-annot addr=0x20C290
 * Purpose: CID 0x91 slot5: ASEG label 0x22 -> launch (state 1)
 * Info: proposed name HandleLeapMessage (T1); class LEAP slot5; TU tu_20B840
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
int func_0020C290(void *p, int msg, struct M *pm) { if (msg == 9 && pm->n8 == 0x22) func_0020C2F8(p, 1); return func_00133590(p, msg, pm); }
