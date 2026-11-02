extern unsigned char D_002AA6A1[];
static inline int ToUpper(int c) { return (D_002AA6A1[c] & 2) ? c - 0x20 : c; }
/** @sly2-annot addr=0x1D8640
 * Purpose: Case-insensitive string hash h=h*33+toupper(c)
 * Info: proposed name UHashPchzCi (T1); TU save
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
int func_001D8640(unsigned char *pch) {
    int h = 0;
    while (*pch) {
        h = h * 33 + ToUpper(*pch);
        pch++;
    }
    return h;
}
