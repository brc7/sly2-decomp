typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct ALO;
struct VENTRY {short delta,pad;void (*fn)(void *,qword);};
struct SPACE {char pad0[0x70];U position;char pad80[0x20];SPACE *parent;};
struct ALO {char pad0[0xC];VENTRY *vt;char pad10[0x48];SPACE *space;char pad5C[0xEC];unsigned long flags;char pad150[0x348];void *control;};
struct ASEG;
struct ASEGA {char pad0[0x40];float playbackRate;};
struct TUNING {int unused0;ASEG *animation;int unused8;float afterStart,afterEnd,flightTime,minimumTime;int unused1C;U acceleration;};
struct P {char pad0[0x10];int state;float stateTime;char pad18[8];U previousVelocity;TUNING *tuning;ALO *subject;ASEGA *job;int index;U target;float arrival,end;ALO *related;int kind;int ballistic,steep;void *constraint;};
struct CLOCK {float time,dt;};
struct GAME;
extern CLOCK D_002DDED0;
extern GAME D_003DD490;
void ApplyAsegCur(ASEG *,ALO *,float,float,int,ASEGA **);
extern "C" {
void SubscribeLoObject(ASEGA *,int);
qword GetSoDvGravity(ALO *);
int FIsBasicDerivedFrom__FP5BASIC3CID(ALO *,int);
void func_0020B210(ALO *);
void func_0020D178(void *);
float func_00196928(qword,qword,float,float,float,float);
qword func_0022FC48(SPACE *,SPACE *,qword);
qword ConvertXfVec(SPACE *,SPACE *,qword);
void *func_0012DE58(ALO *,SPACE *,int,qword,qword,float);
void RecalcSwOxfFilterForObject(GAME *,ALO *);
void SetSoDvGravity(ALO *,qword);
void func_00130D30(ASEGA *,int,float,float);
void func_0020B2A8(ALO *,ALO *,int);
void func_0020C760(P *);
void EnableStuckWatch(void *);
}
static inline qword Scalar(float f){qword r;__asm__("qmtc2 %1,%0":"=j"(r):"r"(f));return r;}
static inline qword Subtract(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Combine(qword a,qword x,qword b,qword y){qword r;__asm__("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x":"=j"(r):"j"(a),"j"(x),"j"(b),"j"(y));return r;}
static inline void Store(U *home,qword value){__asm__("sqc2 %1,%0":"=m"(home->q):"j"(value));}
static inline float Max(float a,float b){return a<b?b:a;}
static inline qword MotionScalar(U *home,float value){
    register qword vector __asm__("vf3");
    __asm__("qmtc2.ni %1,%0":"=j"(vector):"r"(value));
    home->q=vector;
    return home->q;
}
static inline qword MotionAcceleration(qword value,qword scalar){register qword result __asm__("vf2");__asm__("vmulx.xyzw %0,%1,%2x":"=j"(result):"j"(value),"j"(scalar));return result;}
extern "C" void func_0020C2F8(P *p,int state)
{
    U scalar,delta,acceleration,velocity,finalVelocity;
    if(state==p->state)return;
    p->state=state;
    p->stateTime=D_002DDED0.time;
    switch(state){
    case 0:
        ApplyAsegCur(p->tuning->animation,p->subject,0.0f,1.0f,p->index,&p->job);
        SubscribeLoObject(p->job,(int)p);
        p->previousVelocity.q=GetSoDvGravity(p->subject);
        break;
    case 1:{
        if(FIsBasicDerivedFrom__FP5BASIC3CID(p->subject,0x38)){
            ALO *subject=p->subject;
            func_0020B210(subject);
            func_0020D178(subject->control);
        }
        float playbackRate=p->job->playbackRate;
        register ALO *motionSubject __asm__("$3")=p->subject;
        register SPACE *motionSpace __asm__("$4");
        register TUNING *motionTuning __asm__("$3");
        register float square __asm__("$f0");
        __asm__("lw %0,%3\n\tmul.s %1,%4,%4\n\tlw %2,%5"
          :"=&r"(motionSpace),"=f"(square),"=r"(motionTuning)
          :"m"(motionSubject->space),"f"(playbackRate),"m"(p->tuning));
        register qword rateSquared __asm__("vf3");
        register int ballistic __asm__("$2");
        __asm__("lqc2 $vf4,%4\n\tlqc2 $vf1,%5\n\tmfc1 %0,%8\n\tqmtc2.ni %0,%1\n\tvmulx.xyzw $vf2,$vf1,%1x\n\tsqc2 $vf2,%2\n\tlqc2 $vf2,%6\n\tvsub.xyzw $vf1,$vf2,$vf4\n\tsqc2 $vf1,%3\n\tlw %0,%7"
          :"=&r"(ballistic),"=&j"(rateSquared),"=m"(acceleration.q),"=m"(delta.q)
          :"m"(motionSpace->position.q),"m"(motionTuning->acceleration.q),"m"(p->target.q),"m"(p->ballistic),"f"(square):"vf1","vf2","vf4");
        scalar.q=rateSquared;
        float time;
        if(ballistic){
            float low,high,slopeLow,slopeHigh;
            if(p->related&&(FIsBasicDerivedFrom__FP5BASIC3CID(p->related,0x13)||FIsBasicDerivedFrom__FP5BASIC3CID(p->related,0xB))){
                low=1250.0f;slopeLow=1.1f;high=0.0f;slopeHigh=50.0f;
            }else if(p->steep){
                low=1500.0f;slopeLow=1.33f;high=-1500.0f;slopeHigh=-1.33f;
            }else{
                low=1250.0f;slopeLow=1.1f;high=-1250.0f;slopeHigh=-1.1f;
            }
            time=func_00196928(delta.q,acceleration.q,low,high,slopeLow,slopeHigh);
        }else time=motionTuning->flightTime/playbackRate;
        time=Max(p->tuning->minimumTime/playbackRate,time);
        ALO *subject=p->subject;
        VENTRY &entry=subject->vt[35];
        short adjustment=entry.delta;
        void (*applyVelocity)(void *,qword)=entry.fn;
        scalar.q=Scalar(1.0f/time);
        qword inverseTime=scalar.q;
        scalar.q=Scalar(-0.5f*time);
        qword initialVelocity=Combine(delta.q,inverseTime,acceleration.q,scalar.q);
        Store(&velocity,initialVelocity);
        applyVelocity((char *)subject+adjustment,velocity.q);
        p->arrival=D_002DDED0.time+time;
        p->end=p->arrival-p->tuning->minimumTime/playbackRate;
        if(p->ballistic){
            register qword timeScalar __asm__("vf1"),motionVelocity __asm__("vf2"),force __asm__("vf3");
            register ALO *launchSubject __asm__("$3");
            register qword target __asm__("$6");
            __asm__("qmtc2.ni %4,%0\n\tlqc2 %1,%5\n\tlqc2 %2,%6\n\tlw %3,%7"
              :"=j"(timeScalar),"=j"(motionVelocity),"=j"(force),"=r"(launchSubject)
              :"r"(time),"m"(velocity.q),"m"(acceleration.q),"m"(p->subject));
            __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%2,%3x\n\tlq %1,%4"
              :"+j"(motionVelocity),"=r"(target):"j"(force),"j"(timeScalar),"m"(p->target.q));
            register SPACE *parentSpace __asm__("$2");
            __asm__("sqc2 %4,%0\n\tlw %1,%5\n\tsqc2 %3,%2"
              :"=m"(finalVelocity.q),"=r"(parentSpace),"=m"(scalar.q)
              :"j"(timeScalar),"j"(motionVelocity),"m"(launchSubject->space));
            qword position=func_0022FC48(0,parentSpace->parent,target);
            qword speed=ConvertXfVec(0,p->subject->space->parent,finalVelocity.q);
            p->constraint=func_0012DE58(p->subject,p->subject->space,5,position,speed,p->arrival);
            p->subject->flags|=0x04000000ul;
            RecalcSwOxfFilterForObject(&D_003DD490,p->subject);
        }
        SetSoDvGravity(p->subject,acceleration.q);
        break;
    }
    case 2:func_00130D30(p->job,0,p->tuning->afterStart,1.0f);break;
    case 3:
        func_00130D30(p->job,0,p->tuning->afterEnd,1.0f);
        if(p->related)func_0020B2A8(p->subject,p->related,p->kind);
        break;
    case 4:
        if(p->ballistic)func_0020C760(p);
        SetSoDvGravity(p->subject,p->previousVelocity.q);
        if(FIsBasicDerivedFrom__FP5BASIC3CID(p->subject,0x38))EnableStuckWatch(p->subject->control);
        break;
    }
}

