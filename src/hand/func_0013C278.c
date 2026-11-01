struct A { int n; };
struct M { char pad[0x14]; struct A *pa; };
struct S { char pad[0x184]; int st; };
int FIsBasicDerivedFrom__FP5BASIC3CID(int, int);
int func_001F4FC8(struct S *, struct M *);
int func_0013C278(struct S *p, struct M *pm) {
    if (p->st == 5 && FIsBasicDerivedFrom__FP5BASIC3CID(pm->pa->n, 0x34)) return 3;
    return func_001F4FC8(p, pm);
}
