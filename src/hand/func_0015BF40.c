typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x58]; float g58; int pad5c; qword q60; qword v70; float g80; };
struct O { qword q0; qword v10; float g20; float g24; };
extern qword D_002A2DC0;
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    {
    qword r;
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r) : "j"(l));
    return r;
    }
}
static __inline__ void VNormalize(qword *pr, qword c, qword k)
{
    qword l;
    register qword t __asm__("vf1");
    float gl;
    l = VLen(c);
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        __asm__("" : "+r"(k));
        t = k;
    } else {
        __asm__ __volatile__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(t) : "j"(c), "j"(l));
    }
    *pr = t;
}
/** @sly2-annot addr=0x15BF40
 * Purpose: Copies the drift/velocity parameters of an EMITB into an EMITDV
 * Info: proposed name SetEmitdvEmitb (T1); TU emitter
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void func_0015BF40(struct O *po, struct S *ps)
{
    po->q0 = ps->q60;
    VNormalize(&po->v10, ps->v70, D_002A2DC0);
    po->g20 = ps->g80;
    po->g24 = ps->g58;
}
