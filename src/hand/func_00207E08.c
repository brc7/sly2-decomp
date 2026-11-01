long func_00207B10(int a);
void *func_00209C18(void *p, long id);
void func_00207D98(void *p, int a, int b, float c, float d, float e, float f);
void ResolveAnimBinding(void *pe, void *p);
void func_00207E08(void *p, int a, int b, float c, float d, float e, float f)
{
    void *pe = func_00209C18(p, func_00207B10(a));
    func_00207D98(p, a, b, c, d, e, f);
    ResolveAnimBinding(pe, p);
}
