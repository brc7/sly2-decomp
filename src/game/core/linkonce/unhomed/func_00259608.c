// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
typedef struct { float a[2]; } V2; typedef struct { float a[3]; } V3;
typedef struct { int x0; char pad[0x2C]; union { int i; V2 v2; V3 v3; } u; } T;
typedef struct { char pad[0xE4]; void *xE4; } S;
extern V2 D_002BDE50; extern int D_002BDE58; extern V3 D_002BDE60;
T *func_002593B8(S *, int);
void func_00259608(S *p, int k)
{
    T *t = func_002593B8(p, 1);
    if (t->x0 == k) return;
    if (t->x0 != -1) return;
    t->x0 = k;
    switch (k) {
    case 2: t->u.v2 = D_002BDE50; break;
    case 3: t->u.i = D_002BDE58; break;
    case 8: t->u.v3 = D_002BDE60; break;
    }
}
