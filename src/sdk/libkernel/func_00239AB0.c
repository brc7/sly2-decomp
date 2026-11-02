// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
void SetT2(volatile int *addr, int val);
void func_00239AB0(int val)
{
    SetT2((volatile int *)0xB0001000, val);
}
