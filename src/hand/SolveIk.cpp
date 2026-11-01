typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};
struct M3{
 U r[3];M3(){}M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];}
 M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return*this;}
};
struct M4{U r[4];};
struct FRAME{M4 local,world;char pad80[0x20];FRAME*parent;int stamp;};
struct VTE{short delta,pad;void*fn;};struct CONTROL{VTE*vt;};
struct REQUEST{
 FRAME*primary,*current,*output,*ancestor;M4 snapshot;M3 rest;
 qword translation,axis,localPoint,mappedPoint;float firstLength,secondLength,referenceAngle,twist;
 CONTROL*control;float*parameters;
};
/* Every home is written by the native return, matrix-copy or vector phase. */
struct WORK{U row[23];};
inline void*operator new(unsigned int,void*p){return p;}
struct GAME{char pad[0x3D58];int stamp;};extern "C" GAME D_003DD490;
extern "C" float func_002456F0(float);
extern "C" M3 func_0011B500(qword,float);
extern "C" qword*func_00256D18(qword*,qword);
extern "C" float RadBetweenVectors(qword,qword,qword*);
extern "C" qword ConvertXfVec(void*,void*,qword);
static inline float CopySnapshot(REQUEST*q){float high;register FRAME*d __asm__("$4")=q->current;register qword odd __asm__("$3"),even __asm__("$2");__asm__("li.s %2,1.0\n\tlw %0,%3\n\tlq %1,%4":"=&r"(d),"=r"(odd),"=f"(high):"m"(q->current),"m"(q->snapshot.r[0].q):"at");d->local.r[0].q=odd;even=q->snapshot.r[1].q;d->local.r[1].q=even;odd=q->snapshot.r[2].q;d->local.r[2].q=odd;even=q->snapshot.r[3].q;d->local.r[3].q=even;return high;}

