struct G;
extern struct G D_003DD490;
typedef struct ALLOCREQ { int cb; int nKind; } ALLOCREQ;
struct S { char pad[0x50]; int c; void **apv; };
void PostLoLoad(struct S *p);
int CploFindSwObjectsByClass(struct G *pg, int a, int b, int c, int cMax, void **apv);
void *func_001AE1D0(ALLOCREQ *preq);
void func_0020B500(struct S *p)
{
    ALLOCREQ req;
    PostLoLoad(p);
    p->c = CploFindSwObjectsByClass(&D_003DD490, 0x605, 0x38, 0, 0, 0);
    req.cb = p->c * 4;
    req.nKind = 0x10;
    p->apv = func_001AE1D0(&req);
    CploFindSwObjectsByClass(&D_003DD490, 0x405, 0x38, 0, p->c, p->apv);
}
