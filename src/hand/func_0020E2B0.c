struct O { char pad[0xB8]; int nB8; char p2[0xC]; int nC8; };
struct S { char pad[0x3E5C]; int n; };
int CploFindSwObjectsByClass(struct S *p, int a, int b, int c, int cMax, struct O **apo);
void func_001A6D28(struct O *o, int n);
void func_0020E2B0(struct S *p)
{
    struct O *apo[16];
    int c = CploFindSwObjectsByClass(p, 5, 0x31, 0, 16, apo);
    int nMax = 0;
    struct O *poBest = 0;
    int i;
    for (i = 0; i < c; i++) {
        if (apo[i]->nB8 > nMax) {
            nMax = apo[i]->nB8;
            poBest = apo[i];
        }
    }
    if (poBest) {
        p->n = poBest->nC8;
        func_001A6D28(poBest, 0xFF);
    }
}
