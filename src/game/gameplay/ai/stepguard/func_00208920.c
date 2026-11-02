struct S { char pad[0x3A8]; unsigned int c; int a[5]; };
/** @sly2-annot addr=0x208920
 * Purpose: Append cid to +0x3A8 list (max 5)
 * Info: proposed name AddStepguardIgnoreClass (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208920(struct S *p, int n) { if (p->c < 5) p->a[p->c++] = n; }
