// Update attachment correction and decay inactive responses.

typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
struct SPACE;
struct M3{U r[3];};struct M4{U r[4];};struct LM{float low,high;};struct CLOCK{float time,dt;};
struct MODEL{char pad[0x1C];float base;char pad20[0x40-0x20];float scale;};
struct AIM{char pad[0x150];struct SPACE*frame;};struct OWNER{char pad[0x494];struct AIM*aim;};
struct GROUP{char pad[0xA0];struct SPACE*space;};struct FOCUS{char pad[0x58];struct GROUP*group;};
struct DV{char pad[0x20];qword value;};
struct P{char pad[0x60];struct SPACE*space;char pad64[0x1030-0x64];struct MODEL*model;char pad1034[0x10DC-0x1034];struct OWNER*owner;char pad10E0[0x19A0-0x10E0];struct FOCUS*focus;char pad19A4[0x19EC-0x19A4];struct DV*dvResidual;char pad19F0[8];struct DV*dvYaw;struct DV*dvX;char pad1A00[0x1BD4-0x1A00];int valid;float height,time;char pad1BE0[0x10];qword anchor,position,euler,localPoint;};
struct WORK{union{struct M4 predicted;U scalar;}phase;struct M4 transform;U axis;struct M3 rotation;U relative,residual,gain;};
extern float D_002BF380,D_002BF3A0,D_002BF3B8,D_002BF3BC;
extern qword D_002BF390;extern struct LM D_002BF3A8,D_002BF3B0;extern struct CLOCK D_002DDED0;
void FaceStepToward(struct P*,struct AIM*);void LoadIdentityMatrix4(struct SPACE*,struct SPACE*,struct M4*,struct M4*,float);
qword func_0022FC48(struct SPACE*,struct SPACE*,qword);qword ConvertXfVec(struct SPACE*,struct SPACE*,qword);
float func_002458F0(float,float);float func_0011D948(struct LM*,float);
struct M3*LoadRotateMatrixEuler(struct M3*,qword);qword*VECTOR_opAssign(qword*,qword);void func_0012DA30(struct DV*,qword);
static inline qword Add(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Point(const struct M4*m,const qword*a,struct SPACE**source){
 register qword r __asm__("vf6"),row0 __asm__("vf5")=m->r[0].q;register struct SPACE*sourceValue __asm__("$4");const int nullSource=0;
 __asm__ volatile("lqc2 $vf4,%3\n\tlqc2 $vf2,%4\n\tdaddu %1,%z7,$zero\n\tlqc2 $vf1,%5\n\tlqc2 $vf3,%6\n\tvmulax.xyz $ACC,%2,$vf3x\n\tvmadday.xyz $ACC,$vf4,$vf3y\n\tvmaddaz.xyz $ACC,$vf2,$vf3z\n\tvmaddw.xyz %0,$vf1,$vf0w"
 :"=&j"(r),"=&r"(sourceValue):"j"(row0),"m"(m->r[1].q),"m"(m->r[2].q),"m"(m->r[3].q),"m"(*a),"rJ"(nullSource):"vf1","vf2","vf3","vf4");
 *source=sourceValue;return r;
}
static inline float X(qword q){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(q));return x;}
static inline qword Pack(float x,float y,float z){qword q,t;__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(q),"=&r"(t):"f"(x),"f"(y),"f"(z));return q;}
static inline float CorrectionDistance(struct P*p,qword correction){
 register qword d __asm__("vf1");register float result __asm__("$3");
 __asm__ __volatile__("lqc2 %0,%1":"=j"(d):"m"(p->position));
 {register qword c __asm__("vf2")=correction;__asm__("vadd.xyzw %0,%0,%1":"+j"(d):"j"(c));}
 {register qword a __asm__("vf2")=p->anchor;__asm__("vsub.xyzw %0,%0,%1":"+j"(d):"j"(a));}
 {register qword one __asm__("vf2");__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d),"=&j"(one));}
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));__asm__("qmfc2.ni %0,%1":"=r"(result):"j"(d));return result;
}
/** @sly2-annot addr=0x18B460
 * Purpose: jts 11 handler: attachment correction, decays inactive responses
 * Info: proposed name UpdateJtAttachCorrection (T2); TU jt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018B460(struct P*p){
 struct WORK w;register float ratio __asm__("$f20");{register float height __asm__("$f0")=p->height,base __asm__("$f1")=p->model->base,scale __asm__("$f2")=p->model->scale;height=height-base;__asm__("div.s %0,%1,%2":"=f"(ratio):"f"(height),"f"(scale));}
 if(p->owner&&p->owner->aim){
 float zero=0.0f;
 if(zero<ratio){
  register qword mapped __asm__("$17");qword tilt,axis,residual;register float limit __asm__("$f1");struct SPACE*space;register struct SPACE*relativeDestination __asm__("$5");struct M4*frame;float heading;
  FaceStepToward(p,p->owner->aim);
  {register struct OWNER*predictionOwner __asm__("$2")=p->owner;
   register float capped __asm__("$f12");register struct M4*request __asm__("$6"),*optionalOutput __asm__("$7");
   register struct AIM*aim __asm__("$3");register struct SPACE*sourceSpace __asm__("$5");const int nullValue=0;
   __asm__("lwc1 %0,%6\n\tdaddu %1,%7,$zero\n\tlw %2,%8\n\tdaddu %3,%z9,$zero\n\tmin.s %0,%5,%0\n\tdaddu %4,%z9,$zero"
    :"=&f"(capped),"=&r"(request),"=&r"(aim),"=r"(optionalOutput),"=r"(sourceSpace)
    :"f"(ratio),"m"(D_002BF380),"r"(&w.phase.predicted),"m"(predictionOwner->aim),"rJ"(nullValue));
   LoadIdentityMatrix4(aim->frame,sourceSpace,request,optionalOutput,capped);
  }
  {struct SPACE*sourceSpace;qword predictedPoint=Point(&w.phase.predicted,&D_002BF390,&sourceSpace);register unsigned page __asm__("$3");
   __asm__("por %0,$zero,%2\n\tlui %1,%%hi(%3)":"=r"(mapped),"=r"(page):"r"(func_0022FC48(sourceSpace,p->space,predictedPoint)),"i"(&D_002BF3A0));
   {register int zBits __asm__("$2");register float z __asm__("$f12"),x __asm__("$f13"),coefficient __asm__("$f0");
    __asm__("pextuw %0,$zero,%4\n\tlwc1 %3,%%lo(%6)(%5)\n\tmtc1 %0,%1\n\tmtc1 %4,%2":"=&r"(zBits),"=f"(z),"=f"(x),"=f"(coefficient):"r"(mapped),"r"(page),"i"(&D_002BF3A0):"memory");
    heading=func_002458F0(-z,coefficient*x);}}
  heading=func_0011D948(&D_002BF3A8,heading);
  tilt=Pack(zero,heading,zero);w.axis.q=tilt;
  LoadRotateMatrixEuler(&w.rotation,Add(p->euler,w.axis.q));
  frame=&w.transform;
  VECTOR_opAssign(&frame->r[3].q,p->position);
  {
   register qword row0 __asm__("$3"),row1 __asm__("$2"),mappedVector __asm__("vf6");
   __asm__("lq %2,%5\n\tqmtc2.ni %8,%4\n\tlq %3,%6\n\tsq %2,%0\n\tsq %3,0x10(%7)"
   :"=m"(w.transform.r[0].q),"=m"(w.transform.r[1].q),"=&r"(row0),"=&r"(row1),"=j"(mappedVector)
   :"m"(w.rotation.r[0].q),"m"(w.rotation.r[1].q),"r"(frame),"r"(mapped));
   {
    register float one __asm__("$f0")=1.0f;register qword row2 __asm__("$2");register struct FOCUS*focus __asm__("$3");register struct GROUP*group __asm__("$2");
    __asm__("lq %0,%2\n\tlw %1,%3":"=&r"(row2),"=r"(focus):"m"(w.rotation.r[2].q),"m"(p->focus));
    __asm__("sq %1,0x20(%2)":"=m"(w.transform.r[2].q):"r"(row2),"r"(frame));
    __asm__("swc1 %5,%0\n\tswc1 %5,%1\n\tswc1 %5,%2\n\tswc1 %6,0x3c(%7)\n\tlw %4,0x58(%8)"
    :"=m"(w.transform.r[2].f[3]),"=m"(w.transform.r[1].f[3]),"=m"(w.transform.r[0].f[3]),"=m"(w.transform.r[3].f[3]),"=r"(group)
    :"f"(zero),"f"(one),"r"(frame),"r"(focus),"m"(focus->group));
    {
     register qword point __asm__("vf4"),axis2 __asm__("vf2"),axis0 __asm__("vf5"),axis1 __asm__("vf3"),origin __asm__("vf1");
     __asm__("lqc2 %0,%5\n\tlqc2 %1,%6\n\tlqc2 %2,%7\n\tlqc2 %3,%8\n\tlqc2 %4,%9"
     :"=j"(point),"=j"(axis2),"=j"(axis0),"=j"(axis1),"=j"(origin)
     :"m"(p->localPoint),"m"(w.transform.r[2].q),"m"(w.transform.r[0].q),"m"(w.transform.r[1].q),"m"(w.transform.r[3].q));
     __asm__("vmulax.xyz ACC,%2,%5x\n\tvmadday.xyz ACC,%3,%5y\n\tvmaddaz.xyz ACC,%4,%5z\n\tvmaddw.xyz %0,%0,$vf0w\n\tlw %1,%6":"+j"(origin),"=r"(space):"j"(axis0),"j"(axis1),"j"(axis2),"j"(point),"m"(group->space));
     __asm__("vsub.xyzw %0,%0,%1":"+j"(mappedVector):"j"(origin));
     __asm__("sqc2 %2,%0\n\tdaddu %1,%3,$zero":"=m"(w.relative.q),"=r"(relativeDestination):"j"(mappedVector),"r"(space));
    }
   }
  }
  heading=X(ConvertXfVec(p->space,relativeDestination,w.relative.q));heading=func_0011D948(&D_002BF3B0,heading);
  axis=Pack(heading,zero,zero);w.axis.q=axis;
  {
   register qword returned __asm__("vf2")=ConvertXfVec(space,p->space,w.axis.q),relative __asm__("vf1");register unsigned page __asm__("$2");
   __asm__("lqc2 %0,%2\n\tlui %1,%%hi(%3)":"=j"(relative),"=r"(page):"m"(w.relative.q),"i"(&D_002BF3B8));
   __asm__("vsub.xyzw %0,%1,%0":"+j"(returned):"j"(relative));
   __asm__("lwc1 %0,%%lo(%4)(%2)\n\tsqc2 %3,%1":"=f"(limit),"=m"(w.residual.q):"r"(page),"j"(returned),"i"(&D_002BF3B8):"memory");
   residual=returned;
  }
  {register int near __asm__("$2")=1;float distance=CorrectionDistance(p,residual);if(distance<limit){}else near=0;if(near){
   func_0012DA30(p->dvYaw,tilt);func_0012DA30(p->dvResidual,w.residual.q);func_0012DA30(p->dvX,axis);
   p->valid=1;p->time=D_002DDED0.time;return;
  }}
 }}
 {register float remaining __asm__("$f0")=p->time,after __asm__("$f2"),gain __asm__("$f3");
  {register float life __asm__("$f1")=D_002BF3BC,now __asm__("$f3")=D_002DDED0.time;remaining=remaining+life;after=D_002DDED0.dt;remaining=remaining-now;after=remaining-after;}
  {register float zero __asm__("$f1")=0.0f;if(zero<after)gain=after/remaining;else gain=zero;}
  {register qword bits __asm__("$2");__asm__("mfc1 %0,%2\n\tsq %0,%1":"=&r"(bits),"=m"(w.gain.q):"f"(gain));}
  {
   register struct DV*source __asm__("$2"),*receiver __asm__("$4");register qword raw __asm__("$3"),scalar __asm__("vf2");
   __asm__("lw %0,%2\n\tlq %1,%3":"=&r"(source),"=r"(raw):"m"(p->dvYaw),"m"(w.gain.q));
   __asm__("daddu %0,%3,$zero\n\tsq %4,%1\n\tqmtc2.ni %4,%2":"=&r"(receiver),"=m"(w.phase.scalar.q),"=j"(scalar):"r"(source),"r"(raw));
   __asm__("vmulx.xyzw %0,%1,%0x":"+j"(scalar):"j"(source->value));func_0012DA30(receiver,scalar);
  }
  {
   register qword raw __asm__("$2"),scalar __asm__("vf2");register struct DV*receiver __asm__("$4");
   __asm__("lq %0,%1":"=r"(raw):"m"(w.gain.q));
   __asm__("sq %3,%0\n\tqmtc2.ni %3,%1\n\tlw %2,%4":"=m"(w.phase.scalar.q),"=j"(scalar),"=r"(receiver):"r"(raw),"m"(p->dvResidual));
   __asm__("vmulx.xyzw %0,%1,%0x":"+j"(scalar):"j"(receiver->value));func_0012DA30(receiver,scalar);
  }
  {
   register struct DV*source __asm__("$2"),*receiver __asm__("$4");register qword raw __asm__("$3"),scalar __asm__("vf2");
   __asm__("lq %0,%2\n\tlw %1,%3":"=&r"(raw),"=r"(source):"m"(w.gain.q),"m"(p->dvX));
   __asm__("sq %3,%0\n\tqmtc2.ni %3,%1\n\tdaddu %2,%4,$zero":"=m"(w.phase.scalar.q),"=j"(scalar),"=r"(receiver):"r"(raw),"r"(source));
   __asm__("vmulx.xyzw %0,%1,%0x":"+j"(scalar):"j"(source->value));func_0012DA30(receiver,scalar);
  }
  p->valid=0;
 }
}

typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M3_Size[(sizeof(struct M3)==48)?1:-1];
typedef char M4_Size[(sizeof(struct M4)==64)?1:-1];
typedef char LM_Size[(sizeof(struct LM)==8)?1:-1];
typedef char CLOCK_Size[(sizeof(struct CLOCK)==8)?1:-1];
typedef char DV_Size[(sizeof(struct DV)==48)?1:-1];
typedef char WORK_Size[(sizeof(struct WORK)==240)?1:-1];
typedef char MODEL_base_Offset[((unsigned)&((struct MODEL*)0)->base==28)?1:-1];
typedef char MODEL_scale_Offset[((unsigned)&((struct MODEL*)0)->scale==64)?1:-1];
typedef char AIM_frame_Offset[((unsigned)&((struct AIM*)0)->frame==336)?1:-1];
typedef char OWNER_aim_Offset[((unsigned)&((struct OWNER*)0)->aim==1172)?1:-1];
typedef char GROUP_space_Offset[((unsigned)&((struct GROUP*)0)->space==160)?1:-1];
typedef char FOCUS_group_Offset[((unsigned)&((struct FOCUS*)0)->group==88)?1:-1];
typedef char DV_value_Offset[((unsigned)&((struct DV*)0)->value==32)?1:-1];
typedef char P_space_Offset[((unsigned)&((struct P*)0)->space==96)?1:-1];
typedef char P_model_Offset[((unsigned)&((struct P*)0)->model==4144)?1:-1];
typedef char P_owner_Offset[((unsigned)&((struct P*)0)->owner==4316)?1:-1];
typedef char P_focus_Offset[((unsigned)&((struct P*)0)->focus==6560)?1:-1];
typedef char P_dvResidual_Offset[((unsigned)&((struct P*)0)->dvResidual==6636)?1:-1];
typedef char P_dvYaw_Offset[((unsigned)&((struct P*)0)->dvYaw==6648)?1:-1];
typedef char P_dvX_Offset[((unsigned)&((struct P*)0)->dvX==6652)?1:-1];
typedef char P_valid_Offset[((unsigned)&((struct P*)0)->valid==7124)?1:-1];
typedef char P_height_Offset[((unsigned)&((struct P*)0)->height==7128)?1:-1];
typedef char P_time_Offset[((unsigned)&((struct P*)0)->time==7132)?1:-1];
typedef char P_anchor_Offset[((unsigned)&((struct P*)0)->anchor==7152)?1:-1];
typedef char P_position_Offset[((unsigned)&((struct P*)0)->position==7168)?1:-1];
typedef char P_euler_Offset[((unsigned)&((struct P*)0)->euler==7184)?1:-1];
typedef char P_localPoint_Offset[((unsigned)&((struct P*)0)->localPoint==7200)?1:-1];
typedef char WORK_phase_Offset[((unsigned)&((struct WORK*)0)->phase==0)?1:-1];
typedef char WORK_transform_Offset[((unsigned)&((struct WORK*)0)->transform==64)?1:-1];
typedef char WORK_axis_Offset[((unsigned)&((struct WORK*)0)->axis==128)?1:-1];
typedef char WORK_rotation_Offset[((unsigned)&((struct WORK*)0)->rotation==144)?1:-1];
typedef char WORK_relative_Offset[((unsigned)&((struct WORK*)0)->relative==192)?1:-1];
typedef char WORK_residual_Offset[((unsigned)&((struct WORK*)0)->residual==208)?1:-1];
typedef char WORK_gain_Offset[((unsigned)&((struct WORK*)0)->gain==224)?1:-1];
typedef char CLOCK_time_Offset[((unsigned)&((struct CLOCK*)0)->time==0)?1:-1];
typedef char CLOCK_dt_Offset[((unsigned)&((struct CLOCK*)0)->dt==4)?1:-1];
typedef char LM_low_Offset[((unsigned)&((struct LM*)0)->low==0)?1:-1];
typedef char LM_high_Offset[((unsigned)&((struct LM*)0)->high==4)?1:-1];

typedef char Float_Size[(sizeof(float)==4)?1:-1];
typedef char Int_Size[(sizeof(int)==4)?1:-1];
typedef char UInt_Size[(sizeof(unsigned)==4)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char U_Alignment[(__alignof__(U)==16)?1:-1];
typedef char M3_Alignment[(__alignof__(struct M3)==16)?1:-1];
typedef char M4_Alignment[(__alignof__(struct M4)==16)?1:-1];
typedef char WORK_Alignment[(__alignof__(struct WORK)==16)?1:-1];
typedef char U_Qword_Offset[((unsigned)&((U*)0)->q==0)?1:-1];
typedef char U_Float_Offset[((unsigned)&((U*)0)->f==0)?1:-1];
typedef char M3_Rows_Offset[((unsigned)&((struct M3*)0)->r==0)?1:-1];
typedef char M4_Rows_Offset[((unsigned)&((struct M4*)0)->r==0)?1:-1];
typedef char M3_Row_Count[(sizeof(((struct M3*)0)->r)/sizeof(U)==3)?1:-1];
typedef char M4_Row_Count[(sizeof(((struct M4*)0)->r)/sizeof(U)==4)?1:-1];
typedef char PHASE_Size[(sizeof(((struct WORK*)0)->phase)==64)?1:-1];
typedef char PHASE_Predicted_Offset[((unsigned)&((struct WORK*)0)->phase.predicted==0)?1:-1];
typedef char PHASE_Scalar_Offset[((unsigned)&((struct WORK*)0)->phase.scalar==0)?1:-1];
