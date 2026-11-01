typedef int qword __attribute__((mode(TI)));
typedef struct R { char pad[0x1C]; float g; qword v; qword w; } R;
R *func_0012DB50(int k, void *a, void *b, void *c, int d);
void AddXfAct(void *a, void *b, void *p);
R *func_0012DE58(void *a, void *b, void *c, float g, qword v, qword w)
{
    R *p = func_0012DB50(8, a, b, c, 0x11);
    p->g = g;
    p->v = v;
    p->w = w;
    AddXfAct(a, b, p);
    return p;
}
