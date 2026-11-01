struct B { int *p; int pad[3]; int f; };
extern int D_002E0CB4;
int FFindSlEntry(void *a, struct B *b);
void func_0011A070(void *a, struct B *b);
int func_0014BEA8(void *a, struct B *b)
{
    int r = FFindSlEntry(a, b);
    if (r) return r;
    func_0011A070(a, b);
    if (*b->p == D_002E0CB4) b->f = 1;
    else b->f = 0;
}
