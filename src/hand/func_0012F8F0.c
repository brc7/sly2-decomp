typedef struct S8 { int a, b; } S8;
extern S8 D_002A46C8;
extern char D_003DD490[];
void PostLoLoad(void *p);
void PostSwCallback(void *a, void *b, S8 c, int d);
typedef struct T { char pad[0xB0]; int f; } T;
void func_0012F8F0(T *t)
{
    PostLoLoad(t);
    if (t->f)
        PostSwCallback(D_003DD490, t, D_002A46C8, t->f);
}
