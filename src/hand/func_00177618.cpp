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
 int func_00177D20(C*);
 int func_00177D48(C*);
 int func_00177DC0(const STATE*,const STATE*);
 void func_00176750(C*,const STATE*);
 void*func_002333B4(void*,int,unsigned int);
 int func_00208B38(P*,P*);
 float func_00208BB0(P*);
 P*func_0020A408(P*,P*);
 void func_001725E0(H*,P*);
 int func_00172550(H*);
 int func_00174FB8(H*);
 int FIsBasicDerivedFrom__FP5BASIC3CID(P*,int);
 int func_00171458(H*);
 int func_00175810(H*);
 P*func_00174898(void*);
 qword func_001A62E8(P*);
 P*func_00156B98(void*,int,qword,P*);
 void func_00156780(P*);
 void func_00176B80(C*,int,P*);
 int func_0017A9C0(H*);
 int func_0017EAE8(H*);
 int func_0017DE40(H*);
 void func_00177BA0(C*);
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
extern "C" void func_00177618(C*p){
 UpdateBhv(p);
 while(!FBhvBusyAbove(p)){
  STATE previous=p->state;
  STATE*current=&p->state;
  switch(p->state.kind){
  case 1:case 16:case 17:{
   STATE next;Clear(&next);
   if(!func_00177D20(p)){
    P*object=Target(p->actor);
    if(object)Attack(p,&next,object);
    else{
     P*selected=func_0020A408(p->actor,0);
     if(selected){next.kind=2;next.payload.s.object=selected;}
     else if(p->state.kind==17&&D_002DDED0.time<p->expires)next.kind=17;
     else{if(func_00177D48(p))next.kind=16;else next.kind=1;}
    }
   }else next.kind=12;
   func_00176750(p,&next);break;
  }
  case 2:{
   if(func_00177D20(p)){STATE next;Clear(&next);next.kind=12;func_00176750(p,&next);}
   else{
    P*object=Target(p->actor);
    if(object){STATE next;Clear(&next);Attack(p,&next,object);func_00176750(p,&next);}
    else{
     H*h=p->helper;P*old=h->target;P*selected=func_0020A408(p->actor,old);
     if(selected!=old)func_001725E0(h,selected);
     else if(func_00172550(h)){STATE next;Clear(&next);next.kind=1;func_00176750(p,&next);}
    }
   }
   break;
  }
  case 3:{
   if(func_00177D20(p)){STATE next;Clear(&next);next.kind=12;func_00176750(p,&next);}
   else if(func_00174FB8(p->helper)){
    STATE next;Clear(&next);if(FIsBasicDerivedFrom__FP5BASIC3CID(p->actor,0x3B)){next.kind=13;next.payload.s.object=p->state.payload.s.object;}else{next.kind=4;next.payload.s.object=p->state.payload.s.object;}func_00176750(p,&next);
   }
   break;
  }
  case 5:if(func_00171458(p->helper))func_00177BA0(p);break;
  case 4:{
   H*h=p->helper;
   if(func_00177D20(p)){STATE next;Clear(&next);next.kind=12;func_00176750(p,&next);}
   else if(func_00175810(h)){STATE next;Clear(&next);next.kind=1;func_00176750(p,&next);}
   else{
    P*object=Target(p->actor);
    if(object!=p->actor->selected){
     STATE next;Clear(&next);
     if(object)Attack(p,&next,object);
     else{
      next.kind=1;P*other=h->object;
      if(other&&FIsBasicDerivedFrom__FP5BASIC3CID(other,0x34)&&Mode(other)==2){
       P*owner=func_00174898(other->extra);
       if(owner&&owner!=p->actor){next.payload.s.object=owner;next.kind=17;}
      }
     }
     func_00176750(p,&next);
    }
   }
   break;
  }
  case 12:{if(!func_00177D20(p)){STATE next;Clear(&next);next.kind=1;func_00176750(p,&next);}break;}
  case 0:{STATE next;Clear(&next);next.kind=1;func_00176750(p,&next);break;}
  case 8:{
   if(func_00174FB8(p->helper)){
    P*object=func_00156B98(D_003DD490,0,func_001A62E8(p->actor),p->actor);
    STATE next;Clear(&next);next.kind=2;next.payload.s.object=object;func_00176750(p,&next);func_00156780(object);
   }
   break;
  }
  case 7:{
   H*h=p->helper;
   if(func_00174FB8(h)){
    if(h->mode!=-1){if(h->mode==3)func_00177BA0(p);}
    else{func_00176B80(p,-50,h->extra);if(p->state.kind==7)func_00177BA0(p);}
   }
   break;
  }
  case 6:if(func_0017A9C0(p->helper))func_00177BA0(p);break;
  case 9:if(func_00174FB8(p->helper))func_00177BA0(p);break;
  case 10:if(func_00174FB8(p->helper))func_00177BA0(p);break;
  case 19:if(func_0017EAE8(p->helper))func_00177BA0(p);break;
  case 18:if(func_0017DE40(p->helper))func_00177BA0(p);break;
  }
  if(func_00177DC0(current,&previous))break;
 }
}
