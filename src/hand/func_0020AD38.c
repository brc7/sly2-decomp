extern float D_002A88C8;
struct S { char pad[0x50]; int n50; float f54, f58, f5C, f60, f64, f68, f6C; char pad2[0x7C - 0x70]; int n7C; int n80; int n84; char pad3[0xA8 - 0x88]; int nA8; void *pAC; char eB0[4]; };
void func_001A5270(struct S *p);
void func_002090D0(void *pe, void *p);
/** @sly2-annot addr=0x20AD38
 * Purpose: cid 0x78 vt7: InitLo, range LM [0,300], height LM [-5000,5000], cooldown LM [0.2,0.5], angle
 *   10deg, 3 oids -1, release binding
 * Info: proposed name InitCid120Attack (T2); class CID120 slot7; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, vtable)
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AD38(struct S *p)
{
    func_001A5270(p);
    p->n50 = 0;
    p->f54 = 300.0f;
    p->f58 = -5000.0f;
    p->f5C = 5000.0f;
    p->f60 = 0.2f;
    p->f64 = 0.5f;
    p->f68 = 0.17453295f;
    p->nA8 = 1;
    p->n7C = -1;
    p->n80 = -1;
    p->n84 = -1;
    p->f6C = D_002A88C8;
    func_002090D0(p->eB0, p->pAC);
}
