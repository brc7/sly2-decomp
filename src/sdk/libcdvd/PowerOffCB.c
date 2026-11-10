// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern int D_002C6764;
extern volatile int D_002C6788;
int func_00239450(void);
int func_0023ADA8(int, void *, int);
int EIntr(void);
void _sceCd_Poff_Intr(void);
int PowerOffCB(void)
{
    int e;
    D_002C6764 = 1;
    e = func_00239450();
    func_0023ADA8(0x80000012, _sceCd_Poff_Intr, 0);
    if (e) EIntr();
    D_002C6764 = 0;
    D_002C6788 = 1;
    return 1;
}
