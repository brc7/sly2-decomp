int McOpen(void *s, void *a, int n, int *out);
int McRead(void *s, int h, void *b, int c, int *out);
void McClose(void *s, int h);
/** @sly2-annot addr=0x1AC9A0
 * Purpose: McOpen + McRead + McClose
 * Info: proposed name McReadFile (T1); TU memcard
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McReadFile(void *s, void *a, void *b, int c)
{
    int h = -1;
    int v;
    int r = McOpen(s, a, 1, &h);
    if (r)
        return r;
    r = McRead(s, h, b, c, &v);
    if (!r && v != c)
        r = 8;
    McClose(s, h);
    return r;
}
