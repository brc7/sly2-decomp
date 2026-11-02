// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int func_002403D8(void)
{
    int ret;
    int vpu;
    do {
        ret = 0;
        if (*(volatile int *)0x10009000 & 0x100) ret = 1;
        if (*(volatile int *)0x1000A000 & 0x100) ret |= 2;
        if (*(volatile int *)0x10003C00 & 3) ret |= 4;
        __asm__ volatile("cfc2 %0, $vi29" : "=r"(vpu));
        if (vpu & 0x100) ret |= 8;
        if (*(volatile int *)0x10003020 & 0xC00) ret |= 0x10;
    } while (ret);
    return 0;
}
