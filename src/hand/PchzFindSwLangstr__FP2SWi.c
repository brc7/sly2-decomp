struct E { int id; void *pv; };
struct S { char pad[0x4640]; int c; struct E *a; };
extern char D_002A4BE0[];
void *PchzFindSwLangstr__FP2SWi(struct S *p, int id) {
    int i;
    if (id == -1) return D_002A4BE0;
    for (i = 0; i < p->c; i++) {
        if (p->a[i].id == id) return p->a[i].pv;
    }
    return D_002A4BE0;
}
