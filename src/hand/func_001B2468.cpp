typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct V {float x,y,z,w;}__attribute__((aligned(16)));
struct VT {short delta,pad;void*fn;};
struct P {char pad[0xC];VT*vt;char pad10[0x1E8-0x10];int disabled;char pad1EC[0x278-0x1EC];float deadline;char opaque27C[4];qword targetVelocity;};
struct CLOCK {float t,dt;};
extern "C" {
extern CLOCK D_002DDED0;
extern qword D_002A2DD0;
V*func_001B2398(V*,P*,float);
qword func_001A62E8(P*);
void EvaluateBezierPos(float,float,float,qword,qword,qword,qword,qword*,qword*,qword*);
void AddSoAcceleration(P*,qword);
}
int FFloatsNear(float,float,float)__asm__("FFloatsNear__Ffff");
static inline float Y(qword q){qword b;float f;__asm__("prot3w %0,%1":"=r"(b):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(b));return f;}
static inline float Z(qword q){qword b;float f;__asm__("pextuw %0,$zero,%1":"=r"(b):"r"(q));__asm__("mtc1 %1,%0":"=f"(f):"r"(b));return f;}
static inline qword Velocity(P*p){VT*vt=p->vt;return ((qword(*)(void*))vt[33].fn)((char*)p+vt[33].delta);}
static inline float Max(float a,float b){return a>b?a:b;}
static inline float Min(float a,float b){return a<b?a:b;}
static inline int CaptureDestinationAndEqual(P*p,const V*output,V*destination){
 register qword copied __asm__("$3"),target __asm__("$17"),zero __asm__("$16");register qword*origin __asm__("$2");
 register float targetX __asm__("$f13"),epsilon __asm__("$f14"),zeroX __asm__("$f12");
 __asm__("lq %0,%6\n\tlui %1,%%hi(%8)\n\tlq %2,%7\n\taddiu %1,%1,%%lo(%8)\n\tsq %0,%5\n\tmtc1 %2,%3\n\tli.s %4,%9":"=&r"(copied),"=&r"(origin),"=&r"(target),"=f"(targetX),"=f"(epsilon),"=m"(*destination):"m"(*output),"m"(p->targetVelocity),"i"(&D_002A2DD0),"F"(0.0001f));
 __asm__("lq %0,%2\n\tmtc1 %0,%1":"=r"(zero),"=f"(zeroX):"m"(*origin));
 return FFloatsNear(zeroX,targetX,epsilon)&&FFloatsNear(Y(zero),Y(target),0.0001f)&&FFloatsNear(Z(zero),Z(target),0.0001f);
}
struct WORK{V output;U integrated,step;V destination;U scale,delta,negative,copy;};
/** @sly2-annot addr=0x1B2468
 * Purpose: CID-82 vt70: steer velocity toward predicted target position until deadline (+0x278)
 * Info: proposed name PresetSplmissAccel (T2); class CID82 slot70; TU missile
 * Notes: names_v2 C proposed UpdatePrsenLoopShader__FP5PRSEN (0.653)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
extern "C" void func_001B2468(P*p){
 if(p->disabled)return;
 WORK w;
 float remaining;
 {register float deadline __asm__("$f1")=p->deadline;register float now __asm__("$f2")=D_002DDED0.t;
 deadline-=now;register float frameStep __asm__("$f3")=D_002DDED0.dt;remaining=Max(deadline,0.0f)+frameStep;}
 func_001B2398(&w.output,p,Min(remaining,0.5f));
 int stationary=CaptureDestinationAndEqual(p,&w.output,&w.destination);
 if(stationary){
  register qword scaleWord __asm__("$2");
  __asm__("mfc1 %0,%2\n\tsq %0,%1":"=r"(scaleWord),"=m"(w.scale.q):"f"(1.5f/remaining));
  w.integrated.q=scaleWord;
  qword position=func_001A62E8(p);
  register qword destination __asm__("vf1"),delta __asm__("vf2");register VT*table __asm__("$3");register float negative __asm__("$f0");
  __asm__("lqc2 %0,%5\n\tqmtc2.ni %6,%1\n\tlw %2,%7\n\tvsub.xyzw %1,%0,%1\n\tli.s %3,%8\n\tsqc2 %1,%4":"=j"(destination),"=j"(delta),"=r"(table),"=f"(negative),"=m"(w.delta.q):"m"(w.destination),"r"(position),"m"(p->vt),"F"(-0.5f));
  register qword(*method)(void*) __asm__("$2");register void*receiver __asm__("$4");register qword copiedNegative __asm__("$3");
  __asm__("mfc1 %0,%4\n\tlh %1,%5\n\tsq %0,%3\n\tlw %0,%6\n\taddu %1,%7,%1\n\tlq %2,%3":"=&r"(method),"=&r"(receiver),"=r"(copiedNegative),"=m"(w.negative.q):"f"(negative),"m"(table[33].delta),"m"(table[33].fn),"r"(p));
  w.integrated.q=copiedNegative;
  qword velocity=method(receiver);
  register qword factor __asm__("vf2"),difference __asm__("vf3"),minus __asm__("vf4"),value __asm__("vf5"),result __asm__("vf1");
  __asm__("lqc2 %0,%5\n\tlqc2 %1,%6\n\tlqc2 %2,%7\n\tqmtc2.ni %8,%3\n\tvmulax.xyzw ACC,%1,%0x\n\tvmaddx.xyzw %4,%3,%2x":"=j"(factor),"=j"(difference),"=j"(minus),"=j"(value),"=j"(result):"m"(w.scale.q),"m"(w.delta.q),"m"(w.negative.q),"r"(velocity));
  *(qword*)&w.output=result;
 }else *(qword*)&w.output=p->targetVelocity;
 register CLOCK*clock __asm__("$17")=&D_002DDED0;
 float frameDt=clock->dt;
 qword position=func_001A62E8(p);
 qword velocity=Velocity(p);
 register float tension __asm__("$f14");
 register qword startPosition __asm__("$4"),startVelocity __asm__("$5"),endPosition __asm__("$6"),endVelocity __asm__("$7");
 __asm__("lq %2,%5\n\tpor %1,$zero,%6\n\tlq %3,%7\n\tpor %0,$zero,%8\n\tli.s %4,%9":"=r"(startPosition),"=&r"(startVelocity),"=&r"(endPosition),"=&r"(endVelocity),"=f"(tension):"m"(w.destination),"r"(velocity),"m"(w.output),"r"(position),"F"(1.0f));
 EvaluateBezierPos(remaining,frameDt,tension,startPosition,startVelocity,endPosition,endVelocity,0,&w.integrated.q,0);
 w.copy=w.integrated;
 VT*table=p->vt;register qword(*method)(void*) __asm__("$2");register int displacement __asm__("$4");
 __asm__("lw %0,%2\n\tlh %1,%3":"=&r"(method),"=r"(displacement):"m"(table[33].fn),"m"(table[33].delta));
 qword current=method((char*)p+displacement);
 register qword copied __asm__("vf1"),actualCurrent __asm__("vf3");register P*owner __asm__("$4");register unsigned int dtWord __asm__("$17");
 __asm__("lqc2 %0,%4\n\tdaddu %1,%5,$zero\n\tqmtc2.ni %6,%2\n\tlw %3,%7":"=j"(copied),"=r"(owner),"=j"(actualCurrent),"=r"(dtWord):"m"(w.copy.q),"r"(p),"r"(current),"m"(clock->dt));
 register qword difference __asm__("vf2"),dt __asm__("vf1"),acceleration __asm__("vf4");
 __asm__("vsub.xyzw %0,%3,%4\n\tqmtc2.ni %5,%1\n\tvdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %2,%0,Q":"=j"(difference),"=j"(dt),"=j"(acceleration):"j"(copied),"j"(actualCurrent),"r"(dtWord));
 w.step.q=dt;
 AddSoAcceleration(owner,acceleration);
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char U_q_Offset[FIELD_OFFSET(U,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(U,f)==0?1:-1];
typedef char V_x_Offset[FIELD_OFFSET(V,x)==0?1:-1];
typedef char V_y_Offset[FIELD_OFFSET(V,y)==4?1:-1];
typedef char V_z_Offset[FIELD_OFFSET(V,z)==8?1:-1];
typedef char V_w_Offset[FIELD_OFFSET(V,w)==12?1:-1];
typedef char VT_delta_Offset[FIELD_OFFSET(VT,delta)==0?1:-1];
typedef char VT_fn_Offset[FIELD_OFFSET(VT,fn)==4?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_disabled_Offset[FIELD_OFFSET(P,disabled)==488?1:-1];
typedef char P_deadline_Offset[FIELD_OFFSET(P,deadline)==632?1:-1];
typedef char P_targetVelocity_Offset[FIELD_OFFSET(P,targetVelocity)==640?1:-1];
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char WORK_output_Offset[FIELD_OFFSET(WORK,output)==0?1:-1];
typedef char WORK_integrated_Offset[FIELD_OFFSET(WORK,integrated)==16?1:-1];
typedef char WORK_step_Offset[FIELD_OFFSET(WORK,step)==32?1:-1];
typedef char WORK_destination_Offset[FIELD_OFFSET(WORK,destination)==48?1:-1];
typedef char WORK_scale_Offset[FIELD_OFFSET(WORK,scale)==64?1:-1];
typedef char WORK_delta_Offset[FIELD_OFFSET(WORK,delta)==80?1:-1];
typedef char WORK_negative_Offset[FIELD_OFFSET(WORK,negative)==96?1:-1];
typedef char WORK_copy_Offset[FIELD_OFFSET(WORK,copy)==112?1:-1];
typedef char U_PrefixSize[sizeof(U)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char V_PrefixSize[sizeof(V)==16?1:-1];
typedef char V_Alignment[__alignof__(V)==16?1:-1];
typedef char VT_PrefixSize[sizeof(VT)==8?1:-1];
typedef char VT_Alignment[__alignof__(VT)==4?1:-1];
typedef char P_PrefixSize[sizeof(P)==656?1:-1];
typedef char P_Alignment[__alignof__(P)==16?1:-1];
typedef char CLOCK_PrefixSize[sizeof(CLOCK)==8?1:-1];
typedef char CLOCK_Alignment[__alignof__(CLOCK)==4?1:-1];
typedef char WORK_PrefixSize[sizeof(WORK)==128?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char qword_Width[sizeof(qword)==16?1:-1];
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char unsigned_int_Width[sizeof(unsigned int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char short_Width[sizeof(short)==2?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];
