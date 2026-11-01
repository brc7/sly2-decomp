typedef int qword __attribute__((mode(TI)));
typedef struct R { char pad[0x1C]; int x1C; qword v; } R;
R *func_0012DB50(int k, void *a, void *b, void *c, int d);
void AddXfAct(void *a, void *b, void *p);
R *func_0012DC18(void *a, void *b, void *c, int d, int e, qword v)
{
    R *p = func_0012DB50(1, a, b, c, d);
    p->x1C = e;
    p->v = v;
    AddXfAct(a, b, p);
    return p;
}
