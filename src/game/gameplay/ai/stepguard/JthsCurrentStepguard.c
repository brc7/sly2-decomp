unsigned long GrfFromSgf(int n);
struct S { char pad[0x2C0]; unsigned long m; };
/** @sly2-annot addr=0x20B3D0
 * Purpose: STEPGUARD vt77: 2 (dead) if flag 7, 1 (damaged) if flags 5/6, else 0
 * Info: proposed name JthsCurrentStepguard (T1); class STEPGUARD slot77; TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int JthsCurrentStepguard(struct S *p) { if (p->m & GrfFromSgf(7)) return 2; if (p->m & (GrfFromSgf(5) | GrfFromSgf(6))) return 1; return 0; }
