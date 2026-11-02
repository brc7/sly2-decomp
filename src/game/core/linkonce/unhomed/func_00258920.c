// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef int qword __attribute__((mode(TI)));
typedef struct { qword q[4]; } M;
extern M D_002BDE70; extern char D_002DE720[];
typedef struct { int pad; int x4; char pad2[0xB8]; M m; } T;
T *func_00258658(void *, int);
void *func_00258920(void *p, int x)
{
    T *t = func_00258658(p, x);
    if (x == 1 && t->x4 == -1) { t->x4 = x; t->m = D_002BDE70; }
    if (t->x4 != 1) return D_002DE720;
    return &t->m;
}
