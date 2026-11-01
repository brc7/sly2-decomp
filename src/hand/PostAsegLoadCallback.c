typedef struct P { float x; float y; int n; } P;
void func_0012F9B8(void *a, float x, float y, int b, int n, int c);
void PostAsegLoadCallback(void *a, P *p)
{
    func_0012F9B8(a, p->x, p->y, 0, p->n, 0);
}
