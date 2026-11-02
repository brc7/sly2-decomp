typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} V;
struct COEF {float a,b,c;};
struct VE {short d,pad;qword(*fn)(void*);};
struct OWNER {char pad[0xA0];void *parent;char padA4[0x4C];qword force;};
struct S {char pad[0xC];struct VE *vt;char pad10[0x48];struct OWNER *owner;};
struct CLOCK {float time,dt;};
extern struct CLOCK D_002DDED0;
qword func_001A62E8(struct S*);
qword ConvertXfVec(void*,void*,qword);
static inline qword Sub(qword x,qword y){register qword a __asm__("vf1")=x,b __asm__("vf2")=y,r __asm__("vf3");__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword value){register qword a __asm__("vf3")=value,r __asm__("vf1"),one __asm__("vf2");float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(r),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"0"(r));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(r));return f;}
static inline qword LoadVector(const V *value){register qword result asm("v0");__asm__("lq %0,%1":"=r"(result):"m"(value->q));return result;}
static inline qword Scalar(float f){register qword r __asm__("$2");__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;}
static inline qword AddScaled(qword old,qword direction,qword factor){register qword a __asm__("vf2")=old,b __asm__("vf4")=direction,scale __asm__("vf3")=factor,r __asm__("vf1");
__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(a),"j"(b),"j"(scale));return r;}
/** @sly2-annot addr=0x1F5E10
 * Purpose: Spring force toward position (CLQ on distance) and toward target velocity (clamped 1/dt)
 * Info: proposed name AccelSoTowardPosSpring (T1); TU so
 * Notes: names_v2 C proposed ActivateCptn__FP4CPTNPv (0.170)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AccelSoTowardPosSpring(struct S*input,qword position,struct COEF*positionCoefficients,qword velocity,struct COEF*velocityCoefficients)
{
 V argument,point,old1,scale1,speed,old2,scale2;
 register struct S *p __asm__("$17")=input;
 register struct COEF *c __asm__("$16")=positionCoefficients,*d __asm__("$18")=velocityCoefficients;
 register struct OWNER *owner __asm__("$19");
 point.q=position;owner=p->owner;speed.q=velocity;
 if(c){
   qword difference=Sub(point.q,func_001A62E8(p));
   float distance=Length(difference);
   if(1.0e-4f<distance){
     float inverse=c->a/distance;
     float value=c->c*distance+c->b;
     register void *parent __asm__("$5");
     old1.q=owner->force;parent=owner->parent;value+=inverse;
     argument.q=Scalar(value);scale1=argument;
     owner->force=AddScaled(old1.q,ConvertXfVec(0,parent,difference),scale1.q);
   }
 }
 if(d){
   qword difference=Sub(speed.q,p->vt[33].fn((char*)p+p->vt[33].d));
   float distance=Length(difference);
   if(1.0e-4f<distance){
     register void *parent asm("a1");
     float value=(d->c*distance+d->b)+d->a/distance;
     float limit=1.0f/D_002DDED0.dt;
     if(limit<value)value=limit;
     scale2.q=Scalar(value);
     {register qword oldValue asm("v0")=owner->force;old2.q=oldValue;}parent=owner->parent;argument.q=LoadVector(&scale2);
     owner->force=AddScaled(old2.q,ConvertXfVec(0,parent,difference),scale2.q);
   }
 }
}
