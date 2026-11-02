// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { volatile int cnt; int done; int rem; unsigned int addr; int total; } CSC;
#define REG(a) (*(volatile unsigned int *)(a))
static __inline__ void EI(void) { __asm__ volatile("sync\n\tei"); }
int _ch3dmaCSC(void *mp, CSC *c)
{
    REG(0x1000E010) = 8;
    c->cnt++;
    if (REG(0x1000B020) != 0 || (REG(0x1000B000) & 0x100)) {
        c->done = 1;
        EI();
        return 0;
    }
    if (c->cnt < c->total - 1) {
        REG(0x1000B010) = c->addr;
        REG(0x1000B020) = 0xffc0;
        REG(0x1000B000) = 0x100;
        REG(0x10002000) = 0x700003ff;
        c->addr = (c->addr + 0xffc00) & 0x0fffffff;
    } else if (c->cnt == c->total - 1) {
        c->rem -= c->cnt * 1023;
        REG(0x1000B010) = c->addr;
        REG(0x1000B020) = c->rem << 6;
        REG(0x1000B000) = 0x100;
        REG(0x10002000) = c->rem | 0x70000000;
    }
    EI();
    return 0;
}
