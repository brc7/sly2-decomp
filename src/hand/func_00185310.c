typedef int qword __attribute__((mode(TI)));
typedef union U{qword q;float f[4];int i[4];}U;
typedef struct M4{U r[4];}M4;
typedef struct VTE{short delta,pad;void*fn;}VTE;
typedef struct I3{int index[3];}I3;
typedef struct MESH{char pad0[0xC];VTE*vt;char pad10[0xD4];qword*vertices;int count;I3*triangles;}MESH;
typedef struct TRIREF{int index;float u,v;}TRIREF;
typedef struct WORK{M4 matrix;TRIREF candidate;TRIREF best __attribute__((aligned(16)));U closest,scalar;float parameter;U bestNormal,input,normal;}WORK;
extern float D_002A5684;
extern void FindClosestPointOnLineSegment(qword,qword,qword,qword*,float*,float*);
static inline qword Unit(qword input){register qword a __asm__("vf3")=input,d __asm__("vf1"),one __asm__("vf2"),r __asm__("vf1");__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"j"(d));__asm__("vdiv Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"=j"(r):"j"(a),"j"(d));return r;}
/** @sly2-annot addr=0x185310
 * Purpose: Closest point/normal/triangle on the object's triangle mesh to a point (world matrix via vt31)
 * Info: proposed name FindMeshClosestPoint (T2); TU tu_185130
 * Notes: names_v2 C proposed MoveStepguardToGoal__FP9STEPGUARD (0.438)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#tu_185130-0x185130-0x186610
 * @end-sly2-annot */
void func_00185310(MESH*mesh,qword point,void*inputReference,TRIREF*outReference,qword*outPoint,qword*outNormal){
 WORK w;register int i __asm__("$16")=0;register unsigned word __asm__("$2");qword bestPoint;register float bestDistance __asm__("$f22");
 VTE*entry=&mesh->vt[31];
 w.input.q=point;
 word=(unsigned)((M4*(*)(void*))entry->fn)((char*)mesh+entry->delta);
 w.matrix=*(M4*)word;
 bestDistance=D_002A5684;
 if(mesh->count>0){word=12;do{
  register I3*indices __asm__("$4");
  register qword*vertices __asm__("$6");
  /* XYZ updates retain the current native VF12/VF11/VF10 W carriers. */
  register qword a __asm__("vf12"),b __asm__("vf11"),c __asm__("vf10"),rawB __asm__("vf2"),rawC __asm__("vf1");
  register qword row0 __asm__("vf6"),row1 __asm__("vf5"),row2 __asm__("vf4"),row3 __asm__("vf3");
  register unsigned addressB __asm__("$3"),addressC __asm__("$5");
  register qword ac __asm__("vf9"),cc __asm__("vf7"),bc __asm__("vf8");
  register qword edgeA __asm__("vf3"),edgeB __asm__("vf1"),delta __asm__("vf2"),cross __asm__("vf4");
  register float denominator __asm__("$f0"),u __asm__("$f2"),rawV __asm__("$f1");
  register unsigned dotUBits __asm__("$3");
  float v;
  __asm__("lw %0,%4\n\tmult %1,%3,%1\n\tlw %2,%5":"=&r"(indices),"+&r"(word),"=r"(vertices):"r"(i),"m"(mesh->triangles),"m"(mesh->vertices):"hi","lo");
  __asm__("lqc2 %0,0(%6)\n\tlqc2 %1,16(%6)\n\tlqc2 %2,32(%6)\n\taddu %4,%4,%5\n\tlqc2 %3,48(%6)"
   :"=j"(row0),"=j"(row1),"=j"(row2),"=j"(row3),"+&r"(indices):"r"(word),"p"(&w.matrix):"memory");
  __asm__("lw $2,0(%3)\n\tlw %2,8(%3)\n\tlw %1,4(%3)\n\tsll $2,$2,4\n\taddu $2,$2,%4\n\tsll %2,%2,4\n\tlqc2 $vf1,0($2)\n\tsll %1,%1,4\n\tvmulax.xyz $ACC,%5,$vf1x\n\tvmadday.xyz $ACC,%6,$vf1y\n\tvmaddaz.xyz $ACC,%7,$vf1z\n\tvmaddw.xyz %0,%8,$vf0w"
   :"+j"(a),"=&r"(addressB),"=&r"(addressC):"r"(indices),"r"(vertices),"j"(row0),"j"(row1),"j"(row2),"j"(row3):"$2","vf1","memory");
  addressB+=(unsigned)vertices;addressC+=(unsigned)vertices;
  __asm__("lqc2 %1,0(%3)\n\tlqc2 %2,0(%4)\n\tvmulax.xyz $ACC,%5,%1x\n\tvmadday.xyz $ACC,%6,%1y\n\tvmaddaz.xyz $ACC,%7,%1z\n\tvmaddw.xyz %0,%8,$vf0w"
   :"+j"(b),"=&j"(rawB),"=&j"(rawC):"r"(addressB),"r"(addressC),"j"(row0),"j"(row1),"j"(row2),"j"(row3):"memory");
  __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(c):"j"(row0),"j"(row1),"j"(row2),"j"(row3),"j"(rawC));
  __asm__("vmove.xyzw %0,%3\n\tvmove.xyzw %1,%4\n\tvmove.xyzw %2,%5":"=&j"(ac),"=&j"(cc),"=j"(bc):"j"(a),"j"(c),"j"(b));
  __asm__("vsub.xyzw %0,%2,%3\n\tvsub.xyzw %1,%4,%3":"=&j"(edgeA),"=j"(edgeB):"j"(ac),"j"(cc),"j"(bc));
  __asm__("lqc2 $vf4,%6\n\tvopmula.xyz $ACC,%3,%5\n\tvopmsub.xyz %0,%5,%3\n\tsqc2 %0,%1\n\tvsub.xyzw %0,$vf4,%4"
   :"=&j"(delta),"=m"(w.normal.q):"0"(rawB),"j"(edgeA),"j"(cc),"j"(edgeB),"m"(w.input.q):"vf4");
  __asm__("lqc2 %3,%6\n\tvopmula.xyz $ACC,%3,%2\n\tvopmsub.xyz %2,%2,%3\n\tvopmula.xyz $ACC,%0,%1\n\tvopmsub.xyz %3,%1,%0\n\tvmul.xyz %1,%1,%2\n\tvaddw.x $vf5,$vf0,$vf0w\n\tvadday.x $ACC,%1,%1y\n\tvmaddz.x %1,$vf5,%1z\n\tvmul.xyz %0,%0,%2\n\tvaddw.x $vf6,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf6,%0z\n\tqmfc2.ni $2,%1\n\tmtc1 $2,%4\n\tqmfc2.ni %5,%0"
   :"+&j"(delta),"+&j"(edgeA),"+&j"(edgeB),"=&j"(cross),"=f"(denominator),"=r"(dotUBits):"m"(w.normal.q):"$2","vf5","vf6");
  __asm__("div.s %0,%2,%0\n\tmtc1 %3,%1":"+f"(denominator),"=f"(u):"f"(1.0f),"r"(dotUBits));
  __asm__("lqc2 $vf2,%4\n\tvmul.xyz %0,$vf2,%0\n\tvaddw.x $vf1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,$vf1,%0z\n\tqmfc2.ni $2,%0\n\tmtc1 $2,%1\n\tmul.s %2,%3,%2"
   :"+j"(cross),"=&f"(rawV),"+f"(u):"f"(denominator),"m"(w.normal.q):"$2","vf1","vf2");
  v=denominator*rawV;
  if(u<0.0f){
   FindClosestPointOnLineSegment(w.input.q,bc,cc,&w.closest.q,&w.parameter,0);
   w.candidate.u=0.0f;w.candidate.v=1.0f-w.parameter;
  }else if(v<0.0f){
   FindClosestPointOnLineSegment(w.input.q,ac,cc,&w.closest.q,&w.parameter,0);
   w.candidate.u=1.0f-w.parameter;w.candidate.v=0.0f;
  }else if(u+v>1.0f){
   FindClosestPointOnLineSegment(w.input.q,ac,bc,&w.closest.q,&w.parameter,0);
   w.candidate.u=1.0f-w.parameter;w.candidate.v=w.parameter;
  }else{
   __asm__ volatile("sub.s $f0,%9,%7\n\tswc1 %7,%0\n\tmfc1 $3,%7\n\tqmtc2.ni $3,$vf2\n\tmfc1 $4,%8\n\tsub.s $f0,$f0,%8\n\tsqc2 $vf2,%2\n\tqmtc2.ni $4,$vf3\n\tswc1 %8,%1\n\tsqc2 $vf3,%2\n\tmfc1 $5,$f0\n\tqmtc2.ni $5,$vf1\n\tvmulax.xyzw $ACC,%4,$vf2x\n\tvmaddax.xyzw $ACC,%5,$vf3x\n\tvmaddx.xyzw $vf2,%6,$vf1x\n\tsqc2 $vf1,%2\n\tsqc2 $vf2,%3"
    :"=m"(w.candidate.u),"=m"(w.candidate.v),"=m"(w.scalar.q),"=m"(w.closest.q)
    :"j"(a),"j"(b),"j"(c),"f"(u),"f"(v),"f"(1.0f):"$3","$4","$5","$f0","vf1","vf2","vf3");
  }
  {register qword closest __asm__("vf3");float distance;
   __asm__("lqc2 $vf2,%3\n\tlqc2 %0,%2\n\tvsub.xyzw $vf1,%0,$vf2\n\tvmul.xyz $vf1,$vf1,$vf1\n\tvaddw.x $vf2,$vf0,$vf0w\n\tvadday.x $ACC,$vf1,$vf1y\n\tvmaddz.x $vf1,$vf2,$vf1z\n\tqmfc2.ni $2,$vf1\n\tmtc1 $2,%1":"=j"(closest),"=f"(distance):"m"(w.closest.q),"m"(w.input.q):"$2","vf1","vf2");
   if(distance<bestDistance){
    w.candidate.index=i;bestDistance=distance;
    {register qword accepted __asm__("$18");
     __asm__("ldl $3,7+%a3\n\tldr $3,%a3\n\tlw $4,8+%a3\n\tsdl $3,7+%a4\n\tsdr $3,%a4\n\tsw $4,8+%a4\n\tqmfc2.ni %1,%5\n\tlq $3,%6\n\tsq $3,%2"
      :"=m"(w.best),"=&r"(accepted),"=m"(w.bestNormal.q):"p"(&w.candidate),"p"(&w.best),"j"(closest),"m"(w.normal.q),"m"(w.candidate):"$3","$4");
     bestPoint=accepted;}
   }
  }
  word=(unsigned)mesh->count;i++;word=i<(int)word;
  if(!word)break;
  word=12;
 }while(1);}
 if(outReference){
  __asm__("ldl $4,7+%a1\n\tldr $4,%a1\n\tlw $5,8+%a1\n\tsdl $4,7(%2)\n\tsdr $4,0(%2)\n\tsw $5,8(%2)":"=m"(*outReference):"p"(&w.best),"r"(outReference),"m"(w.best):"$4","$5");
 }
 if(outPoint)*outPoint=bestPoint;
 if(outNormal)*outNormal=Unit(w.bestNormal.q);
}

typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char VTE_Size[(sizeof(VTE)==8)?1:-1];
typedef char I3_Size[(sizeof(I3)==12)?1:-1];
typedef char TRIREF_Size[(sizeof(TRIREF)==12)?1:-1];
typedef char WORK_Size[(sizeof(WORK)==0xC0)?1:-1];
typedef char VTE_delta_Offset[((unsigned)&((VTE*)0)->delta==0)?1:-1];
typedef char VTE_fn_Offset[((unsigned)&((VTE*)0)->fn==4)?1:-1];
typedef char MESH_vt_Offset[((unsigned)&((MESH*)0)->vt==12)?1:-1];
typedef char MESH_vertices_Offset[((unsigned)&((MESH*)0)->vertices==228)?1:-1];
typedef char MESH_count_Offset[((unsigned)&((MESH*)0)->count==232)?1:-1];
typedef char MESH_triangles_Offset[((unsigned)&((MESH*)0)->triangles==236)?1:-1];
typedef char TRIREF_index_Offset[((unsigned)&((TRIREF*)0)->index==0)?1:-1];
typedef char TRIREF_u_Offset[((unsigned)&((TRIREF*)0)->u==4)?1:-1];
typedef char TRIREF_v_Offset[((unsigned)&((TRIREF*)0)->v==8)?1:-1];
typedef char WORK_matrix_Offset[((unsigned)&((WORK*)0)->matrix==0)?1:-1];
typedef char WORK_candidate_Offset[((unsigned)&((WORK*)0)->candidate==64)?1:-1];
typedef char WORK_best_Offset[((unsigned)&((WORK*)0)->best==80)?1:-1];
typedef char WORK_closest_Offset[((unsigned)&((WORK*)0)->closest==96)?1:-1];
typedef char WORK_scalar_Offset[((unsigned)&((WORK*)0)->scalar==112)?1:-1];
typedef char WORK_parameter_Offset[((unsigned)&((WORK*)0)->parameter==128)?1:-1];
typedef char WORK_bestNormal_Offset[((unsigned)&((WORK*)0)->bestNormal==144)?1:-1];
typedef char WORK_input_Offset[((unsigned)&((WORK*)0)->input==160)?1:-1];
typedef char WORK_normal_Offset[((unsigned)&((WORK*)0)->normal==176)?1:-1];

typedef char Unsigned_Width[sizeof(unsigned)==4?1:-1];
typedef char Float_Width[sizeof(float)==4?1:-1];
typedef char Int_Width[sizeof(int)==4?1:-1];
typedef char Short_Width[sizeof(short)==2?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char TRIREF_Alignment[__alignof__(TRIREF)==4?1:-1];
