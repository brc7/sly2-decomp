struct K { int a, b; };
extern struct K D_002A96F0;
extern char D_003DD490[];
struct S { char pad[0x13C]; int x; };
void UpdateLo(struct S *p);
void PostSwCallback(void *g, struct S *p, struct K k, int z);
void func_00221388(struct S *p)
{
    UpdateLo(p);
    if (p->x) PostSwCallback(D_003DD490, p, D_002A96F0, 0);
}
