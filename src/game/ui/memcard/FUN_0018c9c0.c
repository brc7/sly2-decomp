int func_001ABE78(int a, int b, int c, int d, int *pn);
/** @sly2-annot addr=0x1ABFC0
 * Purpose: wrapper of 0x1ABE78
 * Info: proposed name ReadMcDirWrapper (EXISTING); TU memcard
 * Notes: agent/verifier proposes ReadMcDirWrapper for this existing name (see existing_name_changes.csv) |
 *   arbitration: draft replacement (not re-derived) | RENAME proposed -> ReadMcDirWrapper
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.35] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018c9c0(int a, int b, int c)
{
    int n;
    int r = func_001ABE78(a, b, 1, c, &n);
    if (r) return r;
    return n == 1 ? 0 : 0xE;
}
