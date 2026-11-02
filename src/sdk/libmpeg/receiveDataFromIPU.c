// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
int DIntr(void);
int EIntr(void);
void receiveDataFromIPU(unsigned int addr, int size)
{
    int r = DIntr();
    if ((addr >> 28) == 7) {
        addr &= 0x0FFFFFFF;
        addr |= 0x80000000;
    } else
        addr &= 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000B010 = addr;
    *(volatile unsigned int *)0x1000B020 = size >> 4;
    *(volatile unsigned int *)0x1000B000 = 0x100;
    if (r) EIntr();
}
