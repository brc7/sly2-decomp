extern int D_002DAA10[];
struct S { int a; int b; int c; int *vt; int d; };
struct S *LEAP_ctor(struct S *p) { p->vt = D_002DAA10; p->d = -1; p->a = 0; p->b = 0; return p; }
