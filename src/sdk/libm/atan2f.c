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
float __ieee754_atan2f(float y, float x);
int func_0024A0C0(float x);
int matherr(struct exception *x);
int *func_002401C8(void);
float atan2f(float y, float x)
{
    struct exception exc;
    float z;

    z = __ieee754_atan2f(y, x);
    if (__fdlib_version == -1 || func_0024A0C0(x) || func_0024A0C0(y))
        return z;
    if (x == (float)0.0 && y == (float)0.0) {
        exc.arg1 = (double)y;
        exc.arg2 = (double)x;
        exc.type = 1;
        exc.name = "atan2f";
        exc.err = 0;
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
