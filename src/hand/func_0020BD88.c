// Initialize curve timing, planar distance and curvature.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } U;
struct VE { short d; short pad; void (*pf)(void *, qword *, qword *, float, float); };
struct CV { struct VE *vt; };
struct C { int a; struct CV *pcv; };
union FLOATWORD {float value;int bits;};
struct MARKER {union FLOATWORD value;int kind,id,unused;};
struct V3 { float x, y, z; };
struct S { int h; void *po; float a; float b; float c; float ca; float cb; union FLOATWORD dist; struct V3 v; int pad; int f; };
extern char D_003DD490[];
void *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int n, int h, void *arg);
struct MARKER *func_0012F938(void *po, int n);
struct C *func_0012FF10(void *po, void *arg, int kind, int check);

void func_0020BD88(struct S *p, void *arg)
{
    struct MARKER *pa, *pb, *pc;
    struct C *c;
    U v0, v1, v2;
    qword d;
    int n;
    register struct CV *thirdCurve __asm__("$7");
    register float start __asm__("$f12");
    register qword *derivative __asm__("$6");
    float h;
    if (p->f)
        return;
    if (!p->po) {
        p->po = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, p->h, arg);
        if (!p->po)
            return;
    }
    pa = func_0012F938(p->po, 0x22);
    if (!pa)
        return;
    pb = func_0012F938(p->po, 0x23);
    if (!pb)
        return;
    pc = func_0012F938(p->po, 0x24);
    if (!pc)
        return;
    p->a = pa->value.value;
    p->b = pb->value.value;
    p->c = pc->value.value;
    p->cb = p->c - p->b;
    p->ca = p->c - p->a;
    c = func_0012FF10(p->po, arg, 1, 1);
    if (!c)
        goto initialized;
    c->pcv->vt[3].pf((char *)c->pcv + c->pcv->vt[3].d, &v0.q, 0, p->a, 1.0f);
    c->pcv->vt[3].pf((char *)c->pcv + c->pcv->vt[3].d, &v1.q, 0, p->c, 1.0f);
    __asm__("vsub.xyzw %0,%2,%3\n\tlwc1 %1,%4":"=j"(d),"=f"(start):"j"(v0.q),"j"(v1.q),"m"(p->a));
    h = p->c;
    __asm__("vmul.xy %0, %0, %0\n\tvaddy.x %0, %0, %0y\n\tvsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q\n\tdaddu %1,%z3,$zero\n\tsub.s %2,%2,%4" : "+j"(d),"=r"(derivative),"+f"(h) : "rJ"((qword*)0),"f"(start));
    h *= 0.5f;
    __asm__("qmfc2.ni %0,%3\n\tsw %0,%1\n\tlw %2,%4":"=&r"(n),"=m"(p->dist.bits),"=r"(thirdCurve):"j"(d),"m"(c->pcv));
    thirdCurve->vt[3].pf((char *)thirdCurve + thirdCurve->vt[3].d, &v2.q, derivative, start + h, 1.0f);
    {
        struct V3 *pv = &p->v;
        pv->x = 0;
        pv->y = 0;
        pv->z = (-v2.f[2] + v0.f[2]) / (h * h * 0.5f);
    }
initialized:
    p->f = 1;
}
