struct E { int a[4]; };
void func_00209590(struct E *pe);
/** @sly2-annot addr=0x208208
 * Purpose: Deep-copy all 26 entries (0x209590)
 * Info: proposed name CloneZapReactions (T2); TU stepguard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00208208(void *p, struct E *pe) { int i; for (i = 25; i >= 0; i--) { func_00209590(pe); pe++; } }
