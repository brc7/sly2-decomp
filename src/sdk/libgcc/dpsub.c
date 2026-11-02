// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 5/25, same as dpadd
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
double __pack_d(fp_number_type *src);
fp_number_type *_fpadd_parts(fp_number_type *a, fp_number_type *b, fp_number_type *tmp);
int __fpcmp_parts_d(fp_number_type *a, fp_number_type *b);
extern const fp_number_type __thenan_df;
static __inline__ fp_number_type *nan(void) { return (fp_number_type *)&__thenan_df; }
static __inline__ int isnan(fp_number_type *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ int isinf(fp_number_type *x) { return x->class == CLASS_INFINITY; }
static __inline__ int iszero(fp_number_type *x) { return x->class == CLASS_ZERO; }

double dpsub(double arg_a, double arg_b)
{
    fp_number_type a;
    fp_number_type b;
    fp_number_type tmp;
    fp_number_type *res;
    FLO_union_type au, bu;

    au.value = arg_a;
    bu.value = arg_b;
    __unpack_d(&au, &a);
    __unpack_d(&bu, &b);
    b.sign ^= 1;
    res = _fpadd_parts(&a, &b, &tmp);
    return __pack_d(res);
}
