struct K { int a, b; };
struct S { char pad[0xE4]; int ne4; };
struct G;
extern struct G D_003DD490;
extern struct K D_002A77C0;
void OnLoAdd(struct S *p);
void EnsureSwCallback(struct G *pg, struct S *p, struct K k, int n);
void OnProxyAdd(struct S *p)
{
    OnLoAdd(p);
    if (!p->ne4) EnsureSwCallback(&D_003DD490, p, D_002A77C0, 0);
}
