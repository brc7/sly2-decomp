// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder
int DIntr(void)
{
    int eie, st;
    int res;
    __asm__ volatile("mfc0 %0, $12" : "=r"(eie));
    eie &= 0x10000;
    res = eie != 0;
    if (eie) {
        __asm__(".p2align 3");
    loop:
        __asm__ volatile("di");
        __asm__ volatile("sync.p");
        __asm__ volatile("mfc0 %0, $12" : "=r"(st));
        st &= 0x10000;
        __asm__ volatile("nop\n\tnop");
        if (st) goto loop;
        return res;
    }
    res = 0;
    __asm__ volatile("" : : "r"(eie));
    return res;
}
