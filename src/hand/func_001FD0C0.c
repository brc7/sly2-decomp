extern int D_002C3C40[];
/** @sly2-annot addr=0x1FD0C0
 * Purpose: Table D_002C3C40[n] for n<6 (else -1): SM oid for dialog speaker role n
 * Info: proposed name OidDialogSmRole (T2); TU sound
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int func_001FD0C0(int a, unsigned int n) { if (n >= 6) return -1; return D_002C3C40[n]; }
