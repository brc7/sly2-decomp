typedef struct CHK { int a[0x4C / 4]; } CHK;
typedef struct CHKMGR { char pad[0x3BF0]; int fRestored; CHK chk; } CHKMGR;
void RestoreChkmgrFromCheckpoint(CHKMGR *pchkmgr, CHK *pchk) __asm__("RestoreChkmgrFromCheckpoint__FP6CHKMGR");
/** @sly2-annot addr=0x20DF80
 * Purpose: fRestored=1; copy CHK record
 * Info: proposed name SetSwTransition (EXISTING); TU sw
 * Notes: RENAME proposed -> SetSwTransition (existing_name_changes.csv) | runtime5b: CONFIRMED (code +
 *   runtime S56): posts the transition request SW+0x3BF0/+0x3BF4 consumed by ExecuteSwTransition; no
 *   checkpoint record
 * Confidence: high [EXISTING 0.95] - evidence: runtime (save-state RAM); arbitration; pre-existing symbol in
 *   symbol_addrs.txt; sources: r7_1F8DD8+arbitration+runtime
 * See: docs/engine/world-sw.md#world-sw-level-loading-transitions-hands-off-callbacks,
 *   docs/engine/world-sw.md#2-loading-a-level, docs/engine/world-sw.md#7-checkpoints-refuted
 * @end-sly2-annot */
void RestoreChkmgrFromCheckpoint(CHKMGR *pchkmgr, CHK *pchk)
{
    pchkmgr->fRestored = 1;
    pchkmgr->chk = *pchk;
}
