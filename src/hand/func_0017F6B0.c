struct K { short x; short y; int i; };
extern struct K D_002A5638;
extern char D_003DD490[];
struct S { char pad[0x180]; int n180; int n184; int pad2; int n18C; };
void ClearSwCallbacks(void *g, int a, void *p, struct K k, int n);
void func_0017F6B0(struct S *p) { p->n184 = p->n180; if (p->n18C) ClearSwCallbacks(D_003DD490, 1, p, D_002A5638, 0); }
