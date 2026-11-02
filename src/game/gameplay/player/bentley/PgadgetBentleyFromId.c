/* Sly 2 version takes a base pointer in $a0 and saak in $a1 and returns a pointer into it. */
/** @sly2-annot addr=0x136A28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name PgadgetBentleyFromId (EXISTING); TU bentley
 * Notes: RENAME proposed -> PgadgetBentleyFromId (existing_name_changes.csv) | runtime5a #5/#6: x16B4 =
 *   Bentley+0x1830 = record for gadget 7 (S50); CbFromSaak name refuted
 * Confidence: name high [EXISTING 0.90]; purpose not reviewed - evidence: runtime (save-state RAM);
 *   arbitration; pre-existing symbol in symbol_addrs.txt
 * See: docs/engine/player.md#7-bentley-bentleycpp-0x134648, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
int PgadgetBentleyFromId(char *p, int saak)
{
    switch (saak) {
    case 5: return (int)(p + 0x1824);
    case 7: return (int)(p + 0x1830);
    case 9: return (int)(p + 0x183c);
    case 11: return (int)(p + 0x1848);
    case 13: return (int)(p + 0x1854);
    case 4: return (int)(p + 0x1860);
    }
    return 0;
}
