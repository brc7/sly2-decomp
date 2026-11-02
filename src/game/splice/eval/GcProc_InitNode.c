// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern char D_002DD2B0[]; extern int D_002DD598;
typedef struct { int x0; unsigned lo : 31; unsigned hi : 1; } S;
void GcProc_PushFront(S *, void *);
S *GcProc_InitNode(S *p) { p->x0 = 0; GcProc_PushFront(p, D_002DD2B0); p->hi = D_002DD598; return p; }
