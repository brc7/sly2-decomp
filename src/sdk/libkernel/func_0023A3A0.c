// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
int func_00239450(void);
int func_0023A2F0(int);
int EIntr(void);
int func_0023A3A0(int id)
{
    int st = func_00239450();
    int r = func_0023A2F0(id);
    if (st) EIntr();
    return r;
}
