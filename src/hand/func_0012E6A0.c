extern char D_002DDF40[];
void AddLoHierarchy(void *p);
void func_0014E328(void *a, int b, void *c, int d, void *e);
typedef struct T { char pad[0x124]; int f; } T;
void func_0012E6A0(T *t)
{
    AddLoHierarchy(t);
    if (t->f)
        func_0014E328(D_002DDF40, 10, D_002DDF40 + 0x4A8, 0, t);
}