#define FIELD_OFFSET(t,m) (((unsigned int)&(((t*)0x10000)->m))-0x10000)
typedef char int_Width[(sizeof(int)==4)?1:-1];
typedef char float_Width[(sizeof(float)==4)?1:-1];
typedef char pointer_Width[(sizeof(void*)==4)?1:-1];
typedef char long_Width[(sizeof(unsigned long)==8)?1:-1];
typedef char qword_Width[(sizeof(qword)==16)?1:-1];
typedef char qword_Alignment[(__alignof__(qword)==16)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char U_Alignment[(__alignof__(U)==16)?1:-1];
typedef char VENTRY_Prefix_Size[(sizeof(VENTRY)==0x8)?1:-1];
typedef char VENTRY_delta_Offset[(FIELD_OFFSET(VENTRY,delta)==0x0)?1:-1];
typedef char VENTRY_pad_Offset[(FIELD_OFFSET(VENTRY,pad)==0x2)?1:-1];
typedef char VENTRY_fn_Offset[(FIELD_OFFSET(VENTRY,fn)==0x4)?1:-1];
typedef char SPACE_Prefix_Size[(sizeof(SPACE)==0xB0)?1:-1];
typedef char SPACE_position_Offset[(FIELD_OFFSET(SPACE,position)==0x70)?1:-1];
typedef char SPACE_parent_Offset[(FIELD_OFFSET(SPACE,parent)==0xA0)?1:-1];
typedef char ALO_Prefix_Size[(sizeof(ALO)==0x4A0)?1:-1];
typedef char ALO_vt_Offset[(FIELD_OFFSET(ALO,vt)==0xC)?1:-1];
typedef char ALO_space_Offset[(FIELD_OFFSET(ALO,space)==0x58)?1:-1];
typedef char ALO_flags_Offset[(FIELD_OFFSET(ALO,flags)==0x148)?1:-1];
typedef char ALO_control_Offset[(FIELD_OFFSET(ALO,control)==0x498)?1:-1];
typedef char ASEGA_Prefix_Size[(sizeof(ASEGA)==0x44)?1:-1];
typedef char ASEGA_playbackRate_Offset[(FIELD_OFFSET(ASEGA,playbackRate)==0x40)?1:-1];
typedef char TUNING_Prefix_Size[(sizeof(TUNING)==0x30)?1:-1];
typedef char TUNING_animation_Offset[(FIELD_OFFSET(TUNING,animation)==0x4)?1:-1];
typedef char TUNING_afterStart_Offset[(FIELD_OFFSET(TUNING,afterStart)==0xC)?1:-1];
typedef char TUNING_afterEnd_Offset[(FIELD_OFFSET(TUNING,afterEnd)==0x10)?1:-1];
typedef char TUNING_flightTime_Offset[(FIELD_OFFSET(TUNING,flightTime)==0x14)?1:-1];
typedef char TUNING_minimumTime_Offset[(FIELD_OFFSET(TUNING,minimumTime)==0x18)?1:-1];
typedef char TUNING_acceleration_Offset[(FIELD_OFFSET(TUNING,acceleration)==0x20)?1:-1];
typedef char P_Prefix_Size[(sizeof(P)==0x70)?1:-1];
typedef char P_state_Offset[(FIELD_OFFSET(P,state)==0x10)?1:-1];
typedef char P_stateTime_Offset[(FIELD_OFFSET(P,stateTime)==0x14)?1:-1];
typedef char P_previousVelocity_Offset[(FIELD_OFFSET(P,previousVelocity)==0x20)?1:-1];
typedef char P_tuning_Offset[(FIELD_OFFSET(P,tuning)==0x30)?1:-1];
typedef char P_subject_Offset[(FIELD_OFFSET(P,subject)==0x34)?1:-1];
typedef char P_job_Offset[(FIELD_OFFSET(P,job)==0x38)?1:-1];
typedef char P_index_Offset[(FIELD_OFFSET(P,index)==0x3C)?1:-1];
typedef char P_target_Offset[(FIELD_OFFSET(P,target)==0x40)?1:-1];
typedef char P_arrival_Offset[(FIELD_OFFSET(P,arrival)==0x50)?1:-1];
typedef char P_end_Offset[(FIELD_OFFSET(P,end)==0x54)?1:-1];
typedef char P_related_Offset[(FIELD_OFFSET(P,related)==0x58)?1:-1];
typedef char P_kind_Offset[(FIELD_OFFSET(P,kind)==0x5C)?1:-1];
typedef char P_ballistic_Offset[(FIELD_OFFSET(P,ballistic)==0x60)?1:-1];
typedef char P_steep_Offset[(FIELD_OFFSET(P,steep)==0x64)?1:-1];
typedef char P_constraint_Offset[(FIELD_OFFSET(P,constraint)==0x68)?1:-1];
typedef char CLOCK_Prefix_Size[(sizeof(CLOCK)==0x8)?1:-1];
typedef char CLOCK_time_Offset[(FIELD_OFFSET(CLOCK,time)==0x0)?1:-1];
typedef char CLOCK_dt_Offset[(FIELD_OFFSET(CLOCK,dt)==0x4)?1:-1];
