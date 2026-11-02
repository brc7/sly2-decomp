// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef int SItype;
typedef unsigned int USItype;
typedef long DItype;
typedef double DFtype;
#define WORD_SIZE 32
#define HIGH_HALFWORD_COEFF (((DItype) 1) << (WORD_SIZE / 2))
#define HIGH_WORD_COEFF (((DItype) 1) << WORD_SIZE)
DFtype __floatdidf(DItype u)
{
    DFtype d;
    d = (SItype)(u >> WORD_SIZE);
    d *= HIGH_HALFWORD_COEFF;
    d *= HIGH_HALFWORD_COEFF;
    d += (USItype)(u & (HIGH_WORD_COEFF - 1));
    return d;
}
