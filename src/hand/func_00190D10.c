typedef int qword __attribute__((mode(TI)));
#include "mat.h"
struct T { char pad[0x30]; qword x30; struct MATRIX4 m40; char pad2[0xA0-0x80]; char *xa0; int xa4; int xa8; };
typedef struct { short d; short pad; void (*pfn)(void *); } VT;
struct P {
    char pad0[0xC]; VT *vt; char pad[0x1960-0x10]; qword x1960; char pad2[0x19A4-0x1970]; void *x19a4;
    char pad3[0x19C4-0x19A8]; struct T *x19c4; int x19c8; struct T *x19cc;
};
struct G { char pad[0x3D58]; int x3d58; };
extern struct G D_003DD490;
qword func_00190C58(struct P *p);
qword func_001A62E8(void *p);
qword ConvertXfVec(int n, void *p, qword q);
void func_00256D18(qword *pq, qword q);
void __ml__FRC7MATRIX4T0(struct MATRIX4 *pm, void *p, struct T *t);
void SolveIk(struct P *p, int n);
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
void func_00190D10(struct P *p)
{
    struct T *t;
    qword a, b, d;
    struct MATRIX4 m;
    int n;

    a = VAdd(func_00190C58(p), p->x1960);
    b = func_001A62E8(p->x19a4);
    d = VSub(a, b);
    a = ConvertXfVec(0, p->x19c4->xa0, d);
    t = p->x19c4;
    func_00256D18(&t->x30, VAdd(t->x30, a));
    ((float *)&t->x30)[3] = 1.0f;
    n = ++D_003DD490.x3d58;
    p->x19c4->xa4 = n;
    {
        struct T *t2 = p->x19c4;
        __ml__FRC7MATRIX4T0(&m, t2->xa0 + 0x40, t2);
        t2->m40 = m;
    }
    SolveIk(p, p->x19cc->xa8);
    p->vt[18].pfn((char *)p + p->vt[18].d);
}
