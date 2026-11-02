int McOpen(void *s, void *a, int n, int *out);
int McWrite(void *s, int h, void *b, int c, int *out);
void McClose(void *s, int h);
/** @sly2-annot addr=0x1AC900
 * Purpose: McOpen(create) + McWrite + McClose
 * Info: proposed name McWriteFile (EXISTING); TU memcard
 * Notes: agent/verifier proposes McWriteFile for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McWriteFile (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int FUN_0018d368(void *s, void *a, void *b, int c)
{
    int h = -1;
    int v;
    int r = McOpen(s, a, 0x202, &h);
    if (r)
        return r;
    r = McWrite(s, h, b, c, &v);
    if (!r && v != c)
        r = 7;
    McClose(s, h);
    return r;
}
