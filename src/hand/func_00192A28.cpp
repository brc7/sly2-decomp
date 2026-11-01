typedef int qword __attribute__((mode(TI)));
extern "C" {
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void *func_00154180(void *a, void *b);
void func_00154080(void *a, void *b, int c);
qword func_001A62E8(void *p);
void IntersectSwBoundingBox(void *g, qword a, qword b, void *pfn, void *o, int *pi, int *pj);
int func_0014CC58(int z, qword a, qword b, int i, int j, int k);
void func_00154568(void);
extern char D_003DD490[];
extern float D_002BF638;
}
class FRAME { public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } };
struct E { int x0; int x4; };
struct F { char pad[0xF8]; int xf8; };
struct O { char pad[0xF64]; F *xf64; char pad2[0xFE0 - 0xF68]; O *xfe0; };
extern "C" O *D_003E1444;
struct P { void *x0; };
extern "C" void func_00192A28(P *p);
void func_00192A28(P *p)
{
    O *o;
    for (o = D_003E1444; o; o = o->xfe0) {
        E *e = (E *)func_00154180(o->xf64, p->x0);
        if (!e || !e->x4 || (o->xf64->xf8 & 1)) continue;
        qword a = func_001A62E8(o);
        qword b = func_001A62E8(p->x0);
        qword d, t;
        float dist;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        __asm__("vmul.xyz %0, %0, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "+j"(d), "=&j"(t));
        __asm__("qmfc2.ni %0, %1" : "=r"(dist) : "j"(d));
        bool far = D_002BF638 < dist;
        if (far) {
            func_00154080(o->xf64, e, 0);
            continue;
        }
        struct { char c; FRAME frame; int i, j; } x;

        IntersectSwBoundingBox(D_003DD490, a, b, (void *)func_00154568, o, &x.i, &x.j);
        if (!func_0014CC58(0, a, b, x.i, x.j, 0)) continue;
        func_00154080(o->xf64, e, 0);
    }
}
