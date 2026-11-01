// func_001F59A0: angular braking and bounded angular-velocity correction.
// coef a = top angular speed, (a-b)/c = angular acceleration, a*a/(2*acceleration) = stopping angle.
// Caps an overspeed, turns the owner toward `orientation` (expressed in its parent space) with a desired
// speed taken from the predicted remaining angle, and adds a force change bounded by acceleration*dt.
// Matched from the codex/lilly drafts (T1623). Notes: notes/1F0000.md.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {U rows[3];M3(){} M3(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];} M3&operator=(const M3&m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];return *this;}};
struct SPACE;
struct OWNER {M3 matrix;char pad30[0x70];SPACE *parent;char padA4[0xC];qword velocity;char padC0[0x30];qword force;};
struct P {char pad5C[0x5C];OWNER *owner;};
struct COEF {float a,b,c;};
struct CLOCK {float time,dt;};
extern CLOCK D_002DDED0;
extern "C" M3 *func_0022FD38(M3 *,SPACE *,SPACE *,const M3 *);
static inline M3 BuildFrame(SPACE*parent,const M3*orientation) return result {func_0022FD38(&result,0,parent,orientation);}
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void func_0011BC00(M3 *,const M3 *,const M3 *);
extern "C" void func_0011BCF8(const M3 *,float *,qword *);
extern "C" float func_0011CFE0(float,float);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Add(qword a,qword b){__asm__("vadd.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
// volatile store: the game's later loads (dt, velocity, the epilogue) wait for these sqc2s
static inline void StoreQ(qword v,U *out){__asm__ __volatile__("sqc2 %1,%0":"=m"(out->q):"j"(v));}
static inline qword Scalar(float f,U *out){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));__asm__("sqc2 %1,%0":"=m"(out->q):"j"(q));return q;}
static inline qword Scale(qword source,qword k){qword v=source;__asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k));return v;}
static inline qword ScaleFactor(qword v,qword k){__asm__("vmulx.xyzw %0,%1,%0x":"+j"(k):"j"(v));return k;}
static inline qword ScaleNew(qword v,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(r):"j"(v),"j"(k));return r;}
static inline qword AddScaled(qword a,qword b,qword k){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(k));return a;}
static inline qword DotQ(qword value,qword normal){qword d=value,b=normal,one;__asm__("vmul.xyz %0,%0,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d),"=&j"(one):"j"(b));return d;}
// x of a qword: the compiler emits the qmfc2, the asm the mtc1 (an asm qmfc2 issues it before the 0.5 load)
static inline float GetX(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Length(qword a){qword d,one;float f;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}

/** @sly2-annot addr=0x1F59A0
 * Purpose: Angular braking + bounded angular-velocity correction toward an orientation (coef top speed /
 *   accel)
 * Info: proposed name AccelSoTowardMatBounded (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F59A0(P *p,const M3 *orientation,const COEF *c)
{
    M3 frame,relative;
    U axis,wanted,change,predicted;
    float angle;
    float maximum=c->a;
    float acceleration=(maximum-c->b)/c->c;
    float square=maximum*maximum;
    float stopAngle;
    OWNER *owner=p->owner;
    // one variable for the speed and, later, the speed along the turn axis (target: f4 for both)
    float speed=Length(owner->velocity);
    stopAngle=square/(acceleration+acceleration);
    float desired,remaining,length,gain;
    CLOCK *clock;
    if(maximum<speed){
        U scalar;float correction=((maximum-speed)*2.5f)/speed;
        change.q=ScaleFactor(owner->velocity,Scalar(correction,&scalar));
        owner->force=Add(owner->force,change.q);
    }
    frame=BuildFrame(owner->parent,orientation);
    NormalizeRotateMatrix3__FP7MATRIX3(&frame);
    {M3 out;func_0011BC00(&out,&owner->matrix,&frame);relative=out;}
    func_0011BCF8(&relative,&angle,&axis.q);
    desired=func_0011CFE0((angle/stopAngle)*maximum,maximum);
    speed=GetX(DotQ(owner->velocity,axis.q));
    clock=&D_002DDED0;
    float half=clock->dt*0.5f;
    remaining=angle-(speed+desired)*half;
    if(remaining<stopAngle)desired=func_0011CFE0((remaining/stopAngle)*maximum,maximum);
    U scalar;
    qword w;
    {qword k=Scalar(desired,&scalar);w=Scale(axis.q,k);StoreQ(w,&wanted);}
    {qword v=owner->velocity;qword k=Scalar(clock->dt,&scalar);predicted.q=AddScaled(v,owner->force,k);}
    qword cv=Sub(w,predicted.q);
    change.q=cv;
    length=Length(cv);
    if(length<acceleration*clock->dt)gain=1.0f/clock->dt;
    else gain=acceleration/length;
    StoreQ(ScaleNew(cv,Scalar(gain,&scalar)),&change);
    owner->force=Add(owner->force,change.q);
}

typedef char check_matrix_size[(sizeof(M3)==0x30)?1:-1];
typedef char check_owner_parent[((unsigned)&((OWNER*)0)->parent==0xA0)?1:-1];
typedef char check_owner_velocity[((unsigned)&((OWNER*)0)->velocity==0xB0)?1:-1];
typedef char check_owner_force[((unsigned)&((OWNER*)0)->force==0xF0)?1:-1];
typedef char check_object_owner[((unsigned)&((P*)0)->owner==0x5C)?1:-1];
typedef char check_clock_dt[((unsigned)&((CLOCK*)0)->dt==4)?1:-1];
