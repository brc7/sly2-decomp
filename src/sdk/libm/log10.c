// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// (Claude agent) -Os: only diff = DOMAIN block 'li a0,2' vs ours 'sw v1 type; li v1,2' (sched/regalloc tie, ~4 insns). -O2 has lui+daddu hi-copy.
struct exception {
    int type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    int err;
};
extern const int __fdlib_version;
extern double __infinity_d;
double func_00246178(double x);
int func_00249C80(double x);
int matherr(struct exception *x);
int *func_002401C8(void);
double log10(double x)
{
    double z;
    struct exception exc;
    z = func_00246178(x);
    if (__fdlib_version == -1 || func_00249C80(x)) return z;
    if (x <= 0.0) {
        exc.name = "log10";
        exc.err = 0;
        exc.arg1 = x;
        exc.arg2 = x;
        if (__fdlib_version == 0)
            exc.retval = -3.40282346638528859812e+38;
        else
            exc.retval = -__infinity_d;
        if (x == 0.0) {
            exc.type = 2;
            if (__fdlib_version == 2)
                *func_002401C8() = 34;
            else if (!matherr(&exc)) {
                *func_002401C8() = 33;
            }
        } else {
            exc.type = 1;
            if (__fdlib_version == 2)
                *func_002401C8() = 33;
            else if (!matherr(&exc)) {
                *func_002401C8() = 33;
            }
        }
        if (exc.err != 0)
            *func_002401C8() = exc.err;
        return exc.retval;
    } else
        return z;
}
