// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef int u_long128 __attribute__((mode(TI)));
extern u_long128 D_002C66C0, D_002C66D0;
void s1_sceGsResetPath(void)
{
    int tmp;
    *(volatile int *)0x10003C10 = 1;
    *(volatile int *)0x10003C20 = 2;
    __asm__ volatile(
        "move %0, $0\n"
        "sync\n"
        "cfc2 %0, $vi28\n"
        "nop\n"
        "ori %0, %0, 0x200\n"
        "ctc2 %0, $vi28\n"
        "sync.p\n"
        "1:\n"
        "cfc2 %0, $vi29\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "andi %0, %0, 0x100\n"
        "bnez %0, 1b\n"
        "addiu %0, $0, 0x404\n"
        "ctc2 %0, $vi28\n"
        "nop\n"
        : "=r"(tmp));
    *(volatile u_long128 *)0x10005000 = D_002C66C0;
    *(volatile u_long128 *)0x10005000 = D_002C66D0;
    *(volatile int *)0x10003000 = 1;
}
