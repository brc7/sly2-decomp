/* RenderGlint (Claude agent, wave-2 lane 7): per-frame draw of a RIP (decal): fade size/opacity polynomials over its lifetime and by camera
 * distance, cull it (RemoveRip) when the camera test fails, otherwise build facing * spin and submit the transform.
 *
 * What made it match (no pins, no allocation-only operands):
 *  - the stack frame is gcc's temp-slot pool: named locals M3 basis/rotation/product and M4 transform at 0x00/0x30/0x60/0x90; the by-value VU_FLOAT / BcG temp
 *    at 0xD0; call results (`basis = f(..)`, `rotation = f(..)`, `transform = f(..)`) return into pool temps that coalesce back into 0xD0/0xE0; Multiply is an
 *    inline with a g++ NAMED RETURN VALUE (its `r` gets the 0x110 slot, the return temp 0xD0, then the copy into `product`);
 *  - Multiply's asm outputs are plain "=j" (the last product row reuses the basis row-0 register);
 *  - Length() leaves the result as a TI qword and `mtc1` takes it with an "r" operand, so the compiler itself emits the qmfc2.ni to a GPR (this changes the
 *    global-alloc priorities of time/distance and the local-alloc order of the address registers);
 *  - `time` is computed before `distance`; the opacity clamp keeps lo/hi loaded before the compare and its result in the callee-saved register that also holds lo.
 */
typedef int qword __attribute__((mode(TI)));
struct M3 {qword r[3];};
struct M4 {qword r[4];};
struct POLY {float a,b,c;};
struct RIP {char pad0[0x14];float start,duration;void *material;POLY size;int unused2C;POLY opacity;char pad3C[0x34];qword position;};
struct CAMERA {char pad0[0x90];qword position;};
struct CLOCK {float time,dt;};
extern CAMERA D_002DDF40;
extern CLOCK D_002DDED0;
extern POLY D_002C1FB0,D_002C1FD0,D_002C1FF0;
extern float D_002C1FC0[2],D_002C1FE0[2],D_002BBB60[2];
extern const qword D_002A2DA0,D_002A2DC0;
extern "C" {
void func_001CE980(RIP *);
int FInsideCmFrustum(CAMERA *,qword,qword);
M3 BuildOrthonormalMatrixZ(qword,qword);
M3 func_0011B500(qword,float);
M4 LoadMatrixFromPosRotScalar(qword,const M3 &,float);
void func_001A5EE8(void *,const M4 &,float,float);
}
struct VU_FLOAT { qword q; VU_FLOAT(float f) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(f)); } };
static inline qword Subtract(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Scale(qword a,VU_FLOAT s){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s.q));return r;}
static inline qword BcG(float f){union{qword q;float f;}u;qword r;__asm__("mfc1 %0,%1":"=r"(r):"f"(f));u.q=r;return r;}
static inline float Length(qword v){qword d,one,l;float f;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));
 __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(l):"j"(d));
 __asm__("mtc1 %1, %0" : "=f"(f) : "r"(l));
 return f;}
static inline float Evaluate(const POLY &p,float t){return p.a+t*(p.b+t*p.c);}
static inline float Clamp(float v,float low,float high){if(v<low)return low;else if(high<v)return high;else return v;}
/* a*b for 3x3 matrices stored as rows (each result row = a.r[0]*b.x + a.r[1]*b.y + a.r[2]*b.z) */
static inline M3 Multiply(const M3&a,const M3&b) return r {
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0]):"j"(a.r[0]),"j"(a.r[1]),"j"(a.r[2]),"j"(b.r[0]));
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1]):"j"(a.r[0]),"j"(a.r[1]),"j"(a.r[2]),"j"(b.r[1]));
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2]):"j"(a.r[0]),"j"(a.r[1]),"j"(a.r[2]),"j"(b.r[2]));
}
/** @sly2-annot addr=0x1D0190
 * Purpose: RIPT 4 Render: large custom draw (fades; RemoveRip when done)
 * Info: proposed name RenderGlint (T1); TU rip
 * Notes: verifier: purpose partly wrong: RIPT 4 VTRIP entry 5 (Render); matches Sly 1 RenderGlint
 *   (dist*0.001, clq size/alpha/spin, camera-facing). RemoveRip happens when the camera cull fails, not 'wh
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
extern "C" void RenderGlint(RIP *p)
{
    M3 basis,rotation,product;
    M4 transform;
    qword direction=Subtract(D_002DDF40.position,p->position);
    float time=(D_002DDED0.time-p->start)/p->duration;
    float distance=Length(direction)*0.001f;
    float size=Evaluate(p->size,time)*Clamp(Evaluate(D_002C1FB0,distance),D_002C1FC0[0],D_002C1FC0[1]);
    if(!FInsideCmFrustum(&D_002DDF40,Scale(direction,VU_FLOAT(-1.0f)),BcG(size*100.0f))){func_001CE980(p);return;}
    /* opacity clamp; the result shares lo's register, hi is loaded before the compare */
    float opacity=D_002BBB60[0];
    float hi=D_002BBB60[1];
    float v=Evaluate(p->opacity,time);
    if(v<opacity){}else if(hi<v)opacity=hi;else opacity=v;
    opacity*=Clamp(Evaluate(D_002C1FD0,distance),D_002C1FE0[0],D_002C1FE0[1]);
    basis=BuildOrthonormalMatrixZ(direction,D_002A2DC0);
    rotation=func_0011B500(D_002A2DA0,Evaluate(D_002C1FF0,time));
    product=Multiply(basis,rotation);
    transform=LoadMatrixFromPosRotScalar(p->position,product,size);
    func_001A5EE8(p->material,transform,opacity,1.0f);
}
