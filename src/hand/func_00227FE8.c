struct O { char pad[0x18]; int id; char pad1[0x16C - 0x1C]; int c; int aid[1]; };
struct O *PloFindSwObject__FP2SWi3OIDP2LO(void *pg, int k, int id, struct O *p);
int CploFindSwObjectsByClass(void *pg, int a, int b, struct O *po, int n, struct O **apo);
struct O *func_00227FE8(void *pg, int id, int k)
{
    struct O *apo[32];
    struct O *poRet;
    struct O *po = 0;
    int n, i;
    int a = 5;
    if (k != -1) {
        po = PloFindSwObject__FP2SWi3OIDP2LO(pg, 5, k, 0);
        if (po) a = 1;
    }
    poRet = 0;
    n = CploFindSwObjectsByClass(pg, a, 0x2A, po, 0x20, apo);
    for (i = 0; i < n; i++) {
        struct O *pe = apo[i];
        int j;
        if (pe->id == id) goto found;
        for (j = 0; j < pe->c; j++) {
            if (pe->aid[j] == id) goto found;
        }
        continue;
found:
        poRet = pe;
    }
    return poRet;
}
