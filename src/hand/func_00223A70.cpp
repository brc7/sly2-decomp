/* (Claude agent) MATCH: the loop's func_0011E1A0 result is a plain qword local; gcc emits the qmtc2 itself and schedules it. */
// Complete camera segment avoidance WIP. FRAME has no instance members.
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct O;
struct SO;
struct N;
struct S {O *object;char pad[0x30];int state;};
struct CAMERA {char pad[0x48];float radius;};
struct WORLD;
extern CAMERA D_002DDF40;
extern WORLD D_003DD490;
extern float D_002A9700;
extern "C" {
void *func_001AE478(void *);
void func_001AE4A0(void *,int);
qword func_001A62E8(void *);
int func_0014F030(void *,SO *);
void func_0020FAA0(WORLD *,qword,qword,int (*)(void *,SO *),void *,int *,SO ***);
qword func_0014EB70(int,SO **,qword,float);
int func_0014EA08(int count,SO **objects,qword origin,qword endpoint,float gap,float *low,float *high);
float func_00223A10(void *,qword,qword,N *);
qword func_0014EDA0(CAMERA *,int,SO **,float (*)(void *,qword,qword,N *),qword,qword,float,float);
void func_0011EF08(qword,float *,float *,float *);
qword func_0011E1A0(float,float,float);
}
struct MARK {MARK(){(void)func_001AE478(this);}~MARK(){func_001AE4A0(this,2);}};
struct WORK {V minimum,maximum,origin,candidate;MARK mark;V offset,rayEnd,best,scalar;int count;SO **objects;float low,high,phase,radius,height;};
static inline qword Min(qword a,qword b){qword r;__asm__("vmini.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Max(qword a,qword b){qword r;__asm__("vmax.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%0,%1":"+j"(a):"j"(b));return a;}
static inline qword Add(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword ToVF(qword q){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(q));return r;}
static inline qword Scalar(float f){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));return r;}
static inline qword ScalarHome(V &home,float value){home.q=Scalar(value);return home.q;}
static inline qword Blend(qword a,qword b,qword left,qword right){__asm__("vmulax.xyzw ACC,%0,%2x\n\tvmaddx.xyzw %0,%1,%3x":"+j"(a):"j"(b),"j"(left),"j"(right));return a;}
static inline qword BlendToRight(qword a,qword b,qword left,qword right){__asm__("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%0,%3x":"+j"(b):"j"(a),"j"(left),"j"(right));return b;}
static inline void StoreVF(qword *p,qword q){__asm__("sqc2 %1,%0":"=m"(*p):"j"(q));}
static inline float Abs(float f){return __builtin_fabsf(f);}
/** @sly2-annot addr=0x223A70
 * Purpose: camera segment avoidance (objects in box, UpdateCmFade)
 * Info: proposed name UpdateCptnSegmentAvoid (T2); TU tn
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
extern "C" void func_00223A70(S *p,qword begin,qword end,qword *out){
    O *object=p->object;
    WORK w;
    qword final;
    float initialAngle=0.0f;
    float one=1.0f;
    w.minimum.q=Min(begin,Min(end,func_001A62E8(object)));
    w.maximum.q=Max(begin,Max(end,func_001A62E8(object)));
    func_0020FAA0(&D_003DD490,w.minimum.q,w.maximum.q,func_0014F030,0,&w.count,&w.objects);
    int count=w.count; SO **objects=w.objects;
    w.origin.q=func_0014EB70(count,objects,func_001A62E8(object),500.0f);
    func_0014EA08(w.count,w.objects,w.origin.q,end,initialAngle,&w.low,&w.high);
    if(w.high<one){
        float angle=initialAngle;
        p->state=4;
        func_0011EF08(Sub(end,begin),&w.phase,&w.radius,&w.height);
        float bestCost=D_002A9700;
        float minGap=initialAngle;
        do{
            qword off=func_0011E1A0(w.phase+angle,w.radius,w.height);
            w.offset.q=off;
            w.rayEnd.q=Add(begin,off);
            func_0014EA08(w.count,w.objects,w.origin.q,w.rayEnd.q,minGap,&w.low,&w.high);
            float cost=(1.25f-w.high)*(Abs(angle)+0.47123894f);
            if(cost<bestCost){
                float left=1.0f-w.high;
                bestCost=cost;
                qword a=ScalarHome(w.scalar,left);
                qword right=ScalarHome(w.scalar,w.high);
                w.best.q=Blend(w.origin.q,w.rayEnd.q,a,right);
            }
            float next=-angle;
            if(angle<=minGap)next+=0.28559935f;
            angle=next;
            if(bestCost<(Abs(next)+0.47123894f)*0.25f)break;
        }while(angle<3.1415927f);
        w.candidate.q=w.best.q;
    }else w.candidate.q=end;
    final=ToVF(func_0014EDA0(&D_002DDF40,w.count,w.objects,func_00223A10,w.candidate.q,w.candidate.q,D_002DDF40.radius,D_002DDF40.radius));
    func_0014EA08(w.count,w.objects,w.candidate.q,final,0.0f,&w.low,&w.high);
    if(w.high!=1.0f){
        float u=(w.low+w.high)*0.5f;
        qword left=ScalarHome(w.offset,1.0f-u);
        qword right=ScalarHome(w.offset,u);
        final=BlendToRight(w.candidate.q,final,left,right);
    }
    StoreVF(out,final);
}
