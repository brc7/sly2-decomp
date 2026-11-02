struct S { char pad[0x1C4]; char dl[0x24]; int f1e8; };
void InitShape(struct S *p);
void InitDl__FP2DLi(void *pdl, int ib);
/** @sly2-annot addr=0x1BC050
 * Purpose: CID 14 vt7: InitShape, owner -1, init DL
 * Info: proposed name InitCid14 (T2); class CID14 slot7; TU tu_1BC050
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/physics-collision.md#6-source-files,
 *   docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC050(struct S *p)
{
    InitShape(p);
    p->f1e8 = -1;
    InitDl__FP2DLi(p->dl, 0x1D0);
}
