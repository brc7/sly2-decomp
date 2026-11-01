struct ALLOCREQ { int cb; int nKind; };
class FRAME {
public:
    int a[4];
    FRAME();
    ~FRAME();
};
class CBinaryInputStream;
struct S { char pad[0x54]; int c54; void *a58; int c5C; void *a60; char pad2[0x8C - 0x64]; int c8C; void *a90; int c94; void *a98; };
extern "C" void *func_001AE318(ALLOCREQ *preq);
extern "C" void *func_001AE238(ALLOCREQ *preq, void *pv);
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
extern "C" void LoadJobFromBrx(S *p, CBinaryInputStream *pbis)
{
    FRAME fr;
    ALLOCREQ req;
    p->a90 = PvAllocReq(&req, 0x200, 0x10);
    p->a98 = PvAllocReq(&req, 0x300, 0x10);
    p->a58 = PvAllocReq(&req, 0x80, 0x10);
    p->a60 = PvAllocReq(&req, 0x80, 0x10);
    LoadLoFromBrx(p, pbis);
    p->a90 = PvShrinkReq(&req, p->c8C * 16, 0x10, p->a90);
    p->a98 = PvShrinkReq(&req, p->c94 * 0x18, 0x10, p->a98);
    p->a58 = PvShrinkReq(&req, p->c54 * 4, 0x10, p->a58);
    p->a60 = PvShrinkReq(&req, p->c5C * 4, 0x10, p->a60);
    CBinaryInputStream_SkipStringSw(pbis);
}
