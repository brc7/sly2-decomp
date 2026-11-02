// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { int ref; char pad[4]; int c; char pad2[4]; void *m; } S;
extern char D_004520A8[];
void func_00256D40(S **pp, int f);
void func_00129EC8(void *); void func_002550F8(S *); void func_0011CD98(void *, S *); void func_001AE520(void *);
void func_00256D40(S **pp, int f)
{
    S *p = *pp;
    if (p) {
        if (--p->ref == 0) {
            func_00256D40((S **)&p->m, 2);
            func_00129EC8(&p->c);
            func_002550F8(p);
            func_0011CD98(D_004520A8, p);
        }
    }
    if (f & 1) func_001AE520(pp);
}
