struct S { int x0; int x4; int pad[4]; int x18; int x1C; int x20; char pad2[0xFD5C - 0x24]; char xFD5C[1]; };
int sceMcGetInfo(int a, int b, int *c, int *d, int *e);
int McSyncResult(void *p, int a, int b, int c);
/** @sly2-annot addr=0x1AC6A0
 * Purpose: sceMcGetInfo + McSyncResult
 * Info: proposed name McGetInfoSync (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int func_001AC6A0(struct S *s, int *out)
{
    void *p = s->xFD5C;
    int h;
    int r;
    h = sceMcGetInfo(s->x0, s->x4, &s->x18, &s->x1C, &s->x20);
    r = McSyncResult(p, h, 1, 0);
    switch (r) {
    case 9:
        r = 0;
        break;
    case 10:
        r = 12;
        break;
    }
    if (r == 0)
        *out = s->x1C << 10;
    else
        *out = 0;
    return r;
}
