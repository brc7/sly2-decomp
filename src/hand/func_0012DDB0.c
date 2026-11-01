typedef struct M4 { int q[16]; } __attribute__((aligned(16))) M4;
typedef struct T { int a[7]; int n; M4 mat; } T;
T *func_0012DB50(int k, void *a, void *b, void *c, void *d);
void AddXfAct(void *a, void *b, T *p);
T *func_0012DDB0(void *a, void *b, void *c, void *d, int n, M4 *pmat)
{
    T *p = func_0012DB50(6, a, b, c, d);
    p->n = n;
    p->mat = *pmat;
    AddXfAct(a, b, p);
    return p;
}
