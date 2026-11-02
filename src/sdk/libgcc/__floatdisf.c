// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef int SItype;
typedef unsigned int USItype;
typedef long DItype;
typedef unsigned long UDItype;
typedef double DFtype;
typedef float SFtype;
#define WORD_SIZE 32
#define DI_SIZE 64
#define DF_SIZE 53
#define SF_SIZE 24
#define HIGH_HALFWORD_COEFF (((UDItype) 1) << (WORD_SIZE / 2))
#define HIGH_WORD_COEFF (((UDItype) 1) << WORD_SIZE)
#define REP_BIT ((UDItype) 1 << (DI_SIZE - DF_SIZE))
SFtype __floatdisf(DItype u)
{
    DFtype f;
    if (DF_SIZE < DI_SIZE && DF_SIZE > (DI_SIZE - DF_SIZE + SF_SIZE)) {
        if (!(-((DItype)1 << DF_SIZE) < u && u < ((DItype)1 << DF_SIZE))) {
            if ((USItype)u & (REP_BIT - 1))
                u |= REP_BIT;
        }
    }
    f = (SItype)(u >> WORD_SIZE);
    f *= HIGH_HALFWORD_COEFF;
    f *= HIGH_HALFWORD_COEFF;
    f += (USItype)(u & (HIGH_WORD_COEFF - 1));
    return (SFtype)f;
}
