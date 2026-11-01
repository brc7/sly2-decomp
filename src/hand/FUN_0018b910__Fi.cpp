/** @sly2-annot addr=0x1AAB48
 * Purpose: sceMcSync state -> internal result: 0 -> 0x1D (done), >0 -> 0x1E, <0 -> 0x1F
 * Info: proposed name McResFromSyncState (EXISTING); TU memcard
 * Notes: agent/verifier proposes McResFromSyncState for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> McResFromSyncState
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018b910(int n)
{
    if (n != 0) return n <= 0 ? 0x1F : 0x1E;
    return 0x1D;
}
