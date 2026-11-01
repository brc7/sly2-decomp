#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
struct P;
struct C;
typedef void(C::*ACTION)();
struct VE{short delta,pad;void(*fn)();};
struct COUNT{int a,b,c;};
struct W{char pad[0x158];COUNT*count;};
struct P{char pad0[12];VE*vt;char pad10[0x29C-0x10];int guard;char pad2A0[0x4C8-0x2A0];int mode;char pad4CC[8];P*delegate;char pad4D8[0xFBC-0x4D8];int state;char padFC0[0x11A8-0xFC0];W*widget;};
struct RECORD{P*object;int pad4,pad8;int flags;int delta;char pad14[0x30-0x14];int a,b,c;char pad3C[0x60-0x3C];}__attribute__((aligned(16)));
struct C{P*actor;int kind;char pad8[8];int*counter;char pad14[8];ACTION action;char pad24[0x30-0x24];RECORD record;int mode;float time,changed;char pad9C[0x108-0x9C];void*effect;void*output;
 void Reaction() __asm__("JtHurtEndCb");
 void Special() __asm__("func_001A2028");
 void Stop() __asm__("func_001A1FA8");
 void Finish() __asm__("ReturnJtToSafePlaceCb");
};
struct CLOCK{float time,dt;};
struct G{char pad[0x4038];int channel;};
extern "C"{
 extern CLOCK D_002DDED0;
 extern char D_002DE578[],D_002DEB30[];
 extern G D_003DD490;
 extern qword D_002A2DD0,D_002BFB10;
 int func_001A2358(C*,RECORD*);
 int func_00155228(void*,int);
 void func_001305B8(void*,P*,float,float,int,void**);
 int FIsBasicDerivedFrom__FP5BASIC3CID(P*,int);
 void func_00143380(P*);
 void BroadcastGameEvent(void*,int,P*);
 void func_001A15B0(C*,ACTION);
 void func_001A16C8(C*);
 void SetJtHurtState(C*,int);
 int CidBasic(P*);
 void*func_001FBB90(int);
 void func_001FB2F8(void*,P*);
 qword func_001A62E8(P*);
 void*func_00156698(int,qword,P*);
}
static inline int Add32(int a,int b){return (int)((unsigned)a+(unsigned)b);}
static inline int Reject(P*p,int flags){VE*v=&p->vt[76];return ((int(*)(P*,int))v->fn)((P*)((char*)p+v->delta),flags);}
static inline int Delegate(P*p,RECORD*r){VE*v=&p->vt[75];return ((int(*)(P*,RECORD*))v->fn)((P*)((char*)p+v->delta),r);}
static inline qword Vector(P*p,int slot){VE*v=&p->vt[slot];return ((qword(*)(P*))v->fn)((P*)((char*)p+v->delta));}
static inline void SetVector(P*p,int slot,qword value){VE*v=&p->vt[slot];((void(*)(P*,qword))v->fn)((P*)((char*)p+v->delta),value);}
static inline void Notify(P*p,int value){VE*v=&p->vt[88];((void(*)(P*,int))v->fn)((P*)((char*)p+v->delta),value);}
static inline float Z(qword value){register int word __asm__("$9");float result;__asm__("pextuw %0,$zero,%1":"=r"(word):"r"(value));__asm__("mtc1 %1,%0":"=f"(result):"r"(word));return result;}
typedef char RECORD_Size[sizeof(RECORD)==96?1:-1];
typedef char ACTION_Size[sizeof(ACTION)==8?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==0?1:-1];
typedef char C_kind_Offset[FIELD_OFFSET(C,kind)==4?1:-1];
typedef char C_counter_Offset[FIELD_OFFSET(C,counter)==16?1:-1];
typedef char C_action_Offset[FIELD_OFFSET(C,action)==28?1:-1];
typedef char C_record_Offset[FIELD_OFFSET(C,record)==48?1:-1];
typedef char C_mode_Offset[FIELD_OFFSET(C,mode)==144?1:-1];
typedef char C_time_Offset[FIELD_OFFSET(C,time)==148?1:-1];
typedef char C_changed_Offset[FIELD_OFFSET(C,changed)==152?1:-1];
typedef char C_effect_Offset[FIELD_OFFSET(C,effect)==264?1:-1];
typedef char C_output_Offset[FIELD_OFFSET(C,output)==268?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_guard_Offset[FIELD_OFFSET(P,guard)==668?1:-1];
typedef char P_mode_Offset[FIELD_OFFSET(P,mode)==1224?1:-1];
typedef char P_delegate_Offset[FIELD_OFFSET(P,delegate)==1236?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==4028?1:-1];
typedef char P_widget_Offset[FIELD_OFFSET(P,widget)==4520?1:-1];
typedef char W_count_Offset[FIELD_OFFSET(W,count)==344?1:-1];
typedef char COUNT_c_Offset[FIELD_OFFSET(COUNT,c)==8?1:-1];
typedef char RECORD_object_Offset[FIELD_OFFSET(RECORD,object)==0?1:-1];
typedef char RECORD_flags_Offset[FIELD_OFFSET(RECORD,flags)==12?1:-1];
typedef char RECORD_delta_Offset[FIELD_OFFSET(RECORD,delta)==16?1:-1];
typedef char RECORD_a_Offset[FIELD_OFFSET(RECORD,a)==48?1:-1];
typedef char RECORD_b_Offset[FIELD_OFFSET(RECORD,b)==52?1:-1];
typedef char RECORD_c_Offset[FIELD_OFFSET(RECORD,c)==56?1:-1];
typedef char G_channel_Offset[FIELD_OFFSET(G,channel)==16440?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char int_Size[sizeof(int)==4?1:-1];
typedef char unsigned_Size[sizeof(unsigned)==4?1:-1];
typedef char float_Size[sizeof(float)==4?1:-1];
typedef char pointer_Size[sizeof(void*)==4?1:-1];
typedef char short_Size[sizeof(short)==2?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char RECORD_Alignment[__alignof__(RECORD)==16?1:-1];
typedef char C_Prefix_Size[sizeof(C)==0x110?1:-1];
typedef char P_Prefix_Size[sizeof(P)==0x11AC?1:-1];
typedef char W_Prefix_Size[sizeof(W)==0x15C?1:-1];
typedef char COUNT_Size[sizeof(COUNT)==12?1:-1];
typedef char G_Prefix_Size[sizeof(G)==0x403C?1:-1];
extern "C" int FTakeJtDamage(C*p,RECORD*input){
 RECORD accepted;int delta;
 if(func_001A2358(p,input)){
  if((unsigned)p->kind-8u<5u)return 0;
  delta=-32768;goto notify;
 }
 if(Reject(p->actor,input->flags))return 0;
 if(p->actor->mode==1)return Delegate(p->actor->delegate,input);
 if(p->actor->guard)delta=0;
 else if(p->mode==1)delta=0;
 else delta=func_00155228(D_002DE578,input->delta);
 if(input->flags==0x100000){
  if(Add32(*p->counter,delta)>0){
   if(p->mode!=1){p->mode=0;p->time=p->changed=D_002DDED0.time;}
   *p->counter=Add32(*p->counter,delta);
   if(p->effect)func_001305B8(p->effect,p->actor,0.0f,1.0f,1,&p->output);
   goto sound;
  }
  register RECORD*destination __asm__("$2");__asm__("daddu %0,%1,$zero":"=r"(destination):"r"(&accepted));
  *destination=*input;input=&accepted;
  accepted.flags=0x200;
  __asm__("sw $zero,4(%3)\n\tsw $zero,8(%3)\n\tsw $zero,%2":"=m"(accepted.b),"=m"(accepted.c),"=m"(accepted.a):"r"(&accepted.a));
 }
 if(input->object&&FIsBasicDerivedFrom__FP5BASIC3CID(input->object,0x38)){
  if(input->object->state==8||input->object->state==13)delta/=2;
 }
notify:
 {
  Notify(p->actor,3);
  W*widget=p->actor->widget;
  if(widget&&(widget->count->c||Add32(*p->counter,delta)<=0))func_00143380(p->actor);
  BroadcastGameEvent(D_002DEB30,0xF,input->object);
  p->record=*input;p->record.delta=delta;func_001A15B0(p,0);
  int n=Add32(*p->counter,p->record.delta);*p->counter=n>=0?n:0;
  if(*p->counter>0){p->mode=1;p->time=D_002DDED0.time;}
  switch(input->flags){
  case 0x100:case 0x200:{
   func_001A16C8(p);
   if(Z(Vector(p->actor,33))>400.0f){
    int effect=-1;
    switch(CidBasic(p->actor)){case 0x35:effect=0x69;break;case 0x36:effect=0x84;break;case 0x37:effect=0x83;break;}
    P*actor=p->actor;void*created=func_001FBB90(effect);func_001FB2F8(created,actor);
   }
   func_001A15B0(p,&C::Reaction);break;
  }
  case 0x8000:SetJtHurtState(p,7);func_001A15B0(p,&C::Special);break;
  case 0x800:case 0x1000:
   SetVector(p->actor,35,D_002A2DD0);SetVector(p->actor,36,D_002A2DD0);
   SetJtHurtState(p,input->flags==0x800?1:2);func_001A15B0(p,&C::Stop);break;
  case 0x2000:if(*p->counter>0){SetJtHurtState(p,3);func_001A15B0(p,&C::Finish);}else SetJtHurtState(p,10);break;
  case 0x400:SetVector(p->actor,35,D_002BFB10);if(*p->counter>0)SetJtHurtState(p,4);else SetJtHurtState(p,12);break;
  case 0x4000:
   if(*p->counter>0){SetJtHurtState(p,5);func_001A15B0(p,&C::Finish);}else SetJtHurtState(p,11);break;
  case 0x40:func_001A16C8(p);func_001A15B0(p,&C::Reaction);break;
  }
 }
sound:
 register G*game __asm__("$16")=&D_003DD490;
 qword point=func_001A62E8(p->actor);int channel=game->channel;
 (void)func_00156698(channel,point,p->actor);
 return 1;
}
