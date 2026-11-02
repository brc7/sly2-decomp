// (Claude agent) 22/79: c.eq.s->bc1f hazard nop (toolchain) + 'lui v1; daddu s1,v1' hi-copy artefact of our PRE (Sony puts lui straight into s1).
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
float __ieee754_fmodf(float x, float y);
int func_0024A0C0(float x);
int matherr(struct exception *x);
int *func_002401C8(void);
float fmodf(float x, float y)
{
    struct exception exc;
    float z;

    z = __ieee754_fmodf(x, y);
    if (__fdlib_version == -1 || func_0024A0C0(y) || func_0024A0C0(x))
        return z;
    if (y == (float)0.0) {
        exc.type = 1;
        exc.name = "fmodf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        if (__fdlib_version == 0)
            exc.retval = x;
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
