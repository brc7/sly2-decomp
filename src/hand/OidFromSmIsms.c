struct E { int a; int b; };
struct S { char pad[0x50]; int c; struct E *ae; };
int OidFromSmIsms(struct S *p, int n) { if (n < 0 || n >= p->c) return -1; return p->ae[n].a; }
