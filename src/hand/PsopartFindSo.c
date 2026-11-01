struct E { int a[3]; int oid; char pad[0xA0]; };
struct S { char pad[0xE0]; int c; struct E *ae; };
struct E *PsopartFindSo(struct S *p, int oid)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->oid == oid) return pe; }
    return 0;
}
