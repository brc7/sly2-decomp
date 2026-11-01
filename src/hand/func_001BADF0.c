struct S;
struct E { struct S *x0; int x4; int x8; int xC; char pad[0x30]; union { unsigned long l; struct { int lo; int hi; } w; } x40; };
struct A { int x0; int x4; };
struct S { char pad[0x150]; int n150; struct A *a154; };
int func_001BBE48(struct S *p, struct E *e);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct S *p, int n);
void func_001BAED8(struct S *p, struct S *a, int b);
int func_001BADF0(struct S *p, struct E *e)
{
    int i;
    if (func_001BBE48(p, e)) return 0;
    if (!e->x40.w.hi) return 0;
    if ((e->x40.l & 0xC00000000UL) != 0x800000000UL) return 1;
    if (p->n150 != 0 && e->x0 != p) {
        for (i = 0; i < p->n150; i++)
            if (FIsBasicDerivedFrom__FP5BASIC3CID(e->x0, p->a154[i].x0)) break;
        if (i >= p->n150) return 0;
    }
    func_001BAED8(p, e->x0, e->xC);
    return 1;
}
