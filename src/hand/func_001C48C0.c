struct C { int pad[2]; int *pn; };
struct B { char pad[0xE4]; struct C *pc; };
struct S { char pad[0x3EFC]; int n; char pad2[0x4778 - 0x3F00]; int c; struct B *a[1]; };
void SetLoParent_1A58D0(struct B *pb, int n, int m);
void func_001C48C0(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct B *pb = p->a[i];
        *pb->pc->pn = p->n;
        SetLoParent_1A58D0(pb, p->n, 0);
    }
}
