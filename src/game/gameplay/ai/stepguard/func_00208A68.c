struct S { char pad[0x3C0]; unsigned int c; int a[2]; };
/** @sly2-annot addr=0x208A68
 * Purpose: Append cid to +0x3C0 list (max 2)
 * Info: proposed name AddStepguardForceClass (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208A68(struct S *p, int n) { if (p->c < 2) p->a[p->c++] = n; }
