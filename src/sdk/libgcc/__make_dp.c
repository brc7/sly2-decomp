// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef enum { CLASS_SNAN, CLASS_QNAN, CLASS_ZERO, CLASS_NUMBER, CLASS_INFINITY } fp_class_type;
typedef unsigned long fractype;
typedef struct { fp_class_type class; unsigned int sign; int normal_exp; union { fractype ll; } fraction; } fp_number_type;
double __pack_d(fp_number_type *);
double __make_dp(fp_class_type class, unsigned int sign, int exp, fractype frac)
{
  fp_number_type in;
  in.class = class;
  in.sign = sign;
  in.normal_exp = exp;
  in.fraction.ll = frac;
  return __pack_d(&in);
}
