struct Q { char pad[0x18]; int x18; };
struct S { char pad[0x238]; int c238; int a23C[4]; int c24C; int a250[4]; };
int FIgnoreBombIntersection(struct S *p, struct Q *q);
int func_00134310(int r, int n);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct Q *q, int k);
int FIgnoreMissileIntersection__FP7MISSILEP2SO(struct S *p, struct Q *q)
{
    int i, j;
    int r = FIgnoreBombIntersection(p, q);
    for (i = 0; i < p->c238; i++)
        if (p->a23C[i] == q->x18) r = func_00134310(r, 1);
    for (j = 0; j < p->c24C; j++)
        if (FIsBasicDerivedFrom__FP5BASIC3CID(q, p->a250[j])) r = func_00134310(r, 1);
    return r;
}
