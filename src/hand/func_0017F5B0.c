struct K { short x; short y; int i; };
extern const struct K D_002A5630;
struct G { int a; };
extern struct G D_003DD490;
struct S { char pad[0x18C]; int n18C; };
void PostSwCallback(struct G *g, void *p, struct K k, int n);
void func_0017F5B0(struct S *p, int n) { if (p->n18C) return; p->n18C = 1; PostSwCallback(&D_003DD490, p, D_002A5630, n); }
