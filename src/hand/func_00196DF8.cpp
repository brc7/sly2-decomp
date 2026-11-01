typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};struct M3{U row[3];};struct M4{U row[4];};
struct VTE{short delta,pad;void*fn;};struct BODY{char pad0[0xB0];U angularVelocity;char padC0[0x30];U angularAcceleration;};struct MOTION{char pad0[0x80];U acceleration;};struct ACTOR{char pad0[12];VTE*vt;char pad10[0x4C];BODY*body;char pad60[0x88];MOTION*motion;char padEC[0x190-0xEC];U velocity;};
struct CONTEXT{ACTOR*actor;int state;char pad8[0x10];float stamp;int mode;char pad20[0x14];float gain;char pad38[0x68];U desired;char padB0[0x110];float targetX,targetZ;};
struct CLOCK{float time,dt;};struct SMP{float maximum,minimum,duration;};struct RANGE{float low,high;};struct POLY{float c0,c1,c2;};struct PARAMS{float values[3];};
/* The basis writer and later vectors share their actual stack homes. */
union BASE_PHASE{M3 returned;struct{U local,acceleration,corrected,scalar;}vectors;};
union WORK{struct{U local,scalar;M3 basis,rotation,returned;U original;}turn;struct{U direction;M3 basis;BASE_PHASE phase;}move;U scalar;};
extern "C"{extern CLOCK D_002DDED0;extern U D_002A2DA0,D_002A2DC0,D_002A2DD0;extern SMP D_002BF7A8,D_002BF7B8,D_002BF7C8;extern float D_002BF7D4,D_002BF7F0,D_002BF7F4,D_002BF7F8,D_002BF810,D_002BF814;extern PARAMS D_002BF7E0;extern POLY D_002BF800;extern RANGE D_002BBB60;
qword ConvertAloVec(ACTOR*,ACTOR*,qword);M3*func_0011B570(M3*,qword);M3*LoadLookAtMatrix(M3*,qword);M4*func_001A6318(ACTOR*);float GSmooth__FfffP3SMPPf(float,float,float,SMP*,float*);qword LimitVectorLength(qword,float);void func_001FAC30(ACTOR*,qword);void func_001F5E10(ACTOR*,qword,PARAMS*,qword,PARAMS*);void func_00196AA8(CONTEXT*,U*);}
static inline qword Velocity(ACTOR*p){VTE&v=p->vt[33];return ((qword(*)(void*))v.fn)((char*)p+v.delta);}
static inline qword Scalar(float f){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));return q;}
static inline qword Scale(qword a,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(k));return r;}
static inline qword Rotate(qword r0,qword r1,qword r2,qword vector,qword seed){__asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"+j"(seed):"j"(r0),"j"(r1),"j"(r2),"j"(vector));return seed;}
static inline qword Rotate(M3*m,qword vector,qword seed){return Rotate(m->row[0].q,m->row[1].q,m->row[2].q,vector,seed);}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char M4_Size[sizeof(M4)==64?1:-1];
typedef char M3_row_Offset[FIELD_OFFSET(M3,row)==0?1:-1];
typedef char M4_row_Offset[FIELD_OFFSET(M4,row)==0?1:-1];
typedef char VTE_Size[sizeof(VTE)==8?1:-1];
typedef char BODY_Size[sizeof(BODY)==256?1:-1];
typedef char MOTION_Size[sizeof(MOTION)==144?1:-1];
typedef char ACTOR_Size[sizeof(ACTOR)==416?1:-1];
typedef char CONTEXT_Size[sizeof(CONTEXT)==464?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char SMP_Size[sizeof(SMP)==12?1:-1];
typedef char RANGE_Size[sizeof(RANGE)==8?1:-1];
typedef char POLY_Size[sizeof(POLY)==12?1:-1];
typedef char PARAMS_Size[sizeof(PARAMS)==12?1:-1];
typedef char BASE_PHASE_Size[sizeof(BASE_PHASE)==64?1:-1];
typedef char WORK_Size[sizeof(WORK)==192?1:-1];
typedef char BODY_angularVelocity_Offset[FIELD_OFFSET(BODY,angularVelocity)==176?1:-1];
typedef char BODY_angularAcceleration_Offset[FIELD_OFFSET(BODY,angularAcceleration)==240?1:-1];
typedef char MOTION_acceleration_Offset[FIELD_OFFSET(MOTION,acceleration)==128?1:-1];
typedef char ACTOR_vt_Offset[FIELD_OFFSET(ACTOR,vt)==12?1:-1];
typedef char ACTOR_body_Offset[FIELD_OFFSET(ACTOR,body)==92?1:-1];
typedef char ACTOR_motion_Offset[FIELD_OFFSET(ACTOR,motion)==232?1:-1];
typedef char ACTOR_velocity_Offset[FIELD_OFFSET(ACTOR,velocity)==400?1:-1];
typedef char CONTEXT_actor_Offset[FIELD_OFFSET(CONTEXT,actor)==0?1:-1];
typedef char CONTEXT_state_Offset[FIELD_OFFSET(CONTEXT,state)==4?1:-1];
typedef char CONTEXT_stamp_Offset[FIELD_OFFSET(CONTEXT,stamp)==24?1:-1];
typedef char CONTEXT_mode_Offset[FIELD_OFFSET(CONTEXT,mode)==28?1:-1];
typedef char CONTEXT_gain_Offset[FIELD_OFFSET(CONTEXT,gain)==52?1:-1];
typedef char CONTEXT_desired_Offset[FIELD_OFFSET(CONTEXT,desired)==160?1:-1];
typedef char CONTEXT_targetX_Offset[FIELD_OFFSET(CONTEXT,targetX)==448?1:-1];
typedef char CONTEXT_targetZ_Offset[FIELD_OFFSET(CONTEXT,targetZ)==452?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char SMP_maximum_Offset[FIELD_OFFSET(SMP,maximum)==0?1:-1];
typedef char SMP_minimum_Offset[FIELD_OFFSET(SMP,minimum)==4?1:-1];
typedef char SMP_duration_Offset[FIELD_OFFSET(SMP,duration)==8?1:-1];
typedef char RANGE_low_Offset[FIELD_OFFSET(RANGE,low)==0?1:-1];
typedef char RANGE_high_Offset[FIELD_OFFSET(RANGE,high)==4?1:-1];
typedef char POLY_c0_Offset[FIELD_OFFSET(POLY,c0)==0?1:-1];
typedef char POLY_c1_Offset[FIELD_OFFSET(POLY,c1)==4?1:-1];
typedef char POLY_c2_Offset[FIELD_OFFSET(POLY,c2)==8?1:-1];
typedef char PARAMS_values_Offset[FIELD_OFFSET(PARAMS,values)==0?1:-1];
typedef char BASE_PHASE_returned_Offset[FIELD_OFFSET(BASE_PHASE,returned)==0?1:-1];
typedef char BASE_PHASE_vectors_local_Offset[FIELD_OFFSET(BASE_PHASE,vectors.local)==0?1:-1];
typedef char BASE_PHASE_vectors_acceleration_Offset[FIELD_OFFSET(BASE_PHASE,vectors.acceleration)==16?1:-1];
typedef char BASE_PHASE_vectors_corrected_Offset[FIELD_OFFSET(BASE_PHASE,vectors.corrected)==32?1:-1];
typedef char BASE_PHASE_vectors_scalar_Offset[FIELD_OFFSET(BASE_PHASE,vectors.scalar)==48?1:-1];
typedef char WORK_turn_local_Offset[FIELD_OFFSET(WORK,turn.local)==0?1:-1];
typedef char WORK_turn_scalar_Offset[FIELD_OFFSET(WORK,turn.scalar)==16?1:-1];
typedef char WORK_turn_basis_Offset[FIELD_OFFSET(WORK,turn.basis)==32?1:-1];
typedef char WORK_turn_rotation_Offset[FIELD_OFFSET(WORK,turn.rotation)==80?1:-1];
typedef char WORK_turn_returned_Offset[FIELD_OFFSET(WORK,turn.returned)==128?1:-1];
typedef char WORK_turn_original_Offset[FIELD_OFFSET(WORK,turn.original)==176?1:-1];
typedef char WORK_move_direction_Offset[FIELD_OFFSET(WORK,move.direction)==0?1:-1];
typedef char WORK_move_basis_Offset[FIELD_OFFSET(WORK,move.basis)==16?1:-1];
typedef char WORK_move_phase_Offset[FIELD_OFFSET(WORK,move.phase)==64?1:-1];
typedef char WORK_scalar_Offset[FIELD_OFFSET(WORK,scalar)==0?1:-1];
extern "C" void func_00196DF8(CONTEXT*p){WORK w;
 switch(p->mode){
 case 20:{
  register float zero __asm__("$f20")=0.0f;
  register qword original __asm__("$2")=Velocity(p->actor);register ACTOR*empty __asm__("$4"),*actor __asm__("$5");
  __asm__("sq %3,%0\n\tdaddu %1,$zero,$zero\n\tlw %2,%4":"=m"(w.turn.original.q),"=r"(empty),"=r"(actor):"r"(original),"m"(p->actor));
  w.turn.local.q=ConvertAloVec(empty,actor,original);
  register unsigned clockWord __asm__("$16");register float updatedX __asm__("$f0"),updatedY __asm__("$f0");register M3*rotationOutput __asm__("$4");register qword rotation __asm__("vf2");
  __asm__("lui %0,%%hi(%6)\n\taddiu %0,%0,%%lo(%6)\n\tli.s $f1,0.5\n\tlw $3,%4\n\taddiu %1,%5,0x50\n\tlwc1 $f0,4(%0)\n\tlw $2,0x5C($3)\n\tmfc1 $3,$f0\n\tmul.s $f0,$f0,$f1\n\tqmtc2.ni $3,$vf5\n\tlqc2 $vf2,0xB0($2)\n\tsqc2 $vf5,%3\n\tmfc1 $3,$f0\n\tqmtc2.ni $3,$vf4\n\tlqc2 $vf3,0xF0($2)\n\tsqc2 $vf4,%3\n\tvadda.xyzw $ACC,$vf2,$vf0\n\tvmaddx.xyzw $vf2,$vf3,$vf5x\n\tlqc2 $vf1,0xB0($2)\n\tvadd.xyzw $vf1,$vf1,$vf2\n\tvmulx.xyzw %2,$vf1,$vf4x"
   :"=&r"(clockWord),"=r"(rotationOutput),"=j"(rotation),"=m"(w.turn.scalar.q):"m"(p->actor),"p"(&w),"i"(&D_002DDED0):"$2","$3","$f0","$f1","vf1","vf3","vf4","vf5","at","memory");
  (void)func_0011B570(rotationOutput,rotation);
  {
   register M4*matrix __asm__("$2")=func_001A6318(p->actor);
   register qword first __asm__("vf3"),second __asm__("vf2"),r0 __asm__("vf4"),r1 __asm__("vf6"),r2 __asm__("vf5");
   register SMP*parameters __asm__("$4");register float*empty __asm__("$5");
   register float current __asm__("$f12"),target __asm__("$f13"),step __asm__("$f14");
   __asm__("lqc2 %0,0(%6)\n\tlui %4,%%hi(%8)\n\tlqc2 %3,0x70(%7)\n\taddiu %4,%4,%%lo(%8)\n\tlqc2 %1,0x50(%7)\n\tdaddu %5,$zero,$zero\n\tlqc2 %2,0x60(%7)"
    :"=j"(first),"=j"(r0),"=j"(r1),"=j"(r2),"=&r"(parameters),"=r"(empty):"r"(matrix),"p"(&w),"i"(&D_002BF7A8):"memory");
   __asm__("vmulax.xyz $ACC,%3,%0x\n\tvmadday.xyz $ACC,%4,%0y\n\tvmaddz.xyz %0,%5,%0z\n\tlwc1 %1,%6\n\tsqc2 %0,%2"
    :"+j"(first),"=f"(current),"=m"(w.turn.basis.row[0].q):"j"(r0),"j"(r1),"j"(r2),"m"(w.turn.local.f[0]));
   __asm__("lqc2 %0,%4\n\tvmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"=&j"(second):"j"(r0),"j"(r1),"j"(r2),"m"(matrix->row[1].q));
   __asm__("lwc1 %0,%4\n\tlwc1 %1,%5\n\tsqc2 %6,%2\n\tsqc2 %7,%3"
    :"=f"(step),"=f"(target),"=m"(w.turn.basis.row[1].q),"=m"(w.turn.returned.row[0].q):"m"(((CLOCK*)clockWord)->dt),"m"(p->targetX),"j"(second),"j"(first));
   /* XYZ changes; each matrix row keeps its actual incoming W carrier. */
   __asm__("lqc2 $vf1,%5\n\tvmulax.xyz $ACC,%0,$vf1x\n\tvmadday.xyz $ACC,%3,$vf1y\n\tvmaddz.xyz %0,%4,$vf1z\n\tsqc2 %6,%1\n\tsqc2 %0,%2"
    :"+j"(r0),"=m"(w.turn.returned.row[1].q),"=m"(w.turn.basis.row[2].q):"j"(r1),"j"(r2),"m"(matrix->row[2].q),"j"(second):"vf1");
   w.turn.returned.row[2].q=r0;
   updatedX=GSmooth__FfffP3SMPPf(current,target,step,parameters,empty);
  }
  {
   register SMP*parameters __asm__("$4");register float current __asm__("$f12"),step __asm__("$f14");
   __asm__("lwc1 %0,%4\n\tlui %2,%%hi(%6)\n\tlwc1 %1,%5\n\taddiu %2,%2,%%lo(%6)\n\tswc1 %7,%3"
    :"=f"(current),"=f"(step),"=&r"(parameters),"=m"(w.turn.local.f[0]):"m"(w.turn.local.f[1]),"m"(((CLOCK*)clockWord)->dt),"i"(&D_002BF7B8),"f"(updatedX));
   updatedY=GSmooth__FfffP3SMPPf(current,zero,step,parameters,0);
  }
  {
   register SMP*parameters __asm__("$4");register float current __asm__("$f12"),step __asm__("$f14"),target __asm__("$f13");
   __asm__("lwc1 %1,%4\n\tlui %2,%%hi(%7)\n\tlwc1 %0,%5\n\taddiu %2,%2,%%lo(%7)\n\tlwc1 %3,%6"
    :"=f"(current),"=f"(step),"=&r"(parameters),"=f"(target):"m"(((CLOCK*)clockWord)->dt),"m"(w.turn.local.f[2]),"m"(p->targetZ),"i"(&D_002BF7C8));
   w.turn.local.f[1]=updatedY;
   w.turn.local.f[2]=GSmooth__FfffP3SMPPf(current,target,step,parameters,0);
  }
  register qword desired __asm__("vf2");register float maximum __asm__("$f12");
  __asm__("lui $2,%%hi(%5)\n\tlqc2 $vf2,0x40(%4)\n\tlqc2 $vf5,0x20(%4)\n\tlqc2 $vf3,0x30(%4)\n\tlqc2 $vf1,0(%4)\n\tvmulax.xyz $ACC,$vf5,$vf1x\n\tvmadday.xyz $ACC,$vf3,$vf1y\n\tvmaddz.xyz $vf1,$vf2,$vf1z\n\tlw %0,4(%0)\n\tlqc2 $vf2,0xB0(%4)\n\tqmtc2.ni %0,$vf4\n\tvsub.xyzw $vf1,$vf1,$vf2\n\tlwc1 %2,%%lo(%5)($2)\n\tvdiv Q,$vf0w,$vf4x\n\tvwaitq\n\tvmulq.xyzw %1,$vf1,Q\n\tsqc2 $vf4,%3"
   :"+r"(clockWord),"=j"(desired),"=f"(maximum),"=m"(w.turn.scalar.q):"p"(&w),"i"(&D_002BF7D4):"$2","vf1","vf3","vf4","vf5","memory");
  register qword acceleration __asm__("$2")=LimitVectorLength(desired,maximum);
  register float z __asm__("$f0");
  __asm__("pextuw $3,$zero,%1\n\tmtc1 $3,%0":"=f"(z):"r"(acceleration):"$3");
  if(z<zero){
   __asm__("prot3w $3,%0\n\tmtc1 %0,$f0\n\tmtc1 $3,$f1\n\tmfc1 %0,$f0\n\tmfc1 $3,$f1\n\tpextlw %0,$3,%0\n\tmfc1 $3,%2\n\tpcpyld %0,$3,%0\n\tsq %0,%1"
    :"+r"(acceleration),"=m"(w.turn.scalar.q):"f"(zero):"$3","$f0","$f1");
  }
  func_001FAC30(p->actor,acceleration);break;
 }
 case 22:{
  register PARAMS*parameters __asm__("$8"),*empty __asm__("$6");register ACTOR*actor __asm__("$4");
  register qword force __asm__("vf4"),origin __asm__("$5");
  __asm__("lui %0,%%hi(%7)\n\tli.s $f0,-1.0\n\taddiu %0,%0,%%lo(%7)\n\tlw %1,%6\n\tlwc1 $f1,4(%0)\n\tdaddu %2,$zero,$zero\n\tmfc1 $2,$f0\n\tdiv.s $f0,$f0,$f1\n\tlqc2 $vf1,0x190(%1)\n\tqmtc2.ni $2,$vf2\n\tsqc2 $vf2,%5\n\tlui $2,%%hi(%8)\n\tvmulx.xyzw $vf2,$vf1,$vf2x\n\taddiu $2,$2,%%lo(%8)\n\tmfc1 $3,$f0\n\tqmtc2.ni $3,$vf3\n\tlw $3,0xE8(%1)\n\tsqc2 $vf3,%5\n\tlqc2 $vf1,0x80($3)\n\tvadda.xyzw $ACC,$vf2,$vf0\n\tvmaddx.xyzw %3,$vf1,$vf3x\n\tlq %4,0($2)"
   :"=&r"(parameters),"=&r"(actor),"=r"(empty),"=j"(force),"=r"(origin),"=m"(w.scalar.q):"m"(p->actor),"i"(&D_002BF7E0),"i"(&D_002A2DD0):"$2","$3","$f0","$f1","vf1","vf2","vf3","at","memory");
  func_001F5E10(actor,origin,empty,force,parameters);break;
 }
 case 6:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 21:break;
 default:{
  register qword word __asm__("$2")=Velocity(p->actor);
  register U*fallback __asm__("$3");register float epsilon __asm__("$f1"),length __asm__("$f0");
  register qword direction __asm__("vf3"),root __asm__("vf2");
  __asm__("sq %0,%1\n\tlui %0,%%hi(%8)\n\tsw $zero,8(%7)\n\taddiu %2,%0,%%lo(%8)\n\tli.s %3,0.0001\n\tlq %0,%1\n\tqmtc2.ni %0,%4\n\tvmul.xyz $vf1,%4,%4\n\tvaddw.x %5,$vf0,$vf0w\n\tvadday.x $ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,%5,$vf1z\n\tvsqrt Q,$vf1x\n\tvwaitq\n\tvaddq.x %5,$vf0,Q\n\tqmfc2.ni %0,%5\n\tmtc1 %0,%6"
   :"+r"(word),"=m"(w.move.direction.q),"=&r"(fallback),"=f"(epsilon),"=j"(direction),"=j"(root),"=f"(length):"p"(&w.move.direction),"i"(&D_002A2DA0):"vf1","at");
  if(length<epsilon)w.move.direction.q=fallback->q;
  else{
   register qword normalized __asm__("vf1");
   __asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(normalized):"j"(root),"j"(direction));
   w.move.direction.q=normalized;
  }
  register float speed __asm__("$f20");register qword axis __asm__("$5");
  __asm__("qmfc2.ni $3,%2\n\tlq %1,%3\n\tmtc1 $3,%0":"=f"(speed),"=r"(axis):"j"(root),"m"(w.move.direction.q):"$3");
  (void)LoadLookAtMatrix(&w.move.phase.returned,axis);
  register unsigned clockHigh __asm__("$4");register float step __asm__("$f1"),inverseTime __asm__("$f0");
  register qword row0 __asm__("vf2"),row1 __asm__("vf3"),row2 __asm__("vf4"),local __asm__("vf1");
  __asm__("lui $2,%%hi(%9)\n\tlqc2 %3,0x40(%7)\n\tdaddu %0,$2,$zero\n\tlqc2 %4,0x50(%7)\n\tlqc2 %5,0x60(%7)\n\taddiu $2,$2,%%lo(%9)\n\tlwc1 %1,4($2)\n\tlqc2 %6,0xA0(%8)\n\tli.s %2,1.0"
   :"=&r"(clockHigh),"=f"(step),"=f"(inverseTime),"=j"(row0),"=j"(row1),"=j"(row2),"=j"(local):"p"(&w),"r"(p),"i"(&D_002DDED0):"$2","at","memory");
  __asm__("vmul.xyzw $vf7,%1,%0\n\tvmaxw.xyzw $vf8,$vf0,$vf0w\n\tvmul.xyzw $vf6,%2,%0\n\tvmul.xyzw $vf5,%3,%0\n\tvadday.x $ACC,$vf7,$vf7y\n\tvmaddz.x %0,$vf8,$vf7z\n\tvaddax.y $ACC,$vf6,$vf6x\n\tvmaddz.y %0,$vf8,$vf6z\n\tvaddax.z $ACC,$vf5,$vf5x\n\tvmaddy.z %0,$vf8,$vf5y"
   :"+j"(local):"j"(row0),"j"(row1),"j"(row2):"vf5","vf6","vf7","vf8");
  register int mode __asm__("$3");
  __asm__("lw %0,%7\n\tsqc2 %4,%1\n\tdiv.s %2,%2,%6\n\tsqc2 %5,%3"
   :"=r"(mode),"=m"(w.move.phase.vectors.local.q),"+f"(inverseTime),"=m"(w.move.basis.row[0].q):"j"(local),"j"(row0),"f"(step),"m"(p->mode));
  w.move.basis.row[1].q=row1;w.move.basis.row[2].q=row2;
  register float side __asm__("$f2"),longitudinal __asm__("$f3");
  register float localX __asm__("$f0");
  if(mode==0||mode==2){side=inverseTime;localX=w.move.phase.vectors.local.f[0];longitudinal=side;}
  else{localX=w.move.phase.vectors.local.f[0];longitudinal=localX<speed?D_002BF7F4:D_002BF7F0;side=D_002BF7F8;}
  register int state __asm__("$3"),twelve __asm__("$2");register float accelerationX __asm__("$f1");
  register float localY __asm__("$f0");
  __asm__("sub.s %1,%5,%6\n\tlw %0,%8\n\tlwc1 %4,%7\n\tli %3,12\n\tsw $zero,%2"
   :"=r"(state),"=f"(accelerationX),"=m"(w.move.phase.vectors.acceleration.i[2]),"=r"(twelve),"=f"(localY):"f"(localX),"f"(speed),"m"(w.move.phase.vectors.local.f[1]),"m"(p->state));
  __asm__("mul.s %1,%3,%1\n\tmul.s %0,%4,%0\n\tswc1 %1,%2"
   :"+f"(accelerationX),"+f"(localY),"=m"(w.move.phase.vectors.acceleration.f[1]):"f"(side),"f"(longitudinal));
  w.move.phase.vectors.acceleration.f[0]=accelerationX;
  register float gain __asm__("$f3");
  if(state==twelve){
   register float elapsed __asm__("$f1"),stamp __asm__("$f3"),low __asm__("$f4"),c1 __asm__("$f0"),c2 __asm__("$f2");
   register unsigned rangeHigh __asm__("$3"),address __asm__("$2");
   __asm__("lwc1 %1,%%lo(%5)(%0)\n\tlui %3,%%hi(%6)\n\tlwc1 %2,%4\n\tlui %0,%%hi(%7)"
    :"+r"(clockHigh),"=f"(elapsed),"=f"(stamp),"=r"(rangeHigh):"m"(p->stamp),"i"(&D_002DDED0),"i"(&D_002BBB60),"i"(&D_002BF800):"memory");
   __asm__("addiu %0,%3,%%lo(%6)\n\tlwc1 %1,%%lo(%7)(%4)\n\tsub.s %2,%2,%5"
    :"=&r"(address),"=&f"(low),"+f"(elapsed):"r"(clockHigh),"r"(rangeHigh),"f"(stamp),"i"(&D_002BF800),"i"(&D_002BBB60):"memory");
   __asm__("lwc1 %1,8(%0)\n\tlwc1 %2,4(%0)\n\tlwc1 %3,%%lo(%6)(%4)\n\taddiu %0,%5,%%lo(%7)"
    :"+r"(address),"=f"(c2),"=f"(c1),"=f"(gain):"r"(clockHigh),"r"(rangeHigh),"i"(&D_002BF800),"i"(&D_002BBB60):"memory");
   c2=elapsed*c2;c1=c1+c2;elapsed=elapsed*c1;gain=gain+elapsed;
   register float high __asm__("$f5")=((RANGE*)address)->high;
   if(gain<low)gain=low;else if(high<gain)gain=high;
  }else gain=p->gain;
  w.move.phase.vectors.scalar.q=Scalar(gain);w.move.phase.vectors.acceleration.q=Scale(w.move.phase.vectors.acceleration.q,w.move.phase.vectors.scalar.q);
  w.move.phase.vectors.corrected.q=Rotate(&w.move.basis,w.move.phase.vectors.acceleration.q,w.move.basis.row[0].q);
  func_00196AA8(p,&w.move.phase.vectors.corrected);
  if(p->mode!=5){
   w.move.direction.q=Velocity(p->actor);
   register float threshold __asm__("$f0"),z __asm__("$f1");
   __asm__("lui $2,%%hi(%2)\n\tlwc1 %0,%%lo(%2)($2)\n\tlwc1 %1,%3":"=f"(threshold),"=f"(z):"i"(&D_002BF810),"m"(w.move.direction.f[2]):"$2","memory");
   if(z<threshold){
    z=z-threshold;threshold=D_002BF814;
    __asm__("lui $3,%%hi(%4)\n\taddiu $3,$3,%%lo(%4)\n\tlqc2 $vf2,%0\n\tmul.s %2,%2,%3\n\tlqc2 $vf3,0($3)\n\tmfc1 $3,%2\n\tqmtc2.ni $3,$vf1\n\tvadda.xyzw $ACC,$vf2,$vf0\n\tvmaddx.xyzw $vf2,$vf3,$vf1x\n\tsqc2 $vf1,%1\n\tsqc2 $vf2,%0"
     :"+m"(w.move.phase.vectors.corrected.q),"=m"(w.move.phase.vectors.scalar.q),"+f"(threshold):"f"(z),"i"(&D_002A2DC0):"$3","vf1","vf2","vf3","memory");
   }
  }
  func_001FAC30(p->actor,w.move.phase.vectors.corrected.q);break;
 }
 }
}
