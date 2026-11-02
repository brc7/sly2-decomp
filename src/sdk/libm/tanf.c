// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union { float value; __uint32_t word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)

float __kernel_tanf(float x, float y, int k);
__int32_t __ieee754_rem_pio2f(float x, float *y);

float tanf(float x)
{
    float y[2], z = 0.0;
    __int32_t n, ix;
    float a, b;
    int k;

    GET_FLOAT_WORD(ix, x);

    ix &= 0x7fffffff;
    if (ix <= 0x3f490fda) {
        a = x; b = z; k = 1;
    } else if (ix >= 0x7f800000) return x - x;
    else {
        n = __ieee754_rem_pio2f(x, y);
        a = y[0]; b = y[1]; k = 1; k -= (n & 1) << 1;
    }
    return __kernel_tanf(a, b, k);
}
