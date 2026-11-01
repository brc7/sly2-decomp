// Movement-state transitions and actor response.
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int bits[4];void Assign(const U&v){q=v.q;}};
union F{float value;int bits;};
struct VE{short delta,pad;void(*fn)();};
struct GROUP{char pad[0xA0];void*space;};
struct TUNE{char pad[0xD0];float speed;};
struct P{char pad0[12];VE*vt;char pad10[0x58-0x10];GROUP*first;GROUP*second;char pad60[0xE8-0x60];TUNE*tuning;char padEC[0x334-0xEC];int attention;char pad338[0x10DC-0x338];P*owner;};
struct ASEGA;
struct ENTRY{float value;int kind,key,pad;};
struct CONTROL{char pad0[4];void*curves;ASEGA*asega;};
struct DELTA;
struct REQUEST{int flag;float a,b;int pad;U point,direction;};
struct C{char pad0[0x10];P*actor;char pad14[0x40-0x14];CONTROL*control;char pad44[12];int mode;float time;int pad58;P*target;int owned;DELTA*delta;};
struct CLOCK{float time,dt;};
extern "C"{
 extern CLOCK D_002DDED0;
 extern float D_002BE6FC,D_002BE700,D_002BE704;
 extern F D_002BE708;
 extern U D_002A2DC0;
 extern char D_003DD490[],D_002DFC00[];
 void func_001F35C8(P*,float);
 void func_00130D30(ASEGA*,int,float,float);
 void func_0012E460(DELTA**);
 void func_002303A0(P*,GROUP*,int);
 void func_00134320(void*,P*);
 void func_001F4E28(P*);
 void func_001749C0(C*,int);
 void func_0017AA78(C*,REQUEST*,U*,U*);
 qword func_001A62E8(P*);
 qword func_0022FC48(void*,void*,qword);
 qword func_0022FCC8(void*,void*,qword);
 DELTA*func_0012DE58(P*,GROUP*,void*,float,qword,qword);
 P*func_001C4840(void*);
 void func_00187E28(void*,int,float);
 ENTRY*func_0012F938(void*,int);
}
static inline void Request(P*p,REQUEST*r,float duration){VE*v=&p->vt[96];((void(*)(P*,REQUEST*,float))v->fn)((P*)((char*)p+v->delta),r,duration);}
static inline void BlendSet(C*p,qword current,U*desired,U*scale,U*scratch){
 register P*actor __asm__("$7");register VE*vt __asm__("$6");register int word __asm__("$4");
 register qword a __asm__("vf1"),b __asm__("vf2"),d __asm__("vf3"),lift __asm__("vf2"),axis __asm__("vf1"),k __asm__("vf4");
 const unsigned gainAddress=0x2BE708;
 register char*gainBase __asm__("$4");
 __asm__("lw %0,%4\n\tqmtc2.ni %5,%1\n\tlqc2 %2,%6\n\tlui %3,%7"
  :"=&r"(actor),"=j"(b),"=j"(a),"=r"(gainBase)
  :"m"(p->actor),"r"(current),"m"(desired->q),"i"((gainAddress+0x8000)>>16));
 const unsigned axisLocation=0x2A2DC0;
 register char*axisBase __asm__("$3");
 __asm__("lw %0,%4\n\tvsub.xyzw %1,%5,%6\n\tlw %2,%7\n\tlui %3,%8"
  :"=&r"(vt),"=j"(d),"=r"(word),"=r"(axisBase)
  :"m"(actor->vt),"j"(a),"j"(b),"m"(*(int*)(gainBase+(short)(gainAddress&0xffff))),"i"((axisLocation+0x8000)>>16));
 register U*axisAddress __asm__("$3");
 __asm__("qmtc2.ni %2,%0\n\taddiu %1,%3,%4"
  :"=j"(lift),"=r"(axisAddress):"r"(word),"r"(axisBase),"i"((short)(axisLocation&0xffff)));
 register int adjustment __asm__("$4");
 register void(*setter)() __asm__("$3");
 __asm__("lqc2 %0,%4\n\tlh %1,%5\n\tlqc2 %2,%6\n\tlw %3,%7"
  :"=j"(axis),"=r"(adjustment),"=j"(k),"=r"(setter)
  :"m"(axisAddress->q),"m"(vt[35].delta),"m"(scale->q),"m"(vt[35].fn));
 register qword packed __asm__("$5");
 register P*adjusted __asm__("$4");
 __asm__("vmulax.xyzw $ACC,%3,%0x\n\tvmaddx.xyzw %0,%4,%5x\n\tqmfc2.ni %1,%0\n\taddu %2,%6,%7"
  :"+j"(k),"=r"(packed),"=r"(adjusted):"j"(d),"j"(axis),"j"(lift),"r"(actor),"r"(adjustment));
 scratch->q=lift;
 ((void(*)(P*,qword))setter)(adjusted,packed);
}
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
typedef char Float_Size[sizeof(float)==4?1:-1];
typedef char Short_Size[sizeof(short)==2?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char REQUEST_Alignment[__alignof__(REQUEST)==16?1:-1];
typedef char C_Size[sizeof(C)==0x68?1:-1];
typedef char P_Size[sizeof(P)==0x10E0?1:-1];
typedef char GROUP_Size[sizeof(GROUP)==0xA4?1:-1];
typedef char TUNE_Size[sizeof(TUNE)==0xD4?1:-1];
typedef char CONTROL_Size[sizeof(CONTROL)==12?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_fn_Offset[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char REQUEST_Size[sizeof(REQUEST)==48?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char F_Size[sizeof(F)==4?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char ENTRY_Size[sizeof(ENTRY)==16?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==16?1:-1];
typedef char C_control_Offset[FIELD_OFFSET(C,control)==64?1:-1];
typedef char C_mode_Offset[FIELD_OFFSET(C,mode)==80?1:-1];
typedef char C_time_Offset[FIELD_OFFSET(C,time)==84?1:-1];
typedef char C_target_Offset[FIELD_OFFSET(C,target)==92?1:-1];
typedef char C_owned_Offset[FIELD_OFFSET(C,owned)==96?1:-1];
typedef char C_delta_Offset[FIELD_OFFSET(C,delta)==100?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_first_Offset[FIELD_OFFSET(P,first)==88?1:-1];
typedef char P_second_Offset[FIELD_OFFSET(P,second)==92?1:-1];
typedef char P_tuning_Offset[FIELD_OFFSET(P,tuning)==232?1:-1];
typedef char P_attention_Offset[FIELD_OFFSET(P,attention)==820?1:-1];
typedef char P_owner_Offset[FIELD_OFFSET(P,owner)==4316?1:-1];
typedef char GROUP_space_Offset[FIELD_OFFSET(GROUP,space)==160?1:-1];
typedef char TUNE_speed_Offset[FIELD_OFFSET(TUNE,speed)==208?1:-1];
typedef char CONTROL_curves_Offset[FIELD_OFFSET(CONTROL,curves)==4?1:-1];
typedef char CONTROL_asega_Offset[FIELD_OFFSET(CONTROL,asega)==8?1:-1];
typedef char REQUEST_flag_Offset[FIELD_OFFSET(REQUEST,flag)==0?1:-1];
typedef char REQUEST_a_Offset[FIELD_OFFSET(REQUEST,a)==4?1:-1];
typedef char REQUEST_b_Offset[FIELD_OFFSET(REQUEST,b)==8?1:-1];
typedef char REQUEST_point_Offset[FIELD_OFFSET(REQUEST,point)==16?1:-1];
typedef char REQUEST_direction_Offset[FIELD_OFFSET(REQUEST,direction)==32?1:-1];
typedef char ENTRY_value_Offset[FIELD_OFFSET(ENTRY,value)==0?1:-1];
typedef char ENTRY_kind_Offset[FIELD_OFFSET(ENTRY,kind)==4?1:-1];
typedef char ENTRY_key_Offset[FIELD_OFFSET(ENTRY,key)==8?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
static inline int Still(int m){int f=0;if(m==4||m==5||m==0)f=1;return f;}
/** @sly2-annot addr=0x17B1A8
 * Purpose: BHVHELD state switch: attach ACT to holder (PactNewE), release ACT/XF roles, mass change, anim
 *   label seeks 0x3C/0x173
 * Info: proposed name SetBhvjuggledState (T1); TU guardbhv
 * Notes: BHV class word normalised: SetBhvheldState -> SetBhvjuggledState | runtime2/4 confirmed the
 *   BHVJUGGLED role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
extern "C" void func_0017B1A8(C*p,int next){
 REQUEST request;
 U point,direction,scratch,scale,cached;
 if(next==p->mode)return;
 switch(p->mode){
 case -1:func_001F35C8(p->actor,D_002BE6FC*p->actor->tuning->speed);break;
 case 3:if(next!=-1)func_00130D30(p->control->asega,0,0.0f,1.0f);break;
 }
 if((unsigned)p->mode<2&&(unsigned)next>=2){
  func_0012E460(&p->delta);func_002303A0(p->actor,p->actor->first,2);func_002303A0(p->actor,p->actor->second,2);
 }
 int was=Still(p->mode),now=Still(next);
 p->mode=next;
 if(was!=now){
  const unsigned gameAddress=0x3DD490;
  register char*gameBase __asm__("$4");
  register P*callbackActor __asm__("$5");
  __asm__("lui %0,%2\n\tlw %1,%3":"=&r"(gameBase),"=r"(callbackActor)
   :"i"((gameAddress+0x8000)>>16),"m"(p->actor));
  func_00134320(gameBase+(short)(gameAddress&0xffff),callbackActor);
  func_001F4E28(p->actor);p->mode=next;
 }
 const unsigned clockAddress=0x2DDED0;
 char*clockHigh;
 register char*clockCarrier __asm__("$19");
 register float newTime __asm__("$f0");
 __asm__("lui %0,%1":"=r"(clockHigh):"i"((clockAddress+0x8000)>>16));
 __asm__("daddu %0,%2,$0\n\tlwc1 %1,%3":"=&r"(clockCarrier),"=f"(newTime)
  :"r"(clockHigh),"m"(*(float*)(clockHigh+(short)(clockAddress&0xffff))));
 p->time=newTime;
 switch(next){
 case -1:func_001F35C8(p->actor,p->actor->tuning->speed/D_002BE6FC);func_001749C0(p,1);break;
 case 0:{
  Request(p->target,&request,D_002BE700);
  func_0017AA78(p,&request,&point,&direction);
  register qword scalar __asm__("$2");
  register P*beforeActor __asm__("$4");
  register float duration __asm__("$f1")=D_002BE700;
  register float factor __asm__("$f0")=D_002BE704;
  __asm__("lq %0,%4\n\tlw %1,%5\n\tdiv.s %2,%2,%6\n\tsq %0,%3\n\tmfc1 %0,%2"
   :"=&r"(scalar),"=&r"(beforeActor),"+f"(factor),"=m"(cached.q)
   :"m"(point.q),"m"(p->actor),"f"(duration));
  scratch.q=scalar;scale=scratch;
  qword current=func_001A62E8(beforeActor);
  BlendSet(p,current,&cached,&scale,&scratch);
  GROUP*first=p->actor->first;
  if(!p->delta){
   float end=*(float*)(clockCarrier+(short)(clockAddress&0xffff))+D_002BE700;
   qword localPoint=func_0022FC48(0,first->space,point.q);
   qword localDirection=func_0022FCC8(0,first->space,direction.q);
   p->delta=func_0012DE58(p->actor,first,0,end,localPoint,localDirection);
  }
  int owned=0;if(request.flag)owned=p->target->owner==p->actor;p->owned=owned;
  if(p->actor->attention){P*focus=func_001C4840(D_003DD490);if(focus&&p->target==focus)func_00187E28(D_002DFC00,3,0.15f);}
  break;
 }
 case 3:{ENTRY*entry=func_0012F938(p->control->curves,0x3C);if(entry)func_00130D30(p->control->asega,0,entry->value,1.0f);break;}
 case 4:case 5:
  if(p->mode==4){ENTRY*entry=func_0012F938(p->control->curves,0x173);if(entry)func_00130D30(p->control->asega,0,entry->value,1.0f);}
  break;
 case 1:case 2:break;
 }
}
