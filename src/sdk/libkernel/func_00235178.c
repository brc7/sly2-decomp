// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
int func_00239450(void);
int EIntr(void);
int func_00234760(int);
int func_00235178(int ch)
{
    int st;
    int r;
    __asm__ volatile("mfc0 %0, $12" : "=r"(st));
    st &= 0x10000;
    if (st) func_00239450();
    r = func_00234760(ch);
    __asm__ volatile("sync");
    if (st) EIntr();
    return r;
}
