typedef int qword __attribute__((mode(TI)));
struct R { char pad[0x14]; int n14; };
struct P { char pad[0x20]; void *p20; void *p24; };
struct R *func_001F8910(struct P *, int, int, int);
void SetActPositionGoal(struct R *, qword);
void func_0012DC18(void *, void *, int, int, int, qword);
void func_001F89E0(struct P *p, int n, int m, qword q)
{
    struct R *r = func_001F8910(p, 1, n, m);
    if (r) {
        r->n14 = n;
        SetActPositionGoal(r, q);
    } else if (n != -1) {
        func_0012DC18(p->p20, p->p24, m, n, 2, q);
    }
}
