struct ALLOCREQ { int cb; int nKind; };
class FRAME {
public:
    int a[4];
    FRAME();
    ~FRAME();
};
class CBinaryInputStream {
public:
    int U8Read();
    int S32Read();
};
struct S { char pad[0x90]; int *a90; int c94; void *a98; int *a9C; int cA0; void *aA4; int cA8; void *aAC; int cB0; void *aB4; };
extern "C" void *func_001AE318(ALLOCREQ *preq);
extern "C" void *func_001AE238(ALLOCREQ *preq, void *pv);
extern "C" void *func_001AE2A0(ALLOCREQ *preq);
extern "C" void *func_001AE1D0(ALLOCREQ *preq);
extern "C" void LoadLoFromBrx(S *p, CBinaryInputStream *pbis);
extern "C" void CBinaryInputStream_SkipStringSw(CBinaryInputStream *pbis);
static inline void *PvAllocReq(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE318(preq);
}
static inline void *PvShrinkReq(ALLOCREQ *preq, int cb, int nKind, void *pv)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE238(preq, pv);
}
static inline void *PvAllocA(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE2A0(preq);
}
static inline void *PvAllocB(ALLOCREQ *preq, int cb, int nKind)
{
    preq->cb = cb;
    preq->nKind = nKind;
    return func_001AE1D0(preq);
}
extern "C" void LoadTaskFromBrx(S *p, CBinaryInputStream *pbis)
{
    int i = 0;
    FRAME fr;
    ALLOCREQ req;
    p->aAC = PvAllocReq(&req, 0x200, 0x10);
    p->aB4 = PvAllocReq(&req, 0x300, 0x10);
    LoadLoFromBrx(p, pbis);
    p->aAC = PvShrinkReq(&req, p->cA8 * 16, 0x10, p->aAC);
    p->aB4 = PvShrinkReq(&req, p->cB0 * 0x18, 0x10, p->aB4);
    p->c94 = pbis->U8Read();
    p->a98 = PvAllocA(&req, p->c94 * 4, 0x10);
    p->a90 = (int *)PvAllocB(&req, p->c94 * 4, 0x10);
    for (; i < p->c94; i++)
        p->a90[i] = pbis->S32Read();
    p->cA0 = pbis->U8Read();
    p->aA4 = PvAllocA(&req, p->cA0 * 4, 0x10);
    p->a9C = (int *)PvAllocB(&req, p->cA0 * 4, 0x10);
    for (int j = 0; j < p->cA0; j++)
        p->a9C[j] = pbis->S32Read();
    CBinaryInputStream_SkipStringSw(pbis);
}
