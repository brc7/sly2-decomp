struct ALLOCREQ { int cb; int nKind; };
class FRAME {
public:
    int a[4];
    FRAME();
    ~FRAME();
};
struct X { char pad[0x94]; int f94; char pad2[0xFC - 0x98]; int fFC; };
struct G { char pad[0x3AFC]; int c; };
extern G D_003DD490;
extern "C" void *func_001AE378(ALLOCREQ *preq);
extern "C" int CploFindSwObjectsByClass(G *pg, int kind, int n, int m, int c, void **apv);
extern "C" void VisitTaskTopo(X *px, int *pc, void *pv, int a, int b);
extern "C" void SortTasksTopologically(int *pc, void **ppv)
{
    ALLOCREQ req;
    G *pg = &D_003DD490;
    *pc = 0;
    req.cb = pg->c * 4;
    req.nKind = 0x10;
    *ppv = func_001AE378(&req);
    FRAME fr;
    req.cb = pg->c * 4;
    req.nKind = 0x10;
    X **apx = (X **)func_001AE378(&req);
    int c = CploFindSwObjectsByClass(pg, 5, 0x85, 0, pg->c, (void **)apx);
    int i;
    for (i = 0; i < c; i++)
        apx[i]->fFC = 0;
    for (int j = 0; j < c; j++) {
        if (!apx[j]->f94)
            VisitTaskTopo(apx[j], pc, *ppv, 0, 0);
    }
}
