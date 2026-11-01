// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { char pad[0x10]; int x10; char pad2[0x1C]; void *x30; } E;
typedef struct { char pad[0xE4]; E *xE4; } S;
typedef struct { int a, b; } P;
E *func_001AE1D0(P *); void func_00159118(E *); void LoadXfmFromBrx(S *, int); void func_00159240(void *, int); void func_002584A8(S *, int);
void func_00258598(S *p, int x)
{
    P t;
    t.a = 0x100; t.b = 0x10;
    func_00159118(p->xE4 = func_001AE1D0(&t));
    LoadXfmFromBrx(p, x);
    if (p->xE4->x10 == 3) func_00159240(p->xE4->x30, x);
    func_002584A8(p, x);
}
