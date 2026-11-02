// WIP (Claude agent): 44 diffs, all in the divide loop: target computes num<den once after the normalise-if (both paths) and jumps past the first in-loop sltu (PRE across loop entry); ours enters at loop top. Flags/do-while no help.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
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


static __inline__ fp_number_type *
_fpdiv_parts (fp_number_type * a, fp_number_type * b)
{
  fractype bit;
  fractype numerator;
  fractype denominator;
  fractype quotient;

  if (isnan (a))
    return a;
  if (isnan (b))
    return b;

  a->sign = a->sign ^ b->sign;

  if (isinf (a) || iszero (a))
    {
      if (a->class == b->class)
        return nan ();
      return a;
    }

  if (isinf (b))
    {
      a->fraction.ll = 0;
      a->normal_exp = 0;
      return a;
    }
  if (iszero (b))
    {
      a->class = CLASS_INFINITY;
      return a;
    }

  {
    a->normal_exp = a->normal_exp - b->normal_exp;
    numerator = a->fraction.ll;
    denominator = b->fraction.ll;

    if (numerator < denominator)
      {
        numerator *= 2;
        a->normal_exp--;
      }
    bit = (fractype)1 << 60;
    quotient = 0;
    while (bit)
      {
        if (numerator >= denominator)
          {
            quotient |= bit;
            numerator -= denominator;
          }
        bit >>= 1;
        numerator *= 2;
      }

    if ((quotient & 0xFF) == 0x80)
      {
        if (quotient & (1 << 8))
          {
            quotient += 0x7F + 1;
          }
        else if (numerator)
          {
            quotient += 0x7F + 1;
          }
      }

    a->fraction.ll = quotient;
    return (a);
  }
}

double dpdiv(double arg_a, double arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type *res;
  FLO_union_type au, bu;

  au.value = arg_a;
  bu.value = arg_b;

  __unpack_d (&au, &a);
  __unpack_d (&bu, &b);

  res = _fpdiv_parts (&a, &b);

  return __pack_d (res);
}
