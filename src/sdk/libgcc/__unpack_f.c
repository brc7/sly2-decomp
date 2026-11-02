// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef unsigned int fractype;
typedef enum { CLASS_SNAN, CLASS_QNAN, CLASS_ZERO, CLASS_NUMBER, CLASS_INFINITY } fp_class_type;
typedef struct {
    fp_class_type class;
    unsigned int sign;
    int normal_exp;
    union { fractype ll; } fraction;
} fp_number_type;
typedef union {
    float value;
    fractype value_raw;
    struct { fractype fraction:23; unsigned int exp:8; unsigned int sign:1; } bits;
} FLO_union_type;

void __unpack_f(FLO_union_type *src, fp_number_type *dst)
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
    }
    else if (exp == 255)
    {
        if (fraction == 0)
        {
            dst->class = CLASS_INFINITY;
        }
        else
        {
            if (fraction & 0x100000)
                dst->class = CLASS_QNAN;
            else
                dst->class = CLASS_SNAN;
            dst->fraction.ll = fraction;
        }
    }
    else
    {
        dst->normal_exp = exp - 127;
        dst->class = CLASS_NUMBER;
        dst->fraction.ll = (fraction << 7) | 0x40000000;
    }
}
