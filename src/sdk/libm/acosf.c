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
float __ieee754_acosf(float x);
int func_0024A0C0(float x);
float func_00244EF8(float x);
int matherr(struct exception *x);
int *func_002401C8(void);
float acosf(float x)
{
    struct exception exc;
    float z;

    z = __ieee754_acosf(x);
    if (__fdlib_version == -1 || func_0024A0C0(x))
        return z;
    if (func_00244EF8(x) > (float)1.0) {
        exc.type = 1;
        exc.name = "acosf";
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
