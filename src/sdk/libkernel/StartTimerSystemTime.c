// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
int func_00239450(void);
int func_002394A0(void);
void func_00239AC0(unsigned int);
int func_0023A1D0(void);
void func_00239CD8(int);
int StartTimerSystemTime(void)
{
    int st = func_00239450();
    unsigned int mode = *(volatile unsigned int *)0x10001010;
    if (mode & 0x80) {
        if (st) func_002394A0();
        return 1;
    }
    func_00239AC0((mode & 0xFFFFF3FF) | 0x80);
    func_00239CD8(func_0023A1D0());
    if (st) func_002394A0();
    return 0;
}
