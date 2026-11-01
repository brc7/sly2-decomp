struct E { void *p0; void *p4; int pad[6]; };
struct S { char pad[0x50]; int c; char pad2[0xC]; struct E a[6]; };
int FIsBasicDerivedFrom__FP5BASIC3CID(void *q, int n);
void func_001C20E8(struct S *p, void *q)
{
    if ((unsigned)p->c < 6) {
        struct E *pe = &p->a[p->c++];
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, 10)) pe->p0 = q; else pe->p4 = q;
    }
}
