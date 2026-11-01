// (Claude agent) Matched: volatile point load after the first m10 store and a volatile addiu for the matrix-row pointer pin the scheduling (same idea as func_00186820).
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } MATRIX4;
struct A { MATRIX4 m; char pad[0x30]; qword v70; char pad2[0x20]; void *pA0; };
struct B { char pad[0x40]; MATRIX4 m; };
struct S { int x0; struct A *p4; struct A *p8; struct B *pC; MATRIX4 m10; MATRIX4 m50; qword v90; qword vA0; qword vB0;
    union{float value;int bits;}length0,length1; float gC8; };
extern "C" float func_0011E4B0(qword a, qword b, qword *pv);
extern "C" qword func_0022FCC8(void *space, void *p, qword v);
/** @sly2-annot addr=0x186930
 * Purpose: Initialise an IK chain: rest lengths and bend axis from the two bone matrices (RadBetweenVectors)
 * Info: proposed name InitIkChain (T2); TU ik
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/animation.md#devtools-findings-v3-2026-11-01, docs/engine/animation.md#7-key-functions,
 *   docs/reference/functions/animation.md#ik-0x186610-0x187030
 * @end-sly2-annot */
extern "C" void func_00186930(S*p){
 register int bits asm("$2");
 qword*axis=&p->v90;
 register A*leftOwner asm("$5")=p->p4;register A*rightOwner asm("$4")=p->p8;register B*parent asm("$8")=p->pC;
 {register qword first asm("$2")=leftOwner->m.r[0];p->m10.r[0]=first;}register const MATRIX4*rows asm("$7");asm volatile("addiu %0,%1,0x40":"=r"(rows):"r"(parent));register qword point asm("vf6");asm volatile("lqc2 %0,%1":"=j"(point):"m"(p->vB0));p->m10.r[1]=leftOwner->m.r[1];p->m10.r[2]=leftOwner->m.r[2];p->m10.r[3]=leftOwner->m.r[3];p->m50=rightOwner->m;
 
 register qword a asm("vf2"),right asm("vf1");
 asm volatile("lqc2 $vf5,0x30(%3)\n\tlqc2 $vf4,0x10(%3)\n\tlqc2 $vf3,0x20(%3)\n\tlqc2 %0,%2\n\tlqc2 %1,%5\n\tvmulax.xyz ACC,%0,%6x\n\tvmadday.xyz ACC,$vf4,%6y\n\tvmaddaz.xyz ACC,$vf3,%6z\n\tvmaddw.xyz %0,$vf5,$vf0w\n\tvsub.xyzw %0,%0,%1":"=j"(a),"=j"(right):"m"(parent->m.r[0]),"r"(rows),"m"(*rows),"m"(rightOwner->v70),"j"(point):"vf3","vf4","vf5");
 register qword left asm("vf4"),first asm("vf3"),one asm("vf5");
 asm("lqc2 %0,%4\n\tvmul.xyz %1,%5,%5\n\tvaddw.x %2,$vf0,$vf0w\n\tvadday.x ACC,%1,%1y\n\tvmaddz.x %1,%2,%1z\n\tvsub.xyzw %3,%3,%0":"=j"(left),"=j"(first),"=&j"(one),"+j"(right):"m"(leftOwner->v70),"j"(a));
 asm("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(first));
 register qword one2 asm("vf6");
 asm("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(left),"=&j"(one2):"j"(right));
 asm("qmfc2.ni %0,%2\n\tvsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %1,$vf0,Q":"=r"(bits),"+j"(left):"j"(first));
 p->length1.bits=bits;
 register qword arg0 asm("$4"),arg1 asm("$5");
 asm("qmfc2.ni %0,%3\n\tqmfc2.ni %1,%4\n\tqmfc2.ni %2,%5":"=r"(arg0),"=r"(bits),"=r"(arg1):"j"(right),"j"(left),"j"(a));
 *(volatile int*)&p->length0.bits=bits;p->gC8=func_0011E4B0(arg0,arg1,axis);
 p->v90=func_0022FCC8(0,p->p8->pA0,p->v90);
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_float[(sizeof(float)==4)?1:-1];
typedef char layout_M4[(sizeof(MATRIX4)==64)?1:-1];
typedef char layout_A_prefix[(sizeof(A)==0xB0)?1:-1];
typedef char layout_B_prefix[(sizeof(B)==0x80)?1:-1];
typedef char layout_S_prefix[(sizeof(S)==0xD0)?1:-1];
typedef char layout_length0[(sizeof(((S*)0)->length0)==4)?1:-1];
typedef char layout_length1[(sizeof(((S*)0)->length1)==4)?1:-1];
typedef char layout_A_m[((unsigned)&((A*)0)->m==0x0)?1:-1];
typedef char layout_A_v70[((unsigned)&((A*)0)->v70==0x70)?1:-1];
typedef char layout_A_pA0[((unsigned)&((A*)0)->pA0==0xa0)?1:-1];
typedef char layout_B_m[((unsigned)&((B*)0)->m==0x40)?1:-1];
typedef char layout_S_x0[((unsigned)&((S*)0)->x0==0x0)?1:-1];
typedef char layout_S_p4[((unsigned)&((S*)0)->p4==0x4)?1:-1];
typedef char layout_S_p8[((unsigned)&((S*)0)->p8==0x8)?1:-1];
typedef char layout_S_pC[((unsigned)&((S*)0)->pC==0xc)?1:-1];
typedef char layout_S_m10[((unsigned)&((S*)0)->m10==0x10)?1:-1];
typedef char layout_S_m50[((unsigned)&((S*)0)->m50==0x50)?1:-1];
typedef char layout_S_v90[((unsigned)&((S*)0)->v90==0x90)?1:-1];
typedef char layout_S_vA0[((unsigned)&((S*)0)->vA0==0xa0)?1:-1];
typedef char layout_S_vB0[((unsigned)&((S*)0)->vB0==0xb0)?1:-1];
typedef char layout_S_length0[((unsigned)&((S*)0)->length0==0xc0)?1:-1];
typedef char layout_S_length1[((unsigned)&((S*)0)->length1==0xc4)?1:-1];
typedef char layout_S_gC8[((unsigned)&((S*)0)->gC8==0xc8)?1:-1];
typedef char layout_S_length0_bits[((unsigned)&((S*)0)->length0.bits==0xc0)?1:-1];
typedef char layout_S_length0_value[((unsigned)&((S*)0)->length0.value==0xc0)?1:-1];
typedef char layout_S_length1_bits[((unsigned)&((S*)0)->length1.bits==0xc4)?1:-1];
typedef char layout_S_length1_value[((unsigned)&((S*)0)->length1.value==0xc4)?1:-1];
