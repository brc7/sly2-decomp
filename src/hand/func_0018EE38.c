typedef int qword __attribute__((mode(TI)));
struct VT { struct { short d; short pad; float *(*pfn)(void *); } a[40]; };
struct P { char pad[0x18]; float g18; };
struct T { char pad[0x50]; int n50; char pad1[0x5C - 0x54]; void *p5C; };
struct O { char pad[0xC]; struct VT *pvt; char pad1[0x150 - 0x10]; struct P *p150; char pad2[0x10DC - 0x154]; char *p10DC;
    char pad3[0x18C8 - 0x10E0]; int n18C8; int x18CC; int n18D0; int x18D4; int n18D8; };
extern qword func_001A62E8(void *p);
extern int func_0011D910(void *p, float g);
extern struct T *func_001EEEE0(void *p, int n);
extern void func_0018C250(struct O *p, int n, int m);
extern float func_002458F0(float y, float x);
extern float RadNormalize__Ff(float g);
extern int func_0018ED98(struct O *p);
extern float D_002BF4AC;
extern char D_002BF4B0[];
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0, $0, %1" : "=r"(z) : "r"(q)); return z; }
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
int func_0018EE38(struct O *p)
{
    int n;
    if (p->p10DC) {
        qword d, l;
        float g;
        int f;
        d = VSub(func_001A62E8(p->p10DC), func_001A62E8(p));
        __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(l) : "j"(d));
        __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(l) : "j"(l));
        __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(l));
        f = 0;
        if (g < D_002BF4AC)
            f = func_0011D910(D_002BF4B0, GetZ(d)) != 0;
        if (f) {
            struct T *pt = func_001EEEE0(p->p10DC + 0x26C, 0xB1);
            if (pt && pt->p5C == p) {
                int k = pt->n50;
                if (k < 2) {
                    if (k >= 0) {
                        func_0018C250(p, 0x10, -1);
                        return -1;
                    }
                }
            }
        }
    }
    if (p->n18D0 == 5) {
        float *pa = p->pvt->a[31].pfn((char *)p + p->pvt->a[31].d);
        float *pb = p->pvt->a[31].pfn((char *)p + p->pvt->a[31].d);
        float d = RadNormalize__Ff(p->p150->g18 - func_002458F0(pa[1], pb[0]));
        float ad = __builtin_fabsf(d);
        if (ad < 0.7853982f)
            n = 0xE;
        else if (2.3561945f < ad)
            n = 0x10;
        else if (0.0f < d)
            n = 0x12;
        else
            n = 0x14;
    } else {
        n = 0xE;
        if (p->n18C8 == 0)
            n = 0xC;
    }
    switch (p->n18D8) {
    case 0xC: case 0xE: case 0x10: case 0x12: case 0x14:
        n++;
        break;
    case 0xD: case 0xF: case 0x11: case 0x13: case 0x15:
        break;
    default:
        if (func_0018ED98(p))
            n++;
        break;
    }
    return n;
}
