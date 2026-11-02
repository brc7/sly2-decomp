// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
int func_00239450(void);
int func_0023A420(int, int, int, int);
int func_002394A0(void);
int SetTimerHandler(int id, int a, int b, int c)
{
    int st = func_00239450();
    int r = func_0023A420(id, a, b, c);
    if (st) func_002394A0();
    return r;
}
