// (Claude agent) TOOLCHAIN near miss: MATCHES if gas does not move the 2nd insn of the li.d (ori;dsll32) expansion into the following jal delay slot (verified by adding a label after li.d in the .s).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/*
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this software is freely
 * granted, provided that this notice is preserved.
 */
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef union {
    double value;
    struct { __uint32_t lsw; __uint32_t msw; } parts;
} ieee_double_shape_type;
#define EXTRACT_WORDS(ix0,ix1,d) do { ieee_double_shape_type ew_u; ew_u.value = (d); (ix0) = ew_u.parts.msw; (ix1) = ew_u.parts.lsw; } while (0)
#define GET_HIGH_WORD(i,d) do { ieee_double_shape_type gh_u; gh_u.value = (d); (i) = gh_u.parts.msw; } while (0)
#define SET_HIGH_WORD(d,v) do { ieee_double_shape_type sh_u; sh_u.value = (d); sh_u.parts.msw = (v); (d) = sh_u.value; } while (0)
double func_0024A738(double, double);
static const double
two54 = 1.80143985094819840000e+16,
twom54 = 5.55111512312578270212e-17,
huge = 1.0e+300,
tiny = 1.0e-300;
double func_00249EE0(double x, int n)
{
    __int32_t k, hx, lx;
    EXTRACT_WORDS(hx, lx, x);
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7fffffff)) == 0) return x;
        x *= two54;
        GET_HIGH_WORD(hx, x);
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000) return tiny * x;
    }
    if (k == 0x7ff) return x + x;
    k = k + n;
    if (k > 0x7fe) return huge * func_0024A738(huge, x);
    if (k > 0) { SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20)); return x; }
    if (k <= -54) {
        if (n > 50000)
            return huge * func_0024A738(huge, x);
        else
            return tiny * func_0024A738(tiny, x);
    }
    k += 54;
    SET_HIGH_WORD(x, (hx & 0x800fffff) | (k << 20));
    return x * twom54;
}
