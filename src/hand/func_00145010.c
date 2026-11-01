void InitBhv(void *p);
struct X { char pad[0x60]; int n; int pad2; float a, b, c, d, e; };
void func_00145010(struct X *p)
{
    InitBhv(p);
    p->n = -1;
    p->a = 0.25f;
    p->b = 250.0f;
    p->c = 350.0f;
    p->d = 1.0f;
    p->e = 1.2f;
}
