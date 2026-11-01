struct K { int a; int b; };
struct S { char pad[0x174]; struct K k; };
void CloneSo(struct S *p);
void func_00183A08(struct S *p)
{
    struct K k = p->k;
    CloneSo(p);
    p->k = k;
}
