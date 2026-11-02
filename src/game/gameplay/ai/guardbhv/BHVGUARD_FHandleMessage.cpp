// Handle movement-state response events.
typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct VE{short delta,pad;void*fn;};
struct ASEG;struct ASEGA;struct TUNE;
struct OBJECT{
 char pad0[0xC];VE*vt;
 char pad10[0x28C];int suppressAmount;
 char pad2A0[0x9C];int reserve,capacity,progress;ASEG*clip;ASEGA*animation;
 unsigned mask9,mask10,mask11;char pad35C[4];float animationTime;
 char pad364[0x28];OBJECT*selected;
 char pad390[0x3C];TUNE*tuning;
 char pad3D0[0xD4];int extraGate;
 char pad4A8[0x9DC];int otherCapacity,otherProgress;ASEG*otherClip;ASEGA*otherAnimation;
 char padE94[0x128];int mode;
 char padFC0[0x410];int state;
 char pad13D4[0x68];int notifyEnabled;
};
struct MSGDATA{
 OBJECT*object;int word4,word8;unsigned kind;int amount;
 char pad14[0x1C];qword direction;int word40;int flags;char pad48[0x18];
};
union MSG{qword q[6];MSGDATA data;};
struct STATE{int kind,pad4,pad8,padC;MSG msg;};
struct HELPER{char pad0[0x58];float until;unsigned flags;};
struct S{char pad0[0x10];OBJECT*actor;int pad14;HELPER*helper;char pad1C[0x14];STATE current;};
struct SLOT{int kind;char pad4[0xC];};
struct M4{qword r[4];};
struct CLOCK{float t,dt;};
struct WORK{STATE next;U scalar,own,other,copy,delta;};
struct OFFSETINPUTS{qword own,other,copy;};
extern "C"{
extern OBJECT*D_002F7900;
extern void*D_003E14BC;extern void*D_003E14B0;extern void*D_003E14B8;struct GAMESTATE{char pad[0x4028];void*context;};extern GAMESTATE D_003DD490;
extern CLOCK D_002DDED0;extern float D_002BE620;
void*func_002333B4(void*,int,unsigned);
void SetBhvguardMode(S*,STATE*);
qword func_001A62E8(void*);M4*func_001A6318(void*);
qword ConvertAloVec(void*,void*,qword);
int FIsBasicDerivedFrom__FP5BASIC3CID(void*,int);
void func_001B9DB8(OBJECT*,qword);
void func_001B9D28(OBJECT*,qword);
OBJECT*func_00156698(void*,qword,OBJECT*);
void SetStepguardSpecial(OBJECT*,int);
void func_001F7A68(OBJECT*,int);
unsigned GrfzpdFromSoGrfzpk(OBJECT*,unsigned);
int func_001F78A0(unsigned);
void func_0020A9A8(TUNE*,int);
void func_0017A5B0(HELPER*,int);
void ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(ASEG*,OBJECT*,float,float,int,ASEGA**);
unsigned long GrfFromSgf(int);
int func_0020A378(OBJECT*,OBJECT*,unsigned long);
int func_0020A298(OBJECT*,OBJECT*,unsigned long);
void*PanimbindBest(OBJECT*,unsigned long);
int BHV_FHandleMessage(S*,int,MSG*);
}
static __inline__ void Clear(STATE*s){(void)func_002333B4(s,0,sizeof(*s));}
static __inline__ int Accept(OBJECT*p,MSG*m){VE&v=p->vt[86];return((int(*)(void*,MSG*))v.fn)((char*)p+v.delta,m);}
static __inline__ SLOT*Slot(OBJECT*p,unsigned kind,int offset){int index=func_001F78A0(kind);return(SLOT*)((char*)p+offset+index*sizeof(SLOT));}
static __inline__ float Max(float a,float b){register float right __asm__("$f1")=b;__asm__("max.s %0,%0,%1":"+f"(a):"f"(right));return a;}
static __inline__ qword Raw(float value){register qword bits __asm__("$10");__asm__("mfc1 %0,%1":"=r"(bits):"f"(value));return bits;}
static __inline__ qword Scalar(float value){int bits;qword result;__asm__("mfc1 %0,%1":"=r"(bits):"f"(value));__asm__("qmtc2.ni %1,%0":"=j"(result):"r"(bits));return result;}
static __inline__ qword Normalize(qword input){
 qword norm,one,result;
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tvsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=&j"(norm),"=&j"(one):"j"(input));
 __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(result):"j"(norm),"j"(input));
 return result;
}
static __inline__ qword ScaleInPlace(qword a,qword s){__asm__("vmulx.xyzw %0,%1,%0x":"+j"(s):"j"(a));return s;}
static __inline__ int Below(int a,int b){return a<b;}
enum KINDENUM{KIND_NONE=-1,KIND_MAX=0x7fffffff};
/** @sly2-annot addr=0x176BE8
 * Purpose: BHVGUARD message handler: msg 12 (zap/damage) -> choose hurt/knockdown/KO by zap kind, reaction
 *   tables +0x8D0/+0xA70/+0xC10 and hit points; special zaps 0x80000 sleep, 0x200000 grab (held),
 *   0x1000000/0x2000000 shrink/rage gadgets, 0x4000/0x2000 relocate; emits noise
 * Info: proposed name BHVGUARD_FHandleMessage (T1); class BHVGUARD slot13; TU guardbhv
 * Notes: runtime4: BHVGUARD zap handler: zap 0x200000 -> hp=max(hp+dhp,5), mode 7 (S38) | runtime4
 *   CORRECTION: pad bit 0x400 = look mode (JTMODE); gadgets are slot buttons 0x4/0x1/0x2 | runtime2/4
 *   confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime+runtime-corrected+verdicts_followups+runtime(class role)
 * See: docs/engine/guards-ai.md#3-the-behaviour-stack-and-the-guard-modes,
 *   docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
extern "C" int BHVGUARD_FHandleMessage(S*p,int event,MSG*msg){
 WORK w;int flags;int originalFlags;
 if(event==12){
 MSG*selected=msg;
 if(Accept(p->actor,msg)){
  if(D_002F7900&&msg->data.object==D_002F7900&&msg->data.kind==0x40000&&msg->data.object->notifyEnabled)
   func_001B9DB8(D_002F7900,func_001A62E8(p->actor));
  if(msg->data.kind==0x40000){void*context=D_003E14BC;qword position=func_001A62E8(p->actor);(void)func_00156698(context,position,p->actor);}
  Clear(&w.next);w.next.kind=11;w.next.msg=*msg;w.next.msg.data.flags=0x40;
  SetBhvguardMode(p,&w.next);return 1;
 }
 switch(p->actor->mode){case 14:SetStepguardSpecial(p->actor,-1);break;case 8:case 13:msg->data.amount<<=2;break;}
 if(p->actor->suppressAmount)msg->data.amount=0;
 if(p->actor->extraGate&&p->current.kind!=11&&p->current.kind!=5&&(msg->data.kind==0x4000||msg->data.kind==0x2000)){
  func_001F7A68(p->actor,0);Clear(&w.next);w.next.kind=19;SetBhvguardMode(p,&w.next);return 1;
 }
 switch((KINDENUM)p->current.kind){
 case 9:
  if(!(GrfzpdFromSoGrfzpk(p->actor,msg->data.kind)&0x40)){
   if(!(msg->data.kind&p->actor->mask9)||(p->helper->flags&p->actor->mask9))return 1;
  }break;
 case 10:if(!(msg->data.kind&p->actor->mask10))return 1;break;
 case 11:
  if(!(msg->data.kind&p->actor->mask11))return 1;
  if(p->helper->flags&p->actor->mask11)return 1;
  break;
 }
 if(p->actor->selected==msg->data.object)func_0020A9A8(p->actor->tuning,5);
 if(msg->data.kind==0x80000&&(msg->data.flags&8)){
  if(p->current.kind==8){
   HELPER*h=p->helper;func_0017A5B0(h,0);h->until=Max(D_002DDED0.t+D_002BE620,h->until);
  }else{
   p->actor->otherProgress+=msg->data.amount;
   if(p->actor->otherProgress<=0){
    p->actor->otherProgress=p->actor->otherCapacity;Clear(&w.next);w.next.kind=8;SetBhvguardMode(p,&w.next);
   }else ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(p->actor->otherClip,p->actor,0.0f,1.0f,1,&p->actor->otherAnimation);
  }goto finish;
 }
 if(msg->data.kind==0x200000&&(msg->data.flags&8)){
  if(func_0020A378(p->actor,msg->data.object,GrfFromSgf(6))){
   {int reserve=p->actor->reserve+msg->data.amount;int limited=4<reserve?reserve:5;p->actor->reserve=limited;}
   (void)func_00156698(D_003DD490.context,func_001A62E8(p->actor),p->actor);
   Clear(&w.next);w.next.kind=7;w.next.msg.data.object=msg->data.object;SetBhvguardMode(p,&w.next);goto finish;
  }
 }
 if(msg->data.kind==0x1000000&&(msg->data.flags&8)){
  Clear(&w.next);if(func_0020A298(p->actor,msg->data.object,GrfFromSgf(0x17)))w.next.kind=10;else w.next.kind=9;
  w.next.msg=*msg;p->current.kind=-1;SetBhvguardMode(p,&w.next);SetStepguardSpecial(p->actor,8);goto finish;
 }
 if(msg->data.kind==0x2000000&&(msg->data.flags&8)){
  Clear(&w.next);if(func_0020A298(p->actor,msg->data.object,GrfFromSgf(0x17)))w.next.kind=10;else w.next.kind=9;
  w.next.msg=*msg;p->current.kind=-1;SetBhvguardMode(p,&w.next);SetStepguardSpecial(p->actor,0x22);goto finish;
 }
 originalFlags=msg->data.flags;
 if(!(originalFlags&0xF0))goto finish;
 flags=originalFlags;
 Clear(&w.next);
 {
  void*context=msg->data.kind==0x40000?D_003E14BC:D_003E14B0;
  qword position=func_001A62E8(p->actor);(void)func_00156698(context,position,p->actor);
 }
 if(Below(p->current.kind,11)&&!Below(p->current.kind,8)&&FIsBasicDerivedFrom__FP5BASIC3CID(msg->data.object,0x65)&&(originalFlags&0xB0))flags=originalFlags|0x40;
 if(func_0020A298(p->actor,msg->data.object,GrfFromSgf(0x17)))flags|=0x80;
 if(flags&0x40)flags&=~0xB0;
 if(flags&0xA0)flags&=~0x10;
 if(flags&0x80){
  flags&=~0x20;
  int type=Slot(p->actor,msg->data.kind,0x8D0)->kind;
  if(type==-1&&Slot(p->actor,msg->data.kind,0xA70)->kind!=type){flags&=~0x80;flags|=0x20;}
  if(Slot(p->actor,msg->data.kind,0x8D0)->kind==-1&&!PanimbindBest(p->actor,GrfFromSgf(6))){flags&=~0x80;flags|=0x20;}
 }
 if(flags&0x10){
  if(p->actor->capacity>0){
   p->actor->progress+=msg->data.amount;
   if(p->actor->progress>0){
    ApplyAsegCur__FP4ASEGP3ALOffiPP5ASEGA(p->actor->clip,p->actor,0.0f,1.0f,1,&p->actor->animation);
    p->actor->animationTime=D_002DDED0.t;
   }else flags|=0x20;
  }else flags|=0x20;
 }
 if(flags&0xA0){
  p->actor->progress=p->actor->capacity;
  int delta=p->actor->capacity>0?-p->actor->capacity:msg->data.amount;
  int reserve=p->actor->reserve+delta;p->actor->reserve=reserve<0?0:reserve;
  if(p->actor->reserve>0){w.next.kind=(flags&0x80)?10:9;w.next.msg=*msg;selected=&w.next.msg;}
  else flags|=0x40;
 }
 if(flags&0x40){w.next.kind=11;w.next.msg=*selected;selected=&w.next.msg;}
 if(flags&0xE0){
  if(FIsBasicDerivedFrom__FP5BASIC3CID(selected->data.object,0x34)){
   qword own=func_001A62E8(p->actor);OBJECT*otherObject=selected->data.object;w.own.q=own;w.other.q=func_001A62E8(otherObject);
   qword scalar=Raw(-150.0f);w.copy.q=scalar;OBJECT*frameObject=selected->data.object;w.scalar.q=scalar;
   M4*frame=func_001A6318(frameObject);
   int special;
   {
    register qword axis __asm__("vf1"),origin __asm__("vf2"),delta __asm__("vf3");
    register OBJECT*receiver __asm__("$4");
    register int queryKind __asm__("$5");
    const int targetKind=0x36;
    const OFFSETINPUTS&inputs=*(const OFFSETINPUTS*)&w.own;
    __asm__("lqc2 %0,%5\n\taddiu %4,$zero,%8\n\tlqc2 %1,16+%6\n\tlqc2 %2,32+%6\n\tvadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%0,%2x\n\tlw %3,%7\n\tlqc2 %1,%6\n\tvsub.xyzw %2,%1,%0"
      :"=&j"(axis),"=&j"(origin),"=&j"(delta),"=&r"(receiver),"=&r"(queryKind)
      :"m"(frame->r[0]),"m"(inputs),"m"(selected->data.object),"i"(targetKind));
    w.delta.q=delta;
    special=FIsBasicDerivedFrom__FP5BASIC3CID(receiver,queryKind);
   }
   qword direction=w.delta.q;
   if(special&&selected->data.object->state==12){
    OBJECT*object=selected->data.object;
    *(qword*)&w.scalar=ConvertAloVec(0,object,direction);w.scalar.f[0]*=0.25f;
    direction=ConvertAloVec(object,0,w.scalar.q);
   }
   qword normalized=Normalize(direction);*(qword*)&w.scalar=Scalar(500.0f);
   selected->data.direction=ScaleInPlace(normalized,w.scalar.q);
  }else if(p->actor->mode==-1&&FIsBasicDerivedFrom__FP5BASIC3CID(selected->data.object,0x38)&&selected->data.object->mode==0x22)SetStepguardSpecial(p->actor,0x22);
  p->current.kind=-1;SetBhvguardMode(p,&w.next);
 }
finish:
 if(D_002F7900&&selected->data.kind==0x20000)func_001B9D28(D_002F7900,func_001A62E8(p->actor));
 return 1;
 }
 return BHV_FHandleMessage(p,event,msg);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_mask[(sizeof(unsigned long)==8)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_U[(sizeof(U)==16)?1:-1];
typedef char layout_VE[(sizeof(VE)==8)?1:-1];
typedef char layout_MSGDATA[(sizeof(MSGDATA)==0x60)?1:-1];
typedef char layout_MSG[(sizeof(MSG)==0x60)?1:-1];
typedef char layout_STATE[(sizeof(STATE)==0x70)?1:-1];
typedef char layout_HELPER[(sizeof(HELPER)==0x60)?1:-1];
typedef char layout_SLOT[(sizeof(SLOT)==0x10)?1:-1];
typedef char layout_M4[(sizeof(M4)==0x40)?1:-1];
typedef char layout_CLOCK[(sizeof(CLOCK)==8)?1:-1];
typedef char layout_U_q[((unsigned long)&((U*)0)->q==0x0)?1:-1];
typedef char layout_U_f[((unsigned long)&((U*)0)->f==0x0)?1:-1];
typedef char layout_VE_delta[((unsigned long)&((VE*)0)->delta==0x0)?1:-1];
typedef char layout_VE_fn[((unsigned long)&((VE*)0)->fn==0x4)?1:-1];
typedef char layout_MSGDATA_object[((unsigned long)&((MSGDATA*)0)->object==0x0)?1:-1];
typedef char layout_MSGDATA_kind[((unsigned long)&((MSGDATA*)0)->kind==0xc)?1:-1];
typedef char layout_MSGDATA_amount[((unsigned long)&((MSGDATA*)0)->amount==0x10)?1:-1];
typedef char layout_MSGDATA_direction[((unsigned long)&((MSGDATA*)0)->direction==0x30)?1:-1];
typedef char layout_MSGDATA_flags[((unsigned long)&((MSGDATA*)0)->flags==0x44)?1:-1];
typedef char layout_MSG_q[((unsigned long)&((MSG*)0)->q==0x0)?1:-1];
typedef char layout_MSG_data[((unsigned long)&((MSG*)0)->data==0x0)?1:-1];
typedef char layout_STATE_kind[((unsigned long)&((STATE*)0)->kind==0x0)?1:-1];
typedef char layout_STATE_msg[((unsigned long)&((STATE*)0)->msg==0x10)?1:-1];
typedef char layout_HELPER_until[((unsigned long)&((HELPER*)0)->until==0x58)?1:-1];
typedef char layout_HELPER_flags[((unsigned long)&((HELPER*)0)->flags==0x5c)?1:-1];
typedef char layout_S_actor[((unsigned long)&((S*)0)->actor==0x10)?1:-1];
typedef char layout_S_helper[((unsigned long)&((S*)0)->helper==0x18)?1:-1];
typedef char layout_S_current[((unsigned long)&((S*)0)->current==0x30)?1:-1];
typedef char layout_SLOT_kind[((unsigned long)&((SLOT*)0)->kind==0x0)?1:-1];
typedef char layout_CLOCK_t[((unsigned long)&((CLOCK*)0)->t==0x0)?1:-1];
typedef char layout_CLOCK_dt[((unsigned long)&((CLOCK*)0)->dt==0x4)?1:-1];
typedef char layout_WORK_next[((unsigned long)&((WORK*)0)->next==0x0)?1:-1];
typedef char layout_WORK_scalar[((unsigned long)&((WORK*)0)->scalar==0x70)?1:-1];
typedef char layout_OBJECT_vt[((unsigned long)&((OBJECT*)0)->vt==0xc)?1:-1];
typedef char layout_OBJECT_suppressAmount[((unsigned long)&((OBJECT*)0)->suppressAmount==0x29c)?1:-1];
typedef char layout_OBJECT_reserve[((unsigned long)&((OBJECT*)0)->reserve==0x33c)?1:-1];
typedef char layout_OBJECT_capacity[((unsigned long)&((OBJECT*)0)->capacity==0x340)?1:-1];
typedef char layout_OBJECT_progress[((unsigned long)&((OBJECT*)0)->progress==0x344)?1:-1];
typedef char layout_OBJECT_clip[((unsigned long)&((OBJECT*)0)->clip==0x348)?1:-1];
typedef char layout_OBJECT_animation[((unsigned long)&((OBJECT*)0)->animation==0x34c)?1:-1];
typedef char layout_OBJECT_mask9[((unsigned long)&((OBJECT*)0)->mask9==0x350)?1:-1];
typedef char layout_OBJECT_mask10[((unsigned long)&((OBJECT*)0)->mask10==0x354)?1:-1];
typedef char layout_OBJECT_mask11[((unsigned long)&((OBJECT*)0)->mask11==0x358)?1:-1];
typedef char layout_OBJECT_animationTime[((unsigned long)&((OBJECT*)0)->animationTime==0x360)?1:-1];
typedef char layout_OBJECT_selected[((unsigned long)&((OBJECT*)0)->selected==0x38c)?1:-1];
typedef char layout_OBJECT_tuning[((unsigned long)&((OBJECT*)0)->tuning==0x3cc)?1:-1];
typedef char layout_OBJECT_extraGate[((unsigned long)&((OBJECT*)0)->extraGate==0x4a4)?1:-1];
typedef char layout_OBJECT_otherCapacity[((unsigned long)&((OBJECT*)0)->otherCapacity==0xe84)?1:-1];
typedef char layout_OBJECT_otherProgress[((unsigned long)&((OBJECT*)0)->otherProgress==0xe88)?1:-1];
typedef char layout_OBJECT_otherClip[((unsigned long)&((OBJECT*)0)->otherClip==0xe8c)?1:-1];
typedef char layout_OBJECT_otherAnimation[((unsigned long)&((OBJECT*)0)->otherAnimation==0xe90)?1:-1];
typedef char layout_OBJECT_mode[((unsigned long)&((OBJECT*)0)->mode==0xfbc)?1:-1];
typedef char layout_OBJECT_state[((unsigned long)&((OBJECT*)0)->state==0x13d0)?1:-1];
typedef char layout_OBJECT_notifyEnabled[((unsigned long)&((OBJECT*)0)->notifyEnabled==0x143c)?1:-1];

typedef char layout_WORK_size[(sizeof(WORK)==0xC0)?1:-1];

typedef char layout_OFFSETINPUTS_size[(sizeof(OFFSETINPUTS)==0x30)?1:-1];
typedef char layout_OFFSETINPUTS_own[((unsigned long)&((OFFSETINPUTS*)0)->own==0x0)?1:-1];
typedef char layout_OFFSETINPUTS_other[((unsigned long)&((OFFSETINPUTS*)0)->other==0x10)?1:-1];
typedef char layout_OFFSETINPUTS_copy[((unsigned long)&((OFFSETINPUTS*)0)->copy==0x20)?1:-1];
typedef char layout_WORK_own[((unsigned long)&((WORK*)0)->own==0x80)?1:-1];
typedef char layout_WORK_other[((unsigned long)&((WORK*)0)->other==0x90)?1:-1];
typedef char layout_WORK_copy[((unsigned long)&((WORK*)0)->copy==0xA0)?1:-1];
typedef char layout_WORK_delta[((unsigned long)&((WORK*)0)->delta==0xB0)?1:-1];
