struct E { char pad[0x94]; int b; int pad2; int a; char pad4[0x60]; };
struct S { char pad[0x50]; int c; struct E *ae; };
struct E *PxfFindSo(struct S *p, int a, int b)
{
    int i;
    for (i = 0; i < p->c; i++) { struct E *pe = &p->ae[i]; if (pe->a == a && pe->b == b) return pe; }
    return 0;
}
