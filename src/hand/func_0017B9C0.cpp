#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct RANGE{float lo,hi;};
struct POLY{float a,b,c;};
struct P{char pad[0x28];unsigned long flags;char pad30[0xF94-0x30];int count;P*choices[4];P*Choice(int i){return choices[i];}int nearCount;P*near[0];};
struct C{char pad[0x10];P*actor;char pad14[0x38-0x14];P*preferred;P*near;P*selected;float time;};
struct WORK{float t,d,t2,d2;U origin,bestPoint;};
struct CLOCK{float time,dt;};
extern "C"{
 extern CLOCK D_002DDED0;
 extern RANGE D_002BE740;
 extern POLY D_002BE720,D_002BE730;
 float GRandInRange__Fff(float,float);
 qword func_001A62E8(P*);
 void FindClosestPointOnLine(qword,qword,qword,qword*,float*,float*);
}
static inline unsigned long Eligible(unsigned long flags){return (flags>>60)&1;}
static inline qword From(qword a){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(a));return r;}
static inline qword Load(U*u){qword r;__asm__("lqc2 %0,%1":"=j"(r):"m"(u->q));return r;}
static inline float X(qword q){float f;__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(q));return f;}
static inline float Length(qword v){register qword one __asm__("vf2"); qword d=v;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d),"=&j"(one):"j"(v));
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));return X(d);
}
static inline float Distance(U*origin,qword point){register qword a __asm__("vf2")=origin->q;qword b=point,d;__asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));return Length(d);}
static inline float DistanceFrom(qword point,U*origin){qword a=point,b=Load(origin),d;__asm__("vsub.xyzw %0,%1,%2":"=&j"(d):"j"(a),"j"(b));return Length(d);}
static inline qword To(qword a){return a;}
static inline void Store(U*u,qword a){__asm__("sqc2 %1,%0":"=m"(u->q):"j"(a));}
static inline void ClampIn(float*x,RANGE*p){float lo=p->lo,hi=p->hi;if(*x<lo)*x=lo;else if(hi<*x)*x=hi;}
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char POLY_Size[sizeof(POLY)==12?1:-1];
typedef char RANGE_Size[sizeof(RANGE)==8?1:-1];
typedef char WORK_Size[sizeof(WORK)==48?1:-1];
typedef char P_flags_Offset[FIELD_OFFSET(P,flags)==40?1:-1];
typedef char P_count_Offset[FIELD_OFFSET(P,count)==3988?1:-1];
typedef char P_choices_Offset[FIELD_OFFSET(P,choices)==3992?1:-1];
typedef char P_nearCount_Offset[FIELD_OFFSET(P,nearCount)==4008?1:-1];
typedef char P_near_Offset[FIELD_OFFSET(P,near)==4012?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==16?1:-1];
typedef char C_preferred_Offset[FIELD_OFFSET(C,preferred)==56?1:-1];
typedef char C_near_Offset[FIELD_OFFSET(C,near)==60?1:-1];
typedef char C_selected_Offset[FIELD_OFFSET(C,selected)==64?1:-1];
typedef char C_time_Offset[FIELD_OFFSET(C,time)==68?1:-1];
typedef char WORK_t_Offset[FIELD_OFFSET(WORK,t)==0?1:-1];
typedef char WORK_d_Offset[FIELD_OFFSET(WORK,d)==4?1:-1];
typedef char WORK_t2_Offset[FIELD_OFFSET(WORK,t2)==8?1:-1];
typedef char WORK_d2_Offset[FIELD_OFFSET(WORK,d2)==12?1:-1];
typedef char WORK_origin_Offset[FIELD_OFFSET(WORK,origin)==16?1:-1];
typedef char WORK_bestPoint_Offset[FIELD_OFFSET(WORK,bestPoint)==32?1:-1];
typedef char POLY_a_Offset[FIELD_OFFSET(POLY,a)==0?1:-1];
typedef char POLY_b_Offset[FIELD_OFFSET(POLY,b)==4?1:-1];
typedef char POLY_c_Offset[FIELD_OFFSET(POLY,c)==8?1:-1];
typedef char RANGE_lo_Offset[FIELD_OFFSET(RANGE,lo)==0?1:-1];
typedef char RANGE_hi_Offset[FIELD_OFFSET(RANGE,hi)==4?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
extern "C" void func_0017B9C0(C*p){
 WORK work;
 P*best;
 float lower;
 qword zero;
 float now=D_002DDED0.time;
 if(now<p->time)return;
 // D_002BE710 holds the random-delay range.
 register unsigned int randomAddress __asm__("$2");
 __asm__("vsub.xyzw %0,$vf0,$vf0\n\tlui %1,0x2c":"=j"(zero),"=r"(randomAddress));
 __asm__("sqc2 %4,%0\n\tdaddu %2,%z5,$zero\n\tlwc1 %1,-0x18f0(%3)\n\taddiu %3,%3,-0x18f0"
   :"=m"(work.bestPoint.q),"=f"(lower),"=&r"(best),"+r"(randomAddress)
   :"j"(zero),"J"(0),"m"(*(float*)(randomAddress-0x18f0)));
 p->time=now+GRandInRange__Fff(lower,((RANGE*)randomAddress)->hi);
 work.origin.q=func_001A62E8(p->actor);
 // D_002A55FC supplies the initial selection cost.
 int i;P*actor;unsigned int limitHigh;
 {register int firstIndex __asm__("$5");register P*firstActor __asm__("$4");register unsigned int high __asm__("$3");
  __asm__("lui %0,0x2a\n\tdaddu %1,$zero,$zero\n\tlw %2,%3"
    :"=&r"(high),"=&r"(firstIndex),"=r"(firstActor):"m"(p->actor));
  limitHigh=high;i=firstIndex;actor=firstActor;
 }
 float bestCost=*(float*)(limitHigh+0x55fc);
 if(actor->count>0)do{
  P*choice=actor->Choice(i);
  if(Eligible(choice->flags)){
  qword candidate=From(func_001A62E8(choice));
  float cost=Distance(&work.origin,candidate);
  if(choice==p->preferred)cost=D_002BE720.a+cost*(D_002BE720.b+cost*D_002BE720.c);
  for(int j=0;j<p->actor->nearCount;j++){
   qword position=func_001A62E8(p->actor->near[j]);
   FindClosestPointOnLine(position,work.origin.q,To(candidate),0,&work.t,&work.d);
   if(work.t<0.0f)continue;
   if(work.t>1.0f)continue;
   float penalty=D_002BE730.a+work.d*(D_002BE730.b+work.d*D_002BE730.c);
   ClampIn(&penalty,&D_002BE740);cost+=penalty;
  }
  if(cost<bestCost){Store(&work.bestPoint,candidate);bestCost=cost;best=choice;}
  }
  ++i;actor=p->actor;
 }while(i<actor->count);
 p->preferred=best;
 P*nearest=0;float distance=800.0f;
 for(int k=0;k<p->actor->nearCount;k++){
  P*choice=p->actor->near[k];
  qword position=From(func_001A62E8(choice));
  float f=DistanceFrom(position,&work.origin);
  if(f>distance)continue;
  if(best){
   FindClosestPointOnLine(To(position),work.origin.q,work.bestPoint.q,0,&work.t2,&work.d2);
   if(work.t2<0.0f)continue;
   if(work.t2>1.0f)continue;
   if(work.d2>500.0f)continue;
  }
  nearest=choice;distance=f;
 }
 p->near=nearest;p->selected=nearest?nearest:p->preferred;
}

typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
typedef char UInt_Size[sizeof(unsigned int)==4?1:-1];
typedef char Float_Size[sizeof(float)==4?1:-1];
typedef char Long_Size[sizeof(unsigned long)==8?1:-1];
typedef char Qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char P_View_Size[sizeof(P)==0xfb0?1:-1];
typedef char P_Alignment[__alignof__(P)==8?1:-1];
typedef char C_Size[sizeof(C)==0x48?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
