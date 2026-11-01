extern char D_002DDF40[];
void PostLoLoad(void *p);
void func_0014E328(void *a, int b, void *c, int d, void *e);
typedef struct T { char pad[0x28]; unsigned long fl; char pad2[0x124 - 0x30]; int f; } T;
void func_0012E648(T *t)
{
    PostLoLoad(t);
    t->f = 1;
    if ((int)(t->fl >> 60) & 1)
        func_0014E328(D_002DDF40, 10, D_002DDF40 + 0x4A8, 0, t);
}
