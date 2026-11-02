typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct VE { short d; short pad; void (*pf)(void *,float,float,qword *,qword *); };
struct CV { struct VE *vt; };
struct C { int a; struct CV *pcv; };
union FLOATWORD { float value; int bits; };
struct O { char pad[0x74]; union FLOATWORD duration; };
struct E { struct O *po; int pad[3]; qword vel; union FLOATWORD speed; int pad2[3]; };
struct S { struct E ae[4]; int fInit; };
extern char D_003DD490[];
struct O *PloFindSwObject__FP2SWi3OIDP2LO(void *,int,int,void *);
struct C *func_0012FF10(struct O *,void *,int,int);

/** @sly2-annot addr=0x20C7E0
 * Purpose: 4 directional ASEGs: root velocity and speed
 * Info: proposed name InitDirAnims (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020C7E0(struct S *p,void *arg)
{
    struct E *e; unsigned int i;
    if (p->fInit)
        return;
    for (i = 0; i < 4; i++) {
        struct O *o;
        struct C *c;
        U v0, v1;
        U u;
        qword d,vel,r,s;
        register qword tmp __asm__("vf4");
        register qword start __asm__("vf2");
        register qword end __asm__("vf3");
        register qword scalar __asm__("vf1");
        int f;
        e = &p->ae[i];
        o = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490,4,(int)e->po,arg);
        e->po = o;
        if (!o)
            continue;
        c = func_0012FF10(o,arg,1,1);
        c->pcv->vt[3].pf((char *)c->pcv + c->pcv->vt[3].d, 0.0f,1.0f,&v0.q,0);
        c->pcv->vt[3].pf((char *)c->pcv + c->pcv->vt[3].d, o->duration.value,1.0f,&v1.q,0);
        { register int bits __asm__("$17")=o->duration.bits;
          __asm__("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tlqc2 %2,%5"
                  : "=j"(scalar),"=j"(start),"=j"(end)
                  : "r"(bits),"m"(v0.q),"m"(v1.q)); }
        u.q=scalar;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "0"(end), "j"(start));
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(vel) : "0"(u.q), "j"(d));
        __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(r), "=&j"(tmp) : "j"(vel));
        e->vel = vel;
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(s) : "0"(r));
        __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(s));
        e->speed.bits=f;
    }
    p->fInit = 1;
}
