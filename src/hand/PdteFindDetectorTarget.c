struct E { int id; int n; float t; int pad; };
struct S { char pad[0xFC]; int c; struct E a[1]; };
struct E *PdteFindDetectorTarget(struct S *p, int id)
{
    int i;
    if (!id)
        return 0;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        if (pe->id == id)
            return pe;
    }
    return 0;
}
