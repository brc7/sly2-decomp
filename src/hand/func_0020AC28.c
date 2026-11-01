typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DD0;
struct A { char pad[0xA0]; int nA0; };
struct S { char pad[0xF08]; struct A *pa; int h; int n; char pad2[0xC]; qword v; };
void ReleaseAct(int *ph);
int func_0012DC18(struct S *p, int a, int b, int c, int d, qword q);
void func_0020AC28(struct S *p, struct A *pa, int n, qword v)
{
    if (p->pa) {
        ReleaseAct(&p->h);
        p->h = 0;
        p->pa = 0;
    }
    p->pa = pa;
    if (pa && pa->nA0) {
        p->h = func_0012DC18(p, pa->nA0, 5, 8, 3, D_002A2DD0);
        p->n = n;
        p->v = v;
    }
}
