// Evaluate a clamped path offset, then rotate and translate it into world space.
typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; int i[4]; };
struct M3 { qword r[3]; };
struct S { char pad[0x10C]; void *curve; char pad1[0x120-0x110]; int polar; char pad120[0x12C-0x124]; int arc; char pad2[0x140-0x130]; float time; char pad3[0x150-0x144]; qword origin; float radius; };
extern "C" {
void EvaluateShapeS(void *, float, qword *, qword *);
M3 BuildOrthonormalMatrixZ(qword,qword);
qword SetVectorCylind(float,float,float);
M3 *func_001A6318(struct S *);
qword ConvertAloPos(void *,void *,qword);
}
extern qword D_002A2DC0;
extern "C" float func_00245A18(float,float);
struct COEF { float a,b,c,d; };
struct BOUND { float low,high; };
static inline qword Sub(qword a,qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b));return r; }
static inline qword Add(qword a,qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b));return r; }
static inline qword Cross(qword a,qword b) { qword r; __asm__("vopmula.xyz ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1" : "=&j"(r) : "j"(a),"j"(b));return r; }
// The VU helper receives vf1 and reuses its length temporary for the result.
static inline qword Normalize(qword arg) { qword d,one,r;
    register qword v __asm__("vf1") = arg;
    qword len;

__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(len),"=&j"(one) : "j"(v));
__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "=j"(len) : "j"(len));
__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q" : "=j"(r) : "j"(v),"0"(len));
return r; }

static inline qword Rotate(const M3 &m,qword v) {
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z" : "+j"(v) : "j"(m.r[0]),"j"(m.r[1]),"j"(m.r[2])); return v;
}
/** @sly2-annot addr=0x2215C0
 * Purpose: evaluates a clamped path offset (fmodf), rotates/translates to world
 * Info: proposed name CalculateTnPos (T2); TU tn
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
extern "C" void func_002215C0(S *p,float *times,COEF *coef,BOUND *bounds,int mode,qword *out,float time,float delta) {
    U pos,normal,center;
    M3 m;
    U rotated;
    float *f=pos.f;
    int i;
    i=2; do {
        float t=*times;
        float x=coef->a+t*(coef->b+t*coef->c);
        float lo=bounds->low,hi=bounds->high;
        if(x<lo) x=lo;
        else if(hi<x) x=hi;
        *f++=x; times++; bounds++; coef++;
    } while(--i>=0);
    if(mode==2) pos.f[0]=-pos.f[0];
    if(p->polar) pos.q=SetVectorCylind(pos.f[0],pos.f[1],pos.f[2]);
    if(p->curve) {
        EvaluateShapeS(p->curve,mode!=1?time-delta:time+delta,&center.q,&normal.q);
        m=BuildOrthonormalMatrixZ(normal.q,D_002A2DC0);
        rotated.q=Rotate(m,pos.q);
    } else if(p->arc) {
        float period=p->radius*6.2831855f;
        float phase=mode!=1?time-delta:time+delta;
        float angle=func_00245A18(phase,period);
        center.q=SetVectorCylind(angle/p->radius,p->radius,0.0f);
        qword arc=center.q;
        center.q=Add(arc,p->origin);
        normal.q=Normalize(Cross(D_002A2DC0,arc));
        m=BuildOrthonormalMatrixZ(normal.q,D_002A2DC0);
        rotated.q=Rotate(m,pos.q);
    } else {
        center.i[0]=center.i[1]=center.i[2]=0;
        pos.f[0]+=delta;
        rotated.q=ConvertAloPos(p,0,pos.q);
    }
    *out=Add(center.q,rotated.q);
}
