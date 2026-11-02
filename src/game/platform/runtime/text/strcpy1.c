unsigned int func_0021ED28(char *s);
void func_001AE5A0(void *dst, void *src, int cb);
/** @sly2-annot addr=0x21EDA0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
char *strcpy1(char *pchzDst, char *pchzSrc)
{
    unsigned int dstLength = func_0021ED28(pchzDst);
    unsigned int srcLength = func_0021ED28(pchzSrc);
    func_001AE5A0(pchzDst + dstLength, pchzSrc, srcLength + 1);
    return pchzDst;
}
