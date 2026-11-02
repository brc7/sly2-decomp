// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern char D_002DD290[]; extern int D_002DD598;
typedef struct { int x0; unsigned lo : 31; unsigned hi : 1; } S;
void func_00255248(S *, void *);
S *func_002553E0(S *p) { p->x0 = 0; func_00255248(p, D_002DD290); p->hi = D_002DD598; return p; }
