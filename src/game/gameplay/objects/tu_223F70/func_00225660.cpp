// Change selection state and prepare the nearest eligible actor.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } VU;
struct VTE { short d; short pad; qword (*pfn)(void *,...); };
struct O { char pad[0xC]; struct VTE *vt; char pad1[0xFC4 - 0x10]; void *xFC4; };
struct P;
struct N {struct P *owner;struct O *object;struct N *next;};
struct P { char pad[0x150]; int k; char pad1[0x1B0 - 0x154]; void *x1B0; struct O *po; struct N hook; };
struct G;
extern struct G D_003DD490;
extern qword D_002A2DE0;
extern float D_002C43F0;
extern float D_002C43F4;
extern "C" void func_00229738(struct O *po, struct N *node);
extern "C" void *func_00205458(struct O *po, int k);
extern "C" void func_0017A8C8(void *pv, int k);
extern "C" void SetActPositionGoal(void *pv, qword q);
extern "C" void func_00224EC8(struct P *p, int k);
extern "C" qword func_001A62E8(void *p);
extern "C" qword SourcePosition(void * source) __asm__("func_001A62E8");

extern "C" int CploFindSwObjectsByClass(struct G *pg, int a, int b, int c, int cMax, struct O **apo);
extern "C" int func_00206EB0(struct O *po, int k);
extern "C" void AddSoXa(struct O *po, struct N *node);
extern "C" void func_00209E28(struct O *po, int k);
static __inline__ float GDist(qword a, qword b)
{
    qword d, t;
    float g;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(d), "=j"(t));
    __asm__("vsqrt Q, %0x\n"
        "vwaitq\n"
        "vaddq.x %0, $vf0, Q" : "+j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(g) : "j"(d));
    return g;
}
// Load the scalar's IEEE word and transfer it directly into a VU register.
static inline qword LoadCoefficient(){
    int bits;
    qword scalar;
    __asm__("lui %0,%%hi(%1)":"=r"(bits):"i"(&D_002C43F0));
    __asm__("lw %0,%%lo(%1)(%0)":"+r"(bits):"i"(&D_002C43F0):"memory");
    __asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(bits));
    return scalar;
}
struct Vec {qword q;Vec(const qword &v):q(v){}operator qword()const{return q;}};
/** @sly2-annot addr=0x225660
 * Purpose: change selection state and prepare nearest eligible actor
 * Info: proposed name SenseCid102 (T2); class CID102 slot76; TU tu_223F70
 * Notes: names_v2 C proposed SenseCamsen__FP6CAMSENP7SENSORS (0.438) | agent qualifier: select nearest actor
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
extern "C" void func_00225660(struct P *p, int k)
{
    if (k == p->k)
        return;
    if (p->k == 5 && p->po) {
        void *pv;
        func_00229738(p->po, &p->hook);
        pv = func_00205458(p->po, 0xB0);
        if (pv)
            func_0017A8C8(pv, 2);
        if (p->po->xFC4)
            SetActPositionGoal(p->po->xFC4, D_002A2DE0);
        p->po = 0;
    }
    func_00224EC8(p, k);
    if (k == 5) {

        struct O *apo[0x20];
        int c, i;
        float gBest;
        struct O *poBest;
        Vec pos(p->x1B0?SourcePosition(p->x1B0):func_001A62E8(p));
        c = CploFindSwObjectsByClass(&D_003DD490, 0x105, 0x38, 0, 0x20, apo);
        gBest = 400.0f;
        poBest = 0;
        for (i = 0; i < c; i++) {
            struct O *po = apo[i];
            if (func_00206EB0(po, 0xB)) {
                float g = GDist(func_001A62E8(po), pos);
                if (g < gBest) {
                    gBest = g;
                    poBest = po;
                }
            }
        }
        if (poBest) {
            VU u, v;
            qword q, s, r;
            p->po = poBest;
            p->hook.owner = p;
            p->hook.object = poBest;
            AddSoXa(poBest, &p->hook);
            q = poBest->vt[33].pfn((char *)poBest + poBest->vt[33].d);
            s=LoadCoefficient();
            u.q = q;
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(q), "j"(s));
            u.q = r;
            v.q = s;
            u.f[2] = D_002C43F4;
            ((void (*)(void *, qword))poBest->vt[35].pfn)((char *)poBest + poBest->vt[35].d, u.q);
            func_00209E28(poBest, 0);
        }
    }
}
