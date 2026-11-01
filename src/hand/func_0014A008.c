struct S { int a, b; };
struct F { int a[4]; };
struct P { char pad[0x54]; int n54; int n58; };
struct F *func_001AE478(struct F *f);
int func_001AE378(struct S *s);
void LoadLoFromBrx(struct P *p, int a);
int func_001AE238(struct S *s, int n);
void func_001AE4A0(struct F *f, int n);
static inline void InitS(struct S *ps, int a, int b)
{
    ps->a = a * 0xC;
    ps->b = b;
}
void func_0014A008(struct P *p, int a)
{
    struct F f;
    struct S s;
    func_001AE478(&f);
    InitS(&s, 0x100, 0x10);
    p->n58 = func_001AE378(&s);
    LoadLoFromBrx(p, a);
    InitS(&s, p->n54, 0x10);
    p->n58 = func_001AE238(&s, p->n58);
    func_001AE4A0(&f, 2);
}
