struct S { char pad[0x298]; int a; char pad2[0x90]; int b; };
int func_00209A68(struct S *p, int n);
/** @sly2-annot addr=0x207598
 * Purpose: STEPGUARD vt76: +0x32C or +0x298 set, or flag 0x2A
 * Info: proposed name FInvulnerableStepguard (T1); class STEPGUARD slot76; TU stepguard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00207598(struct S *p) { if (p->b || p->a) return 1; return func_00209A68(p, 0x2A) != 0; }
