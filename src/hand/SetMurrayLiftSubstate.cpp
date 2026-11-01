#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
union U { qword q; float f[4]; int i[4]; };
struct M4 { U row[4]; };
struct VTE { short delta,pad; void *fn; };
struct NODE { M4 local,world; char pad80[0x18]; short state; char pad9A[6]; NODE*parent; };
struct OWNER { char pad[0xC]; VTE*vt; char pad10[0x18]; ulong flags; char pad30[0x28]; NODE*left,*right; };
struct ASEG;
struct EVENT { float time; int kind,tag,pad; };
struct ANIM { char pad[0x14]; ASEG*clip; int pad18; float time; char pad20[0x20]; float rate; };
struct ROOT { char pad[0x60]; NODE*node; };
struct PROXY { char pad[0x1C]; int active; };
struct GOAL { char pad[0xC]; VTE*vt; };
struct STRETCH { void*frame; NODE*first,*second; char padC[0x94]; U position; char padB0[0x1C]; float tuning; };
struct MOTION;
struct LINK { char pad[0x180]; OWNER*owner; char pad184[0xC]; GOAL*first; int pad194; GOAL*second; };
struct P { char pad[0xC]; VTE*vt; char pad10[0x50]; NODE*node; char pad64[0x12C]; U velocity; char pad1A0[0xE90]; ANIM*anim; char pad1034[0x54]; PROXY*proxy; char pad108C[0x360]; ROOT*root; LINK*link; MOTION*m13F4,*m13F8,*m13FC,*m1400; int oldLeft,oldRight; char pad140C[0x3C]; int flag,pad144C,state; float time; char pad1458[0x19C]; NODE*targetFirst; int pad15F8; NODE*targetSecond; int pad1600; MOTION*m1604; int pad1608; MOTION*m160C; STRETCH*first,*second; MOTION*m1618,*m161C,*m1620,*m1624; };
struct CLOCK { float time,dt; };
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M4_Size[sizeof(M4)==64?1:-1];
typedef char VTE_Size[sizeof(VTE)==8?1:-1];
typedef char EVENT_Size[sizeof(EVENT)==16?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char STRETCH_Size[sizeof(STRETCH)==208?1:-1];
typedef char VTE_delta_Offset[FIELD_OFFSET(VTE,delta)==0?1:-1];
typedef char VTE_fn_Offset[FIELD_OFFSET(VTE,fn)==4?1:-1];
typedef char M4_row_Offset[FIELD_OFFSET(M4,row)==0?1:-1];
typedef char NODE_state_Offset[FIELD_OFFSET(NODE,state)==152?1:-1];
typedef char NODE_parent_Offset[FIELD_OFFSET(NODE,parent)==160?1:-1];
typedef char OWNER_vt_Offset[FIELD_OFFSET(OWNER,vt)==12?1:-1];
typedef char OWNER_flags_Offset[FIELD_OFFSET(OWNER,flags)==40?1:-1];
typedef char OWNER_left_Offset[FIELD_OFFSET(OWNER,left)==88?1:-1];
typedef char OWNER_right_Offset[FIELD_OFFSET(OWNER,right)==92?1:-1];
typedef char EVENT_time_Offset[FIELD_OFFSET(EVENT,time)==0?1:-1];
typedef char ANIM_clip_Offset[FIELD_OFFSET(ANIM,clip)==20?1:-1];
typedef char ANIM_time_Offset[FIELD_OFFSET(ANIM,time)==28?1:-1];
typedef char ANIM_rate_Offset[FIELD_OFFSET(ANIM,rate)==64?1:-1];
typedef char ROOT_node_Offset[FIELD_OFFSET(ROOT,node)==96?1:-1];
typedef char PROXY_active_Offset[FIELD_OFFSET(PROXY,active)==28?1:-1];
typedef char GOAL_vt_Offset[FIELD_OFFSET(GOAL,vt)==12?1:-1];
typedef char STRETCH_frame_Offset[FIELD_OFFSET(STRETCH,frame)==0?1:-1];
typedef char STRETCH_first_Offset[FIELD_OFFSET(STRETCH,first)==4?1:-1];
typedef char STRETCH_second_Offset[FIELD_OFFSET(STRETCH,second)==8?1:-1];
typedef char STRETCH_position_Offset[FIELD_OFFSET(STRETCH,position)==160?1:-1];
typedef char STRETCH_tuning_Offset[FIELD_OFFSET(STRETCH,tuning)==204?1:-1];
typedef char LINK_owner_Offset[FIELD_OFFSET(LINK,owner)==384?1:-1];
typedef char LINK_first_Offset[FIELD_OFFSET(LINK,first)==400?1:-1];
typedef char LINK_second_Offset[FIELD_OFFSET(LINK,second)==408?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_node_Offset[FIELD_OFFSET(P,node)==96?1:-1];
typedef char P_velocity_Offset[FIELD_OFFSET(P,velocity)==400?1:-1];
typedef char P_anim_Offset[FIELD_OFFSET(P,anim)==4144?1:-1];
typedef char P_proxy_Offset[FIELD_OFFSET(P,proxy)==4232?1:-1];
typedef char P_root_Offset[FIELD_OFFSET(P,root)==5100?1:-1];
typedef char P_link_Offset[FIELD_OFFSET(P,link)==5104?1:-1];
typedef char P_m13F4_Offset[FIELD_OFFSET(P,m13F4)==5108?1:-1];
typedef char P_m13F8_Offset[FIELD_OFFSET(P,m13F8)==5112?1:-1];
typedef char P_m13FC_Offset[FIELD_OFFSET(P,m13FC)==5116?1:-1];
typedef char P_m1400_Offset[FIELD_OFFSET(P,m1400)==5120?1:-1];
typedef char P_oldLeft_Offset[FIELD_OFFSET(P,oldLeft)==5124?1:-1];
typedef char P_oldRight_Offset[FIELD_OFFSET(P,oldRight)==5128?1:-1];
typedef char P_flag_Offset[FIELD_OFFSET(P,flag)==5192?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==5200?1:-1];
typedef char P_time_Offset[FIELD_OFFSET(P,time)==5204?1:-1];
typedef char P_targetFirst_Offset[FIELD_OFFSET(P,targetFirst)==5620?1:-1];
typedef char P_targetSecond_Offset[FIELD_OFFSET(P,targetSecond)==5628?1:-1];
typedef char P_m1604_Offset[FIELD_OFFSET(P,m1604)==5636?1:-1];
typedef char P_m160C_Offset[FIELD_OFFSET(P,m160C)==5644?1:-1];
typedef char P_first_Offset[FIELD_OFFSET(P,first)==5648?1:-1];
typedef char P_second_Offset[FIELD_OFFSET(P,second)==5652?1:-1];
typedef char P_m1618_Offset[FIELD_OFFSET(P,m1618)==5656?1:-1];
typedef char P_m161C_Offset[FIELD_OFFSET(P,m161C)==5660?1:-1];
typedef char P_m1620_Offset[FIELD_OFFSET(P,m1620)==5664?1:-1];
typedef char P_m1624_Offset[FIELD_OFFSET(P,m1624)==5668?1:-1];
extern "C" CLOCK D_002DDED0;
extern "C" M4 D_002A2D30;
extern "C" float D_002C0A3C,D_002C0A40;
extern "C" void ReleaseAct(MOTION**);
extern "C" void SetSoCnstrForce(OWNER*,int);
extern "C" void SetLoParent_1A58D0(OWNER*,int,int);
extern "C" void func_001F6860(OWNER*,int);
extern "C" EVENT*func_0012F938(ASEG*,int);
extern "C" void LoadIdentityMatrix4(void*,void*,M4*,M4*,float);
extern "C" M4 ConvertAloMat4(void*,void*,M4*);
extern "C" void func_001B72B0(P*,NODE*,const M4*,qword*,qword*);
extern "C" int FIsBasicDerivedFrom__FP5BASIC3CID(OWNER*,int);
extern "C" void func_00209E28(OWNER*,int);
extern "C" void func_00209E88(OWNER*);
extern "C" void func_001B7700(P*,qword,float*,float*,qword*,qword*);
extern "C" MOTION*func_0012DE58(void*,void*,void*,float,qword,qword);
extern "C" qword ConvertXfVec(NODE*,NODE*,qword);
extern "C" MOTION*func_0012DC18(void*,void*,void*,int,int,qword);
extern "C" MOTION*func_0012DD28(void*,void*,void*,STRETCH*);
extern "C" MOTION*func_0012DDB0(void*,void*,void*,void*,int,M4*);
extern "C" void DetachMurrayCarried(P*);
extern "C" void SetMurrayState(P*,int);
static inline void ClearXYZ(U*v){v->i[0]=v->i[1]=v->i[2]=0;}
extern "C" void SetMurrayLiftSubstate(P*p,int state) {
 if(p->state==state)return;
 if(p->state==1){ReleaseAct(&p->m13FC);ReleaseAct(&p->m1400);}
 p->state=state;
 p->time=D_002DDED0.time;
 OWNER*owner;
 switch(state){
 case 0:p->flag=0;break;
 case 1:{
  if(!p->link)goto failed;
  owner=p->link->owner;
  if((int)(owner->flags>>60)&1)goto attach;else goto failed;
 attach:
  SetSoCnstrForce(owner,0);
  p->oldLeft=owner->left->state;
  p->oldRight=owner->right->state;
  SetLoParent_1A58D0(owner,0,15);
  func_001F6860(owner,1);
  M4 predicted;
  float duration;
  EVENT*event=func_0012F938(p->anim->clip,0x1C7);
  duration=(event->time-p->anim->time)/p->anim->rate;
  LoadIdentityMatrix4(p->root->node,p->node,&predicted,0,duration);
  M4 work=ConvertAloMat4(p,0,&predicted);
  predicted=work;
  func_001B72B0(p,0,&predicted,&work.row[0].q,&work.row[1].q);
  if(FIsBasicDerivedFrom__FP5BASIC3CID(owner,0x38))func_00209E28(owner,(int)p);
  func_001B7700(p,work.row[1].q,&event->time,&duration,&work.row[2].q,&work.row[3].q);
  p->m13FC=func_0012DE58(owner,owner->left,(void*)5,D_002DDED0.time+duration,work.row[1].q,work.row[3].q);
  VTE*entry=&owner->vt[35];
  void*receiver=(char*)owner+entry->delta;
  qword velocity=ConvertXfVec(owner->left->parent,0,work.row[2].q);
  ((void(*)(void*,qword))entry->fn)(receiver,velocity);
  p->m1400=func_0012DC18(owner,owner->right,(void*)5,7,2,work.row[0].q);
  break;
 }
 case 2:{
  if(!p->link)goto failed;
  owner=p->link->owner;
  if((int)(owner->flags>>60)&1)goto valid;
 failed:
 DetachMurrayCarried(p);
 p->link=0;
 SetMurrayState(p,0);
  break;
 valid:
  ROOT*root=p->root;
  SetLoParent_1A58D0(owner,(int)root,15);
  func_001F6860(owner,1);
  U position,rotation;
  M4 frame;
  func_001B72B0(p,root->node,&D_002A2D30,&rotation.q,&position.q);
  p->m13F8=func_0012DC18(owner,owner->right,(void*)5,7,2,rotation.q);
  p->m13F4=func_0012DC18(owner,owner->left,(void*)5,7,2,position.q);
  if(p->link->first){
   GOAL*goal=p->link->first;
   STRETCH*stretch=p->first;
   stretch->frame=((void*(*)(void*,M4*))goal->vt[32].fn)((char*)goal+goal->vt[32].delta,&frame);
   stretch->position=frame.row[3];
   stretch->tuning=D_002C0A3C;
   p->m1618=func_0012DD28(p,stretch->first,(void*)5,stretch);
   p->m161C=func_0012DD28(p,stretch->second,(void*)5,stretch);
   p->m1604=func_0012DDB0(p,p->targetFirst,(void*)5,(void*)5,(int)stretch->frame,&frame);
  }
  if(p->link->second){
   GOAL*goal=p->link->second;
   STRETCH*stretch=p->second;
   stretch->frame=((void*(*)(void*,M4*))goal->vt[32].fn)((char*)goal+goal->vt[32].delta,&frame);
   stretch->position=frame.row[3];
   stretch->tuning=D_002C0A40;
   p->m1620=func_0012DD28(p,stretch->first,(void*)5,stretch);
   p->m1624=func_0012DD28(p,stretch->second,(void*)5,stretch);
   p->m160C=func_0012DDB0(p,p->targetSecond,(void*)5,(void*)5,(int)stretch->frame,&frame);
  }
  if(FIsBasicDerivedFrom__FP5BASIC3CID(owner,0x38))func_00209E88(owner);
  ClearXYZ(&p->velocity);
  break;
 }
 default:break;
 }
 do{if(p->state>=2)break;if(p->state<0)break;p->proxy->active=1;goto end;}while(0);p->proxy->active=0;end:;
}
