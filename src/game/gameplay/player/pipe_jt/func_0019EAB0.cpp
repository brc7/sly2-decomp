typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};
struct VE{short delta,pad;void*fn;};
struct ASEG{char pad[0x74];float duration;};
struct ASEGA;struct COMPONENT;struct EFFECT;
struct P{char pad0[12];VE*vt;char pad10[0x190-0x10];U velocity;char pad1a0[0x1010-0x1A0];U offset;char pad1020[0x10];ASEGA*asega;COMPONENT*component;char pad1038[0x1060-0x1038];U goal,motion;char pad1080[0x14D4-0x1080];int alternate;};
struct PATH{char pad0[12];VE*vt;char pad10[0x190-0x10];void*focus;char pad194[0x1D0-0x194];int attachEffectId;int releaseEffectId;};
struct C{P*actor;int state;float time;PATH*path;float distance;int pad14;int descending;float reset;int previous;char pad24[12];U offset;char pad40[0x30];ASEG*clip2;ASEG*clip0;ASEG*clip1;ASEG*fallback;};
struct LM{float low,high;};
struct WORK{U ahead,behind;LM limit;U point,world,target;};
struct CLOCK{float t,dt;};
extern "C"{
extern CLOCK D_002DDED0;extern U D_002A2DC0;extern P*D_002E1E40;
void SubscribeLoObject(PATH*,P*);void UnsubscribeLoObject(PATH*,P*);
void func_00204F78(P*,PATH*,int);
float SClampShape(PATH*,float);void EvaluateShapeS(PATH*,float,U*,U*);
void func_0019E110(C*,LM*);float func_0011D948(LM*,float);
qword ConvertAloPos(P*,P*,qword);qword func_001A62E8(P*);
void SetStepplayerTnOverride(P*,void*);
EFFECT*PsndStartSound(int);void SetSndSourceLo(EFFECT*,void*);void SetSndSourcePos(EFFECT*,qword);
void SetJtJtcs__FP2JTi(P*,int);void ReleaseSma(COMPONENT**);
void func_001305B8(ASEG*,P*,float,float,int,ASEGA**);
}
static inline void PathState(PATH*p,int state){VE&v=p->vt[3];((void(*)(void*,int,void*,void*,void*,void*))v.fn)((char*)p+v.delta,state,0,0,0,0);}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Vector(qword q){qword v;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(q));return v;}
static inline qword Direction(U*a,U*b,U*&axis){
 register qword first __asm__("vf2"),second __asm__("vf1"),direction __asm__("vf3");register U*address __asm__("$2");
 __asm__("lqc2 %0,%4\n\tlui %2,%%hi(%6)\n\tlqc2 %1,%5\n\taddiu %2,%2,%%lo(%6)\n\tvsub.xyzw %3,%0,%1":"=j"(first),"=j"(second),"=&r"(address),"=j"(direction):"m"(a->q),"m"(b->q),"i"(&D_002A2DC0));
 axis=address;return direction;
}
static inline qword Normal(qword v,U*axis){
 register qword d __asm__("vf1"),one __asm__("vf2"),r __asm__("vf2");float length;register int bits __asm__("$3");
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(v));
 register qword fallback __asm__("$2")=axis->q;
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
 __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(d));
 __asm__("mtc1 %1,%0":"=f"(length):"r"(bits));
 if(length<0.0001f)return fallback;
 __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(d),"j"(v));
 qword q;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(r));return q;
}
static inline qword GoalInputs(U*target,qword position,void*&focus){
 register qword a __asm__("vf2"),b __asm__("vf3"),r __asm__("vf1");register void*nullFocus __asm__("$5");
 __asm__("lqc2 %0,%3\n\tdaddu %2,$zero,$zero\n\tqmtc2.ni %4,%1":"=j"(a),"=j"(b),"=&r"(nullFocus):"m"(target->q),"r"(position));
 focus=nullFocus;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;
}
static inline qword WorldInputs(C*p,U*point,U*world,qword offset,P*&owner){
 register P*actor __asm__("$2");register qword value __asm__("vf1");
 __asm__("lw %0,%3\n\tlqc2 %1,%4\n\tsqc2 %5,%2":"=&r"(actor),"=&j"(value),"=m"(world->q):"m"(p->actor),"m"(point->q),"j"(offset));
 owner=actor;return value;
}
static inline float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static inline void Clear(U*v){v->i[0]=v->i[1]=v->i[2]=0;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char WORK_Size[sizeof(WORK)==96?1:-1];
typedef char LM_Size[sizeof(LM)==8?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==0?1:-1];
typedef char C_state_Offset[FIELD_OFFSET(C,state)==4?1:-1];
typedef char C_time_Offset[FIELD_OFFSET(C,time)==8?1:-1];
typedef char C_path_Offset[FIELD_OFFSET(C,path)==12?1:-1];
typedef char C_distance_Offset[FIELD_OFFSET(C,distance)==16?1:-1];
typedef char C_descending_Offset[FIELD_OFFSET(C,descending)==24?1:-1];
typedef char C_reset_Offset[FIELD_OFFSET(C,reset)==28?1:-1];
typedef char C_previous_Offset[FIELD_OFFSET(C,previous)==32?1:-1];
typedef char C_offset_Offset[FIELD_OFFSET(C,offset)==48?1:-1];
typedef char C_clip2_Offset[FIELD_OFFSET(C,clip2)==112?1:-1];
typedef char C_clip0_Offset[FIELD_OFFSET(C,clip0)==116?1:-1];
typedef char C_clip1_Offset[FIELD_OFFSET(C,clip1)==120?1:-1];
typedef char C_fallback_Offset[FIELD_OFFSET(C,fallback)==124?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_velocity_Offset[FIELD_OFFSET(P,velocity)==400?1:-1];
typedef char P_offset_Offset[FIELD_OFFSET(P,offset)==4112?1:-1];
typedef char P_asega_Offset[FIELD_OFFSET(P,asega)==4144?1:-1];
typedef char P_component_Offset[FIELD_OFFSET(P,component)==4148?1:-1];
typedef char P_goal_Offset[FIELD_OFFSET(P,goal)==4192?1:-1];
typedef char P_motion_Offset[FIELD_OFFSET(P,motion)==4208?1:-1];
typedef char P_alternate_Offset[FIELD_OFFSET(P,alternate)==5332?1:-1];
typedef char PATH_vt_Offset[FIELD_OFFSET(PATH,vt)==12?1:-1];
typedef char PATH_focus_Offset[FIELD_OFFSET(PATH,focus)==400?1:-1];
typedef char PATH_attachEffectId_Offset[FIELD_OFFSET(PATH,attachEffectId)==464?1:-1];
typedef char PATH_releaseEffectId_Offset[FIELD_OFFSET(PATH,releaseEffectId)==468?1:-1];
typedef char ASEG_duration_Offset[FIELD_OFFSET(ASEG,duration)==116?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_fn_Offset[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char WORK_ahead_Offset[FIELD_OFFSET(WORK,ahead)==0?1:-1];
typedef char WORK_behind_Offset[FIELD_OFFSET(WORK,behind)==16?1:-1];
typedef char WORK_limit_Offset[FIELD_OFFSET(WORK,limit)==32?1:-1];
typedef char WORK_point_Offset[FIELD_OFFSET(WORK,point)==48?1:-1];
typedef char WORK_world_Offset[FIELD_OFFSET(WORK,world)==64?1:-1];
typedef char WORK_target_Offset[FIELD_OFFSET(WORK,target)==80?1:-1];
typedef char LM_low_Offset[FIELD_OFFSET(LM,low)==0?1:-1];
typedef char LM_high_Offset[FIELD_OFFSET(LM,high)==4?1:-1];
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
/** @sly2-annot addr=0x19EAB0
 * Purpose: Pipe sub-state switch: from -1 mount (subscribe, register, find curve s +-50), per-state
 *   clips/sounds, cane reach via SetJtJtcs
 * Info: proposed name SetJtPipeState (T1); TU pipe_jt
 * Notes: names_v2 C proposed UpdateSuvSounds__FP3SUVf (0.200)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
extern "C" void func_0019EAB0(C*p,int next){
 int old=p->state;if(next==old)return;
 WORK w;
 switch(old){
 case -1:{
  PathState(p->path,5);SubscribeLoObject(p->path,p->actor);func_00204F78(p->actor,p->path,1);
  p->previous=old;
  EvaluateShapeS(p->path,SClampShape(p->path,p->distance+50.0f),&w.ahead,0);
  EvaluateShapeS(p->path,SClampShape(p->path,p->distance-50.0f),&w.behind,0);
  U*axis; qword direction=Direction(&w.ahead,&w.behind,axis);
  p->descending=Z(Normal(direction,axis))<-0.5f;
  func_0019E110(p,&w.limit);p->distance=func_0011D948(&w.limit,p->distance);
  EvaluateShapeS(p->path,p->distance,&w.point,0);
  qword offset=Vector(ConvertAloPos(p->actor,0,p->offset.q));P*owner;
  qword point=WorldInputs(p,&w.point,&w.world,offset,owner);
  owner->offset.q=Sub(point,offset);
  p->actor->motion.q=Sub(p->actor->goal.q,w.point.q);
  p->actor->motion.i[2]=0;
  SetStepplayerTnOverride(p->actor,p->path->focus);
  if(p->path->attachEffectId){EFFECT*e=PsndStartSound(p->path->attachEffectId);SetSndSourceLo(e,0);SetSndSourcePos(e,w.point.q);}
  break;
 }
 case 0:if(p->actor==D_002E1E40)SetJtJtcs__FP2JTi(p->actor,p->actor->alternate?10:2);break;
 }
 p->state=next;p->time=D_002DDED0.t;
 ASEG*clip=0;float position=0.0f,speed=1.0f;int flags=1;
 switch(next){
 case -1:{
  PathState(p->path,6);UnsubscribeLoObject(p->path,p->actor);func_00204F78(p->actor,p->path,0);
  P*actor=p->actor;w.target=actor->goal;
  void*focus;actor->motion.q=GoalInputs(&w.target,func_001A62E8(actor),focus);
  p->actor->motion.i[2]=0;SetStepplayerTnOverride(p->actor,focus);
  if(p->path->releaseEffectId){EFFECT*e=PsndStartSound(p->path->releaseEffectId);SetSndSourceLo(e,0);SetSndSourcePos(e,func_001A62E8(p->actor));}
  p->path=0;return;
 }
 case 0:
  clip=p->clip0;
  if(!clip){clip=p->fallback;register float half __asm__("$f0")=0.5f;position=clip->duration*half;speed=half;}
  p->reset=0;break;
 case 1:clip=p->clip1;flags=9;break;
 case 2:clip=p->clip2;flags=9;break;
 case 3:clip=p->clip2;speed=-1.0f;flags=9;break;
 case 4:p->descending=!p->descending;clip=p->fallback;break;
 }
 ReleaseSma(&p->actor->component);
 func_001305B8(clip,p->actor,position,speed,flags,&p->actor->asega);
 Clear(&p->actor->velocity);
}
