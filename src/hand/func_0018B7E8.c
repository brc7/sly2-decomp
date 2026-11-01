
typedef int qword __attribute__((mode(TI)));
typedef union{qword q;float f[4];}U;
struct VTE{short delta,pad;void*fn;};
struct STATE{char pad[0x18];float heading,amount;};
struct MODEL{char pad14[0x14];void*table;char pad18[4];float base;char pad20[0x20];float scale;};
struct COMPONENT{struct VTE*vt;};struct BINDING{int key;struct COMPONENT*component;};
struct P{
 char padC[0xC];struct VTE*vt;
 char pad10[0x150-0x10];struct STATE*state;
 char pad154[0x190-0x154];qword velocity;
 char pad1A0[8];float heading;
 char pad1AC[0x1F4-0x1AC];float factor;
 char pad1F8[0x1030-0x1F8];struct MODEL*model;
 char pad1034[0x10DC-0x1034];void*owner;
 char pad10E0[0x1310-0x10E0];void*animation;int animationKind;
 char pad1318[0x14D0-0x1318];int axesAEnabled;
 char pad14D4[0x18C8-0x14D4];int mode;char pad18CC[4];int submode;
 char pad18D4[0x1BC0-0x18D4];int axesBEnabled;
 char pad1BC4[0x1D1C-0x1BC4];void*table;
 char pad1D20[0xC];int enabled;
 char pad1D30[0x2014-0x1D30];float left;
 char pad2018[8];float middle;char pad2024[8];float right;
 char pad2030[8];void*blend;
};
union PHASE{qword direction;U scalar;};
extern qword D_002A2DD0;extern char D_003DD490[];
extern float D_002BF3C0,D_002BF3C4,D_002BF3C8,D_002BF3CC,D_002BF35C;
float func_00204740(struct P*);float RadNormalize__Ff(float);
void func_00194A40(void*,struct P*,unsigned int);
void func_0018B2C0(struct P*);void func_0018A8C8(struct P*);void func_0018F0A8(struct P*);
struct BINDING*func_0012FF10(void*,struct P*,void*,int);
void func_00190520(struct P*);void func_00197F98(void*);void func_00191A10(void*);void func_00137218(void*);
void func_001A0720(struct P*,void*,int,float,float);void func_0018B460(struct P*);
void func_0020BB58(void*,float);void func_002046F0(struct P*,void*);
qword SetVectorCylind(float,float,float);qword ConvertAloVec(void*,void*,qword);
int func_001DAEA8(int);void func_0018E1E0(struct P*);qword func_00204E38(struct P*,struct MODEL*,float);
void func_00131A00(struct MODEL*,float);void func_00204660(struct P*,float);
void func_001441E8(struct P*,float);void func_0018C250(struct P*,int,int);
float func_0011CFE0(float,float);void func_001384C0(void*);
void func_001A1358(void*);void func_0019FC30(struct P*);int func_00131968(void*);void func_001306A8(void**);
static inline void*At(struct P*p,int offset){return(char*)p+offset;}
static inline void ReadBinding(struct BINDING*b,struct MODEL*m,qword*out){
 struct COMPONENT*c=b->component;struct VTE*entry=&c->vt[3];
 ((void(*)(void*,int,qword*,float,float))entry->fn)((char*)c+entry->delta,0,out,m->base,m->scale);
}
static inline qword ScaleIn(qword a,qword b){__asm__("vmulx.xyzw %0,%0,%1x":"+j"(a):"j"(b));return a;}
static inline qword FirstNegative(const qword*input,U*out,float g){qword a,s;register qword bits __asm__("$2");__asm__("lqc2 %0,%3\n\tmfc1 %1,%4\n\tqmtc2.ni %1,%2":"=j"(a),"=&r"(bits),"=j"(s):"m"(*input),"f"(g));out->q=s;return ScaleIn(a,out->q);}
static inline qword SecondNegative(const qword*input,U*out,float g){qword a,s;register qword bits __asm__("$3");__asm__("lqc2 %0,%3\n\tmfc1 %1,%4\n\tqmtc2.ni %1,%2":"=j"(a),"=&r"(bits),"=j"(s):"m"(*input),"f"(g));out->q=s;return ScaleIn(a,out->q);}
static inline qword Scale(qword a,qword b){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword NegativeModelNative(struct P*p,U*u){qword v=func_00204E38(p,p->model,p->model->base),s;register qword bits __asm__("$4");__asm__("mfc1 %0,%1":"=r"(bits):"f"(-1.0f));__asm__("qmtc2.ni %1,%0":"=j"(s):"r"(bits));u->q=s;return Scale(v,u->q);}

static inline qword NegativeWithState(struct P*p,U*u,struct STATE**stateOut,float*zeroOut){
 qword input=func_00204E38(p,p->model,p->model->base);register qword v __asm__("vf3");register qword s __asm__("vf1");register qword result __asm__("vf2");register float zero __asm__("$f1");register qword bits __asm__("$4");register struct STATE*state __asm__("$3");
 __asm__("qmtc2.ni %3,%0\n\tlw %1,%4\n\tmfc1 %2,%5":"=j"(v),"=r"(state),"=&r"(bits):"r"(input),"m"(p->state),"f"(-1.0f));
 __asm__("qmtc2.ni %2,%0\n\tmtc1 $zero,%1":"=j"(s),"=f"(zero):"r"(bits));u->q=s;*stateOut=state;*zeroOut=zero;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(result):"j"(v),"j"(u->q));return result;
}

