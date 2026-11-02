// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
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

static __inline__ int isnan(fp_number_type *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ int isinf(fp_number_type *x) { return x->class == CLASS_INFINITY; }
static __inline__ int iszero(fp_number_type *x) { return x->class == CLASS_ZERO; }

double __pack_d(fp_number_type *src)
{
    FLO_union_type dst;
    fractype fraction = src->fraction.ll;
    int sign = src->sign;
    int exp = 0;

    if (isnan(src))
    {
        exp = 0x7FF;
        if (src->class == CLASS_QNAN || 1)
        {
            fraction |= 0x8000000000000L;
        }
    }
    else if (isinf(src))
    {
        exp = 0x7FF;
        fraction = 0;
    }
    else if (iszero(src))
    {
        exp = 0;
        fraction = 0;
    }
    else if (fraction == 0)
    {
        exp = 0;
    }
    else
    {
        if (src->normal_exp < -1022)
        {
            int shift = -1022 - src->normal_exp;
            exp = 0;
            if (shift > 64 - 8)
                fraction = 0;
            else
                fraction >>= shift;
            fraction >>= 8;
        }
        else if (src->normal_exp > 1023)
        {
            exp = 0x7FF;
            fraction = 0;
        }
        else
        {
            exp = src->normal_exp + 1023;
            if ((fraction & 0xFF) == 0x80)
            {
                if (fraction & (1 << 8))
                    fraction += 0x7F + 1;
            }
            else
            {
                fraction += 0x7F;
            }
            if (fraction >= 0x2000000000000000L)
            {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exp = exp;
    dst.bits.sign = sign;
    return dst.value;
}
