struct S { char pad[0xEB8]; int a[8]; unsigned int c; };
/** @sly2-annot addr=0x20AA40
 * Purpose: Append attack oid (max 8) to +0xEB8
 * Info: proposed name AddStepguardAttackOid (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AA40(struct S *p, int n) { if (n != -1) { if (p->c < 8) p->a[p->c++] = n; } }
