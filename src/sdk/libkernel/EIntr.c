// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int EIntr(void)
{
    int status;
    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    status &= 0x10000;
    __asm__ volatile("ei");
    return status != 0;
}
