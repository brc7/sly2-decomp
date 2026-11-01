typedef struct T { int a[7]; float x; float y; } T;
T *func_0012DB50(int k, void *a, void *b, void *c, void *d);
void AddXfAct(void *a, void *b, T *p);
T *func_0012DCA0(void *a, void *b, void *c, void *d, float x, float y)
{
    T *p = func_0012DB50(4, a, b, c, d);
    p->x = x;
    p->y = y;
    AddXfAct(a, b, p);
    return p;
}
