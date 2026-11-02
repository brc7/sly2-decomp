// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { float a[4]; } V4;
typedef struct { int x0; char pad[0x2C]; union { float f; V4 v; } u; } T;
typedef struct { char pad[0xC0]; int xC0; } E;
typedef struct { char pad[0xE4]; E *xE4; } S;
extern V4 D_002BDDF0; extern float D_002BDE00;
T *func_00259330(S *, int);
void func_00259830(S *p, int k)
{
    T *t = func_00259330(p, 1);
    if (t->x0 == k) return;
    if (p->xE4->xC0 != 0) return;
    t->x0 = k;
    switch (k) {
    case 2: t->u.v = D_002BDDF0; break;
    case 1: t->u.f = D_002BDE00; break;
    }
}
