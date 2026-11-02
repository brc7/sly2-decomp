// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
/* libm finite (double) */
typedef union { double value; struct { unsigned int lsw; unsigned int msw; } parts; } ieee_double_shape_type;
#define GET_HIGH_WORD(i,d) do { ieee_double_shape_type gh_u; gh_u.value = (d); (i) = gh_u.parts.msw; } while (0)
int func_00249C60(double x)
{
    int hx;
    GET_HIGH_WORD(hx, x);
    return (int)((unsigned int)((hx & 0x7fffffff) - 0x7ff00000) >> 31);
}
