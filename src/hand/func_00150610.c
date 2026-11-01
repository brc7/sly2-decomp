struct A { void *p0; int pad[3]; float g10; float g14; int pad2[7]; void *p34; void *p38; int pad3[4]; float g4C; float g50; };
extern char D_002DDF40[];
void func_001FB098(void *p, float g);
void func_00258040(void **pp, int n);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
void func_0014D8D8(void *p, float a, float b);
void func_0014FCE8(struct A *a, int n, void *b);
static inline void Release(void **pp, float g)
{
    if (pp && *pp) {
        func_001FB098(*pp, g);
        func_00258040(pp, 0);
    }
}
void func_00150610(struct A *a, int n, void *b)
{
    Release(&a->p34, 0.0f);
    Release(&a->p38, 0.5f);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(a->p0, 0x34))
        func_0014D8D8(D_002DDF40, a->g10 + 3.1415927f, -a->g14);
    else
        func_0014D8D8(D_002DDF40, a->g4C + 3.1415927f, -a->g50);
    func_0014FCE8(a, n, b);
}
