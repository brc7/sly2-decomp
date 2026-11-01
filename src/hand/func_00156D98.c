struct K { short x; short y; int i; };
struct S { char pad[0x104]; struct K k; };
void CloneXfm(struct S *p, int n);
void func_00156D98(struct S *p, int n)
{
    struct K k = p->k;
    CloneXfm(p, n);
    p->k = k;
}
