struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x150 - 0x10]; int n150; };
extern char D_002BD380[];
void func_001F36A8(void *p);
void func_001A64E8(void *p, int n, void *q);
/** @sly2-annot addr=0x147220
 * Purpose: vt15: PostSoLoad, load class table, state -1, vt9
 * Info: proposed name PostCid104Load (T2); class CID104 slot15; TU tu_1471C0
 * Notes: Cid68->Cid104 (CID 0x68=104, vtable)
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20
 * @end-sly2-annot */
void func_00147220(struct O *p)
{
    struct VTE *e;
    func_001F36A8(p);
    func_001A64E8(p, 2, D_002BD380);
    e = &p->vt[9];
    p->n150 = -1;
    ((void (*)(void *))e->pfn)((char *)p + e->d);
}
