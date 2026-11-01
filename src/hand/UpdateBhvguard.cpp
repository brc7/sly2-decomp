#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
struct P;
struct VE{short delta,pad;void(*fn)();};
struct P{char pad0[12];VE*vt;char pad10[0x38C-0x10];P*selected;char pad390[0xE20-0x390];void*extra;};
struct H{char pad0[0x30];P*object;char pad34[16];P*target;char pad48[8];int mode;char pad54[8];P*extra;};
struct STATE{int kind,pad4,pad8,padC;union{qword data[6];struct{P*object;float amount;char pad18[0x60-8];}s;}payload;};
struct C{char pad0[16];P*actor;int pad14;H*helper;char pad1C[0x30-0x1C];STATE state;float started,expires;};
struct CLOCK{float time,dt;};
extern "C"{
 extern CLOCK D_002DDED0;
 extern char D_003DD490[];
 void UpdateBhv(C*);
 int FBhvBusyAbove(C*);
 int FBhvguardHasTargetList(C*);
 int FBhvguardEscortActive(C*);
 int FBhvguardModeSame(const STATE*,const STATE*);
 void SetBhvguardMode(C*,const STATE*);
 void*func_002333B4(void*,int,unsigned int);
 int func_00208B38(P*,P*);
 float func_00208BB0(P*);
 P*func_0020A408(P*,P*);
 void func_001725E0(H*,P*);
 int func_00172550(H*);
 int FBhvanimDone(H*);
 int FIsBasicDerivedFrom__FP5BASIC3CID(P*,int);
 int func_00171458(H*);
 int FBhvcombatDone(H*);
 P*PstepguardFromLo(void*);
 qword func_001A62E8(P*);
 P*EmitSwNoise(void*,int,qword,P*);
 void func_00156780(P*);
 void InflictBhvguardZap(C*,int,P*);
 int func_0017A9C0(H*);
 int func_0017EAE8(H*);
 int func_0017DE40(H*);
 void ResumeBhvguard(C*);
}
static inline P*Target(P*p){VE*v=&p->vt[88];return ((P*(*)(P*))v->fn)((P*)((char*)p+v->delta));}
static inline int Mode(P*p){VE*v=&p->vt[77];return ((int(*)(P*))v->fn)((P*)((char*)p+v->delta));}
static inline void Clear(STATE*s){func_002333B4(s,0,sizeof(*s));}
static inline void Attack(C*p,STATE*s,P*object){
 if(func_00208B38(p->actor,object)){s->kind=18;s->payload.s.object=object;s->payload.s.amount=func_00208BB0(p->actor);}
 else{s->kind=3;s->payload.s.object=object;}
}
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char STATE_Size[sizeof(STATE)==112?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char VE_delta_Offset[FIELD_OFFSET(VE,delta)==0?1:-1];
typedef char VE_fn_Offset[FIELD_OFFSET(VE,fn)==4?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_selected_Offset[FIELD_OFFSET(P,selected)==908?1:-1];
typedef char P_extra_Offset[FIELD_OFFSET(P,extra)==3616?1:-1];
typedef char H_object_Offset[FIELD_OFFSET(H,object)==48?1:-1];
typedef char H_target_Offset[FIELD_OFFSET(H,target)==68?1:-1];
typedef char H_mode_Offset[FIELD_OFFSET(H,mode)==80?1:-1];
typedef char H_extra_Offset[FIELD_OFFSET(H,extra)==92?1:-1];
typedef char STATE_kind_Offset[FIELD_OFFSET(STATE,kind)==0?1:-1];
typedef char STATE_payload_Offset[FIELD_OFFSET(STATE,payload)==16?1:-1];
typedef char STATE_payload_s_object_Offset[FIELD_OFFSET(STATE,payload.s.object)==16?1:-1];
typedef char STATE_payload_s_amount_Offset[FIELD_OFFSET(STATE,payload.s.amount)==20?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==16?1:-1];
typedef char C_helper_Offset[FIELD_OFFSET(C,helper)==24?1:-1];
typedef char C_state_Offset[FIELD_OFFSET(C,state)==48?1:-1];
typedef char C_started_Offset[FIELD_OFFSET(C,started)==160?1:-1];
typedef char C_expires_Offset[FIELD_OFFSET(C,expires)==164?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
extern "C" void UpdateBhvguard(C*p){
 UpdateBhv(p);
 while(!FBhvBusyAbove(p)){
  STATE previous=p->state;
  STATE*current=&p->state;
  switch(p->state.kind){
  case 1:case 16:case 17:{
   STATE next;Clear(&next);
   if(!FBhvguardHasTargetList(p)){
    P*object=Target(p->actor);
    if(object)Attack(p,&next,object);
    else{
     P*selected=func_0020A408(p->actor,0);
     if(selected){next.kind=2;next.payload.s.object=selected;}
     else if(p->state.kind==17&&D_002DDED0.time<p->expires)next.kind=17;
     else{if(FBhvguardEscortActive(p))next.kind=16;else next.kind=1;}
    }
   }else next.kind=12;
   SetBhvguardMode(p,&next);break;
  }
  case 2:{
   if(FBhvguardHasTargetList(p)){STATE next;Clear(&next);next.kind=12;SetBhvguardMode(p,&next);}
   else{
    P*object=Target(p->actor);
    if(object){STATE next;Clear(&next);Attack(p,&next,object);SetBhvguardMode(p,&next);}
    else{
     H*h=p->helper;P*old=h->target;P*selected=func_0020A408(p->actor,old);
     if(selected!=old)func_001725E0(h,selected);
     else if(func_00172550(h)){STATE next;Clear(&next);next.kind=1;SetBhvguardMode(p,&next);}
    }
   }
   break;
  }
  case 3:{
   if(FBhvguardHasTargetList(p)){STATE next;Clear(&next);next.kind=12;SetBhvguardMode(p,&next);}
   else if(FBhvanimDone(p->helper)){
    STATE next;Clear(&next);if(FIsBasicDerivedFrom__FP5BASIC3CID(p->actor,0x3B)){next.kind=13;next.payload.s.object=p->state.payload.s.object;}else{next.kind=4;next.payload.s.object=p->state.payload.s.object;}SetBhvguardMode(p,&next);
   }
   break;
  }
  case 5:if(func_00171458(p->helper))ResumeBhvguard(p);break;
  case 4:{
   H*h=p->helper;
   if(FBhvguardHasTargetList(p)){STATE next;Clear(&next);next.kind=12;SetBhvguardMode(p,&next);}
   else if(FBhvcombatDone(h)){STATE next;Clear(&next);next.kind=1;SetBhvguardMode(p,&next);}
   else{
    P*object=Target(p->actor);
    if(object!=p->actor->selected){
     STATE next;Clear(&next);
     if(object)Attack(p,&next,object);
     else{
      next.kind=1;P*other=h->object;
      if(other&&FIsBasicDerivedFrom__FP5BASIC3CID(other,0x34)&&Mode(other)==2){
       P*owner=PstepguardFromLo(other->extra);
       if(owner&&owner!=p->actor){next.payload.s.object=owner;next.kind=17;}
      }
     }
     SetBhvguardMode(p,&next);
    }
   }
   break;
  }
  case 12:{if(!FBhvguardHasTargetList(p)){STATE next;Clear(&next);next.kind=1;SetBhvguardMode(p,&next);}break;}
  case 0:{STATE next;Clear(&next);next.kind=1;SetBhvguardMode(p,&next);break;}
  case 8:{
   if(FBhvanimDone(p->helper)){
    P*object=EmitSwNoise(D_003DD490,0,func_001A62E8(p->actor),p->actor);
    STATE next;Clear(&next);next.kind=2;next.payload.s.object=object;SetBhvguardMode(p,&next);func_00156780(object);
   }
   break;
  }
  case 7:{
   H*h=p->helper;
   if(FBhvanimDone(h)){
    if(h->mode!=-1){if(h->mode==3)ResumeBhvguard(p);}
    else{InflictBhvguardZap(p,-50,h->extra);if(p->state.kind==7)ResumeBhvguard(p);}
   }
   break;
  }
  case 6:if(func_0017A9C0(p->helper))ResumeBhvguard(p);break;
  case 9:if(FBhvanimDone(p->helper))ResumeBhvguard(p);break;
  case 10:if(FBhvanimDone(p->helper))ResumeBhvguard(p);break;
  case 19:if(func_0017EAE8(p->helper))ResumeBhvguard(p);break;
  case 18:if(func_0017DE40(p->helper))ResumeBhvguard(p);break;
  }
  if(FBhvguardModeSame(current,&previous))break;
 }
}
