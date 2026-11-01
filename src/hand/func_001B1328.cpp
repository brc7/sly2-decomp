// Mover/follower state controller: switch from the current state to `next`.
// Leaving states -1/0/2/3 restores the group's default action; entering a state sets up its velocity, mode, timers or
// effect; afterwards the new state, its time stamp and (except for state 2) the state's animation entry are recorded.
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};
union F{float value;int bits;};
struct VE{short delta,pad;void*fn;};
struct M4{U row[4];};struct M3{U row[3];};
struct PATH;struct ASEG;struct ASEGA;struct EFFECT;struct SOURCE;
struct ENT{ASEG*clip;int flags;int pad;};   // per-state animation table entry (12 bytes)
struct P{char pad0[12];VE*vt;char pad10[0x5C-0x10];void*group;char pad60[0x158-0x60];float low0,high0,low1,high1,lead;char pad16c[4];PATH*path;float parameter,phase;char pad17c[4];ENT aseg[4];char pad1B0[0x1C4-0x1B0];ASEGA*animation;char pad1c8[8];F speed;int oldMode;float heading;char pad1dc[4];M3 basis;float spin[3];float ratio;int state;float changed,randomDeadline,deadline;int amount;char pad234[12];U damageOffset;};
struct WORK{U first,second,direction,velocity,scalar;float angle,parameter;};
struct CLOCK{float t,dt;};
extern "C"{
 extern CLOCK D_002DDED0;extern U D_002A2DD0,D_002A2DA0;extern char D_003DD490[],D_002DE578[];
 void func_002303A0(P*,void*,int);void func_0022C840(P*,void*);void func_0022C818(P*,void*);
 float GRandInRange__Fff(float,float);void GetVectorSphere(qword,float*,float*,float*);
 void func_001B1818(P*,float);void func_001B17B8(P*,float);
 qword func_001A62E8(P*);void FindShapeClosestPoint(PATH*,qword,void*,U*,U*,float*,float*);
 float DuGetShapeSearchIncrement(PATH*);float SMaxShape(PATH*);P*PpoClosestSw(void*,qword);
 EFFECT*func_001FBB90(int);void func_001FB2F8(EFFECT*,SOURCE*);void func_001FB3F0(EFFECT*,qword);
 qword ConvertAloPos(P*,P*,qword);void func_00154E10(void*,qword,int,int,P*);
 void func_001305B8(ASEG*,P*,float,float,int,ASEGA**);
}
static inline void SetVelocity(P*p,qword q){VE&v=p->vt[35];((void(*)(void*,qword))v.fn)((char*)p+v.delta,q);}
static inline void SetZeroVelocity(P*p){SetVelocity(p,D_002A2DD0.q);}
// broadcast a float into a VU register and keep the scratch store the original left in `home`
static inline qword SplatInto(float f,U*home){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));home->q=q;return q;}
static inline qword ScaleX(qword a,qword s){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
// length of a vector, in two steps (sum of squares, then sqrt) so the stores/loads of the callers can sit between them
static inline qword LengthV(qword v){qword d,one;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));return d;}
static inline float FloatOf(qword a){float f;__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(a));return f;}
// v * (1/n.x); the "memory" clobbers (here and on LEN1M/LEN2M) keep the surrounding loads/stores in source order
static inline qword DivBy(qword v,qword n){qword r;__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(n),"j"(v):"memory");return r;}
static inline qword SplatBits(int bits){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;}
#define LEN1M(d,one,v) __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(v):"memory")
#define LEN2M(d) __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d): :"memory")
static inline void Call(P*p,int index){VE&v=p->vt[index];((void(*)(void*))v.fn)((char*)p+v.delta);}
static inline void SetMode(P*p){VE&v=p->vt[47];((void(*)(void*,int))v.fn)((char*)p+v.delta,4);}
static inline M4*GetMatrix(P*p){VE&v=p->vt[31];return ((M4*(*)(void*))v.fn)((char*)p+v.delta);}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char P_Prefix_Size[sizeof(P)==0x250?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char M4_Size[sizeof(M4)==64?1:-1];
typedef char WORK_Size[sizeof(WORK)==96?1:-1];
typedef char P_group_Offset[FIELD_OFFSET(P,group)==92?1:-1];
typedef char P_low0_Offset[FIELD_OFFSET(P,low0)==344?1:-1];
typedef char P_lead_Offset[FIELD_OFFSET(P,lead)==360?1:-1];
typedef char P_path_Offset[FIELD_OFFSET(P,path)==368?1:-1];
typedef char P_phase_Offset[FIELD_OFFSET(P,phase)==376?1:-1];
typedef char P_aseg_Offset[FIELD_OFFSET(P,aseg)==384?1:-1];
typedef char P_animation_Offset[FIELD_OFFSET(P,animation)==452?1:-1];
typedef char P_speed_Offset[FIELD_OFFSET(P,speed)==464?1:-1];
typedef char P_oldMode_Offset[FIELD_OFFSET(P,oldMode)==468?1:-1];
typedef char P_ratio_Offset[FIELD_OFFSET(P,ratio)==540?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==544?1:-1];
typedef char P_changed_Offset[FIELD_OFFSET(P,changed)==548?1:-1];
typedef char P_deadline_Offset[FIELD_OFFSET(P,deadline)==556?1:-1];
typedef char P_amount_Offset[FIELD_OFFSET(P,amount)==560?1:-1];
typedef char P_damageOffset_Offset[FIELD_OFFSET(P,damageOffset)==576?1:-1];
extern "C" void func_001B1328(P*p,int next){
 int old=p->state;if(old==next)return;WORK w;
 switch(old){case -1:case 0:case 2:case 3:func_002303A0(p,p->group,p->oldMode);func_0022C840(p,p->group);break;}
 switch(next){
 case 0:
  SetVelocity(p,D_002A2DD0.q);func_0022C818(p,p->group);{float now=D_002DDED0.t;p->randomDeadline=now+GRandInRange__Fff(p->low0,p->high0);}break;
 case 1:
  SetMode(p);{float now=D_002DDED0.t;p->randomDeadline=now+GRandInRange__Fff(p->low1,p->high1);}break;
 case 2:{
  p->phase=-p->phase;float negative=-1.0f;M4*m=GetMatrix(p);
  qword row=m->row[0].q;qword factor=SplatInto(negative,&w.first);qword scaled=ScaleX(row,factor);
  GetVectorSphere(scaled,&w.angle,0,0);func_001B1818(p,w.angle);SetZeroVelocity(p);func_002303A0(p,p->group,1);func_0022C840(p,p->group);break;
 }
 case 3:
  SetZeroVelocity(p);func_001B17B8(p,p->ratio+p->ratio);
  func_002303A0(p,p->group,1);func_0022C840(p,p->group);break;
 case 4:{
  w.first.q=func_001A62E8(p);
  if(p->path){
   FindShapeClosestPoint(p->path,w.first.q,0,&w.second,0,&p->parameter,&w.parameter);
   float phase=DuGetShapeSearchIncrement(p->path);
   qword delta=Sub(w.first.q,w.second.q);
   float speed=p->speed.value;
   float travel=FloatOf(LengthV(delta));
   p->phase=phase;
   travel/=speed;
   float end=SMaxShape(p->path);
   p->deadline=D_002DDED0.t+travel+(end-w.parameter)/p->speed.value-p->lead;
  }else{
   P*near=PpoClosestSw(D_003DD490,w.first.q);w.second=w.first;w.first.q=func_001A62E8(near);p->deadline=D_002DDED0.t+2.0f;
  }
  qword ff=w.first.q;qword ss=w.second.q;qword gap=Sub(ss,ff);
  w.direction.q=gap;w.direction.i[2]=0;
  qword point=w.direction.q;
  // register pin: local-alloc would otherwise give vf1 to the unused `one` scratch of LEN1M (the original has the length in vf1)
  register qword norm __asm__("vf1");qword one;LEN1M(norm,one,point);
  qword fallback=D_002A2DA0.q;LEN2M(norm);float length=FloatOf(norm);qword direction;
  if(length<0.0001f)direction=fallback;else direction=DivBy(point,norm);
  qword factor=SplatBits(p->speed.bits);
  w.scalar.q=factor;
  qword velocity=ScaleX(direction,factor);
  w.velocity.q=velocity;SetVelocity(p,velocity);SetMode(p);break;
 }
 case 5:{
  EFFECT*e=func_001FBB90(0x14);func_001FB2F8(e,0);func_001FB3F0(e,func_001A62E8(p));
  func_00154E10(D_002DE578,ConvertAloPos(p,0,p->damageOffset.q),p->amount,-100,p);Call(p,9);break;
 }
 }
 p->state=next;p->changed=D_002DDED0.t;
 if(next!=2){
  func_001305B8(p->aseg[next].clip,p,0.0f,1.0f,p->aseg[next].flags|1,&p->animation);
 }
}
