// Create position and orientation jobs for the selected animated object.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
union M4 {qword rows[4];float values[16];};
struct M3 {qword rows[3];};
struct VTE {short delta,pad;void *fn;};
struct SPACE;
struct JOB;
struct CHANNEL {char pad90[0x90];JOB *current;char pad94[0xC];SPACE *space;int wordA4;float endTime;};
struct MOTION;
struct ANIM {char pad14[0x14];MOTION *motion;int word18;float start;char pad20[0x20];float duration;};
struct FOLLOW {char pad80[0x80];qword velocity;};
struct LO {char padC[0xC];VTE *vt;char pad10[0x48];CHANNEL *position,*orientation;SPACE *space;char pad64[0x84];FOLLOW *follow;char padEC[0xC4];M3 basis;char pad1E0[0xE50];ANIM *playing;};
struct SOURCE {char padC[0xC];VTE *vt;};
struct P {LO *owner;char pad4[0x30];SOURCE *source;char pad38[0x5C];JOB *positionJob,*otherJob,*orientationJob;};
struct CLOCK {float time,dt;};
union VIEWS {struct {M4 intermediate,product,inverse;} matrices;struct {M3 angles,converted;} basis;U scalar;};
struct WORK {M4 original,sample,start,final;VIEWS views;qword velocity,destination,delta,endVelocity;};
extern "C" {
extern qword D_002C1EE0;
extern float D_002C1EF0;
extern CLOCK D_002DDED0;
void ReleaseAct(JOB **);
float func_0012F990(MOTION *,int);
void func_001300B8(MOTION *,float,float,LO *,SPACE *,SPACE *,M4 *,M4 *);
qword func_001A62E8(LO *);
qword *VECTOR_opAssign(qword *,qword);
M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *);
LO *func_001CCA40(LO *,LO *);
M4 ConvertAloMat4(LO *,LO *,const M4 *);
M4 InvertMatrix4Rigid(const M4 *);
void SetLoParent_1A58D0(LO *,int,int);
float func_00196928(qword,qword,float,float,float,float);
void func_00131A00(ANIM *,float);
qword func_0022FC48(SPACE *,SPACE *,qword);
qword ConvertXfVec(SPACE *,SPACE *,qword);
JOB *func_0012DE58(LO *,CHANNEL *,int,float,qword,qword);
M3 *ConvertXfMat(M3 *,SPACE *,SPACE *,const M3 *);
qword func_0011BFF0(const M3 *);
JOB *func_0012DC18(LO *,CHANNEL *,int,int,int,qword);
void func_001F6860(LO *,int);
}
static inline void*operator new(unsigned size,void*destination){return destination;}
static inline SPACE *SourceMatrix(SOURCE *p,M4 *out){VTE &v=p->vt[32];return ((SPACE*(*)(SOURCE *,M4 *))v.fn)((SOURCE *)((char *)p+v.delta),out);}
static inline M4 *WorldMatrix(LO *p){VTE &v=p->vt[31];return ((M4*(*)(LO *))v.fn)((LO *)((char *)p+v.delta));}
static inline float Max(float a,float b){float r;__asm__("max.s %0,%1,%2":"=f"(r):"f"(a),"f"(b));return r;}
static inline qword*TranslationSlot(WORK&w){register qword*slot __asm__("$4");__asm__("addiu %0,%1,0xB0":"=r"(slot):"r"(&w));return slot;}
static inline M4*SampleOutput(M4*value){register M4*result __asm__("$8");__asm__("daddu %0,%1,$zero":"=r"(result):"r"(value));return result;}
static inline void DeltaForDuration(WORK&w,qword raw,float&bound,qword&speed){register qword destination __asm__("vf1"),difference __asm__("vf2");register unsigned page __asm__("$3");
 __asm__("lqc2 %0,%3\n\tlui %1,%%hi(D_002C1EF0)\n\tqmtc2.ni %4,%2":"=j"(destination),"=&r"(page),"=j"(difference):"m"(w.destination),"r"(raw));
 register float limit __asm__("$f12");register qword velocity __asm__("$5");
 __asm__("lwc1 %0,%%lo(D_002C1EF0)(%4)\n\tvsub.xyzw %1,%5,%1\n\tlq %2,%6\n\tsqc2 %1,%3":"=f"(limit),"+j"(difference),"=r"(velocity),"=m"(w.delta):"r"(page),"j"(destination),"m"(w.velocity),"m"(*(float*)(page+0x1EF0)));
 bound=limit;speed=velocity;
}
struct __attribute__((packed)) BASIS_TAIL {float firstW;qword row1,row2;};
/** @sly2-annot addr=0x1CCC88
 * Purpose: Create position and orientation jobs that carry the selected animated object along the predicted
 *   clip path to the hand (ballistic time, SetLoParent)
 * Info: proposed name StartJtHandsCarry (T2); TU jthands
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
extern "C" void func_001CCC88(P *input,LO *other){
 register P*p __asm__("$22");register LO*object __asm__("$20");
 __asm__("daddu %0,%1,$zero":"=r"(p):"r"(input));
 __asm__("daddu %0,%1,$zero":"=r"(object):"r"(other));
 WORK work;
 ReleaseAct(&p->positionJob);register float zero __asm__("$f21")=0.0f;ReleaseAct(&p->otherJob);ReleaseAct(&p->orientationJob);
 ANIM *anim=p->owner->playing;MOTION *motion=anim->motion;
 float time=func_0012F990(motion,0x1C7),phase=(time-anim->start)/anim->duration;
 SPACE *space=SourceMatrix(p->source,&work.original);register M4*workingBase __asm__("$19")=&work.sample;
 func_001300B8(motion,time,zero,p->owner,space,p->owner->space,SampleOutput(workingBase),0);
 LO *owner=p->owner;
 M4 *start=&work.start;M3 *basis=&owner->basis;register M4*product __asm__("$21");
 qword location=func_001A62E8(owner);
 VECTOR_opAssign(TranslationSlot(work),location);
 {
 register qword firstRow __asm__("$2");register M4*intermediate __asm__("$16");register float homogeneous __asm__("$f0");
 __asm__("lq %0,%2\n\taddiu %1,%3,0x140":"=&r"(firstRow),"=r"(product):"m"(owner->basis.rows[0]),"r"(&work));
 __asm__(".set noat\n\tlui $at,0x3F80\n\tmtc1 $at,%0\n\t.set at\n\taddiu %1,%3,0x100\n\tsq %4,%2":"=f"(homogeneous),"=&r"(intermediate),"=m"(work.start.rows[0]):"r"(&work),"r"(firstRow):"$1");
 register const M4*left __asm__("$5"),*right __asm__("$6");register M4*destination __asm__("$4");
 __asm__("daddu %0,%3,$zero\n\tdaddu %1,%4,$zero\n\tdaddu %2,%5,$zero":"=&r"(left),"=&r"(right),"=r"(destination):"r"(start),"r"(workingBase),"r"(product));
 register qword row1 __asm__("$3"),row2 __asm__("$2");
 __asm__("lq %0,16(%4)\n\tdaddu %2,%6,$zero\n\tsq %0,16(%5)\n\tlq %1,32(%4)\n\tsq %1,32(%5)\n\tswc1 %7,%3\n\tswc1 %7,32+%3\n\tswc1 %7,16+%3"
  :"=&r"(row1),"=&r"(row2),"=&r"(workingBase),"=m"(*(BASIS_TAIL*)((char*)&work.start+12)):"r"(basis),"r"(start),"r"(intermediate),"f"(zero));
 start->values[15]=homogeneous;__ml__FRC7MATRIX4T0(destination,left,right);
 register const M4*originalArgument __asm__("$6");register M4*secondDestination __asm__("$4");
 __asm__("daddu %0,%2,$zero\n\tdaddu %1,%3,$zero":"=r"(originalArgument),"=r"(secondDestination):"r"(&work.original),"r"(intermediate));
 __ml__FRC7MATRIX4T0(secondDestination,product,originalArgument);
 }
 LO*counterpart;
 {
 register qword first __asm__("$7"),row1 __asm__("$2"),row2 __asm__("$3"),row3 __asm__("$6");register LO*objectArgument __asm__("$5"),*ownerArgument __asm__("$4");
 __asm__("lq %0,%2\n\tdaddu %1,%3,$zero":"=&r"(first),"=r"(objectArgument):"m"(work.views.matrices.intermediate.rows[0]),"r"(object));
 __asm__("lq %0,%6\n\tlq %1,%7\n\tlq %2,%8\n\tlw %3,%9\n\tsq %5,%4\n\tsq %0,16+%4\n\tsq %1,32+%4"
  :"=&r"(row1),"=&r"(row2),"=&r"(row3),"=&r"(ownerArgument),"=m"(*(M3*)&work.final):"r"(first),"m"(work.views.matrices.intermediate.rows[1]),"m"(work.views.matrices.intermediate.rows[2]),"m"(work.views.matrices.intermediate.rows[3]),"m"(p->owner));
 work.final.rows[3]=row3;counterpart=func_001CCA40(ownerArgument,objectArgument);
 }
 register M4*finalMatrix __asm__("$17")=&work.final;
 if(counterpart){
  M4*counterMatrix=WorldMatrix(counterpart);register const M4*counterArgument __asm__("$7");register LO*counterSpace __asm__("$6");
  __asm__("daddu %0,%2,$zero\n\tdaddu %1,%3,$zero":"=&r"(counterArgument),"=r"(counterSpace):"r"(counterMatrix),"r"(object));
  new(product)M4(ConvertAloMat4(0,counterSpace,counterArgument));
  register M4*inverse __asm__("$16");
  {
  register qword row1 __asm__("$6"),row2 __asm__("$3"),row3 __asm__("$2"),row0 __asm__("$7");register M4*inverseArgument __asm__("$4");register const M4*sourceArgument __asm__("$5");
  __asm__("lq %0,%4\n\taddiu %1,%5,0x180\n\tlq %2,%6\n\tdaddu %3,%1,$zero":"=&r"(row1),"=&r"(inverse),"=r"(row2),"=r"(inverseArgument):"m"(work.views.matrices.product.rows[1]),"r"(&work),"m"(work.views.matrices.product.rows[2]));
  __asm__("lq %0,%4\n\tdaddu %1,%5,$zero\n\tlq %2,%6\n\tsq %7,%3\n\tsq %8,16+%3\n\tsq %0,32+%3"
   :"=&r"(row3),"=&r"(sourceArgument),"=&r"(row0),"=m"(*(M3*)&work.views.matrices.intermediate.rows[1]):"m"(work.views.matrices.product.rows[3]),"r"(workingBase),"m"(work.views.matrices.product.rows[0]),"r"(row1),"r"(row2));
  work.views.matrices.intermediate.rows[0]=row0;new(inverseArgument)M4(InvertMatrix4Rigid(sourceArgument));
  }
  __ml__FRC7MATRIX4T0(product,finalMatrix,inverse);
  {
  register qword row0 __asm__("$2"),row1 __asm__("$3"),row2 __asm__("$4"),row3 __asm__("$5");
  __asm__("lq %0,%5\n\tlq %1,%6\n\tlq %2,%7\n\tlq %3,%8\n\tsq %0,%4\n\tsq %1,16+%4\n\tsq %2,32+%4\n\tsq %3,48+%4"
   :"=&r"(row0),"=&r"(row1),"=&r"(row2),"=&r"(row3),"=m"(work.final):"m"(work.views.matrices.product.rows[0]),"m"(work.views.matrices.product.rows[1]),"m"(work.views.matrices.product.rows[2]),"m"(work.views.matrices.product.rows[3]));
  }
 }
 SetLoParent_1A58D0(object,0,15);
 const qword*velocityAddress=object->follow?&object->follow->velocity:&D_002C1EE0;register float unit __asm__("$f20");register LO*initialOwner __asm__("$4");{register qword initialVelocity __asm__("$2");
 __asm__("lq %0,%4\n\tdaddu %1,%5,$zero\n\t.set noat\n\tlui $at,0x3F80\n\tmtc1 $at,%2\n\t.set at\n\tsq %0,%3":"=r"(initialVelocity),"=r"(initialOwner),"=f"(unit),"=m"(work.velocity):"m"(*velocityAddress),"r"(object):"$1");}
 {register qword destination __asm__("$2");__asm__("lq %0,%1":"=r"(destination):"m"(work.final.rows[3]));work.destination=destination;}
 float limit;qword initialSpeed;DeltaForDuration(work,func_001A62E8(initialOwner),limit,initialSpeed);
 register float negativeLimit __asm__("$f13"),minimum __asm__("$f15"),maximum __asm__("$f14");
 __asm__("neg.s %0,%3\n\t.set noat\n\tlui $at,0xBF80\n\tmtc1 $at,%1\n\t.set at\n\tmov.s %2,%4":"=f"(negativeLimit),"=f"(minimum),"=f"(maximum):"f"(limit),"f"(unit):"$1");
 float duration=Max(phase,func_00196928(work.delta,initialSpeed,limit,negativeLimit,maximum,minimum));
 float speed=phase/duration;phase=duration;
 func_00131A00(anim,speed);
 {
 register float negativeTime __asm__("$f0"),positiveTime __asm__("$f1");
 register qword first __asm__("vf4"),speedVector __asm__("vf5");
 __asm__("div.s %4,%4,%5\n\t.set noat\n\tlui $at,0xBF00\n\tmtc1 $at,%0\n\tlui $at,0x3F00\n\tmtc1 $at,%1\n\t.set at\n\tmul.s %0,%5,%0\n\tlqc2 %2,%6\n\tmul.s %1,%5,%1\n\tlqc2 %3,%7"
  :"=&f"(negativeTime),"=&f"(positiveTime),"=j"(speedVector),"=j"(first),"+&f"(unit):"f"(phase),"m"(work.velocity),"m"(work.delta):"$1");
 register qword negativeOrEnd __asm__("vf2"),inverse __asm__("vf3");register int positiveBits __asm__("$3");register VTE*table __asm__("$2");
 __asm__("mfc1 %2,%4\n\tmfc1 %3,%5\n\tqmtc2.ni %2,%0\n\tqmtc2.ni %3,%1\n\tmfc1 %2,%6\n\tlw %3,%7"
  :"=j"(negativeOrEnd),"=j"(inverse),"=&r"(positiveBits),"=&r"(table):"f"(negativeTime),"f"(unit),"f"(positiveTime),"m"(object->vt));
 __asm__("vmulax.xyzw $ACC,%0,%2x\n\tvmaddx.xyzw %0,%3,%4x\n\tsqc2 %2,%1":"+j"(first),"=m"(work.views.scalar.q):"j"(inverse),"j"(speedVector),"j"(negativeOrEnd));
 register qword positive __asm__("vf1"),callbackVelocity __asm__("$5");
 __asm__("qmtc2.ni %4,%0\n\tsqc2 %1,%3\n\tlqc2 %1,%5\n\tvmulax.xyzw $ACC,%1,%6x\n\tvmaddx.xyzw %1,%7,%0x\n\tqmfc2.ni %2,%8"
  :"=&j"(positive),"+&j"(negativeOrEnd),"=r"(callbackVelocity),"=m"(work.views.scalar.q):"r"(positiveBits),"m"(work.delta),"j"(inverse),"j"(speedVector),"j"(first));
 typedef void(*APPLY)(LO*,qword);register APPLY apply __asm__("$6");register LO*receiver __asm__("$4");
 __asm__("lh %0,%4\n\tlw %1,%5\n\tsqc2 %6,%2\n\taddu %0,%8,%0\n\tsqc2 %7,%3"
  :"=&r"(receiver),"=&r"(apply),"=m"(work.views.scalar.q),"=m"(work.endVelocity):"m"(table[35].delta),"m"(table[35].fn),"j"(inverse),"j"(negativeOrEnd),"r"(object));
 work.views.scalar.q=positive;apply(receiver,callbackVelocity);
 }
 float endTime=D_002DDED0.time+phase;
 qword position=func_0022FC48(0,object->position->space,work.final.rows[3]);
 qword endVelocity=ConvertXfVec(0,object->position->space,work.endVelocity);
 {
 register CHANNEL*channel __asm__("$5");register qword velocityArgument __asm__("$8"),positionArgument __asm__("$7");register float timeArgument __asm__("$f12");register LO*receiver __asm__("$4");
 __asm__("lw %0,%5\n\tpor %1,$zero,%6\n\tmov.s %2,%7\n\tpor %3,$zero,%8\n\tdaddu %4,%9,$zero"
  :"=&r"(channel),"=&r"(velocityArgument),"=f"(timeArgument),"=&r"(positionArgument),"=r"(receiver):"m"(object->position),"r"(endVelocity),"f"(endTime),"r"(position),"r"(object));
 p->positionJob=func_0012DE58(receiver,channel,5,timeArgument,positionArgument,velocityArgument);
 }
 ConvertXfMat(&work.views.basis.converted,0,object->orientation->space,(M3*)finalMatrix);
 work.views.basis.angles=work.views.basis.converted;
 qword angles=func_0011BFF0((M3*)workingBase);
 register CHANNEL*orientationChannel __asm__("$5");register LO*orientationOwner __asm__("$4");register qword anglesArgument __asm__("$9");
 __asm__("lw %0,%3\n\tdaddu %1,%4,$zero\n\tpor %2,$zero,%5":"=&r"(orientationChannel),"=&r"(orientationOwner),"=r"(anglesArgument):"m"(object->orientation),"r"(object),"r"(angles));
 JOB *job=func_0012DC18(orientationOwner,orientationChannel,5,9,2,anglesArgument);p->orientationJob=job;
 if(object->orientation->current==job)object->orientation->endTime=endTime;
 func_001F6860(object,1);
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char motion_job_layout[(sizeof(void*)==4 && sizeof(qword)==16 && sizeof(U)==16 && sizeof(M3)==0x30 && sizeof(M4)==0x40 && sizeof(VTE)==8 && sizeof(VIEWS)==0xC0 && sizeof(WORK)==0x200 && OFF(VTE,delta)==0 && OFF(VTE,fn)==4 && OFF(CHANNEL,current)==0x90 && OFF(CHANNEL,space)==0xA0 && OFF(CHANNEL,endTime)==0xA8 && OFF(ANIM,motion)==0x14 && OFF(ANIM,start)==0x1C && OFF(ANIM,duration)==0x40 && OFF(FOLLOW,velocity)==0x80 && OFF(LO,vt)==0xC && OFF(LO,position)==0x58 && OFF(LO,orientation)==0x5C && OFF(LO,space)==0x60 && OFF(LO,follow)==0xE8 && OFF(LO,basis)==0x1B0 && OFF(LO,playing)==0x1030 && OFF(SOURCE,vt)==0xC && OFF(P,owner)==0 && OFF(P,source)==0x34 && OFF(P,positionJob)==0x94 && OFF(P,otherJob)==0x98 && OFF(P,orientationJob)==0x9C && OFF(CLOCK,time)==0 && OFF(VIEWS,matrices.intermediate)==0 && OFF(VIEWS,matrices.product)==0x40 && OFF(VIEWS,matrices.inverse)==0x80 && OFF(VIEWS,basis.angles)==0 && OFF(VIEWS,basis.converted)==0x30 && OFF(WORK,original)==0 && OFF(WORK,sample)==0x40 && OFF(WORK,start)==0x80 && OFF(WORK,final)==0xC0 && OFF(WORK,views)==0x100 && OFF(WORK,velocity)==0x1C0 && OFF(WORK,destination)==0x1D0 && OFF(WORK,delta)==0x1E0 && OFF(WORK,endVelocity)==0x1F0)?1:-1];

typedef char inverse_field_layout[(__alignof__(M4)==0x10 && OFF(WORK,views)+OFF(VIEWS,matrices.inverse)==0x180 && OFF(WORK,views)+OFF(VIEWS,matrices.intermediate)==0x100)?1:-1];

typedef char frame_matrix_result_layout[(OFF(WORK,views)+OFF(VIEWS,matrices.product)==0x140)?1:-1];
typedef char ccc88_final_0[(sizeof(int)==4)?1:-1];
typedef char ccc88_final_1[(sizeof(unsigned)==4)?1:-1];
typedef char ccc88_final_2[(sizeof(short)==2)?1:-1];
typedef char ccc88_final_3[(sizeof(float)==4)?1:-1];
typedef char ccc88_final_4[(sizeof(CLOCK)==8)?1:-1];
typedef char ccc88_final_5[(__alignof__(qword)==16)?1:-1];
typedef char ccc88_final_6[(__alignof__(U)==16)?1:-1];
typedef char ccc88_final_7[(__alignof__(M4)==16)?1:-1];
typedef char ccc88_final_8[(__alignof__(M3)==16)?1:-1];
typedef char ccc88_final_9[(__alignof__(WORK)==16)?1:-1];
typedef char ccc88_final_10[(__alignof__(VIEWS)==16)?1:-1];
typedef char ccc88_final_11[(sizeof(BASIS_TAIL)==36)?1:-1];
typedef char ccc88_final_12[(__alignof__(BASIS_TAIL)==1)?1:-1];
typedef char ccc88_final_13[(OFF(BASIS_TAIL,firstW)==0)?1:-1];
typedef char ccc88_final_14[(OFF(BASIS_TAIL,row1)==4)?1:-1];
typedef char ccc88_final_15[(OFF(BASIS_TAIL,row2)==20)?1:-1];
typedef char ccc88_final_16[(OFF(WORK,start)+12==0x8C)?1:-1];
typedef char ccc88_final_17[(OFF(WORK,start)+28==0x9C)?1:-1];
typedef char ccc88_final_18[(OFF(WORK,start)+44==0xAC)?1:-1];
typedef char ccc88_final_19[(OFF(U,q)==0)?1:-1];
typedef char ccc88_final_20[(OFF(U,f)==0)?1:-1];
typedef char ccc88_final_21[(OFF(M4,rows)==0)?1:-1];
typedef char ccc88_final_22[(OFF(M4,values)==0)?1:-1];
typedef char ccc88_final_23[(OFF(M4,values[3])==0xC)?1:-1];
typedef char ccc88_final_24[(OFF(M4,values[7])==0x1C)?1:-1];
typedef char ccc88_final_25[(OFF(M4,values[11])==0x2C)?1:-1];
typedef char ccc88_final_26[(OFF(M4,values[15])==0x3C)?1:-1];
typedef char ccc88_final_27[(OFF(LO,basis.rows[1])==0x1C0)?1:-1];
typedef char ccc88_final_28[(OFF(LO,basis.rows[2])==0x1D0)?1:-1];
typedef char ccc88_final_29[(31*sizeof(VTE)+OFF(VTE,delta)==0xF8)?1:-1];
typedef char ccc88_final_30[(31*sizeof(VTE)+OFF(VTE,fn)==0xFC)?1:-1];
typedef char ccc88_final_31[(32*sizeof(VTE)+OFF(VTE,delta)==0x100)?1:-1];
typedef char ccc88_final_32[(32*sizeof(VTE)+OFF(VTE,fn)==0x104)?1:-1];
typedef char ccc88_final_33[(35*sizeof(VTE)+OFF(VTE,delta)==0x118)?1:-1];
typedef char ccc88_final_34[(35*sizeof(VTE)+OFF(VTE,fn)==0x11C)?1:-1];
typedef char ccc88_final_35[(sizeof(((VTE*)0)->delta)==2)?1:-1];
typedef char ccc88_final_36[(sizeof(((VTE*)0)->fn)==4)?1:-1];
typedef char ccc88_final_37[(sizeof(((WORK*)0)->velocity)==16)?1:-1];
typedef char ccc88_final_38[(sizeof(((WORK*)0)->destination)==16)?1:-1];
typedef char ccc88_final_39[(sizeof(((WORK*)0)->delta)==16)?1:-1];
typedef char ccc88_final_40[(sizeof(((WORK*)0)->endVelocity)==16)?1:-1];
typedef char ccc88_final_41[(OFF(WORK,views)+OFF(VIEWS,scalar)==0x100)?1:-1];
typedef char ccc88_final_42[(OFF(WORK,views)+OFF(VIEWS,basis.angles)==0x100)?1:-1];
typedef char ccc88_final_43[(OFF(WORK,views)+OFF(VIEWS,basis.converted)==0x130)?1:-1];
typedef char ccc88_final_44[(OFF(WORK,views)+OFF(VIEWS,matrices.intermediate.rows[1])==0x110)?1:-1];
typedef char ccc88_final_45[(OFF(WORK,views)+OFF(VIEWS,matrices.product.rows[3])==0x170)?1:-1];
typedef char ccc88_final_46[(OFF(WORK,final.rows[3])==0xF0)?1:-1];
typedef char ccc88_final_47[(sizeof(((CHANNEL*)0)->endTime)==4)?1:-1];
