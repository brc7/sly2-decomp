struct DL { void *pvFirst; void *pvLast; int ibDle; };
struct S { char pad[0x1D8]; void *powner; };
extern struct DL D_003E1A4C;
void RemoveLoHierarchy(struct S *p);
void func_001BD218(void *p);
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
int FFindDlEntry__FP2DLPv(struct DL *pdl, void *pv);
void func_001BD1E0(void *p);
/** @sly2-annot addr=0x1BC198
 * Purpose: CID 14 vt12: RemoveLoHierarchy, detach from merged owner, unregister obstacle, leave pending list
 * Info: proposed name RemoveCid14Hierarchy (T2); class CID14 slot12; TU tu_1BC050
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC198(struct S *p)
{
    struct DL *pdl;
    RemoveLoHierarchy(p);
    if (p->powner) func_001BD218(p->powner);
    func_001BD1E0(p);
    pdl = &D_003E1A4C;
    if (FFindDlEntry__FP2DLPv(pdl, p)) RemoveDlEntry__FP2DLPv(pdl, p);
}
