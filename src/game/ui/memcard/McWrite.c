int sceMcWrite(int a, int b, int c);
void McSyncResult(void *p, int r, int n, int d);
struct S { char pad[0xFD5C]; char x[4]; };
/** @sly2-annot addr=0x1AC8A8
 * Purpose: sceMcWrite + McSyncResult
 * Info: proposed name McWrite (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void McWrite(struct S *p, int a, int b, int c, int d)
{
    McSyncResult(p->x, sceMcWrite(a, b, c), 6, d);
}
