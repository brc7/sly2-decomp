// Complete vector-state update with the observed scalar scratch writes.
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int n[4];} VU;
struct G {char pad[0x10];qword v;};
struct Q {char pad[0x10];qword v,w;};
struct S {int x0,k;char pad[0xD8];qword vE0;};
union F32 {float f;int bits;};
extern G D_002DDFA0;
extern float D_002C4108;
extern F32 D_002C410C;
extern qword D_002A2DD0;

/** @sly2-annot addr=0x220388
 * Purpose: vector-state update of hold point for states 4..6 (gain D_002C4108, normalize)
 * Info: proposed name UpdateJtthrowHoldPos (T2); TU throw
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
extern "C" void func_00220388(S *owner,Q *value){
 VU plane,scratch;
 register float gain asm("$f1");register G *movementAddress asm("$3");register qword prior asm("vf2"),position asm("vf5");
 asm("lui $2,%%hi(%3)\n\tlui %1,%%hi(%4)\n\tlwc1 %0,%%lo(%3)($2)\n\taddiu %1,%1,%%lo(%4)\n\tlqc2 %2,%5":"=f"(gain),"=&r"(movementAddress),"=j"(prior):"i"(&D_002C4108),"i"(&D_002DDFA0),"m"(value->v):"$2","memory");
 asm("mfc1 $2,%2\n\tqmtc2.ni $2,$vf3\n\tlqc2 $vf1,%4\n\tsqc2 $vf3,%1\n\tvadda.xyzw ACC,%3,$vf0\n\tvmaddx.xyzw %0,$vf1,$vf3x":"=j"(position),"=m"(plane.q):"f"(gain),"j"(prior),"m"(movementAddress->v):"$2","vf1","vf3");
 value->v=position;
 if(owner->k<7){qword *destination=&value->w;if(owner->k>=4){
 register float minusOne asm("$f0"),negativeGain asm("$f1")=gain;
 register qword motion asm("vf2");
 asm("li.s %0,-1.0\n\tneg.s %1,%1\n\tlqc2 %2,%3":"=&f"(minusOne),"+f"(negativeGain),"=j"(motion):"m"(movementAddress->v):"$1");
 register qword target asm("vf1"),negative asm("vf4");register unsigned high asm("$2");
 asm("mfc1 %2,%4\n\tqmtc2.ni %2,$vf3\n\tlqc2 %0,%8\n\tmfc1 %2,%5\n\tqmtc2.ni %2,%1\n\tsqc2 $vf3,%3\n\tvadda.xyzw ACC,%0,$vf0\n\tvmaddax.xyzw ACC,%6,$vf3x\n\tvmaddx.xyzw %0,%7,%1x\n\tlui %2,%%hi(%9)"
 :"=&j"(target),"=&j"(negative),"=&r"(high),"=m"(scratch.q):"f"(minusOne),"f"(negativeGain),"j"(position),"j"(motion),"m"(owner->vE0),"i"(&D_002C410C):"vf3");
 plane.q=target;plane.n[2]=0;
 register int raw asm("$2");register qword direction asm("vf3"),scale asm("vf6");
 asm("lw %0,%%lo(%5)(%3)\n\tlqc2 %1,%4\n\tqmtc2.ni %0,%2":"=r"(raw),"=j"(direction),"=j"(scale):"r"(high),"m"(plane.q),"i"(&D_002C410C),"m"(*(const int *)(high+0x410C)));
 register qword length asm("vf1"),unit asm("vf2");register float tolerance asm("$f1");
 asm("vmul.xyz %0,%3,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tli.s %2,0.0001\n\tvsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=&j"(length),"=&j"(unit),"=f"(tolerance):"j"(direction):"$1");
 register qword fallback asm("$2");register int bits asm("$3");register float size asm("$f0");
 asm("lui %0,%%hi(%4)\n\tqmfc2.ni %1,%3\n\taddiu %0,%0,%%lo(%4)\n\tmtc1 %1,%2\n\tlq %0,0(%0)":"=&r"(fallback),"=&r"(bits),"=f"(size):"j"(length),"i"(&D_002A2DD0):"memory");
 // Preserve the full native negative-gain scratch publication.
 *(volatile qword *)&scratch.q=negative;
 scratch.q=scale;scale=scratch.q;
 if(size<tolerance)length=fallback;
 else asm("vdiv Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%1,Q":"+j"(length):"j"(direction));
 asm("vmulx.xyzw %0,%0,%1x":"+j"(length):"j"(scale));asm("sqc2 %1,%0":"=m"(*destination):"j"(length));
}}
}
