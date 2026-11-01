// Update the controller target and consume state/animation transitions.
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct RANGE {float lo,hi;};
struct O;
struct S;
struct ASEG;
struct VE {short delta,pad;union {void (*plain)(void *);void (*integer)(void *,int);void (*vector)(void *,qword);void (*evaluate)(void *,int,V *,float,float);} f;};
struct TRACK {VE *vt;};
struct TRACKREF {int tag;TRACK *track;};
struct ASEGA {char pad[0x14];ASEG *definition;char pad18[4];float time;char pad20[0x20];float scale;};
struct CONTROL {char pad[0x18];float heading,speed;};
struct O {char pad[0xC];VE *vt;int id;char pad14[0x13C];CONTROL *control;int field154,sentinel,actorId;char pad160[0x30];V target;char pad1A0[4];int movement;char pad1A8[0x38];V direction;char pad1F0[0xE40];ASEGA *animation;char pad1034[0x39C];int mode;};
struct EFFECT {char pad[0xC];VE *vt;};
struct S {O *owner;int state;float time;O *peer,*selected;float idleThreshold,activeThreshold;RANGE bounds;float radius,scale;char pad2C[0x14];float blend0;char pad44[8];float blend1;char pad50[8];ASEG *animation0,*animation1,*animation2,*animation3;char pad68[0x30];ASEG *animation4,*animation5,*animation6;char padA4[0x18];ASEG *animation7;V targetLocal,secondary,targetWorld,velocity,transitionBegin,transitionEnd;float transitionStart,transitionDuration;int transitioning;EFFECT *effect;float distance;};
struct CLOCK {float t,dt;};
union SCRATCH {V vector;RANGE range;};
struct WORK {SCRATCH scratch;V delta;float angle,radius;V ownerPosition,speed,vector,playerPosition;};
extern O *D_002DD5BC;
extern CLOCK D_002DDED0;
extern float D_002C40E0;
extern V D_002A2DC0,D_002A2DA0,D_002A2DD0;
extern "C" {
qword func_001A62E8(O *);
int func_001DAEA8(int);
qword SetVectorCylind(float,float,float);
void GetVectorCylind(qword,float *,float *,float *);
float func_0011D948(RANGE *,float);
void UpdateJtThrowArc(S *,int);
float func_002458F0(float,float);
float RadStepHeading(O *);
float RadNormalize__Ff(float);
void SetJtthrowState(S *,int);
void func_001384C0(ASEG *);
int FAsegaRetracted(ASEGA *);
int FIsBasicDerivedFrom__FP5BASIC3CID(O *,int);
int GrfConsumePressed__5POCTLUif(CONTROL *,int,float);
void func_00136530(O *,int);
TRACKREF *func_0012FF10(ASEG *,O *,void *,int);
}
static inline void IntCall(O *o,int slot,int value){VE *v=o->vt;v[slot].f.integer((char *)o+v[slot].delta,value);}
static inline void EffectCall(EFFECT *o,int slot){VE *v=o->vt;v[slot].f.plain((char *)o+v[slot].delta);}
static inline void Evaluate(TRACK *o,int optional,V *result,float time,float scale){VE *v=o->vt;v[3].f.evaluate((char *)o+v[3].delta,optional,result,time,scale);}
static inline qword CPUFloat(float f){register qword r asm("$2");__asm__("mfc1 %0,%1":"=r"(r):"f"(f));return r;}
static inline qword Scalar(float f){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));return r;}
static inline qword ScalarHome(V &home,qword value){home.q=value;return home.q;}
static inline qword LoadVF(const V &v){qword q;__asm__("lqc2 %0,%1":"=j"(q):"m"(v));return q;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Lerp(qword a,qword b,qword wa,qword wb){__asm__("vmulax.xyzw ACC,%0,%2x\n\tvmaddx.xyzw %0,%1,%3x":"+j"(a):"j"(b),"j"(wa),"j"(wb));return a;}
static inline int SaveOwnerPosition(V &home,qword value,const S *state){
 register int result asm("$3");
 asm("sq %2,%1\n\tlw %0,%3":"=r"(result),"=m"(home.q):"r"(value),"m"(state->state));return result;
}
static inline qword PolarOperands(const V &saved,qword movement,float *angle,float *radius,float *&angleArgument,float *&radiusArgument){
 register qword sum asm("vf1"),offset asm("vf3");register float *a asm("$5"),*r asm("$6");
 asm("lqc2 %0,%4\n\tla %2,%a5\n\tqmtc2.ni %6,%1\n\tla %3,%a7\n\tvadd.xyzw %0,%0,%1":"=&j"(sum),"=j"(offset),"=&r"(a),"=r"(r):"m"(saved),"p"(angle),"r"(movement),"p"(radius));
 angleArgument=a;radiusArgument=r;return sum;
}
static inline void StoreTarget(V &target,V &delta,const V &owner,qword polar){
 register qword local asm("vf1"),origin asm("vf4"),sum asm("vf2");
 asm("qmtc2.ni %5,%0\n\tlqc2 %1,%6\n\tvadd.xyzw %2,%1,%0\n\tsqc2 %0,%3\n\tsqc2 %2,%4":"=j"(local),"=j"(origin),"=j"(sum),"=m"(delta.q),"=m"(target.q):"r"(polar),"m"(owner));
}
static inline qword IdleDifference(qword selected,const V &owner){register qword point asm("vf2"),origin asm("vf3"),difference asm("vf1");
 asm("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tvsub.xyzw %2,%0,%1":"=j"(point),"=j"(origin),"=j"(difference):"r"(selected),"m"(owner));return difference;
}
static inline qword ActiveDifference(const V &target,const V &owner){register qword origin asm("vf2"),difference asm("vf1");
 asm("lqc2 %0,%3\n\tlqc2 %1,%2\n\tvsub.xyzw %1,%1,%0":"=j"(origin),"=j"(difference):"m"(target),"m"(owner));return difference;
}
static inline void ProjectionInputs(qword current,const V &player,qword &currentVF,qword &playerVF,V *&high,V *&savedHigh){
 register V *upper asm("$3"),*saved asm("$4");
 register qword origin asm("vf1"),point asm("vf4");
 asm("lui %0,%%hi(%6)\n\tqmtc2.ni %4,%2\n\tlqc2 %3,%5\n\tdaddu %1,%0,$zero":"=&r"(upper),"=r"(saved),"=j"(origin),"=j"(point):"r"(current),"m"(player),"i"(&D_002A2DC0));
 currentVF=origin;playerVF=point;high=upper;savedHigh=saved;
}
static inline V *InitialUpAddress(V *upper){register V *address asm("$3");
 asm("addiu %0,%1,%%lo(%2)":"=r"(address):"r"(upper),"i"(&D_002A2DC0));return address;
}
static inline qword PointDifference(qword player,qword current){register qword point asm("vf4")=player,origin asm("vf1")=current;
 asm("vsub.xyzw %0,%0,%1":"+j"(point):"j"(origin));return point;
}
static inline qword PlayerVF(qword player){register qword value asm("vf4");asm("qmtc2.ni %1,%0":"=j"(value):"r"(player));return value;}
static inline float LaunchLengthX(qword length){register int bits asm("$3");register float x asm("$f0");
 asm("qmfc2.ni %0,%2\n\tmtc1 %0,%1":"=r"(bits),"=f"(x):"j"(length));return x;
}
static inline qword LaunchVelocity(qword direction,qword speed,qword up,qword height){register qword result asm("vf1")=up,d asm("vf3")=direction,s asm("vf4")=speed,h asm("vf2")=height;
 asm("vmulax.xyzw ACC,%1,%2x\n\tvmaddx.xyzw %0,%0,%3x":"+j"(result):"j"(d),"j"(s),"j"(h));return result;
}
static inline void LaunchVectorCall(O *owner,const V &home){qword velocity=LoadVF(home);VE *table=owner->vt;
 register int delta asm("$4");register qword argument asm("$5");
 asm("lh %0,%2\n\tqmfc2.ni %1,%3":"=&r"(delta),"=r"(argument):"m"(table[35].delta),"j"(velocity));
 table[35].f.vector((char *)owner+delta,argument);
}
static inline qword PlanarUnit(qword input,qword up){
 register qword point asm("vf4")=input,normal asm("vf1")=up,projection asm("vf2"),direction asm("vf3");
 register const V*address asm("$2");register float epsilon asm("$f1");
 asm("lui %3,%%hi(%6)\n\tvmul.xyz %0,%4,%5\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tli.s %2,0.0001\n\tvmulx.xyzw %0,%5,%0x\n\taddiu %3,%3,%%lo(%6)"
  :"=&j"(projection),"=&j"(direction),"=f"(epsilon),"=&r"(address):"j"(point),"j"(normal),"i"(&D_002A2DA0):"$1");
 asm("vsub.xyzw %0,%1,%2":"=j"(direction):"j"(point),"j"(projection));
 register qword fallback asm("$2");
 asm("lq %0,0(%1)":"=r"(fallback):"r"(address),"m"(*address));
 register qword length asm("vf1"),one asm("vf2");
 asm("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=&j"(length),"=&j"(one):"j"(direction));
 if(LaunchLengthX(length)<epsilon)direction=fallback;
 else asm("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%0,Q":"+j"(direction):"j"(length));
 return direction;
}
static inline qword LiftFromUpper(float height,const V &speed,V *upper,qword &speedValue,qword &upValue){
 register V *address asm("$2");register qword normal asm("vf1"),scalar asm("vf2"),velocity asm("vf4");register int bits asm("$5");
 asm("addiu %0,%5,%%lo(%9)\n\tlqc2 %1,0(%0)\n\tmfc1 %2,%6\n\tqmtc2.ni %2,%3\n\tlqc2 %4,%7":"=&r"(address),"=j"(normal),"=&r"(bits),"=j"(scalar),"=j"(velocity):"r"(upper),"f"(height),"m"(speed),"m"(*(const V *)((const char *)upper+0x2DC0)),"i"(&D_002A2DC0));
 upValue=normal;speedValue=velocity;return scalar;
}
static inline qword SavedPlayerAndOwner(qword player,const S *state,O *&owner){
 register qword point asm("vf4");register O *receiver asm("$4");
 asm("qmtc2.ni %2,%0\n\tlw %1,%3":"=j"(point),"=r"(receiver):"r"(player),"m"(state->owner));owner=receiver;return point;
}
static inline qword TransitionBlend(const V &begin,const V &end,float numerator,float duration,V &home,qword &toward){
 register qword from asm("vf2"),to asm("vf4"),wa asm("vf1"),wb asm("vf3");
 register float alpha asm("$f0")=numerator,inverse asm("$f1")=1.0f,total asm("$f2")=duration;
 asm("lqc2 %0,%4\n\tlqc2 %1,%5\n\tdiv.s %2,%2,%6\n\tsub.s %3,%3,%2"
 :"=&j"(from),"=&j"(to),"+&f"(alpha),"+&f"(inverse):"m"(begin),"m"(end),"f"(total));
 register int alphaBits asm("$2"),inverseBits asm("$3");
 asm("mfc1 %0,%2\n\tqmtc2.ni %0,%1":"=&r"(alphaBits),"=j"(wb):"f"(alpha));
 asm("mfc1 %2,%4\n\tqmtc2.ni %2,%1\n\tsqc2 %1,%3\n\tvmulax.xyzw ACC,%0,%1x\n\tvmaddx.xyzw %0,%5,%6x"
 :"+j"(from),"=&j"(wa),"=&r"(inverseBits),"=m"(home.q):"f"(inverse),"j"(to),"j"(wb));toward=wb;return from;
}
static inline float PlanarSnapshot(qword value,V &home,const float &clock){
 register float dt asm("$f0");
 asm("lwc1 %0,%3\n\tsqc2 %4,%1\n\tsw $zero,%2":"=f"(dt),"=m"(home.q),"=m"(home.f[2]):"m"(clock),"j"(value));return dt;
}
static inline void LaunchPublication(qword value,V &home,O *&owner,int &flag){
 register O *receiver asm("$4");register int mode asm("$5");
 asm volatile("lw %0,%3\n\taddiu %1,$zero,1\n\tsqc2 %4,%2":"=r"(receiver),"=r"(mode),"=m"(home.q):"m"(D_002DD5BC),"j"(value));owner=receiver;flag=mode;
}
typedef int RangeBits __attribute__((mode(DI)));
static inline void CopyRange(RANGE &destination,const RANGE &source){
 register RangeBits value asm("$5");
 asm("ldl %0,7+%2\n\tldr %0,%2\n\tsdl %0,7+%1\n\tsdr %0,%1":"=&r"(value),"=m"(destination):"m"(source));
}
/** @sly2-annot addr=0x21F418
 * Purpose: per-frame carry/aim controller: target selection, aim angles (atan2f, GLimitLm), arc (0x21FB70),
 *   state transitions (0x2206C8)
 * Info: proposed name UpdateJtthrow (T1); TU throw
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
extern "C" void UpdateJtthrow(S *s){
 WORK w;
 register CONTROL *control asm("$19")=s->owner->control;
 qword initial=func_001A62E8(s->owner);
 int initialState=SaveOwnerPosition(w.ownerPosition,initial,s);
 if(initialState<7){
 if(initialState>=4){
  if(s->transitioning){
   float begin=s->transitionStart,end=begin+s->transitionDuration,time=D_002DDED0.t;
   if(end<=time){s->transitioning=0;s->targetWorld.q=s->transitionEnd.q;}
   else{
    qword toward;s->targetWorld.q=TransitionBlend(s->transitionBegin,s->transitionEnd,time-begin,end-begin,w.scratch.vector,toward);
    w.scratch.vector.q=toward;
   }
  }
  else{
   CopyRange(w.scratch.range,s->bounds);
   if(func_001DAEA8(0x24))w.scratch.range.hi*=D_002C40E0;
   float dt=PlanarSnapshot(Sub(s->targetWorld.q,w.ownerPosition.q),w.delta,D_002DDED0.dt);
   float elevation;
   float scale;qword delta;
   float speed;
   {
    register float rate asm("$f13"),factor asm("$f1");register qword vector asm("vf1");
    asm("lwc1 %0,%3\n\tlwc1 %1,%4\n\tmul.s %0,%0,%5\n\tlqc2 %2,%6":"=&f"(rate),"=&f"(factor),"=j"(vector):"m"(control->speed),"m"(s->scale),"f"(dt),"m"(w.delta));
    scale=factor;delta=vector;speed=rate;
   }
   float heading,advance;
   {
    register float z asm("$f20"),direction asm("$f12"),step asm("$f13")=speed;
    asm("lwc1 %0,%4\n\tlwc1 %1,%5\n\tmul.s %2,%2,%6\n\tsqc2 %7,%3":"=&f"(z),"=&f"(direction),"+f"(step),"=m"(w.vector.q):"m"(w.delta.f[2]),"m"(control->heading),"f"(scale),"j"(delta));
    elevation=z;heading=direction;advance=step;
   }
   qword movement=SetVectorCylind(heading,advance,elevation);
   float *angleArgument,*radiusArgument;
   qword sum=PolarOperands(w.vector,movement,&w.angle,&w.radius,angleArgument,radiusArgument);
   w.delta.q=sum;
   GetVectorCylind(sum,angleArgument,radiusArgument,0);
   w.radius=func_0011D948(&w.scratch.range,w.radius);
   qword polar=SetVectorCylind(w.angle,w.radius,elevation);
   StoreTarget(s->targetWorld,w.delta,w.ownerPosition,polar);
  }
  UpdateJtThrowArc(s,1);
 }
 }
 for(;;){
  int previous=s->state;
  ASEGA *animation=s->owner->animation;
  switch(previous){
  case 0:{
   w.scratch.vector.q=IdleDifference(func_001A62E8(s->selected),w.ownerPosition);
   float heading=func_002458F0(w.scratch.vector.f[1],w.scratch.vector.f[0]);
   float angle=RadNormalize__Ff(heading-RadStepHeading(s->owner));
   float threshold=s->idleThreshold;
   if(threshold<angle)SetJtthrowState(s,2);
   else if(angle<-threshold)SetJtthrowState(s,1);
   else{s->blend1=angle/threshold*0.5f+0.5f;s->blend0=1.0f-s->blend1;func_001384C0(s->animation0);}
   break;
  }
  case 1:case 2:
   if(!animation||FAsegaRetracted(animation))SetJtthrowState(s,0);
   break;
  case 3:
   if(!animation||FAsegaRetracted(animation)){if(D_002DD5BC->mode==8)SetJtthrowState(s,4);else SetJtthrowState(s,0);}
   break;
  case 4:case 5:case 6:{
   if(FIsBasicDerivedFrom__FP5BASIC3CID(s->selected,0x64)&&s->selected->sentinel!=-1){IntCall(s->owner,88,0);break;}
   float margin=0.1f;
   if(GrfConsumePressed__5POCTLUif(control,0xC0,margin)){SetJtthrowState(s,7);break;}
   if(GrfConsumePressed__5POCTLUif(control,0x30,margin)){
    if(s->animation0)SetJtthrowState(s,0);else IntCall(s->owner,88,0);
    O *selected=s->selected;if(selected==D_002DD5BC&&selected->mode==8){
     w.speed.q=ScalarHome(w.scratch.vector,CPUFloat(400.0f));
     O *positionOwner;w.playerPosition.q=SavedPlayerAndOwner(func_001A62E8(selected),s,positionOwner);
     qword position=func_001A62E8(positionOwner),currentVF,playerVF;V *upper,*savedUpper;ProjectionInputs(position,w.playerPosition,currentVF,playerVF,upper,savedUpper);
     qword direction=PointDifference(playerVF,currentVF);qword up=LoadVF(*InitialUpAddress(upper));direction=PlanarUnit(direction,up);
     qword speedValue,upValue; qword height=LiftFromUpper(600.0f,w.speed,savedUpper,speedValue,upValue);
     O *launchOwner;int launchFlag;LaunchPublication(LaunchVelocity(direction,speedValue,upValue,height),w.vector,launchOwner,launchFlag);
     w.scratch.vector.q=height;
     func_00136530(launchOwner,launchFlag);LaunchVectorCall(D_002DD5BC,w.vector);
    }
    if(s->effect)EffectCall(s->effect,9);
    return;
   }
   if(!animation||FAsegaRetracted(animation))SetJtthrowState(s,4);
   else if(s->state==4){
    w.scratch.vector.q=ActiveDifference(s->targetWorld,w.ownerPosition);
    float heading=func_002458F0(w.scratch.vector.f[1],w.scratch.vector.f[0]);
    float angle=RadNormalize__Ff(heading-RadStepHeading(s->owner));
    float threshold=s->activeThreshold;
    if(threshold<angle)SetJtthrowState(s,6);else if(angle<-threshold)SetJtthrowState(s,5);
   }
   break;
  }
  case 7:
   if(!animation||FAsegaRetracted(animation)){if(s->animation0)SetJtthrowState(s,0);else IntCall(s->owner,88,0);}
   break;
  }
  if(s->state==previous||s->state==-1)break;
 }
 O *owner=s->owner;
 if(owner->movement!=1)return;
 ASEGA *animation=owner->animation;
 TRACKREF *reference=animation?func_0012FF10(animation->definition,owner,(void *)6,1):0;
 if(reference){
  Evaluate(reference->track,0,&w.scratch.vector,animation->time,animation->scale);
  V *direction=&s->owner->direction;float height=w.scratch.vector.f[2];direction->f[0]=0.0f;direction->f[2]=height;direction->f[1]=0.0f;
 }
 else s->owner->direction.q=D_002A2DD0.q;
}
