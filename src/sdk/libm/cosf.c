// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union { float value; __uint32_t word; } ieee_float_shape_type;
#define GET_FLOAT_WORD(i,d) do { ieee_float_shape_type gf_u; gf_u.value = (d); (i) = gf_u.word; } while (0)

float __kernel_cosf(float x, float y);
float __kernel_sinf(float x, float y, int iy);
__int32_t __ieee754_rem_pio2f(float x, float *y);

float cosf(float x)
{
    float y[2], z = 0.0;
    __int32_t n, ix;

    GET_FLOAT_WORD(ix, x);

    ix &= 0x7fffffff;
    if (ix <= 0x3f490fd8) return __kernel_cosf(x, z);

    else if (ix >= 0x7f800000) return x - x;

    else {
        n = __ieee754_rem_pio2f(x, y);
        switch (n & 3) {
        case 0: return __kernel_cosf(y[0], y[1]);
        case 1: return -__kernel_sinf(y[0], y[1], 1);
        case 2: return -__kernel_cosf(y[0], y[1]);
        default:
            return __kernel_sinf(y[0], y[1], 1);
        }
    }
}
