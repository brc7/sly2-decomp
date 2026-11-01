/** @sly2-annot addr=0x1AAB68
 * Purpose: sceMcSync cmd result -> 0x21 (0), 0x22 (1), 0x20 (-1), else 0x1F
 * Info: proposed name McResFromSyncCmd (EXISTING); TU memcard
 * Notes: agent/verifier proposes McResFromSyncCmd for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> McResFromSyncCmd
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.50] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018b930(int n)
{
    switch (n) {
    case 0: return 0x21;
    case 1: return 0x22;
    case -1: return 0x20;
    }
    return 0x1F;
}
