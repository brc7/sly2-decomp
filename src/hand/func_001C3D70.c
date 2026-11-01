struct S { char pad[0x40]; int x40; char pad2[0x118]; int n15c; int a[2]; void *p168; };
void func_001F2108(struct S *p);
/** @sly2-annot addr=0x1C3D70
 * Purpose: PO::Init (vt slot 7): InitSo; pzi.fCollectEnabled(+0x15C)=1; pzi.paloCollect(+0x168)=this; set
 *   flag 2 in grfUnk40
 * Info: proposed name InitPo (T1); class PO slot7; TU po
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/player.md#10-source-files, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void func_001C3D70(struct S *p)
{
    int v;
    func_001F2108(p);
    p->n15c = 1;
    p->p168 = p;
    v = p->x40;
    if ((v ^ 1) & 1) *(int *)v |= 2; else p->x40 = v | 2;
}
