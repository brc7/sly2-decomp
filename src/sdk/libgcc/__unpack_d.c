// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* gcc fp-bit.c unpack_d with NO_DENORMALS: the dead denormal while-loop leaves its loop-head .p2align 3 (the target's nop). */
typedef unsigned long fractype;
typedef enum { CLASS_SNAN, CLASS_QNAN, CLASS_ZERO, CLASS_NUMBER, CLASS_INFINITY } fp_class_type;
typedef struct {
    fp_class_type class;
    unsigned int sign;
    int normal_exp;
    union { fractype ll; } fraction;
} fp_number_type;
typedef union {
    double value;
    fractype value_raw;
    struct { fractype fraction:52; unsigned int exp:11; unsigned int sign:1; } bits;
} FLO_union_type;

void __unpack_d(FLO_union_type *src, fp_number_type *dst)
{
    fractype fraction;
    int exp;
    int sign;

    sign = src->bits.sign;
    exp = src->bits.exp;
    fraction = src->bits.fraction;

    dst->sign = sign;
    if (exp == 0)
    {
        if (fraction == 0 || 1)
        {
            dst->class = CLASS_ZERO;
        }
        else
        {
            dst->normal_exp = exp - 1023 + 1;
            fraction <<= 8;
            dst->class = CLASS_NUMBER;
            while (fraction < 0x1000000000000000L)
            {
                fraction <<= 1;
                dst->normal_exp--;
            }
            dst->fraction.ll = fraction;
        }
    }
    else if (exp == 0x7FF)
    {
        if (fraction == 0)
        {
            dst->class = CLASS_INFINITY;
        }
        else
        {
            if (fraction & 0x8000000000000L)
                dst->class = CLASS_QNAN;
            else
                dst->class = CLASS_SNAN;
            dst->fraction.ll = fraction;
        }
    }
    else
    {
        dst->normal_exp = exp - 1023;
        dst->class = CLASS_NUMBER;
        dst->fraction.ll = (fraction << 8) | 0x1000000000000000L;
    }
}
