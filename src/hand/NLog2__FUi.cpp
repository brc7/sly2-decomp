typedef unsigned int uint;
/** @sly2-annot addr=0x1703C0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
uint NLog2(uint value)
{
    uint log2 = 0;
    uint bit = 1;
    while (bit < value && log2 < 31)
    {
        bit <<= 1;
        log2++;
    }
    return log2;
}
