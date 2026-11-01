struct K { int a; int b; };
extern struct K D_002A4BB0;
extern char D_003DD490[];
void func_00183FB8(void *);
void EnsureSwCallback(void *, void *, struct K, int);
void func_0013CFD8(void *p) { func_00183FB8(p); EnsureSwCallback(D_003DD490, p, D_002A4BB0, 0); }
