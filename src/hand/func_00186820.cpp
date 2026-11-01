// Paired-distance update; volatile on the first two VU asm groups keeps the p4 load between them (Claude agent).
typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[4]; } MATRIX4;
struct A { MATRIX4 m; char pad[0x30]; qword v70; char pad2[0x20]; void *pA0; };
struct B { char pad[0x40]; MATRIX4 m; };
struct S { int x0; struct A *p4; struct A *p8; struct B *pC; MATRIX4 m10; MATRIX4 m50; qword v90; qword vA0; qword vB0;
    float gC0; float gC4; float gC8; };
struct N { int k; char pad[0xC]; struct N *next; char pad2[8]; struct S *ps; };
struct E { char pad[0x90]; struct N *pn; char pad2[0x100 - 0x94]; };
struct O { char pad[0x50]; int c; struct E *ae; char pad2[0x148 - 0x58]; unsigned long grf; };
static inline N*Entry(O*owner,int i){register E*entries asm("$2")=owner->ae;register int offset asm("$3")=i*256;entries=(E*)((int)entries+offset);return entries->pn;}
/** @sly2-annot addr=0x186820
 * Purpose: For SOs with bit 36, update each IK node's bone lengths/distances
 * Info: proposed name UpdateSoIkLengths (T2); TU ik
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/animation.md#ik-0x186610-0x187030,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_00186820(O*owner){
 if(!(owner->grf&(1UL<<36)))return;
 for(int i=0;i<owner->c;i++)for(N*node=Entry(owner,i);node;node=node->next)if(node->k==7){
  register S*p asm("$4")=node->ps;
  register B*parent asm("$3")=p->pC;register A*rightOwner asm("$6")=p->p8;register const MATRIX4*rows asm("$2")=&parent->m;
  register qword transformed asm("vf2"),right asm("vf1");
  asm volatile("lqc2 %0,%2\n\tlqc2 $vf4,0x20(%3)\n\tlqc2 $vf6,0x30(%3)\n\tlqc2 $vf5,0x10(%3)\n\tlqc2 $vf3,%5\n\tlqc2 %1,%6\n\tvmulax.xyz ACC,%0,$vf3x\n\tvmadday.xyz ACC,$vf5,$vf3y\n\tvmaddaz.xyz ACC,$vf4,$vf3z\n\tvmaddw.xyz %0,$vf6,$vf0w":"=j"(transformed),"=j"(right):"m"(parent->m.r[0]),"r"(rows),"m"(*rows),"m"(p->vB0),"m"(rightOwner->v70):"vf3","vf4","vf5","vf6");
  register A*leftOwner asm("$5")=p->p4;
  register qword one asm("vf4"),left asm("vf3");register int bits asm("$2");
  asm volatile("vsub.xyzw %0,%0,%2\n\tvmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(transformed),"=&j"(one):"j"(right));
  asm("lqc2 %1,%2\n\tvsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(transformed),"=j"(left):"m"(leftOwner->v70));
  asm("vsub.xyzw %1,%1,%2\n\tqmfc2.ni %0,%3\n\tvmul.xyz %1,%1,%1\n\tvaddw.x %2,$vf0,$vf0w\n\tvadday.x ACC,%1,%1y\n\tvmaddz.x %1,%2,%1z":"=r"(bits),"+j"(right),"+j"(left):"j"(transformed));
  *(int*)&p->gC4=bits;
  asm("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %1,$vf0,Q\n\tqmfc2.ni %0,%1":"=r"(bits),"+j"(right));
  *(int*)&p->gC0=bits;
 }
}

typedef char layout_pointer[(sizeof(void*)==4)?1:-1];
typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_M4[(sizeof(MATRIX4)==64)?1:-1];
typedef char layout_A_prefix[(sizeof(A)==0xB0)?1:-1];
typedef char layout_B_prefix[(sizeof(B)==0x80)?1:-1];
typedef char layout_S_prefix[(sizeof(S)==0xD0)?1:-1];
typedef char layout_N_prefix[(sizeof(N)==0x20)?1:-1];
typedef char layout_E[(sizeof(E)==0x100)?1:-1];
typedef char layout_O_prefix[(sizeof(O)==0x150)?1:-1];
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
typedef char layout_S_gC0[((unsigned)&((S*)0)->gC0==0xc0)?1:-1];
typedef char layout_S_gC4[((unsigned)&((S*)0)->gC4==0xc4)?1:-1];
typedef char layout_S_gC8[((unsigned)&((S*)0)->gC8==0xc8)?1:-1];
typedef char layout_N_k[((unsigned)&((N*)0)->k==0x0)?1:-1];
typedef char layout_N_next[((unsigned)&((N*)0)->next==0x10)?1:-1];
typedef char layout_N_ps[((unsigned)&((N*)0)->ps==0x1c)?1:-1];
typedef char layout_E_pn[((unsigned)&((E*)0)->pn==0x90)?1:-1];
typedef char layout_O_c[((unsigned)&((O*)0)->c==0x50)?1:-1];
typedef char layout_O_ae[((unsigned)&((O*)0)->ae==0x54)?1:-1];
typedef char layout_O_grf[((unsigned)&((O*)0)->grf==0x148)?1:-1];
