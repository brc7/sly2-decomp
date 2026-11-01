struct C { char pad[0x44]; int a; int b; };
struct S { char pad[0x48]; int x48; int x4c; int x50; int x54; char pad2[0x28]; short x80; short pad3; int x84; char pad4[0x38]; struct C *pc; char pad5[0x34]; int xf8; char xfc[4]; };
void ClearSl(void *p);
void func_00212780(struct S *p)
{
    struct C *pc;
    p->xf8 = 0;
    p->x50 = 0;
    p->x54 = 0;
    p->x80 = 0;
    p->x84 = 0;
    ClearSl(p->xfc);
    pc = p->pc;
    pc->a = 0;
    pc->b = 0;
}
