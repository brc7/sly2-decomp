struct E { char pad[0x10]; long id; char pad2[8]; };
struct S { char pad[0x4C0]; int c; char pad2[0xC]; struct E a[1]; };
struct E *func_00209BE0(struct S *p, long id);
void ReleaseAnimBinding(struct E *pe, struct S *p);
struct E *func_00209C18(struct S *p, long id)
{
    struct E *pe = func_00209BE0(p, id);
    if (!pe) {
        struct E *pn = &p->a[p->c++];
        pn->id = id;
        pe = pn;
    }
    ReleaseAnimBinding(pe, p);
    return pe;
}
