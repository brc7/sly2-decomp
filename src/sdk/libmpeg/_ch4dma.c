// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { unsigned int n; unsigned int addr; } BUF;
#define REG(a) (*(volatile unsigned int *)(a))
static __inline__ void EI(void) { __asm__ volatile("sync\n\tei"); }
int _ch4dma(void *mp, BUF *b)
{
    unsigned int qwc = 0xffff;
    REG(0x1000E010) = 0x10;
    if (b->n == 0) {
        EI();
        return 1;
    }
    if (b->n > qwc) {
        REG(0x1000B410) = b->addr;
        REG(0x1000B420) = qwc;
        REG(0x1000B400) = 0x101;
        b->addr = (b->addr + qwc * 16) & 0x0fffffff;
        b->n -= qwc;
    } else {
        REG(0x1000B410) = b->addr;
        REG(0x1000B420) = b->n;
        REG(0x1000B400) = 0x101;
        b->n = 0;
    }
    EI();
    return 0;
}
