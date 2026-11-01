typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
struct OWNER {M3 matrix;char pad30[0x70];OWNER *parent;char padA4[0xC];qword velocity;char padC0[0x30];qword force;};
struct P {char pad5C[0x5C];OWNER *owner;};
struct COEF {float a,b,c;};
struct CLOCK {float time,dt;};
extern CLOCK D_002DDED0;
extern "C" M3 func_0022FD38(OWNER *,OWNER *,const M3 *);
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" M3 func_0011BC00(const M3 *,const M3 *);
extern "C" void func_0011BCF8(const M3 *,float *,qword *);
extern "C" qword func_0022FCC8(OWNER *,OWNER *,qword);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword a){qword d,one;float f;
__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(d),"=&j"(one):"j"(a));
__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(d):"0"(d));
__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword AddScaled(qword old,qword v,qword s){__asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(old):"j"(v),"j"(s));return old;}
/** @sly2-annot addr=0x1F5C30
 * Purpose: Spring torque toward orientation (quadratic CLQ on angle) and toward target angular velocity
 *   (clamped 1/dt)
 * Info: proposed name AccelSoTowardMatSpring (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F5C30(P*p,const M3*orientation,const COEF*c,qword velocity,const COEF*d)
{
    OWNER *owner=p->owner;
    if(c){
        M3 frame,relative;
        U axis;
        float angle;
        frame=func_0022FD38(0,owner->parent,orientation);
        NormalizeRotateMatrix3__FP7MATRIX3(&frame);
        relative=func_0011BC00(&owner->matrix,&frame);
        func_0011BCF8(&relative,&angle,&axis.q);
        if(0.0001f<angle){
            float gain=c->c*angle*angle+c->b*angle+c->a;
            U scalar;
            qword k=Scalar(gain);
            {qword result=AddScaled(owner->force,axis.q,k);scalar.q=k;owner->force=result;}
        }
    }
    if(d){
        qword transformed=func_0022FCC8(0,owner->parent,velocity);
        qword direction=Sub(transformed,owner->velocity);
        float length=Length(direction);
        if(0.0001f<length){
            float gain=(d->c*length+d->b)+d->a/length;
            float limit=1.0f/D_002DDED0.dt;
            U scalar;
            qword k;
            if(limit<gain)gain=limit;
            k=Scalar(gain);
            {qword result=AddScaled(owner->force,direction,k);scalar.q=k;owner->force=result;}
        }
    }
}
