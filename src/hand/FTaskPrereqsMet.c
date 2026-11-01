struct X { char pad[0x54]; int n; char pad2[0x6C - 0x58]; int f6c; };
struct S { char pad[0x94]; int c; struct X **apx; };
int FTaskPrereqsMet(struct S *p)
{
    int f = 1;
    int i;
    for (i = 0; f && i < p->c; i++) {
        struct X *px = p->apx[i];
        f = 0;
        if (px->n >= 2 || (px->n == 1 && px->f6c))
            f = 1;
    }
    return f;
}
