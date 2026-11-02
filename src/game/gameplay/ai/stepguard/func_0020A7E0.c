struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002C3DB4;
struct S { int a; float t; };
/** @sly2-annot addr=0x20A7E0
 * Purpose: time since record stamp > D_002C3DB4
 * Info: proposed name FTargetRecordStale (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020A7E0(struct S *p) { return D_002DDED0.t - p->t > D_002C3DB4; }
