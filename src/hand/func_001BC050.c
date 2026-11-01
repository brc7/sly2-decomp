struct S { char pad[0x1C4]; char dl[0x24]; int f1e8; };
void func_001E5D80(struct S *p);
void func_0011A1F8(void *pdl, int ib);
/** @sly2-annot addr=0x1BC050
 * Purpose: CID 14 vt7: InitShape, owner -1, init DL
 * Info: proposed name InitCid14 (T2); class CID14 slot7; TU tu_1BC050
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/physics-collision.md#6-source-files,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC050(struct S *p)
{
    func_001E5D80(p);
    p->f1e8 = -1;
    func_0011A1F8(p->dl, 0x1D0);
}
