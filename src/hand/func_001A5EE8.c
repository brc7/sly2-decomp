typedef int qword __attribute__((mode(TI)));
typedef struct { qword q[4]; } M4;
struct VTE { short d; short pad; void *pfn; };
struct O { int x0, x4, x8; struct VTE *vt; };
void InvertMatrix4Rigid(M4 *out, void *in);
void __ml__FRC7MATRIX4T0(M4 *out, void *a, M4 *b);
void func_001A5EE8(struct O *o, void *m, float a, float b)
{
    M4 t;
    M4 r;
    M4 inv;
    M4 *pr = &r;
    M4 *pinv = &inv;
    struct VTE *e = &o->vt[0xF8 / 8];
    InvertMatrix4Rigid(pinv, ((void *(*)(void *))e->pfn)((char *)o + e->d));
    __ml__FRC7MATRIX4T0(pr, m, pinv);
    t = r;
    e = &o->vt[0xA0 / 8];
    ((void (*)(void *, M4 *, int, float, float))e->pfn)((char *)o + e->d, &t, 0, a, b);
}
