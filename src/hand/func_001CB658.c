struct F { int v; char pad[0x4C]; };
struct E { int pad[3]; int iFirst; char p2[0x68 - 0x10]; struct F *af; };
struct A { int id; int pad[8]; int f24; };
struct S { char pad[0x3C]; struct A *pa; };
extern struct E *D_003E1398;
int IsrcFromIdProxy__Fi(int id);
int IcopyFromIdProxy(int id);
int func_001CB658(struct S *p)
{
    struct A *pa = p->pa;
    int i, j;
    struct E *pe;
    struct F *pf;
    if (!pa || !pa->f24) return 0;
    i = IsrcFromIdProxy__Fi(pa->id);
    j = IcopyFromIdProxy(p->pa->id);
    pe = &D_003E1398[i];
    pf = &pe->af[j - pe->iFirst];
    return pf->v;
}
