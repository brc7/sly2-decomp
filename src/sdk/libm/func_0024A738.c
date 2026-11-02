// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned long ulong;
union DOUBLEWORD {double value;ulong bits;};
static inline void SplitSignBits(ulong magnitude,ulong signBits,long *high,long *sign,ulong *low){
 register ulong input asm("a0")=magnitude;
 register long top asm("v0"),signWord asm("a1")=(long)signBits;
 register ulong bottom asm("a2");
 asm("daddu %0,%3,$0\n\tdsra32 %0,%0,0\n\tdsra32 %2,%2,0\n\tdaddu %1,%3,$0":"=&r"(top),"=r"(bottom),"+r"(signWord):"r"(input));
 *high=top;*low=bottom;*sign=signWord;
}
static inline ulong JoinWords(ulong low,ulong high){
 register ulong lo asm("a2")=low,hi asm("v0")=high;
 register ulong result asm("a0");
 asm("or %0,%1,%2":"=r"(result):"r"(lo),"r"(hi));
 return result;
}
static inline void MaskHighWords(long *high,long *sign){
 register long magnitude asm("v0")=*high,signWord asm("a1")=*sign;
 register unsigned int highMask asm("v1")=0x7FFFFFFFU;
 register long signMask asm("a0")=-2147483647L-1;
 asm("and %1,%1,%3\n\tand %0,%0,%2":"+r"(magnitude),"+&r"(signWord):"r"(highMask),"r"(signMask));
 *high=magnitude;*sign=signWord;
}
static inline void PackWordParts(ulong *low,ulong *high){
 register ulong bottom asm("a2")=*low,top asm("v0")=*high;
 register ulong mask asm("a0")=0xFFFFFFFFUL;
 asm("and %0,%0,%2\n\tdsll32 %1,%1,0":"+&r"(bottom),"+r"(top):"r"(mask));
 *low=bottom;*high=top;
}
double func_0024A738(double x,double y){
 union DOUBLEWORD magnitude,signValue,result;
 ulong low,combined;long high,sign;
 magnitude.value=x;signValue.value=y;
 SplitSignBits(magnitude.bits,signValue.bits,&high,&sign,&low);
 MaskHighWords(&high,&sign);
 combined=(ulong)high|(ulong)sign;
 PackWordParts(&low,&combined);
 result.bits=JoinWords(low,combined);
 return result.value;
}
