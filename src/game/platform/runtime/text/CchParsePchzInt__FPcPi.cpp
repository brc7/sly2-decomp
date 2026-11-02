static inline int FDigit(unsigned char ch) { return ch >= '0' && ch <= '9'; }
/** @sly2-annot addr=0x21D638
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#6-source-files, docs/engine/gameplay-objects.md#source-files,
 *   docs/reference/functions/core-library.md#text-0x21d638-0x21efd0
 * @end-sly2-annot */
int CchParsePchzInt(char *pchz, int *pn)
{
    unsigned char *pch = (unsigned char *)pchz;
    int sign = (*pch == '-') ? -1 : 1;
    int n = 0;
    int cch = 0;
    if (sign == -1) {
        pch++;
        cch = 1;
    }
    for (; FDigit(*pch); pch++) { n = n * 10 + (*pch - '0'); cch++; }
    *pn = n * sign;
    return cch;
}
