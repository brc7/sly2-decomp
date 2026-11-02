// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned long fractype;
typedef long intfrac;
typedef enum { CLASS_SNAN, CLASS_QNAN, CLASS_ZERO, CLASS_NUMBER, CLASS_INFINITY } fp_class_type;
typedef struct {
    fp_class_type class;
    unsigned int sign;
    int normal_exp;
    union { fractype ll; } fraction;
} fp_number_type;

extern fp_number_type D_00450230;
#define nan() (&D_00450230)
#define IMPLICIT_1 ((fractype)1 << 60)
#define IMPLICIT_2 ((fractype)1 << 61)
#define LSHIFT(X) (X = ((X) & 1) | ((X) >> 1))

static __inline__ int isnan(fp_number_type *x) { return x->class == CLASS_SNAN || x->class == CLASS_QNAN; }
static __inline__ int isinf(fp_number_type *x) { return x->class == CLASS_INFINITY; }
static __inline__ int iszero(fp_number_type *x) { return x->class == CLASS_ZERO; }

fp_number_type *_fpadd_parts(fp_number_type *a, fp_number_type *b, fp_number_type *tmp)
{
    intfrac tfraction;
    int a_normal_exp;
    int b_normal_exp;
    fractype a_fraction;
    fractype b_fraction;

    if (isnan(a))
        return a;
    if (isnan(b))
        return b;
    if (isinf(a))
    {
        if (isinf(b) && a->sign != b->sign)
            return nan();
        return a;
    }
    if (isinf(b))
        return b;
    if (iszero(b))
    {
        if (iszero(a))
        {
            *tmp = *a;
            tmp->sign = a->sign & b->sign;
            return tmp;
        }
        return a;
    }
    if (iszero(a))
        return b;

    {
        int diff;

        a_normal_exp = a->normal_exp;
        b_normal_exp = b->normal_exp;
        a_fraction = a->fraction.ll;
        b_fraction = b->fraction.ll;

        diff = a_normal_exp - b_normal_exp;

        if (diff < 0)
            diff = -diff;
        if (diff < 64)
        {
            while (a_normal_exp > b_normal_exp)
            {
                b_normal_exp++;
                LSHIFT(b_fraction);
            }
            while (b_normal_exp > a_normal_exp)
            {
                a_normal_exp++;
                LSHIFT(a_fraction);
            }
        }
        else
        {
            if (a_normal_exp > b_normal_exp)
            {
                b_normal_exp = a_normal_exp;
                b_fraction = 0;
            }
            else
            {
                a_normal_exp = b_normal_exp;
                a_fraction = 0;
            }
        }
    }

    if (a->sign != b->sign)
    {
        if (a->sign)
        {
            tfraction = -a_fraction + b_fraction;
        }
        else
        {
            tfraction = a_fraction - b_fraction;
        }
        if (tfraction >= 0)
        {
            tmp->sign = 0;
            tmp->normal_exp = a_normal_exp;
            tmp->fraction.ll = tfraction;
        }
        else
        {
            tmp->sign = 1;
            tmp->normal_exp = a_normal_exp;
            tmp->fraction.ll = -tfraction;
        }

        while (tmp->fraction.ll < IMPLICIT_1 && tmp->fraction.ll)
        {
            tmp->fraction.ll <<= 1;
            tmp->normal_exp--;
        }
    }
    else
    {
        tmp->sign = a->sign;
        tmp->normal_exp = a_normal_exp;
        tmp->fraction.ll = a_fraction + b_fraction;
    }
    tmp->class = CLASS_NUMBER;

    if (tmp->fraction.ll >= IMPLICIT_2)
    {
        LSHIFT(tmp->fraction.ll);
        tmp->normal_exp++;
    }
    return tmp;
}
