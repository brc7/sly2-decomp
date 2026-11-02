extern "C" char D_002C40C8[];
extern "C" void *CopyAb__FPvT0Ui(void *dst, const void *src, int cb);
/** @sly2-annot addr=0x21DC70
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int CchAchFromUl(char *ach, int cch, unsigned long ul, unsigned int radix)
{
    char *pchEnd = ach + cch;
    char *pch = pchEnd;
    do {
        *--pch = D_002C40C8[(int)(ul % radix)];
        ul /= radix;
    } while (ul != 0 && pch >= ach);
    int c = pchEnd - pch;
    if (c != 0 && ach < pch)
        CopyAb__FPvT0Ui(ach, pch, c);
    return c;
}
