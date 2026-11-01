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
