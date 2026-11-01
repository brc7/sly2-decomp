/** @sly2-annot addr=0x207A20
 * Purpose: Bit mask of guard flag n (1<<n, 0 for -1)
 * Info: proposed name GrfFromSgf (T1); TU stepguard
 * Notes: runtime: 0x207A20 returns 1<<n (dsllv) confirmed
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r7b_guard+runtime
 * See: docs/engine/guards-ai.md#2-the-guard-flag-word-0x2c0, docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
unsigned long func_00207A20(int n) { if (n == -1) return 0; return 1L << n; }
