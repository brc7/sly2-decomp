struct S { char pad[0x394]; unsigned int c; int a[3]; };
/** @sly2-annot addr=0x208700
 * Purpose: Append cid to +0x394 list (max 3)
 * Info: proposed name AddStepguardPreferClass (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208700(struct S *p, int n) { unsigned int c = p->c; if (c < 3) { int *pn = &p->a[c]; p->c = c + 1; *pn = n; } }
