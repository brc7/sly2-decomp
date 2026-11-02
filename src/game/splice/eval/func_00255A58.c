// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern char D_002DD2B0[]; extern int D_002DD598;
typedef struct { int x0; unsigned lo : 31; unsigned hi : 1; } S;
void func_002558C0(S *, void *);
S *func_00255A58(S *p) { p->x0 = 0; func_002558C0(p, D_002DD2B0); p->hi = D_002DD598; return p; }
