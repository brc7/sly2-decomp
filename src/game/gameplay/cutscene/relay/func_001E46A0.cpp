typedef int qword __attribute__((mode(TI)));
typedef unsigned int uint;
union U {qword q;float f[4];};
union ARG {int id;float gain;};
struct PATH {char padE4[0xE4];int cyclic;};
struct PARAM {int kind;char pad4[0xC];int target;ARG value;char pad18[0xC];int flag;};
struct ACTOR {char padC[0xC];void *vtable;char pad10[0x4C8-0x10];int mode,active;int word4D0;PATH *motion;int word4D8;float motionTime;char pad4E0[0xA7C];PATH *curve;float curveTime;char padF64[0xA8C];qword oldPosition;char pad1A00[0x20];qword position;};
struct P {char pad24[0x24];ACTOR *overrideActor;char pad28[0x40-0x28];PARAM *param;ACTOR *actor;int blocked;char pad4C[0x2C];float previousMotion,previousCurve;};
struct OUTPUT {char pad10[0x10];float x,y,heading,speed;char pad20[0x10];uint flags;char pad34[0xC];qword basis[3];};
struct WORK {U from,to,normal;float heading;qword first,second;};
extern "C" {
extern qword D_002A2DA0;
extern float D_002C3630[3],D_002BBB60[2],D_002C3640;
float SClampShape(PATH*,float);
void EvaluateShapeS(PATH*,qword*,qword*,float);
qword func_001C4768(OUTPUT*,qword);
float func_002458F0(float,float);
float SMaxShape(PATH*),UMaxShape(PATH*);
float RadNormalize__Ff(float);
ACTOR *func_001E1DB8(P*,int);
void func_001E4D40(P*,ACTOR*,qword*,float*);
void func_00204660(ACTOR*,float);
void EvaluateShapeU(PATH*,float,qword*,qword*);
qword func_001A62E8(ACTOR*);
qword ConvertAloVec(void*,ACTOR*,qword);
void UpdatePoctlPressed(OUTPUT*);
int func_0011D4D0(float,float,float);
}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword ToVU(qword a){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(a));return r;}
static inline float X(qword a){float r;__asm__("mtc1 %1,%0":"=f"(r):"r"(a));return r;}
static inline float Y(qword a){float r;__asm__("prot3w %0,%1":"=r"(r):"r"(a));return r;}
static inline float Z(qword a){float r;__asm__("pextuw %0,$zero,%1":"=r"(r):"r"(a));return r;}
static inline qword Root(qword a){__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(a));return a;}
static inline float LengthXY(qword a){qword r;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(r):"j"(a));return X(Root(r));}
static inline void NormalizeTangent(U &output){
 register qword vector __asm__("vf3");
 register qword length __asm__("vf1");
 register qword one __asm__("vf2");
 qword alternative;
 register const qword *fallbackAddress __asm__("$2");
 register float epsilon __asm__("$f1");
 const int zero=0;const float threshold=0.0001f;
 __asm__("sw %z3,%0\n\tlui %1,%%hi(%5)\n\tli.s %2,%4\n\taddiu %1,%1,%%lo(%5)"
  :"=m"(output.f[2]),"=r"(fallbackAddress),"=f"(epsilon):"rJ"(zero),"F"(threshold),"i"(&D_002A2DA0));
 register int word __asm__("$3");
 register float value __asm__("$f0");
 __asm__("lqc2 %0,%4\n\tvmul.xyz %1,%0,%0\n\tvaddw.x %2,$vf0,$vf0w\n\tvadday.x $ACC,%1,%1y\n\tvmaddz.x %1,%2,%1z\n\tlq %3,0(%3)":"=j"(vector),"=j"(length),"=j"(one),"=r"(alternative):"m"(output.q),"3"(fallbackAddress),"m"(*fallbackAddress));
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(length));
 __asm__("qmfc2.ni %0,%1":"=r"(word):"j"(length));
 __asm__("mtc1 %1,%0":"=f"(value):"r"(word));
 if(value<epsilon){length=alternative;}
 else {__asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"+j"(length):"j"(vector));}
 __asm__("sqc2 %1,%0":"=m"(output.q):"j"(length));
}
static inline void TargetDisplacement(U &output,const qword &captured,qword actorPosition){
 register qword current __asm__("vf3");
 register qword target __asm__("vf2");
 register qword delta __asm__("vf1");
 __asm__("qmtc2.ni %1,%0":"=j"(current):"r"(actorPosition));
 __asm__("lqc2 %0,%1":"=j"(target):"m"(captured));
 __asm__("vsub.xyzw %0,%1,%2":"=j"(delta):"j"(target),"j"(current));
 __asm__("sqc2 %1,%0":"=m"(output.q):"j"(delta));
}
static inline qword NormalizeDirection(qword input,float &speed,const float &sourceSpeed){
 register qword vector __asm__("vf2");
 register qword length __asm__("vf1");
 register qword one __asm__("vf3");
 register float scale __asm__("$f2");
 __asm__("qmtc2.ni %2,%0\n\tlwc1 %1,%3":"=j"(vector),"=f"(scale):"r"(input),"m"(sourceSpeed));
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(length),"=&j"(one):"j"(vector));
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(length));
 __asm__("vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q":"+j"(length):"j"(vector));
 speed=scale;return length;
}
static inline int NearYInputs(qword previous,qword current,float tolerance){
 register qword currentWord __asm__("$2"),previousWord __asm__("$3");
 register float currentInput __asm__("$f12"),previousInput __asm__("$f13");
 __asm__("prot3w %0,%1":"=r"(currentWord):"r"(current));
 __asm__("prot3w %0,%1":"=r"(previousWord):"r"(previous));
 __asm__("mtc1 %1,%0":"=f"(currentInput):"r"(currentWord));
 __asm__("mtc1 %1,%0":"=f"(previousInput):"r"(previousWord));
 return func_0011D4D0(currentInput,previousInput,tolerance);
}
static inline int NearZInputs(qword previous,qword current,float tolerance){
 register qword currentWord __asm__("$2"),previousWord __asm__("$3");
 register float currentInput __asm__("$f12"),previousInput __asm__("$f13");
 __asm__("pextuw %0,$zero,%1":"=r"(currentWord):"r"(current));
 __asm__("pextuw %0,$zero,%1":"=r"(previousWord):"r"(previous));
 __asm__("mtc1 %1,%0":"=f"(currentInput):"r"(currentWord));
 __asm__("mtc1 %1,%0":"=f"(previousInput):"r"(previousWord));
 return func_0011D4D0(currentInput,previousInput,tolerance);
}
/** @sly2-annot addr=0x1E46A0
 * Purpose: Per-frame steering of a script-controlled player character: follow a path/curve or head to a
 *   target and write stick/heading/speed into the step's control output
 * Info: proposed name UpdateJsgJtControl (T2); TU relay
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
extern "C" void func_001E46A0(P *p,OUTPUT *out){
 WORK work;PARAM *param=p->param;ACTOR *actor=p->actor;
 typedef char layout_local_work[(sizeof(work)==0x60)?1:-1];
 typedef char layout_local_work_align[(__alignof__(work)==16)?1:-1];

 switch(param->kind){
 case 0x1A:{
  int reverse=0;
  float t=SClampShape(actor->curve,actor->curveTime+50.0f);
  EvaluateShapeS(actor->curve,&work.from.q,0,t);
  t=SClampShape(actor->curve,actor->curveTime-50.0f);
  EvaluateShapeS(actor->curve,&work.to.q,0,t);
  qword direction=func_001C4768(out,Sub(work.from.q,work.to.q));
  float x=X(direction),y=Y(direction);
  out->heading=func_002458F0(y,x);out->x=x;out->y=y;
  if(actor->curve->cyclic){
   float forward,backward;
   if(p->previousCurve<actor->curveTime){forward=actor->curveTime-p->previousCurve;backward=SMaxShape(actor->curve)+p->previousCurve-actor->curveTime;}
   else {backward=p->previousCurve-actor->curveTime;forward=SMaxShape(actor->curve)+actor->curveTime-p->previousCurve;}
   if(forward<backward)reverse=1;
  }else if(p->previousCurve<actor->curveTime)reverse=1;
  if(reverse){out->heading=RadNormalize__Ff(out->heading+3.1415927f);out->x=-out->x;out->y=-out->y;}
  out->speed=1.0f;break;
 }
 case 0x1D:case 0x1E:case 0x1F:case 0x20:case 0x21:case 0x22:
 stopped:
  out->speed=0.0f;if(actor->active!=-1)out->flags|=0x20;break;
 case 0x23:{
  ACTOR *target=func_001E1DB8(p,param->value.id);
  if(!target)goto stopped;
  func_001E4D40(p,target,0,&work.heading);
  func_00204660(actor,work.heading);out->heading=work.heading;
  goto stopped;
 }
 case 0x24:{
  out->speed=__builtin_fabsf(param->value.gain);
  if(param->flag)out->flags|=8;
  ACTOR *target=func_001E1DB8(p,param->target);
  if(!target)goto activityFlags;
  if(actor->active!=-1&&actor->mode==3){
   PATH *motion=actor->motion;
   EvaluateShapeU(motion,actor->motionTime,&work.from.q,&work.to.q);
   NormalizeTangent(work.to);
   int reverse=0;
   out->heading=func_002458F0(work.to.f[1],work.to.f[0]);
   if(motion->cyclic){
    float forward,backward;
    if(p->previousMotion<actor->motionTime){forward=actor->motionTime-p->previousMotion;backward=UMaxShape(motion)+p->previousMotion-actor->motionTime;}
    else {backward=p->previousMotion-actor->motionTime;forward=UMaxShape(motion)+actor->motionTime-p->previousMotion;}
    if(forward<backward)reverse=1;
   }else if(p->previousMotion<actor->motionTime)reverse=1;
   if(reverse)out->heading=RadNormalize__Ff(out->heading+3.1415927f);
  }else{
   func_001E4D40(p,target,&work.from.q,0);
   work.first=work.from.q;
   TargetDisplacement(work.to,work.first,func_001A62E8(actor));
   out->heading=func_002458F0(work.to.f[1],work.to.f[0]);
   float distance=LengthXY(work.to.q);
   float gain=D_002C3630[0]+distance*(D_002C3630[1]+distance*D_002C3630[2]);
   float low=D_002BBB60[0],high=D_002BBB60[1];
   if(gain<low)gain=low;else if(high<gain)gain=high;
   out->speed*=gain;
  }
 }
 activityFlags:
  if(actor->active!=-1&&(actor->mode==0||actor->mode==0xD))out->flags|=0x20;
  break;
 case 0x29:{
  out->speed=__builtin_fabsf(param->value.gain);
  ACTOR *target=func_001E1DB8(p,param->target);
  if(!target||actor->active==-1||actor->mode!=8)break;
  func_001E4D40(p,target,&work.from.q,0);
  {register qword captured __asm__("$3")=work.from.q;work.second=captured;}
  {
   qword actorPosition=func_001A62E8(actor);
   register qword targetPoint __asm__("vf2");
   register qword currentPoint __asm__("vf3");
   register qword displacement __asm__("vf1");
   register void *sourceInput __asm__("$4");
   register ACTOR *targetInput __asm__("$5");
   void *const sourceSpace=0;
   __asm__("lqc2 %0,%6\n\tdaddu %5,%8,$zero\n\tqmtc2.ni %7,%1\n\tdaddu %4,%z9,$zero\n\tvsub.xyzw %2,%0,%1\n\tsqc2 %2,%3"
    :"=j"(targetPoint),"=j"(currentPoint),"=j"(displacement),"=m"(work.to.q),"=r"(sourceInput),"=&r"(targetInput)
    :"m"(work.second),"r"(actorPosition),"r"(actor),"rJ"(sourceSpace));
   work.to.f[2]=0.0f;
   float speed;
   work.normal.q=NormalizeDirection(ConvertAloVec(sourceInput,targetInput,work.to.q),speed,out->speed);
   out->y=work.normal.f[0]*speed;out->x=-work.normal.f[1]*speed;
  }
  break;
 }
 case 0x58:
  out->speed=0.0f;out->flags|=0x20;UpdatePoctlPressed(out);break;
 case 0x5A:{
  float tolerance;
  qword oldPosition,position;
  int near;
  const int defaultStatus=0;
  __asm__("lq %0,0x19F0(%1)\n\tlwc1 %2,%7\n\tdaddu %3,%z8,$zero\n\tlq %1,0x1A20(%1)"
   :"=&r"(oldPosition),"=r"(position),"=f"(tolerance),"=&r"(near)
   :"1"(actor),"m"(actor->oldPosition),"m"(actor->position),"m"(D_002C3640),"rJ"(defaultStatus));
  register float previousX __asm__("$f13"),currentX __asm__("$f12");
  __asm__("mtc1 %1,%0":"=f"(previousX):"r"(oldPosition));
  __asm__("mtc1 %1,%0":"=f"(currentX):"r"(position));
  if(func_0011D4D0(currentX,previousX,tolerance)&&NearYInputs(oldPosition,position,tolerance))near=NearZInputs(oldPosition,position,tolerance)!=0;
  if(near)out->flags|=0x40;
  out->speed=0.0f;UpdatePoctlPressed(out);break;
 }
 case 0x2A:out->speed=0.0f;break;
 case 0x50:p->blocked=1;break;
 case 0x51:p->blocked=0;break;
 }
 if(p->blocked){out->flags|=0x10;if(param->kind==0x50)UpdatePoctlPressed(out);}
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_int[(sizeof(int)==4)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_qword_align[(__alignof__(qword)==16)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_U_align[(__alignof__(U)==16)?1:-1];
typedef char layout_ARG[(sizeof(ARG)==4)?1:-1];
typedef char layout_PARAM_prefix[(sizeof(PARAM)==0x28)?1:-1];
typedef char layout_ACTOR_prefix[(sizeof(ACTOR)==0x1A30)?1:-1];
typedef char layout_P_prefix[(sizeof(P)==0x80)?1:-1];
typedef char layout_OUTPUT_prefix[(sizeof(OUTPUT)==0x70)?1:-1];
typedef char layout_OUTPUT_align[(__alignof__(OUTPUT)==16)?1:-1];
typedef char layout_PATH_prefix[(sizeof(PATH)==0xE8)?1:-1];
typedef char layout_WORK[(sizeof(WORK)==0x60)?1:-1];
typedef char layout_WORK_align[(__alignof__(WORK)==16)?1:-1];
typedef char layout_PARAM_kind[((unsigned)&((PARAM*)0)->kind==0x0)?1:-1];
typedef char layout_PARAM_target[((unsigned)&((PARAM*)0)->target==0x10)?1:-1];
typedef char layout_PARAM_value[((unsigned)&((PARAM*)0)->value==0x14)?1:-1];
typedef char layout_PARAM_flag[((unsigned)&((PARAM*)0)->flag==0x24)?1:-1];
typedef char layout_ACTOR_vtable[((unsigned)&((ACTOR*)0)->vtable==0xc)?1:-1];
typedef char layout_ACTOR_mode[((unsigned)&((ACTOR*)0)->mode==0x4c8)?1:-1];
typedef char layout_ACTOR_active[((unsigned)&((ACTOR*)0)->active==0x4cc)?1:-1];
typedef char layout_ACTOR_motion[((unsigned)&((ACTOR*)0)->motion==0x4d4)?1:-1];
typedef char layout_ACTOR_motionTime[((unsigned)&((ACTOR*)0)->motionTime==0x4dc)?1:-1];
typedef char layout_ACTOR_curve[((unsigned)&((ACTOR*)0)->curve==0xf5c)?1:-1];
typedef char layout_ACTOR_curveTime[((unsigned)&((ACTOR*)0)->curveTime==0xf60)?1:-1];
typedef char layout_ACTOR_oldPosition[((unsigned)&((ACTOR*)0)->oldPosition==0x19f0)?1:-1];
typedef char layout_ACTOR_position[((unsigned)&((ACTOR*)0)->position==0x1a20)?1:-1];
typedef char layout_PATH_cyclic[((unsigned)&((PATH*)0)->cyclic==0xe4)?1:-1];
typedef char layout_P_overrideActor[((unsigned)&((P*)0)->overrideActor==0x24)?1:-1];
typedef char layout_P_param[((unsigned)&((P*)0)->param==0x40)?1:-1];
typedef char layout_P_actor[((unsigned)&((P*)0)->actor==0x44)?1:-1];
typedef char layout_P_blocked[((unsigned)&((P*)0)->blocked==0x48)?1:-1];
typedef char layout_P_previousMotion[((unsigned)&((P*)0)->previousMotion==0x78)?1:-1];
typedef char layout_P_previousCurve[((unsigned)&((P*)0)->previousCurve==0x7c)?1:-1];
typedef char layout_OUTPUT_x[((unsigned)&((OUTPUT*)0)->x==0x10)?1:-1];
typedef char layout_OUTPUT_y[((unsigned)&((OUTPUT*)0)->y==0x14)?1:-1];
typedef char layout_OUTPUT_heading[((unsigned)&((OUTPUT*)0)->heading==0x18)?1:-1];
typedef char layout_OUTPUT_speed[((unsigned)&((OUTPUT*)0)->speed==0x1c)?1:-1];
typedef char layout_OUTPUT_flags[((unsigned)&((OUTPUT*)0)->flags==0x30)?1:-1];
typedef char layout_OUTPUT_basis_0[((unsigned)&((OUTPUT*)0)->basis[0]==0x40)?1:-1];
typedef char layout_OUTPUT_basis_1[((unsigned)&((OUTPUT*)0)->basis[1]==0x50)?1:-1];
typedef char layout_OUTPUT_basis_2[((unsigned)&((OUTPUT*)0)->basis[2]==0x60)?1:-1];
typedef char layout_WORK_from[((unsigned)&((WORK*)0)->from==0x0)?1:-1];
typedef char layout_WORK_to[((unsigned)&((WORK*)0)->to==0x10)?1:-1];
typedef char layout_WORK_normal[((unsigned)&((WORK*)0)->normal==0x20)?1:-1];
typedef char layout_WORK_heading[((unsigned)&((WORK*)0)->heading==0x30)?1:-1];
typedef char layout_WORK_first[((unsigned)&((WORK*)0)->first==0x40)?1:-1];
typedef char layout_WORK_second[((unsigned)&((WORK*)0)->second==0x50)?1:-1];
typedef char layout_OUTPUT_basis_span[(sizeof(((OUTPUT*)0)->basis)==0x30)?1:-1];
typedef char layout_U_qword[((unsigned)&((U*)0)->q==0)?1:-1];
typedef char layout_U_float[((unsigned)&((U*)0)->f==0)?1:-1];
