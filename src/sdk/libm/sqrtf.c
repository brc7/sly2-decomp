// (Claude agent) near miss 18/69: needs 'extern const' _LIB_VERSION (value kept in s0); remaining: hi-part copied v1->s1, and the c.lt.s->bc1f hazard nop that Sony's gas inserts before a noreorder branch (toolchain).
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
float __ieee754_sqrtf(float x);
int func_0024A0C0(float x);
int matherr(struct exception *x);
int *func_002401C8(void);
float sqrtf(float x)
{
    struct exception exc;
    float z;

    z = __ieee754_sqrtf(x);
    if (__fdlib_version == -1 || func_0024A0C0(x))
        return z;
    if (x < (float)0.0) {
        exc.type = 1;
        exc.name = "sqrtf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = (double)x;
        if (__fdlib_version == 0)
            exc.retval = 0.0;
        else
            exc.retval = 0.0 / 0.0;
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
