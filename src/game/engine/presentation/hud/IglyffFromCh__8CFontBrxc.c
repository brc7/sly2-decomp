struct CFontBrx { char pad[0x20]; int c; unsigned int *ach; };
/** @sly2-annot addr=0x215A50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name IglyffFromCh__8CFontBrxc (EXISTING); TU hud
 * Notes: RENAME proposed -> IglyffFromCh__8CFontBrxc (existing_name_changes.csv)
 * Confidence: name high [EXISTING 0.60]; purpose not reviewed - evidence: arbitration; pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
int IglyffFromCh__8CFontBrxc(struct CFontBrx *p, unsigned char ch)
{
    int lo = 0;
    int hi = p->c - 1;
    while (lo <= hi) {
        int mid = (hi + lo) / 2;
        int d = ch - p->ach[mid];
        if (d == 0)
            return mid + 1;
        if (d > 0)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;
}
