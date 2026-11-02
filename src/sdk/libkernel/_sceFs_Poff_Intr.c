// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { void (*func)(void *); void *arg; } PoffCb;
extern void *D_0044BE40;
void _sceFs_Poff_Intr(void *pkt, PoffCb *cb)
{
    void (*f)(void *) = cb->func;
    if (f) {
        void *gp;
        __asm__ volatile("move %0, $gp\n\tlw $gp, %1" : "=&r"(gp) : "m"(D_0044BE40));
        f(cb->arg);
        __asm__ volatile("move $gp, %0" : : "r"(gp));
    }
    __asm__ volatile("sync");
    __asm__ volatile("ei");
}
