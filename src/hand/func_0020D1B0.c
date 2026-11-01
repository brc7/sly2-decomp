struct S { int a; int b; int c; int d; int e; };
/** @sly2-annot addr=0x20D1B0
 * Purpose: po, state -1, enabled
 * Info: proposed name InitHazardWatch (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D1B0(struct S *p, int a) { p->a = a; p->b = -1; p->e = 1; }
