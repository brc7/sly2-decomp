struct B { char data[0x5464]; };
struct S { int x0; int x4; int x8; int xC; int pad[5]; struct B a[3]; char pad2[0xFD5C - 0x24 - 3 * 0x5464]; char xFD5C[1]; };
extern char D_002A6BB0[];
int func_002446F0(int a, int b, char *c, int d, int e, int f);
int McSyncResult(void *p, int a, int b, int *c);
int func_001AC6A0(struct S *s, int *out);
int func_001AC4C8(struct S *s, struct B *b);
/** @sly2-annot addr=0x1ABA10
 * Purpose: Thread job: sceMcGetInfo + sceMcGetDir of save dir, read slot headers (0x1AC4C8)
 * Info: proposed name McJobGetDir (EXISTING); TU memcard
 * Notes: agent/verifier proposes McJobGetDir for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> McJobGetDir (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.60] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+arbitration
 * See: docs/engine/save-memcard.md#2-memory-card-memcardcpp-0x1aa978-0x1ae130,
 *   docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
int McJobGetDir(struct S *s)
{
    void *p = s->xFD5C;
    int n[2];
    int r;
    int i;
    r = McSyncResult(p, func_002446F0(s->x0, s->x4, D_002A6BB0, 0, -1, 0), 0xD, &n[0]);
    if (n[0] <= 0) {
        func_001AC6A0(s, &n[1]);
        if (n[1] <= 0x18FFF)
            s->xC = 4;
        return 0xE;
    }
    for (i = 0; r == 0 && i < 3; i++)
        func_001AC4C8(s, &s->a[i]);
    if (r == 0 && s->xC < 5)
        s->xC = 5;
    return r;
}
