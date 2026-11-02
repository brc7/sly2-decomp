// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned long ulong;
union DOUBLEWORD {double value;ulong bits;};
static inline void SplitMagnitudeBits(ulong bits,long *high,ulong *low){
 register ulong input asm("a0")=bits;
 register long top asm("v0");
 register ulong bottom asm("a1");
 asm("daddu %0,%2,$0\n\tdsra32 %0,%0,0\n\tdaddu %1,%2,$0":"=&r"(top),"=r"(bottom):"r"(input));
 *high=top;*low=bottom;
}
static inline ulong JoinWords(ulong low,ulong high){
 register ulong lo asm("a1")=low,hi asm("v0")=high;
 register ulong result asm("a0");
 asm("or %0,%1,%2":"=r"(result):"r"(lo),"r"(hi));
 return result;
}
static inline void MaskMagnitudeWords(long *high,ulong *low){
 register long magnitude asm("v0")=*high;
 register ulong bottom asm("a1")=*low;
 register unsigned int highMask asm("v1")=0x7FFFFFFFU;
 register ulong lowMask asm("a0")=0xFFFFFFFFUL;
 asm("and %1,%1,%3\n\tand %0,%0,%2":"+r"(magnitude),"+&r"(bottom):"r"(highMask),"r"(lowMask));
 *high=magnitude;*low=bottom;
}
double fabs(double x){
 union DOUBLEWORD value;
 ulong bits,low;long high;
 value.value=x;bits=value.bits;SplitMagnitudeBits(bits,&high,&low);
 MaskMagnitudeWords(&high,&low);
 value.bits=JoinWords(low,(ulong)high<<32);
 return value.value;
}
