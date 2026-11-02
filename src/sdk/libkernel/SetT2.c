// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void SetT2(volatile int *addr, int val)
{
    int eie, st;
    __asm__ volatile("mfc0 %0, $12" : "=r"(eie));
    eie &= 0x10000;
    if (eie) {
        do {
            __asm__ volatile("di");
            __asm__ volatile("sync.p");
            __asm__ volatile("mfc0 %0, $12" : "=r"(st));
            st &= 0x10000;
            __asm__ volatile("nop\n\tnop\n\tnop");
        } while (st);
    }
    __asm__ volatile(
        "mfc0 %0, $12\n"
        "sync.p\n"
        "ori %0, %0, 6\n"
        "xori %0, %0, 2\n"
        "or %0, %0, %1\n"
        "mtc0 %0, $12\n"
        "sync.p\n"
        "sw %3, 0(%2)\n"
        "mtc0 $31, $30\n"
        "sync.p\n"
        "eret\n"
        : "=&r"(st) : "r"(eie), "r"(addr), "r"(val));
}
