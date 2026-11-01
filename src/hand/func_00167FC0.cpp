#include "vifs.h"
struct X { char pad[0x120]; QW amat[3]; int f150[4]; int f160; };
struct G;
extern G D_003DD490;
extern char D_0025E090[];
extern char D_0025E668[];
extern "C" void func_00160918(VIFS *pvifs, void *pvStart, void *pvEnd);
extern "C" int CploFindSwObjectsByClass(G *pg, int kind, int n, int m, int c, void **apv);
extern "C" void func_001AE5A0(void *pvDst, void *pvSrc, unsigned int cb);
static inline int Min(int a, int b) { return a < b ? a : b; }
extern "C" void func_00167FC0(VIFS *pvifs)
{
    X *apx[0x180];
    int i;
    X *px;
    int cActive = 0;
    func_00160918(pvifs, D_0025E090, D_0025E668);
    pvifs->AddDmaCnt();
    pvifs->AddVifFlusha();
    pvifs->AddVifBaseOffset(0x32, 0);
    int c = CploFindSwObjectsByClass(&D_003DD490, 0x505, 4, 0, 0x180, (void **)apx);
    for (i = 0; i < c; i++) {
        px = apx[i];
        if (!px->f160)
            cActive++;
    }
    if (cActive > 0x100)
        cActive = 0x100;
    uint *pun;
    pvifs->UnpackHelper(UPK_V4_32, 1, 0, 0, &pun);
    pun[0] = 1;
    pun[1] = cActive * 3 + 1;
    uint *pb = 0;
    int n = 0;
    int iEnd = 0;
    for (i = 0; i < c; i++) {
        px = apx[i];
        if (px->f160)
            continue;
        if (n >= iEnd) {
            iEnd = n + Min(cActive - n, 32);
            pvifs->UnpackHelper(UPK_V4_32, (iEnd - n) * 3, n * 3 + 1, 0, &pb);
        }
        func_001AE5A0(pb, px->amat, 0x30);
        n++;
        pb = (uint *)((char *)pb + 0x30);
        if (n >= 0x100)
            break;
    }
    pvifs->EndDmaCnt();
}
