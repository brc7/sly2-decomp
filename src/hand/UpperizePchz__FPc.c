extern unsigned char D_002AA6A1[];
/** @sly2-annot addr=0x21EE30
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
void UpperizePchz__FPc(char *pchz)
{
    unsigned char *pch;
    for (pch = (unsigned char *)pchz; *pch; pch++) {
        int c = *pch;
        int u = c - 0x20;
        if (!(D_002AA6A1[c] & 2))
            u = c;
        *pch = u;
    }
}
