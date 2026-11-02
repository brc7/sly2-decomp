// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef int SItype;
typedef unsigned int USItype;
typedef long DItype;
typedef unsigned long UDItype;
typedef double DFtype;
#define WORD_SIZE 32
#define HIGH_WORD_COEFF (((UDItype) 1) << WORD_SIZE)
UDItype __fixunsdfdi(DFtype a)
{
    DFtype b;
    UDItype v;
    if (a < 0)
        return 0;
    b = a / HIGH_WORD_COEFF;
    v = (USItype)b;
    v <<= WORD_SIZE;
    a -= (DFtype)v;
    if (a < 0)
        v -= (USItype)(-a);
    else
        v += (USItype)a;
    return v;
}
