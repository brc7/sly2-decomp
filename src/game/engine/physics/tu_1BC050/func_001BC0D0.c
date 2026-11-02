struct ALLOCREQ { int cb; int nKind; };
struct T8 { int a, b; };
struct P { char pad[0xE8]; int cE8; char pad1[0x1D0 - 0xEC]; struct T8 t1D0; int pad2[2]; struct T8 t1E0; int pad3; void *pv1EC; };
void CloneShape(struct P *p, int a);
void *func_001AE2A0(struct ALLOCREQ *preq);
static __inline__ void *PvAllocA(struct ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE2A0(preq);
}
/** @sly2-annot addr=0x1BC0D0
 * Purpose: CID 14 vt41: CloneShape, copy plane array
 * Info: proposed name CloneCid14 (T2); class CID14 slot41; TU tu_1BC050
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
void func_001BC0D0(struct P *p, int x)
{
    struct T8 a = p->t1D0;
    struct T8 b = p->t1E0;
    struct ALLOCREQ req;
    CloneShape(p, x);
    p->t1D0 = a;
    p->t1E0 = b;
    p->pv1EC = PvAllocA(&req, p->cE8 * 16 - 16, 0x10);
}
