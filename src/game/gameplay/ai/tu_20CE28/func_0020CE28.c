struct S { int a; int b; int c; int d; };
/** @sly2-annot addr=0x20CE28
 * Purpose: po, state -1, enabled
 * Info: proposed name InitStuckWatch (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud+verdicts_A2
 * See: docs/engine/animation.md#6-source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020CE28(struct S *p, int a) { p->a = a; p->b = -1; p->d = 1; }
