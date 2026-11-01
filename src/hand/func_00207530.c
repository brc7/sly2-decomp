struct Q { char pad[0xC]; int k; int n; };
struct S { char pad[0x338]; int n; };
/** @sly2-annot addr=0x207530
 * Purpose: STEPGUARD vt86: true if zap damage >= +0x338 and zap kind not in {0,1,0x80000,0x100000}
 * Info: proposed name FLethalStepguardZap (T2); class STEPGUARD slot86; TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00207530(struct S *p, struct Q *q) { if (-q->n < p->n) return 0; switch (q->k) { case 0: case 1: case 0x80000: case 0x100000: return 0; } return 1; }
