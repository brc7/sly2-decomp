// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int dpcmp(double, double);
struct exception { int type; char *name; double arg1; };
int matherr(struct exception *x)
{
    int n = 0;
    if (x->arg1 != x->arg1) return 0;
    return n;
}
