#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
struct VTE {short delta,pad;void*fn;};
struct PL {char pad[0x1C];float amount;char pad20[0x10];unsigned flags;};
struct CONTROL {int word0,state;char pad8[0x14];int count;};
struct GATE {int word0,word4,mode;};
struct P {char pad[0xC];VTE*vt;char pad10[0x140];PL*input;char pad154[0xB8];float changed;char pad210[0xB0];CONTROL control;char pad2E0[0x1E0];GATE gate;char pad4CC[0x924];char action[0x110];char altAction[0x50];char physics[0xE0];void*animation;void*other;char pad1038[0x398];int state;char pad13D4[0x14];int pending;void*root;char pad13F0[0x5C];int flag;char pad1450[0x3F0];int kind;char pad1844[0xCC];char extra[1];};
struct CLOCK {float time,dt;};
typedef char VTE_Size[sizeof(VTE)==8?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char PL_Size[sizeof(PL)==52?1:-1];
typedef char CONTROL_Size[sizeof(CONTROL)==32?1:-1];
typedef char GATE_Size[sizeof(GATE)==12?1:-1];
typedef char VTE_delta_Offset[FIELD_OFFSET(VTE,delta)==0?1:-1];
typedef char VTE_fn_Offset[FIELD_OFFSET(VTE,fn)==4?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char PL_amount_Offset[FIELD_OFFSET(PL,amount)==28?1:-1];
typedef char PL_flags_Offset[FIELD_OFFSET(PL,flags)==48?1:-1];
typedef char CONTROL_state_Offset[FIELD_OFFSET(CONTROL,state)==4?1:-1];
typedef char CONTROL_count_Offset[FIELD_OFFSET(CONTROL,count)==28?1:-1];
typedef char GATE_mode_Offset[FIELD_OFFSET(GATE,mode)==8?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_input_Offset[FIELD_OFFSET(P,input)==336?1:-1];
typedef char P_changed_Offset[FIELD_OFFSET(P,changed)==524?1:-1];
typedef char P_control_Offset[FIELD_OFFSET(P,control)==704?1:-1];
typedef char P_gate_Offset[FIELD_OFFSET(P,gate)==1216?1:-1];
typedef char P_action_Offset[FIELD_OFFSET(P,action)==3568?1:-1];
typedef char P_altAction_Offset[FIELD_OFFSET(P,altAction)==3840?1:-1];
typedef char P_physics_Offset[FIELD_OFFSET(P,physics)==3920?1:-1];
typedef char P_animation_Offset[FIELD_OFFSET(P,animation)==4144?1:-1];
typedef char P_other_Offset[FIELD_OFFSET(P,other)==4148?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==5072?1:-1];
typedef char P_pending_Offset[FIELD_OFFSET(P,pending)==5096?1:-1];
typedef char P_root_Offset[FIELD_OFFSET(P,root)==5100?1:-1];
typedef char P_flag_Offset[FIELD_OFFSET(P,flag)==5196?1:-1];
typedef char P_kind_Offset[FIELD_OFFSET(P,kind)==6208?1:-1];
typedef char P_extra_Offset[FIELD_OFFSET(P,extra)==6416?1:-1];
extern "C" CLOCK D_002DDED0;
extern "C" float D_002C08DC,D_002C08E0;
extern "C" void func_00195A00(void*,int,void*,void*);
extern "C" void func_0019CC50(void*,int,void*,void*);
extern "C" int func_00140AF8(P*,int);
extern "C" void func_001B6078(P*,int);
extern "C" unsigned func_001C46C0(PL*,unsigned,float);
extern "C" unsigned func_001C4660(PL*,unsigned,float);
extern "C" void func_001C4738(PL*,int);
extern "C" int func_001B84E0(P*);
extern "C" void func_001B85D0(P*);
extern "C" void func_001B8508(P*);
extern "C" int func_001B70A0(P*);
extern "C" int func_001938C0(void*,unsigned,int);
extern "C" void func_00197500(void*);
extern "C" int func_00143D20(P*,float);
extern "C" void func_001911A8(void*);
extern "C" void func_001A12F0(void*);
extern "C" void func_001B6BA8(P*);
extern "C" void func_001B5380(P*);
extern "C" void func_001B5820(P*);
extern "C" void func_001B92F8(P*);
extern "C" int func_00131968(void*);
extern "C" int func_001F0638(void*);
extern "C" void func_001B8988(P*);
extern "C" void func_001B5990(P*);
extern "C" void func_001B8EC0(P*);
extern "C" void func_0021F418(void*);
extern "C" void func_0019E1E0(void*);
extern "C" int func_00141EA8(P*,int);
static inline PL*CopyPL(PL*source){PL*result;__asm__("daddu %0,%1,$zero":"=r"(result):"r"(source));return result;}
static inline int Can(P*p,int kind){return ((int(*)(void*,int))p->vt[99].fn)((char*)p+p->vt[99].delta,kind);}
/** @sly2-annot addr=0x1B4AA0
 * Purpose: Per-frame input-driven state machine (loops until state stable): idle/move -> jump (Cross 0x40),
 *   lift (Circle 0x20), punch (Square 0x80), alt attack (0x10), gadget (0x400 + FCanUseGadget(3)), strafe
 *   (0x140AF8); dispatches per-state updates (carry, flop, lift, attack, power, struggle, jtthrow)
 * Info: proposed name UpdateMurrayStateMachine (T1); TU murray
 * Notes: runtime4: the per-state switch (no stored per-state fn pointers in MURRAY); 0x400 = look mode
 *   (JTMODE), not gadget | runtime4 CORRECTION: pad bit 0x400 = look mode (JTMODE); gadgets are slot buttons
 *   0x4/0x1/0x2
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r5b_rest+runtime+runtime-corrected
 * See: docs/engine/player.md#6-murray-murraycpp-0x1b4728, docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
extern "C" void func_001B4AA0(P*p){
 int state;
 do{
  state=p->state;
  switch(state){
  case 2:case 3:case 4:case 6:case 7:case 8:case 14:case 15:case 16:case 17:case 18:case 20:break;
  default:
   if(D_002DDED0.time-p->changed>0.1f){
    switch(p->state){
    case 5:func_00195A00(&p->control,16,0,0);break;
    case 12:if(p->kind==3)func_00195A00(&p->control,11,0,0);break;
    case 13:if(p->gate.mode!=1&&p->gate.mode!=12)func_00195A00(&p->control,0,0,0);break;
    default:func_00195A00(&p->control,0,0,0);break;
    }
   }
  }
  int finished;
  switch(p->state){
  case 0:case 1:case 10:{
   if(!(p->input->flags&8)&&func_00140AF8(p,0)){func_001B6078(p,9);break;}
   if(func_001C46C0(p->input,0x40,0.1f)){func_00195A00(&p->control,1,0,0);break;}
   if(func_001C4660(p->input,0x400,0.15f)&&Can(p,3)){
    func_001C4738(p->input,0x400);
    func_0019CC50(p->altAction,0,0,0);
    break;
   }
   if(func_001B84E0(p)&&func_001C46C0(p->input,0x10,D_002C08DC)){func_001B85D0(p);break;}
   if(func_001B70A0(p))break;
   if(func_001C46C0(p->input,0x20,0.2f)){
    if(!func_001938C0(&p->gate,0xF8FA,0)&&p->root){p->flag=0;func_001B6078(p,4);}
    break;
   }
   if(func_001B84E0(p)&&func_001C46C0(p->input,0x80,D_002C08DC)){func_001B8508(p);break;}
   PL*input=p->input;PL*flagsInput=CopyPL(input);register int current __asm__("$3")=p->state;
   if(current==0){if(input->amount>0.1f)goto moving;}
   else if(input->amount>0.0f)goto moving;
   goto idle;
  moving:
   if(flagsInput->flags&8)func_001B6078(p,1);else func_001B6078(p,10);
   break;
  }
  case 2:
   func_00197500(&p->control);
   switch(p->control.state){case 0:case 11:case 15:case 16:break;
   default:
    if(func_001C4660(p->input,0x90,D_002C08DC)&&Can(p,2)&&p->control.count<6&&p->control.count>0&&!func_00143D20(p,D_002C08E0)){
     func_001C4738(p->input,0x90);
     p->pending=0;
     func_001B6078(p,3);
    }
   }
   break;
  case 13:func_001911A8(&p->gate);break;
  case 14:func_001A12F0(p->action);break;
  case 3:func_001B6BA8(p);break;
  case 5:func_001B5380(p);break;
  case 6:func_001B5820(p);break;
  case 4:func_001B92F8(p);break;
  case 7:case 8:finished=func_00131968(p->animation);goto checkIdle;
  case 11:func_001B8988(p);break;
  case 9:func_001B5990(p);break;
  case 12:func_001B8EC0(p);break;
  case 15:func_0021F418(p->extra);break;
  case 20:func_0019E1E0(p->physics);break;
  case 17:
   if(p->animation&&func_00131968(p->animation))goto idle;
   if(!p->other)break;
   finished=func_001F0638(p->other);
  checkIdle:
   if(!finished)break;
  idle:
   func_001B6078(p,0);
   break;
  case 21:
   if(func_001C46C0(p->input,0x40,0.0f))func_00195A00(&p->control,13,0,0);
   else if(!(p->input->flags&func_00141EA8(p,0x10)))func_001B6078(p,0);
   break;
  default:break;
  }
 }while(p->state!=state);
}
