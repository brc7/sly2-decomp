extern int D_002C3D10[];
int func_00132B10(void *p, int n);
/** @sly2-annot addr=0x206D48
 * Purpose: True if the guard derives from any of cids D_002C3D10 = {0x3B,0x40,0x3D,0x3E,0x41}
 * Info: proposed name FIsStepguardRageImmune (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_00206D48(void *p) { unsigned int i; for (i = 0; i < 5; i++) if (func_00132B10(p, D_002C3D10[i])) return 1; return 0; }
