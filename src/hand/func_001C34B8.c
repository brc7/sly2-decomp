struct K { int a, b; };
struct S { char pad[0x1B4]; struct K k; };
void CloneShape(struct S *p);
void func_001C34B8(struct S *p)
{
    struct K k = p->k;
    CloneShape(p);
    p->k = k;
}
