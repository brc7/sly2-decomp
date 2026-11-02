typedef int qword __attribute__((mode(TI)));typedef unsigned long ulong;
union U{qword q;float f[4];int i[4];};struct VE{short delta,pad;void*fn;};
struct INPUT{char pad0[0x10];float x,y;char pad18[4];float amount;};
struct ACTION;struct ASEGA;struct ANIMATION;
struct P{char pad0[12];VE*vt;char pad10[0x150-0x10];INPUT*input;char pad154[0x2C0-0x154];char controller[0xF00-0x2C0];char action[0x1010-0xF00];U offset;char pad1020[0x10];ASEGA*asega;};
struct PATH{char pad0[0x28];ulong flags;char pad30[0xE4-0x30];int cyclic;};
struct C{P*actor;int state;float time;PATH*path;float distance,speed;int descending;float reset;int previous;char pad24[12];U offset;char pad40[0x10];float weight0;char pad54[8];float speed0,weight1;char pad64[8];float speed1;ANIMATION*animation;};
struct CLOCK{float time,dt;};struct SMP{float maximum,minimum,duration;};struct LM{float low,high;};struct WORK{U ahead,behind;union{U input;SMP smooth;}reuse;LM limit;};
extern "C"{
extern CLOCK D_002DDED0;extern U D_002A2DD0,D_002A2DC0;extern SMP D_002BF928;
qword func_0011F088(qword,qword,float,SMP*,U*);
void func_00195A00(void*,int,void*,void*);int GrfConsumePressed__5POCTLUif(INPUT*,int,float);int GrfPressedWithin__5POCTLUif(INPUT*,int,float);void ClearPoctlJoyPressed(INPUT*,int);
void func_0019CC50(void*,int,void*,void*);int FAsegaRetracted(ASEGA*);void func_0019EAB0(C*,int);
float SClampShape(PATH*,float);void EvaluateShapeS(PATH*,float,U*,U*);qword func_001C4768(INPUT*,qword);
void func_0019E110(C*,LM*);float GSmooth__FfffP3SMPPf(float,float,float,SMP*,float*);void func_001F4BF8(P*);float SMaxShape(PATH*);float GModPositive__Fff(float,float);void func_001384C0(ANIMATION*);
float fabsf(float);
}
static inline ulong Eligible(PATH*p){return (p->flags>>60)&1;}
static inline int CanAct(P*p,int kind){VE&v=p->vt[99];return ((int(*)(void*,int))v.fn)((char*)p+v.delta,kind);}
static inline qword Sub(qword a,qword b){qword q;__asm__("vsub.xyzw %0,%1,%2":"=j"(q):"j"(a),"j"(b));return q;}
static inline qword Direction(U*a,U*b,U*&axis){
 register qword direction __asm__("vf3");register U*address __asm__("$2");
 __asm__("lqc2 $vf2,%2\n\tlui %0,%%hi(%4)\n\tlqc2 $vf1,%3\n\taddiu %0,%0,%%lo(%4)\n\tvsub.xyzw %1,$vf2,$vf1":"=&r"(address),"=j"(direction):"m"(a->q),"m"(b->q),"i"(&D_002A2DC0):"vf1","vf2");
 axis=address;return direction;
}
static inline qword Normal(qword v,U*axis){
 register qword d __asm__("vf1"),r __asm__("vf1");float length;register int bits __asm__("$3");
 __asm__("vmul.xyz %0,%1,%1\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf2,%0z":"=j"(d):"j"(v):"vf2");
 register qword fallback __asm__("$2")=axis->q;
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(d));
 __asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(d));
 __asm__("mtc1 %1,%0":"=f"(length):"r"(bits));
 if(length<0.0001f){r=fallback;}
 else{__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(d),"j"(v));}
 qword q;__asm__("qmfc2.ni %0,%1":"=r"(q):"j"(r));return q;
}
static inline float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char VE_Size[sizeof(VE)==8?1:-1];
typedef char INPUT_Size[sizeof(INPUT)==32?1:-1];
typedef char P_Size[sizeof(P)==4160?1:-1];
typedef char PATH_Size[sizeof(PATH)==232?1:-1];
typedef char C_Size[sizeof(C)==128?1:-1];
typedef char SMP_Size[sizeof(SMP)==12?1:-1];
typedef char LM_Size[sizeof(LM)==8?1:-1];
typedef char WORK_Size[sizeof(WORK)==64?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char INPUT_x_Offset[FIELD_OFFSET(INPUT,x)==16?1:-1];
typedef char INPUT_y_Offset[FIELD_OFFSET(INPUT,y)==20?1:-1];
typedef char INPUT_amount_Offset[FIELD_OFFSET(INPUT,amount)==28?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_input_Offset[FIELD_OFFSET(P,input)==336?1:-1];
typedef char P_controller_Offset[FIELD_OFFSET(P,controller)==704?1:-1];
typedef char P_action_Offset[FIELD_OFFSET(P,action)==3840?1:-1];
typedef char P_offset_Offset[FIELD_OFFSET(P,offset)==4112?1:-1];
typedef char P_asega_Offset[FIELD_OFFSET(P,asega)==4144?1:-1];
typedef char PATH_flags_Offset[FIELD_OFFSET(PATH,flags)==40?1:-1];
typedef char PATH_cyclic_Offset[FIELD_OFFSET(PATH,cyclic)==228?1:-1];
typedef char C_actor_Offset[FIELD_OFFSET(C,actor)==0?1:-1];
typedef char C_state_Offset[FIELD_OFFSET(C,state)==4?1:-1];
typedef char C_time_Offset[FIELD_OFFSET(C,time)==8?1:-1];
typedef char C_path_Offset[FIELD_OFFSET(C,path)==12?1:-1];
typedef char C_distance_Offset[FIELD_OFFSET(C,distance)==16?1:-1];
typedef char C_speed_Offset[FIELD_OFFSET(C,speed)==20?1:-1];
typedef char C_descending_Offset[FIELD_OFFSET(C,descending)==24?1:-1];
typedef char C_reset_Offset[FIELD_OFFSET(C,reset)==28?1:-1];
typedef char C_previous_Offset[FIELD_OFFSET(C,previous)==32?1:-1];
typedef char C_offset_Offset[FIELD_OFFSET(C,offset)==48?1:-1];
typedef char C_weight0_Offset[FIELD_OFFSET(C,weight0)==80?1:-1];
typedef char C_speed0_Offset[FIELD_OFFSET(C,speed0)==92?1:-1];
typedef char C_weight1_Offset[FIELD_OFFSET(C,weight1)==96?1:-1];
typedef char C_speed1_Offset[FIELD_OFFSET(C,speed1)==108?1:-1];
typedef char C_animation_Offset[FIELD_OFFSET(C,animation)==112?1:-1];
typedef char SMP_maximum_Offset[FIELD_OFFSET(SMP,maximum)==0?1:-1];
typedef char SMP_minimum_Offset[FIELD_OFFSET(SMP,minimum)==4?1:-1];
typedef char SMP_duration_Offset[FIELD_OFFSET(SMP,duration)==8?1:-1];
typedef char LM_low_Offset[FIELD_OFFSET(LM,low)==0?1:-1];
typedef char LM_high_Offset[FIELD_OFFSET(LM,high)==4?1:-1];
typedef char WORK_ahead_Offset[FIELD_OFFSET(WORK,ahead)==0?1:-1];
typedef char WORK_behind_Offset[FIELD_OFFSET(WORK,behind)==16?1:-1];
typedef char WORK_reuse_Offset[FIELD_OFFSET(WORK,reuse)==32?1:-1];
typedef char WORK_limit_Offset[FIELD_OFFSET(WORK,limit)==48?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
/** @sly2-annot addr=0x19E1E0
 * Purpose: Per-frame pipe climbing: smooth offset, X jumps off (SetJtAirJump 2), 0x400 starts JtMode, climb
 *   up/down/spin along curve within GetJtPipeSRange, sounds
 * Info: proposed name UpdateJtActivePipe (T1); TU pipe_jt
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
extern "C" void UpdateJtActivePipe(C*p){
 WORK w;float lowSpeed,highSpeed;P*actor=p->actor;actor->offset.q=func_0011F088(actor->offset.q,D_002A2DD0.q,D_002DDED0.dt,&D_002BF928,0);
 register float amount __asm__("$f4"),dot __asm__("$f3"),zero __asm__("$f2");register int previous __asm__("$5");
 {register INPUT*input __asm__("$18")=p->actor->input;
 if(!Eligible(p->path)){func_00195A00(p->actor->controller,0,0,0);return;}
 if(p->state&&GrfConsumePressed__5POCTLUif(input,0x40,0.25f)){func_00195A00(p->actor->controller,2,0,0);return;}
 if(GrfPressedWithin__5POCTLUif(input,0x400,0.15f)&&CanAct(p->actor,3)){ClearPoctlJoyPressed(input,0x400);func_0019CC50(p->actor->action,0,0,0);}
 int old;do{old=p->state;if((old==0||old==4)&&FAsegaRetracted(p->actor->asega))func_0019EAB0(p,1);}while(p->state!=old);
 if(p->state==0||p->state==4)return;
 EvaluateShapeS(p->path,SClampShape(p->path,p->distance+50.0f),&w.ahead,0);
 EvaluateShapeS(p->path,SClampShape(p->path,p->distance-50.0f),&w.behind,0);
 U*axis;qword direction=Direction(&w.ahead,&w.behind,axis);qword normalized=Normal(direction,axis);float z=Z(normalized);
 if(fabsf(z)>0.5f){
  if(z>0.0f){if(!p->descending)goto checkMemory;goto turn;}
  if(!p->descending)goto turn;
 }
 checkMemory:
 if(p->reset>0.4f&&p->descending&&Z(normalized)>-0.3f)goto turn;
 if(p->reset<-0.4f&&!p->descending&&Z(normalized)<0.3f)goto turn;
 goto proceed;
 turn:func_0019EAB0(p,4);return;
 proceed:;
 register qword tangent __asm__("$2")=func_001C4768(input,Sub(w.ahead.q,w.behind.q));
 __asm__("lwc1 $f1,0x10(%6)\n\tqmtc2.ni %5,$vf4\n\tlwc1 $f0,0x14(%6)\n\tmtc1 $zero,%2\n\tmfc1 $3,$f1\n\tmfc1 $4,$f0\n\tpextlw $3,$4,$3\n\tmfc1 $4,%2\n\tpcpyld $3,$4,$3\n\tlwc1 %0,0x1c(%6)\n\tqmtc2.ni $3,$vf3\n\tlw %1,0x20(%7)\n\tvmul.xyz $vf1,$vf4,$vf3\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tsq $3,%3\n\tqmfc2.ni $2,$vf1\n\tmtc1 $2,%4":"=f"(amount),"=r"(previous),"=f"(zero),"=m"(w.reuse.input.q),"=f"(dot):"r"(tangent),"r"(input),"r"(p):"$2","$3","$4","$f0","$f1","vf1","vf2","vf3","vf4","memory");
 }
 switch(previous){case 0:if(amount<0.25f)p->previous=-1;else if(dot<-0.7f)p->previous=1;break;
 case 1:if(amount<0.25f)p->previous=-1;else if(dot>0.7f)p->previous=0;break;
 case -1:if(amount>0.35f){if(zero<=dot)p->previous=0;else p->previous=1;}break;}
 int mode=-1;register float blend __asm__("$f3")=dot;
 register int selected __asm__("$3")=p->previous;
 switch(selected){case 0:mode=p->descending?3:2;blend=amount;break;case 1:mode=p->descending?2:3;blend=amount;break;case -1:mode=1;blend=0.0f;break;}

 float speed;register float maximumInput __asm__("$f1");
 if(blend>0.01f){
  register float initialPart __asm__("$f0")=1.0f;
  maximumInput=p->speed1;
  initialPart-=blend;
  register float low __asm__("$f2")=p->speed0;
  blend*=maximumInput;
  initialPart*=low;
  speed=initialPart+blend;if(mode==3)speed=-speed;
 }
 else{speed=0.0f;mode=1;maximumInput=p->speed1;}
 if(p->descending)speed=-speed;
 register CLOCK*clock __asm__("$19")=&D_002DDED0;
 register float nextMemory __asm__("$f0");register PATH*path __asm__("$3");
 __asm__("abs.s $f5,%2\n\tlwc1 $f2,4(%7)\n\t.set noat\n\tlui $1,0x3f80\n\tmtc1 $1,%2\n\t.set at\n\tneg.s %0,$f2\n\tlwc1 $f4,0x1c(%8)\n\tsub.s %2,%2,$f2\n\t.set noat\n\tlui $1,0x3e4c\n\tori $1,$1,0xcccd\n\tmtc1 $1,$f3\n\t.set at\n\tswc1 $f5,%3\n\tmul.s %0,%0,%6\n\tswc1 $f3,%4\n\tmul.s %2,%2,$f4\n\tsw $zero,%5\n\tdiv.s %0,%0,$f5\n\tlw %1,0xc(%8)\n\tadd.s %0,%0,%2":"=f"(nextMemory),"=r"(path),"+f"(maximumInput),"=m"(w.reuse.smooth.maximum),"=m"(w.reuse.smooth.duration),"=m"(w.reuse.smooth.minimum):"f"(speed),"r"(clock),"r"(p):"$1","$f2","$f3","$f4","$f5","memory");
 p->reset=nextMemory;
 if(!path->cyclic){func_0019E110(p,&w.limit);(void)GSmooth__FfffP3SMPPf(p->distance,w.limit.low,clock->dt,&w.reuse.smooth,&lowSpeed);(void)GSmooth__FfffP3SMPPf(p->distance,w.limit.high,clock->dt,&w.reuse.smooth,&highSpeed);float low=-highSpeed,high=-lowSpeed;if(speed<low)speed=low;else if(high<speed)speed=high;}
 register float stopped __asm__("$f21")=0.0f;
 p->speed=-speed;
 if(speed!=stopped){p->distance+=p->speed*D_002DDED0.dt;func_001F4BF8(p->actor);}
 if(p->path->cyclic)p->distance=GModPositive__Fff(p->distance,SMaxShape(p->path));
 register float magnitude __asm__("$f2")=fabsf(speed);
 if(magnitude<0.01f)mode=1;
 else{
  register float initial __asm__("$f0")=p->speed0,one __asm__("$f4"),weight __asm__("$f3");
  __asm__("lwc1 $f1,%4\n\tabs.s %0,%0\n\t.set noat\n\tlui $1,0x3f80\n\tmtc1 $1,%1\n\t.set at\n\tabs.s $f1,$f1\n\tsub.s %2,%2,%0\n\tsub.s $f1,$f1,%0\n\tdiv.s %3,%2,$f1":"+f"(initial),"=f"(one),"+f"(magnitude),"=f"(weight):"m"(p->speed1):"$1","$f1");
  if(weight<stopped)weight=stopped;else if(one<weight)weight=one;
  register ANIMATION*animation __asm__("$4");register float complement __asm__("$f0");
  __asm__(".set noat\n\tlui $1,0x3f80\n\tmtc1 $1,%0\n\t.set at\n\tlw %1,0x70(%3)\n\tsub.s %0,%0,%4\n\tswc1 %4,%2":"=f"(complement),"=r"(animation),"=m"(p->weight1):"r"(p),"f"(weight):"$1","memory");
  p->weight0=complement;
  func_001384C0(animation);
 }
 func_0019EAB0(p,mode);
}
