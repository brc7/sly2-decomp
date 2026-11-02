int func_002441C0(int a, int b, int c);
void func_001AB098(void *p, int r, int n, int d);
struct S { char pad[0xFD5C]; char x[4]; };
/** @sly2-annot addr=0x1AC850
 * Purpose: sceMcRead + McSyncResult
 * Info: proposed name McRead (T1); TU memcard
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r4_17F090
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void McRead(struct S *p, int a, int b, int c, int d)
{
    func_001AB098(p->x, func_002441C0(a, b, c), 5, d);
}
