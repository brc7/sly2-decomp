// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
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
} FLO_union_type;

void __unpack_d(FLO_union_type *src, fp_number_type *dst);

static __inline__ int isnan(fp_number_type *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ int isinf(fp_number_type *x) { return x->class == CLASS_INFINITY; }
static __inline__ int iszero(fp_number_type *x) { return x->class == CLASS_ZERO; }

unsigned int dptoul(double arg_a)
{
    fp_number_type a;
    FLO_union_type au;

    au.value = arg_a;
    __unpack_d(&au, &a);
    if (iszero(&a))
        return 0;
    if (isnan(&a))
        return 0;
    if (a.sign)
        return 0;
    if (isinf(&a))
        return 0xFFFFFFFF;
    if (a.normal_exp < 0)
        return 0;
    if (a.normal_exp > 32 - 1)
        return 0xFFFFFFFF;
    else if (a.normal_exp <= 52 + 8)
        return a.fraction.ll >> ((52 + 8) - a.normal_exp);
    else
        return a.fraction.ll << (a.normal_exp - (52 + 8));
}
