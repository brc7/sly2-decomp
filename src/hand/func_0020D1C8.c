struct S { int a; int b; int c; int d; int e; };
void func_0020D3C0(struct S *p, int n);
/** @sly2-annot addr=0x20D1C8
 * Purpose: state 0
 * Info: proposed name ResetHazardWatch (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D1C8(struct S *p) { p->b = -1; p->e = 1; func_0020D3C0(p, 0); }
