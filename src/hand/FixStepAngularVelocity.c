typedef int qword __attribute__((mode(TI)));
typedef struct { short d; short pad; void *pfn; } VTE;
typedef union { qword q; float f[4]; } U;
struct B { char pad[0xB0]; qword v; };
struct S { char pad[0xC]; VTE *vt; char pad2[0x5C - 0x10]; struct B *pb; char pad3[0x1A8 - 0x60]; float g; char pad4[0x1F4 - 0x1AC]; int x; };
float func_002458F0(float y, float x);
void func_0011D2D0(float a, float b, float c, int *p, float *pf);
void FixStepAngularVelocity(struct S *p)
{
    U v;
    float *a, *b;
    v.q = p->pb->v;
    a = ((float *(*)(void *))p->vt[31].pfn)((char *)p + p->vt[31].d);
    b = ((float *(*)(void *))p->vt[31].pfn)((char *)p + p->vt[31].d);
    func_0011D2D0(func_002458F0(a[1], b[0]), p->g, 0.0f, &p->x, &v.f[2]);
    ((void (*)(void *, qword))p->vt[36].pfn)((char *)p + p->vt[36].d, v.q);
}
