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

static __inline__ int isnan(fp_number_type *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ int isinf(fp_number_type *x) { return x->class == CLASS_INFINITY; }
static __inline__ int iszero(fp_number_type *x) { return x->class == CLASS_ZERO; }

float __pack_f(fp_number_type *src)
{
    FLO_union_type dst;
    fractype fraction = src->fraction.ll;
    int sign = src->sign;
    int exp = 0;

    if (isnan(src))
    {
        exp = 255;
        if (src->class == CLASS_QNAN || 1)
        {
            fraction |= 0x100000;
        }
    }
    else if (isinf(src))
    {
        exp = 255;
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
        if (src->normal_exp < -126)
        {
            int shift = -126 - src->normal_exp;
            exp = 0;
            if (shift > 32 - 7)
                fraction = 0;
            else
                fraction >>= shift;
            fraction >>= 7;
        }
        else if (src->normal_exp > 127)
        {
            exp = 255;
            fraction = 0;
        }
        else
        {
            exp = src->normal_exp + 127;
            if ((fraction & 0x7F) == 0x40)
            {
                if (fraction & (1 << 7))
                    fraction += 0x3F + 1;
            }
            else
            {
                fraction += 0x3F;
            }
            if (fraction >= 0x80000000)
            {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 7;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exp = exp;
    dst.bits.sign = sign;
    return dst.value;
}
