struct S { int n0; int n4; int n8; };
struct L { int a; int b; int c; int d; };
void PopBhv(struct L *pl);
void ClearBhvstk__FP6BHVSTK(struct S *p)
{
    struct L l;
    if (p->n4 > 0) {
        l.a = p->n8;
        PopBhv(&l);
    }
    p->n4 = 0;
}
