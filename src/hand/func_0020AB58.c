/** @sly2-annot addr=0x20AB58
 * Purpose: clear both attack lists
 * Info: proposed name ClearStepguardAttacks (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AB58(char *p) { *(int *)(p + 0xED8) = 0; *(int *)(p + 0xEFC) = 0; }
