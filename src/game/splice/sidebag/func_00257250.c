// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { int ref; char pad[8]; int c; char pad2[4]; void *m; } S;
extern char D_004520D0[];
void func_00257250(S **pp, int f);
void func_00129EC8(void *); void func_00255438(S *); void func_0011CD98(void *, S *); void func_001AE520(void *);
void func_00257250(S **pp, int f)
{
    S *p = *pp;
    if (p) {
        if (--p->ref == 0) {
            func_00257250((S **)&p->m, 2);
            func_00129EC8(&p->c);
            func_00255438(p);
            func_0011CD98(D_004520D0, p);
        }
    }
    if (f & 1) func_001AE520(pp);
}
