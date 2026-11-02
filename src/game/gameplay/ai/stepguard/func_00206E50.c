extern int D_002C3D10[];
void func_00208948(void *p, int n);
/** @sly2-annot addr=0x206E50
 * Purpose: Remove the 5 guard cids from +0x3A8
 * Info: proposed name RemoveStepguardRageIgnoreClasses (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00206E50(void *p) { unsigned int i; for (i = 0; i < 5; i++) func_00208948(p, D_002C3D10[i]); }