static inline void WriteVectorPair(qword value,qword*destination,qword scalar,qword*scalarOut){__asm__("sqc2 %2,%0\n\tsqc2 %3,%1":"=m"(*destination),"=m"(*scalarOut):"j"(value),"j"(scalar));}
static inline void PrepareAnimation(struct P*p,U*u,struct MODEL**modelOut,struct STATE**stateOut){
 qword input=func_00204E38(p,p->model,p->model->base);register qword v __asm__("vf3");register qword scalar __asm__("vf2");register qword result __asm__("vf1");
 register qword bits __asm__("$3");register struct MODEL*model __asm__("$4");register struct STATE*state __asm__("$3");
 __asm__("qmtc2.ni %3,%0\n\tlw %1,%4\n\tmfc1 %2,%5":"=j"(v),"=r"(model),"=&r"(bits):"r"(input),"m"(p->model),"f"(-1.0f));
 __asm__("qmtc2.ni %1,%0":"=j"(scalar):"r"(bits));
 __asm__("vmulx.xyzw %0,%2,%3x\n\tlw %1,%4":"=j"(result),"=r"(state):"j"(v),"j"(scalar),"m"(p->state));
 WriteVectorPair(result,&p->velocity,scalar,&u->q);*modelOut=model;*stateOut=state;
}

static inline float Z(qword q){float r;__asm__("pextuw %0,$zero,%1":"=r"(r):"r"(q));return r;}
void func_0018B7E8(struct P*p){
 union PHASE phase;U scalar;float initialHeading;
 initialHeading=func_00204740(p);(void)RadNormalize__Ff(p->state->heading-initialHeading);
 func_00194A40(D_003DD490,p,0xFFFF);
 func_0018B2C0(p);func_0018A8C8(p);func_0018F0A8(p);
 switch(p->mode){
 case 4:case 9:{
  struct MODEL*m=p->model;struct BINDING*b;p->velocity=D_002A2DD0;
  if(m){b=func_0012FF10(m->table,p,(void*)1,1);
   if(b){ReadBinding(b,m,&phase.direction);p->velocity=FirstNegative(&phase.direction,&scalar,-1.0f);}
  }break;
 }
 case 3:func_00190520(p);break;
 case 2:func_00197F98(At(p,0x2C0));break;
 case 7:func_00191A10(At(p,0x4C0));break;
 case 6:func_00137218(At(p,0x1FF0));break;
 case 10:func_001A0720(p,At(p,0x1AC0),p->axesBEnabled,p->state->heading,p->state->amount);break;
 case 11:func_0018B460(p);break;
 case 12:func_0020BB58(At(p,0x1C30),p->state->amount);func_00204660(p,p->state->heading);break;
 case 5:switch(p->submode){
  case 4:if(p->owner)func_002046F0(p,p->owner);break;
  case 5:func_001A0720(p,At(p,0x13D0),p->axesAEnabled,p->state->heading,p->state->amount);break;
  case 8:case 9:phase.direction=SetVectorCylind(p->state->heading+3.14159265f,p->state->amount*D_002BF3C0,0.0f);p->velocity=ConvertAloVec(0,p,phase.direction);break;
 }break;
 case 16:
  if(func_001DAEA8(0x1C))func_0018E1E0(p);
  else if(p->enabled){struct BINDING*b=func_0012FF10(p->table,p,(void*)1,1);if(b){ReadBinding(b,p->model,&phase.direction);p->velocity=SecondNegative(&phase.direction,&scalar,-1.0f);}}
  break;
 case 19:p->velocity=NegativeModelNative(p,&phase.scalar);break;
 case 20:{struct MODEL*model;struct STATE*state;PrepareAnimation(p,&phase.scalar,&model,&state);func_00131A00(model,state->amount);func_00204660(p,p->state->heading);break;}
 case 17:{struct STATE*state;float zero;p->velocity=NegativeWithState(p,&phase.scalar,&state,&zero);if(zero<state->amount)func_00204660(p,state->heading);break;}
 case 24:{
  register float one __asm__("$f20");register float weight __asm__("$f1");qword velocity;struct VTE*entry;
  func_001441E8(p,D_002BF3C4);
  if(p->submode==12){float angle=RadNormalize__Ff(func_00204740(p)-p->heading);if(__builtin_fabsf(angle)<D_002BF3C8)func_0018C250(p,24,13);}
  else func_00204660(p,p->state->heading);
  one=1.0f;entry=&p->vt[34];velocity=((qword(*)(void*))entry->fn)((char*)p+entry->delta);
  weight=D_002BF3CC*func_0011CFE0(Z(velocity)/(p->factor*D_002BF35C),one);
  {register float zero __asm__("$f2")=0.0f;register float magnitude __asm__("$f3");register float left __asm__("$f0");__asm__("abs.s %0,%2\n\tmax.s %1,%3,%2\n\tneg.s %2,%2":"=f"(magnitude),"=f"(left),"+f"(weight):"f"(zero));__asm__("swc1 %4,%0\n\tsub.s %1,%1,%5\n\tmax.s %2,%2,%6\n\tswc1 %2,%3":"=m"(p->left),"+f"(one),"+f"(zero),"=m"(p->right):"f"(left),"f"(magnitude),"f"(weight));p->middle=one;}
  func_001384C0(p->blend);break;
 }
 case 0:case 1:break;
 }
 func_001A1358(At(p,0xDF0));func_0019FC30(p);
 if(p->animation&&func_00131968(p->animation)){func_001306A8(&p->animation);p->animationKind=-1;}
}

typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char VTE_Size[(sizeof(struct VTE)==8)?1:-1];
typedef char PHASE_Size[(sizeof(union PHASE)==16)?1:-1];
typedef char STATE_Size[(sizeof(struct STATE)==32)?1:-1];
typedef char BINDING_Size[(sizeof(struct BINDING)==8)?1:-1];
typedef char VTE_delta_Offset[((unsigned)&((struct VTE*)0)->delta==0)?1:-1];
typedef char VTE_fn_Offset[((unsigned)&((struct VTE*)0)->fn==4)?1:-1];
typedef char STATE_heading_Offset[((unsigned)&((struct STATE*)0)->heading==24)?1:-1];
typedef char STATE_amount_Offset[((unsigned)&((struct STATE*)0)->amount==28)?1:-1];
typedef char MODEL_table_Offset[((unsigned)&((struct MODEL*)0)->table==20)?1:-1];
typedef char MODEL_base_Offset[((unsigned)&((struct MODEL*)0)->base==28)?1:-1];
typedef char MODEL_scale_Offset[((unsigned)&((struct MODEL*)0)->scale==64)?1:-1];
typedef char COMPONENT_vt_Offset[((unsigned)&((struct COMPONENT*)0)->vt==0)?1:-1];
typedef char BINDING_component_Offset[((unsigned)&((struct BINDING*)0)->component==4)?1:-1];
typedef char P_vt_Offset[((unsigned)&((struct P*)0)->vt==12)?1:-1];
typedef char P_state_Offset[((unsigned)&((struct P*)0)->state==336)?1:-1];
typedef char P_velocity_Offset[((unsigned)&((struct P*)0)->velocity==400)?1:-1];
typedef char P_heading_Offset[((unsigned)&((struct P*)0)->heading==424)?1:-1];
typedef char P_factor_Offset[((unsigned)&((struct P*)0)->factor==500)?1:-1];
typedef char P_model_Offset[((unsigned)&((struct P*)0)->model==4144)?1:-1];
typedef char P_owner_Offset[((unsigned)&((struct P*)0)->owner==4316)?1:-1];
typedef char P_animation_Offset[((unsigned)&((struct P*)0)->animation==4880)?1:-1];
typedef char P_animationKind_Offset[((unsigned)&((struct P*)0)->animationKind==4884)?1:-1];
typedef char P_axesAEnabled_Offset[((unsigned)&((struct P*)0)->axesAEnabled==5328)?1:-1];
typedef char P_mode_Offset[((unsigned)&((struct P*)0)->mode==6344)?1:-1];
typedef char P_submode_Offset[((unsigned)&((struct P*)0)->submode==6352)?1:-1];
typedef char P_axesBEnabled_Offset[((unsigned)&((struct P*)0)->axesBEnabled==7104)?1:-1];
typedef char P_table_Offset[((unsigned)&((struct P*)0)->table==7452)?1:-1];
typedef char P_enabled_Offset[((unsigned)&((struct P*)0)->enabled==7468)?1:-1];
typedef char P_left_Offset[((unsigned)&((struct P*)0)->left==8212)?1:-1];
typedef char P_middle_Offset[((unsigned)&((struct P*)0)->middle==8224)?1:-1];
typedef char P_right_Offset[((unsigned)&((struct P*)0)->right==8236)?1:-1];
typedef char P_blend_Offset[((unsigned)&((struct P*)0)->blend==8248)?1:-1];
