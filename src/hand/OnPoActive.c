struct G { char pad[0x3B0]; int a; };
struct S { char pad[0x16C]; void *p; };
extern struct G D_002DDF40;
void func_00221268(void *p, int f);
void func_0014E328(struct G *pg, int n, void *pv, void *p, int m);
void func_0014E4C8(struct G *pg, int a, int b, void *c, void *d, int e);
void OnPoActive(struct S *p, int f)
{
    if (f) {
        if (p->p) func_00221268(p->p, 1);
        else func_0014E328(&D_002DDF40, 1, &D_002DDF40.a, p, 0);
    } else {
        if (p->p) func_00221268(p->p, 0);
        else func_0014E4C8(&D_002DDF40, 0xF, 1, &D_002DDF40.a, p, 0);
    }
}
