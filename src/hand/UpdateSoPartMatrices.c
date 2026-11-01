typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M m; char pad2[0xA4 - 0x80]; int nA4; };
struct E { struct O *po; int pad[3]; ulong fl; int pad2[2]; struct M m; char pad3[0xAC - 0x60]; int nAC; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void UpdateSoPartMatrices(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) {
            struct O *po = pe->po;
            pe->m = po->m;
            pe->nAC = po->nA4;
        }
    }
}
