// (Claude agent) only diff: Sony gas nop between c.eq/c.lt.s and bc1f (libm FPU compare hazard, toolchain); otherwise exact.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    int err;
};
extern const int __fdlib_version;
float __ieee754_asinf(float x);
int func_0024A0C0(float x) __asm__("isnanf");
float func_00244EF8(float x) __asm__("fabsf");
int matherr(struct exception *x);
int *func_002401C8(void);
float asinf(float x)
{
    struct exception exc;
    float z;

    z = __ieee754_asinf(x);
    if (__fdlib_version == -1 || func_0024A0C0(x))
        return z;
    if (func_00244EF8(x) > (float)1.0) {
        exc.type = 1;
        exc.name = "asinf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = (double)x;
        exc.retval = 0.0;
        if (__fdlib_version == 2)
            *func_002401C8() = 33;
        else if (!matherr(&exc)) {
            *func_002401C8() = 33;
        }
        if (exc.err != 0)
            *func_002401C8() = exc.err;
        return (float)exc.retval;
    } else
        return z;
}

/* Semantics a port must keep (docs/guides/portability.md, "libm"): this is newlib's fdlibm wrapper, and the game's
   __fdlib_version is 1 (_POSIX_), not _IEEE_ (-1): for |x| > 1 the wrapper calls matherr and returns exc.retval,
   0.0, where a host libm returns NaN. The game passes normalised dot products that round just past 1 (vec.cpp
   AngleBetweenVectors). A NaN argument returns __ieee754_asinf's result (NaN). */
