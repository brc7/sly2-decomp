int func_001AC7A0(void *s, void *a, int n, int *out);
int func_001AC850(void *s, int h, void *b, int c, int *out);
void func_001AC808(void *s, int h);
/** @sly2-annot addr=0x1AC9A0
 * Purpose: McOpen + McRead + McClose
 * Info: proposed name McReadFile (T1); TU memcard
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001AC9A0(void *s, void *a, void *b, int c)
{
    int h = -1;
    int v;
    int r = func_001AC7A0(s, a, 1, &h);
    if (r)
        return r;
    r = func_001AC850(s, h, b, c, &v);
    if (!r && v != c)
        r = 8;
    func_001AC808(s, h);
    return r;
}
