struct O { char pad[0x40]; int f; };
struct S { char pad[0x16C]; struct O *a; struct O *b; struct O *c; };
void PostSoLoad(struct S *p);
static __inline__ void ClearBit(struct O *po)
{
    int f = po->f;
    if ((f ^ 1) & 1) {
        *(int *)f &= ~8;
    } else {
        po->f = f & ~8;
    }
}
void func_00229F88(struct S *p)
{
    PostSoLoad(p);
    if (p->a) ClearBit(p->a);
    if (p->b) ClearBit(p->b);
    if (p->c) ClearBit(p->c);
}
