struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct S { int a; float t; int n; };
/** @sly2-annot addr=0x20A9A8
 * Purpose: record state + time
 * Info: proposed name SetTargetRecordState (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020A9A8(struct S *p, int n) { p->t = D_002DDED0.t; p->n = n; }
