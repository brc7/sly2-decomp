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

static __inline__ int isnan(fp_number_type *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ int isinf(fp_number_type *x) { return x->class == CLASS_INFINITY; }
static __inline__ int iszero(fp_number_type *x) { return x->class == CLASS_ZERO; }

int __fpcmp_parts_d(fp_number_type *a, fp_number_type *b)
{
    if (isnan(a) || isnan(b))
        return 1;
    if (isinf(a) && isinf(b))
        return b->sign - a->sign;
    if (isinf(a))
        return a->sign ? -1 : 1;
    if (isinf(b))
        return b->sign ? 1 : -1;
    if (iszero(a) && iszero(b))
        return 0;
    if (iszero(a))
        return b->sign ? 1 : -1;
    if (iszero(b))
        return a->sign ? -1 : 1;
    if (a->sign != b->sign)
        return a->sign ? -1 : 1;
    if (a->normal_exp > b->normal_exp)
        return a->sign ? -1 : 1;
    if (a->normal_exp < b->normal_exp)
        return a->sign ? 1 : -1;
    if (a->fraction.ll > b->fraction.ll)
        return a->sign ? -1 : 1;
    if (a->fraction.ll < b->fraction.ll)
        return a->sign ? 1 : -1;
    return 0;
}
