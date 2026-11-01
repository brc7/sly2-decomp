struct S { char pad[0x168]; void *p168; int a16c; };
extern char D_002C0CB0[];
void func_001F36A8(struct S *p);
void func_001A64E8(struct S *p, int n, void *q);
void func_001F1448(void *a, struct S *p, int b, int c, int *pn);
/** @sly2-annot addr=0x1C2D20
 * Purpose: CID 99 vt15: PostSoLoad, BindLoOidTable(4,D_002C0CB0), SMA binding
 * Info: proposed name PostCid99Load (T2); class CID99 slot15; TU tu_1C2D20
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/world-sw.md#9-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_1c2d20-0x1c2d20-0x1c3390
 * @end-sly2-annot */
void func_001C2D20(struct S *p)
{
    func_001F36A8(p);
    func_001A64E8(p, 4, D_002C0CB0);
    func_001F1448(p->p168, p, -1, 0, &p->a16c);
}
