struct S { char pad[0xE94]; unsigned int c; int a[8]; };
/** @sly2-annot addr=0x20A518
 * Purpose: Append cid to +0xE94 list (max 8)
 * Info: proposed name AddStepguardClassE94 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020A518(struct S *p, int n) { if (p->c < 8) p->a[p->c++] = n; }