extern "C" void SolveIk(void*context,REQUEST*request){
 register REQUEST*q __asm__("$17")=request;
 WORK w;
 register float low __asm__("$f5")=-1.0f,high __asm__("$f6"),cosine __asm__("$f12");
 register M3*originalBasis __asm__("$20");
 high=CopySnapshot(q);
 /* Capture the native inherited VF4.W; no C input value exists. */
 register qword inheritedBase __asm__("vf4");
 __asm__ __volatile__("lw $3,4(%4)\n\tlw $4,0(%4)\n\tlw $5,0xA0($3)\n\tlqc2 $vf3,0x40(%4)\n\taddiu $6,$4,0x40\n\taddiu $2,$5,0x40\n\tlqc2 $vf7,0x40($5)\n\tlqc2 $vf2,0x10($2)\n\tlqc2 $vf1,0x20($2)\n\tlqc2 $vf6,0x30($2)\n\tvmulax.xyz $ACC,$vf7,$vf3x\n\tvmadday.xyz $ACC,$vf2,$vf3y\n\tvmaddaz.xyz $ACC,$vf1,$vf3z\n\tvmaddw.xyz %1,$vf6,$vf0w\n\tlqc2 $vf5,0x40($4)\n\tlqc2 $vf8,0x30($6)\n\tsqc2 %1,%2\n\tlqc2 %1,0xA0(%4)\n\tlqc2 $vf2,0x10($6)\n\tlqc2 $vf1,0x20($6)\n\tvmulax.xyz $ACC,$vf5,%1x\n\tvmadday.xyz $ACC,$vf2,%1y\n\tvmaddaz.xyz $ACC,$vf1,%1z\n\tvmaddw.xyz $vf5,$vf8,$vf0w\n\tlwc1 $f1,0xC0(%4)\n\tlqc2 $vf1,%2\n\tvsub.xyzw $vf1,$vf5,$vf1\n\tmul.s $f4,$f1,$f1\n\tsqc2 $vf1,%3\n\tadd.s $f1,$f1,$f1\n\tlqc2 $vf3,%3\n\tvmul.xyz $vf1,$vf1,$vf3\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x $ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tlwc1 $f2,0xC4(%4)\n\tvsqrt Q,$vf1x\n\tvwaitq\n\tvaddq.x $vf1,$vf0,Q\n\tqmfc2.ni $2,$vf1\n\tmul.s $f3,$f2,$f2\n\tmtc1 $2,$f0\n\tmul.s $f1,$f1,$f2\n\tmul.s $f0,$f0,$f0\n\tsub.s $f0,$f0,$f4\n\tsub.s $f0,$f0,$f3\n\tdiv.s %0,$f0,$f1"
  :"=f"(cosine),"=&j"(inheritedBase),"=m"(w.row[21].q),"=m"(w.row[22].q):"r"(q):"$2","$3","$4","$5","$6","$f0","$f1","$f2","$f3","$f4","vf1","vf2","vf3","vf5","vf6","vf7","vf8","memory");
 if(cosine<low)cosine=low;else if(high<cosine)cosine=high;
 originalBasis=(M3*)&q->snapshot;
 float initialAngle=func_002456F0(cosine);
 (void)new(&w.row[0])M3(func_0011B500(q->axis,initialAngle-q->referenceAngle));
 register FRAME*output __asm__("$16");register M3*rest __asm__("$2");
 register qword r0 __asm__("vf3"),r1 __asm__("vf5"),r2 __asm__("vf4"),first __asm__("vf2");
 __asm__("lqc2 %0,0x50(%6)\n\taddiu %1,%6,0x50\n\tlqc2 %3,0x10(%7)\n\tlqc2 %2,0(%7)\n\tlqc2 %4,0x20(%7)\n\tlw %5,8(%6)\n\tvmulax.xyz $ACC,%2,%0x\n\tvmadday.xyz $ACC,%3,%0y\n\tvmaddz.xyz %0,%4,%0z\n\tsqc2 %0,0x30(%7)"
  :"=j"(first),"=&r"(rest),"=j"(r0),"=j"(r1),"=j"(r2),"=&r"(output):"r"(q),"p"(&w):"memory");
 register qword*translationOutput __asm__("$4");register qword translation __asm__("$5");
 __asm__("addiu %0,%9,0x30\n\tlqc2 $vf1,0x10(%6)\n\tlq %1,0x80(%7)\n\tvmulax.xyz $ACC,%3,$vf1x\n\tvmadday.xyz $ACC,%4,$vf1y\n\tvmaddz.xyz $vf1,%5,$vf1z\n\tsqc2 $vf1,0x40(%8)\n\tsqc2 $vf1,0x70(%8)\n\tsqc2 %2,0x60(%8)"
  :"=&r"(translationOutput),"=&r"(translation):"j"(first),"j"(r0),"j"(r1),"j"(r2),"r"(rest),"r"(q),"p"(&w),"r"(output):"vf1","memory");
 __asm__("lqc2 $vf2,%4\n\tvmulax.xyz $ACC,%0,$vf2x\n\tvmadday.xyz $ACC,%2,$vf2y\n\tvmaddz.xyz %0,%3,$vf2z\n\tsqc2 %0,%1"
  :"+j"(r0),"=m"(w.row[5].q):"j"(r1),"j"(r2),"m"(rest->r[2].q):"vf2");
 w.row[8].q=r0;
 (void)func_00256D18(translationOutput,translation);
 register U*axis __asm__("$6");register M3*alignedReturn __asm__("$18"),*twistedReturn __asm__("$19");
 register qword firstRow __asm__("$3"),secondRow __asm__("$2");register float one __asm__("$f0");
 __asm__("lq %0,0x30(%4)\n\taddiu %1,%4,0x30\n\tli.s %2,1.0\n\taddiu %3,%4,0x90"
  :"=&r"(firstRow),"=&r"(axis),"=f"(one),"=r"(alignedReturn):"p"(&w):"at","memory");
 __asm__("sq %3,%2\n\taddiu %0,%4,0xC0\n\tlq %1,0x40(%4)"
  :"=&r"(twistedReturn),"=r"(secondRow),"=m"(output->local.r[0].q):"r"(firstRow),"p"(&w):"memory");
 output->local.r[1].q=secondRow;register qword thirdRow __asm__("$3")=w.row[5].q;output->local.r[3].f[3]=one;output->local.r[2].q=thirdRow;
 __asm__("sw $zero,%0\n\tsw $zero,%1\n\tsw $zero,%2":"=m"(output->local.r[2].i[3]),"=m"(output->local.r[1].i[3]),"=m"(output->local.r[0].i[3]));
 register FRAME*end __asm__("$2"),*cursor __asm__("$3");
 __asm__("lw %0,4(%2)\n\tlw %1,0xC(%2)\n\tlw %0,0xA0(%0)":"=&r"(end),"=r"(cursor):"r"(q):"memory");
 register qword transported __asm__("vf5")=q->mappedPoint;
 while(cursor!=end){
  register qword row0 __asm__("vf4")=cursor->local.r[0].q;
  __asm__("lqc2 $vf3,0x10(%0)\n\tlqc2 $vf2,0x20(%0)\n\tlqc2 $vf1,0x30(%0)\n\tlw %0,0xA0(%0)\n\tvmulax.xyz $ACC,%2,%1x\n\tvmadday.xyz $ACC,$vf3,%1y\n\tvmaddaz.xyz $ACC,$vf2,%1z\n\tvmaddw.xyz %1,$vf1,$vf0w"
   :"+r"(cursor),"+j"(transported):"j"(row0):"vf1","vf2","vf3","memory");
 }
 register qword worldDelta __asm__("vf1"),deltaArgument __asm__("$5");
 __asm__("lw $4,%3\n\tlq %1,%4\n\tlw $3,0xA0($4)\n\taddiu $2,$3,0x40\n\tlqc2 $vf4,0x40($3)\n\tlqc2 $vf3,0x30($2)\n\tlqc2 $vf2,0x10($2)\n\tlqc2 $vf1,0x20($2)\n\tvmulax.xyz $ACC,$vf4,%2x\n\tvmadday.xyz $ACC,$vf2,%2y\n\tvmaddaz.xyz $ACC,$vf1,%2z\n\tvmaddw.xyz %2,$vf3,$vf0w\n\tlqc2 %0,%5\n\tvsub.xyzw %0,%2,%0"
  :"=&j"(worldDelta),"=&r"(deltaArgument),"+j"(transported):"m"(q->current),"m"(w.row[22].q),"m"(w.row[21].q):"$2","$3","$4","vf2","vf3","vf4","memory");
 register float angle __asm__("$f20")=RadBetweenVectors(worldDelta,deltaArgument,&axis->q);
 register qword converted __asm__("$2")=ConvertXfVec(0,q->current->parent,w.row[3].q);
 register M3*destination __asm__("$4");register qword normalized __asm__("vf3");register float keptAngle __asm__("$f12");
 __asm__("qmtc2.ni %4,$vf1\n\tdaddu %0,%5,$zero\n\tvmul.xyz $vf2,$vf1,$vf1\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x $ACC,$vf2,$vf2y\n\tvmaddz.x $vf2,$vf3,$vf2z\n\tmov.s %2,%6\n\tvsqrt Q,$vf2x\n\tvwaitq\n\tvaddq.x $vf2,$vf0,Q\n\tsqc2 $vf1,%3\n\tvdiv Q,$vf0w,$vf2x\n\tvwaitq\n\tvmulq.xyzw %1,$vf1,Q"
  :"=r"(destination),"=j"(normalized),"=f"(keptAngle),"=m"(w.row[3].q):"r"(converted),"r"(alignedReturn),"f"(angle):"vf1","vf2");
 (void)new(destination)M3(func_0011B500(normalized,keptAngle));
 register qword aligned0 __asm__("$2"),aligned1 __asm__("$3"),aligned2 __asm__("$4");register CONTROL*control __asm__("$8");
 __asm__("lq %0,0x90(%4)\n\tlq %1,0xA0(%4)\n\tlq %2,0xB0(%4)\n\tlw %3,%5"
  :"=&r"(aligned0),"=&r"(aligned1),"=&r"(aligned2),"=r"(control):"p"(&w),"m"(q->control):"memory");
 w.row[4].q=aligned0;w.row[5].q=aligned1;w.row[6].q=aligned2;
 if(control){
  typedef void(*Evaluate)(void*,float*,int,float,float);
  register float*twistOutput __asm__("$5");register int zero __asm__("$6"),delta __asm__("$4");
  register Evaluate evaluate __asm__("$7");register float parameter __asm__("$f12"),zeroFloat __asm__("$f13");
  __asm__("lw $2,0(%6)\n\taddiu %0,%7,0xCC\n\tlw $3,0xD4(%7)\n\tdaddu %1,$zero,$zero\n\tlh %2,0x10($2)\n\tlw %3,0x14($2)\n\tlwc1 %4,0($3)\n\tmtc1 $zero,%5"
   :"=&r"(twistOutput),"=r"(zero),"=r"(delta),"=r"(evaluate),"=f"(parameter),"=f"(zeroFloat):"r"(control),"r"(q):"$2","$3","memory");
  evaluate((char*)control+delta,twistOutput,zero,parameter,zeroFloat);
 }
 register qword convertedTurn __asm__("$2")=ConvertXfVec(0,q->current->parent,w.row[22].q);
 register M3*turnDestination __asm__("$4");register qword normalizedTurn __asm__("vf4");register float twist __asm__("$f12");
 __asm__("qmtc2.ni %3,$vf2\n\tlwc1 %2,%5\n\tvmul.xyz $vf1,$vf2,$vf2\n\tvaddw.x $vf3,$vf0,$vf0w\n\tvadday.x $ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf3,$vf1z\n\tdaddu %0,%4,$zero\n\tvsqrt Q,$vf1x\n\tvwaitq\n\tvaddq.x $vf1,$vf0,Q\n\tvdiv Q,$vf0w,$vf1x\n\tvwaitq\n\tvmulq.xyzw %1,$vf2,Q"
  :"=r"(turnDestination),"=j"(normalizedTurn),"=f"(twist):"r"(convertedTurn),"r"(twistedReturn),"m"(q->twist):"vf1","vf2","vf3");
 (void)new(turnDestination)M3(func_0011B500(normalizedTurn,twist));
 register FRAME*current __asm__("$4");register GAME*game __asm__("$5");
 __asm__("lqc2 $vf9,0xC0(%2)\n\tlui %1,%%hi(%5)\n\tlqc2 $vf6,0xD0(%2)\n\taddiu %1,%1,%%lo(%5)\n\tlqc2 $vf7,0xE0(%2)\n\tlqc2 $vf2,0x40(%2)\n\tlqc2 $vf3,0x50(%2)\n\tvmulax.xyz $ACC,$vf9,$vf2x\n\tvmadday.xyz $ACC,$vf6,$vf2y\n\tvmaddz.xyz $vf2,$vf7,$vf2z\n\tlqc2 $vf1,0x60(%2)\n\tvmulax.xyz $ACC,$vf9,$vf3x\n\tvmadday.xyz $ACC,$vf6,$vf3y\n\tvmaddz.xyz $vf3,$vf7,$vf3z\n\tlqc2 $vf8,0x20(%4)\n\tvmulax.xyz $ACC,$vf9,$vf1x\n\tvmadday.xyz $ACC,$vf6,$vf1y\n\tvmaddz.xyz $vf1,$vf7,$vf1z\n\tlqc2 $vf5,0x10(%3)\n\tvmulax.xyz $ACC,$vf2,$vf8x\n\tvmadday.xyz $ACC,$vf3,$vf8y\n\tvmaddz.xyz $vf8,$vf1,$vf8z\n\tlqc2 $vf4,0x10(%4)\n\tvmulax.xyz $ACC,$vf2,$vf5x\n\tvmadday.xyz $ACC,$vf3,$vf5y\n\tvmaddz.xyz $vf5,$vf1,$vf5z\n\tsqc2 $vf2,0x120(%2)\n\tvmulax.xyz $ACC,$vf2,$vf4x\n\tvmadday.xyz $ACC,$vf3,$vf4y\n\tvmaddz.xyz $vf4,$vf1,$vf4z\n\tsqc2 $vf3,0x130(%2)\n\tsqc2 $vf1,0x140(%2)\n\tlw %0,4(%3)\n\tsqc2 $vf4,0xB0(%2)\n\tsqc2 $vf8,0xC0(%2)\n\tsqc2 $vf9,0x70(%2)\n\tsqc2 $vf6,0x80(%2)\n\tsqc2 $vf7,0x90(%2)\n\tsqc2 $vf2,0xF0(%2)\n\tsqc2 $vf3,0x100(%2)\n\tsqc2 $vf1,0x110(%2)\n\tsqc2 $vf5,0x120(%2)\n\tsqc2 $vf5,0xA0(%2)\n\tsqc2 $vf4,0x130(%2)\n\tsqc2 $vf8,0x140(%2)\n\tsqc2 $vf5,0(%0)"
  :"=&r"(current),"=&r"(game):"p"(&w),"r"(q),"r"(originalBasis),"i"(&D_003DD490):"vf1","vf2","vf3","vf4","vf5","vf6","vf7","vf8","vf9","memory");
 /* Publish the actual affine rows and shared generation stamp. */
 register int stamp __asm__("$2");
 __asm__ __volatile__("lq %1,0xB0(%4)\n\tsq %1,0x10(%0)\n\tlq $3,0xC0(%4)\n\tsw $zero,0xC(%0)\n\tsq $3,0x20(%0)\n\tsw $zero,0x2C(%0)\n\tsw $zero,0x1C(%0)\n\tlw %1,0x3D58(%2)\n\taddiu %1,%1,1\n\tsw %1,0x3D58(%2)\n\tlw $3,4(%3)\n\tlw %0,8(%3)\n\tsw %1,0xA4($3)"
  :"+r"(current),"=&r"(stamp):"r"(game),"r"(q),"p"(&w):"$3","memory");
 current->stamp=stamp;
}
typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M3_Size[(sizeof(M3)==0x30)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char VTE_Size[(sizeof(VTE)==8)?1:-1];
typedef char REQUEST_Size[(sizeof(REQUEST)==0xE0)?1:-1];
typedef char FRAME_Size[(sizeof(FRAME)==0xB0)?1:-1];
typedef char FRAME_local_Offset[((unsigned)&((FRAME*)0)->local==0)?1:-1];
typedef char FRAME_world_Offset[((unsigned)&((FRAME*)0)->world==64)?1:-1];
typedef char FRAME_parent_Offset[((unsigned)&((FRAME*)0)->parent==160)?1:-1];
typedef char FRAME_stamp_Offset[((unsigned)&((FRAME*)0)->stamp==164)?1:-1];
typedef char VTE_delta_Offset[((unsigned)&((VTE*)0)->delta==0)?1:-1];
typedef char VTE_fn_Offset[((unsigned)&((VTE*)0)->fn==4)?1:-1];
typedef char CONTROL_vt_Offset[((unsigned)&((CONTROL*)0)->vt==0)?1:-1];
typedef char REQUEST_primary_Offset[((unsigned)&((REQUEST*)0)->primary==0)?1:-1];
typedef char REQUEST_current_Offset[((unsigned)&((REQUEST*)0)->current==4)?1:-1];
typedef char REQUEST_output_Offset[((unsigned)&((REQUEST*)0)->output==8)?1:-1];
typedef char REQUEST_ancestor_Offset[((unsigned)&((REQUEST*)0)->ancestor==12)?1:-1];
typedef char REQUEST_snapshot_Offset[((unsigned)&((REQUEST*)0)->snapshot==16)?1:-1];
typedef char REQUEST_rest_Offset[((unsigned)&((REQUEST*)0)->rest==80)?1:-1];
typedef char REQUEST_translation_Offset[((unsigned)&((REQUEST*)0)->translation==128)?1:-1];
typedef char REQUEST_axis_Offset[((unsigned)&((REQUEST*)0)->axis==144)?1:-1];
typedef char REQUEST_localPoint_Offset[((unsigned)&((REQUEST*)0)->localPoint==160)?1:-1];
typedef char REQUEST_mappedPoint_Offset[((unsigned)&((REQUEST*)0)->mappedPoint==176)?1:-1];
typedef char REQUEST_firstLength_Offset[((unsigned)&((REQUEST*)0)->firstLength==192)?1:-1];
typedef char REQUEST_secondLength_Offset[((unsigned)&((REQUEST*)0)->secondLength==196)?1:-1];
typedef char REQUEST_referenceAngle_Offset[((unsigned)&((REQUEST*)0)->referenceAngle==200)?1:-1];
typedef char REQUEST_twist_Offset[((unsigned)&((REQUEST*)0)->twist==204)?1:-1];
typedef char REQUEST_control_Offset[((unsigned)&((REQUEST*)0)->control==208)?1:-1];
typedef char REQUEST_parameters_Offset[((unsigned)&((REQUEST*)0)->parameters==212)?1:-1];
typedef char GAME_stamp_Offset[((unsigned)&((GAME*)0)->stamp==15704)?1:-1];

typedef char WORK_Size[(sizeof(WORK)==0x170)?1:-1];
typedef char WORK_row_Offset[((unsigned)&((WORK*)0)->row==0)?1:-1];
typedef char WORK_firstReturn_Offset[((unsigned)&((WORK*)0)->row[0]==0)?1:-1];
typedef char WORK_firstProduct_Offset[((unsigned)&((WORK*)0)->row[3]==0x30)?1:-1];
typedef char WORK_firstCopy_Offset[((unsigned)&((WORK*)0)->row[6]==0x60)?1:-1];
typedef char WORK_alignmentReturn_Offset[((unsigned)&((WORK*)0)->row[9]==0x90)?1:-1];
typedef char WORK_twistReturn_Offset[((unsigned)&((WORK*)0)->row[12]==0xC0)?1:-1];
typedef char WORK_composedCopy_Offset[((unsigned)&((WORK*)0)->row[15]==0xF0)?1:-1];
typedef char WORK_updatedCopy_Offset[((unsigned)&((WORK*)0)->row[18]==0x120)?1:-1];
typedef char WORK_base_Offset[((unsigned)&((WORK*)0)->row[21]==0x150)?1:-1];
typedef char WORK_delta_Offset[((unsigned)&((WORK*)0)->row[22]==0x160)?1:-1];
typedef char CONTROL_Size[(sizeof(CONTROL)==4)?1:-1];
typedef char GAME_Size[(sizeof(GAME)==0x3D5C)?1:-1];
