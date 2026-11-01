// Build the skew derivative matrix in the supplied basis.
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int n[4]; } VU;
typedef struct { qword r[3]; } M3;
typedef struct { VU r[4]; } M4;
static inline void MultiplyPublished(M4 &destination,const M3 &basis,M3 &first,M3 &second){
 const M3 *pa=(const M3 *)&destination,*pb=&basis;
 register qword a0 asm("vf3"),a1 asm("vf2"),a2 asm("vf1"),b0 asm("vf4"),b1 asm("vf6"),b2 asm("vf5");
 asm("lqc2 %1,16(%6)\n\tlqc2 %5,32(%7)\n\tlqc2 %3,0(%7)\n\tlqc2 %0,0(%6)\n\tlqc2 %2,32(%6)\n\tlqc2 %4,16(%7)":"=j"(a0),"=j"(a1),"=j"(a2),"=j"(b0),"=j"(b1),"=j"(b2):"r"(pa),"r"(pb),"m"(*pa),"m"(*pb));
 asm("vmulax.xyz ACC,%0,%2x\n\tvmadday.xyz ACC,%4,%2y\n\tvmaddz.xyz %2,%5,%2z\n\tvmulax.xyz ACC,%0,%1x\n\tvmadday.xyz ACC,%4,%1y\n\tvmaddz.xyz %1,%5,%1z\n\tsqc2 %2,32+%3\n\tvmulax.xyz ACC,%0,%6x\n\tvmadday.xyz ACC,%4,%6y\n\tvmaddz.xyz %0,%5,%6z\n\tsqc2 %1,%3\n\tsqc2 %0,16+%3":"+j"(a0),"+j"(b0),"+j"(b2),"=m"(*(M3 *)&destination):"j"(a1),"j"(a2),"j"(b1));
 asm volatile("sqc2 %6,%3\n\tsqc2 %6,%0\n\tsqc2 %7,%4\n\tsqc2 %7,%1\n\tsqc2 %8,%5\n\tsqc2 %8,%2":"=m"(first.r[0]),"=m"(first.r[1]),"=m"(first.r[2]),"=m"(second.r[0]),"=m"(second.r[1]),"=m"(second.r[2]):"j"(b0),"j"(a0),"j"(b2));
}
extern "C" void *func_002333B4(void *p, int c, unsigned int n);
static inline M4 *PublishSkew(M4 &m,qword value){
 register M4 *result asm("$2");
 asm("pextuw %0,$zero,%7\n\tmtc1 %7,$f4\n\tmtc1 %0,$f1\n\tprot3w %0,%7\n\tneg.s $f3,$f4\n\tmtc1 %0,$f0\n\tneg.s $f2,$f1\n\tswc1 $f1,%1\n\tswc1 $f0,%2\n\tdaddu %0,%8,$zero\n\tneg.s $f0,$f0\n\tswc1 $f3,%3\n\tswc1 $f2,%4\n\tswc1 $f4,%5\n\tswc1 $f0,%6"
 :"=&r"(result),"=m"(m.r[0].f[1]),"=m"(m.r[2].f[0]),"=m"(m.r[2].f[1]),"=m"(m.r[1].f[0]),"=m"(m.r[1].f[2]),"=m"(m.r[0].f[2])
 :"r"(value),"r"(&m):"$f0","$f1","$f2","$f3","$f4");return result;
}
static inline void RetainMatrices(M4 *incomingDestination,const M3 *incomingBasis,M4 *&outDestination,const M3 *&outBasis){register M4 *destination asm("$17");register const M3 *basis asm("$18");asm("daddu %0,%2,$zero\n\tdaddu %1,%3,$zero":"=&r"(destination),"=&r"(basis):"r"(incomingDestination),"r"(incomingBasis));outDestination=destination;outBasis=basis;}
/** @sly2-annot addr=0x22CCD0
 * Purpose: hand comment
 * Info: TU xf
 * Notes: descriptive only: xf: skew derivative matrix | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
extern "C" M4 *func_0022CCD0(M4 *destination, void *pv, const M3 *basis, qword v)
{
    M4 *pm;const M3 *pmB;RetainMatrices(destination,basis,pm,pmB);func_002333B4(destination, 0, 0x40);
    M4 *result=PublishSkew(*pm,v);
    M3 first,second;MultiplyPublished(*pm,*pmB,first,second);
    pm->r[1].n[3] = 0;
    pm->r[0].n[3] = 0;
    pm->r[2].n[3] = 0;
    return result;
}
