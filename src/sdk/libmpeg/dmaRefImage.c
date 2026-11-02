// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    unsigned int sadr;
    int pad4;
    unsigned int y[4];
    unsigned int c[(0x12C - 0x18) / 4];
    int n;
    char pad130[0x140 - 0x130];
} REFS;
typedef struct {
    char pad0[0x5A0];
    REFS refs[2];
    int cur;
    char pad824[0x87C - 0x824];
    int busy;
} MP;
extern unsigned long D_00451E80[];
int DI(void);
void EI(void);
static inline unsigned long mktag(unsigned int addr, int id, int qwc)
{
    return ((unsigned long)addr << 32) | ((unsigned long)id << 28) | qwc;
}
void dmaRefImage(MP *mp)
{
    if (mp->busy == 0) {
        int i;
        int n = mp->refs[mp->cur].n;
        unsigned long *tag = (unsigned long *)(((unsigned int)D_00451E80 & 0x0FFFFFFF) | 0x20000000);
        int r;
        for (i = 0; i < n; i++) {
            tag[0] = mktag(mp->refs[mp->cur].y[i] & 0x0FFFFFFF, 3, 0x30);
            tag[2] = mktag(mp->refs[mp->cur].c[i] & 0x0FFFFFFF, (i == n - 1) ? 0 : 3, 0x30);
            tag += 4;
        }
        r = DI();
        __asm__ __volatile__("sync");
        *(volatile unsigned int *)0x1000D480 = mp->refs[mp->cur].sadr;
        *(volatile unsigned int *)0x1000D430 = (unsigned int)D_00451E80;
        *(volatile unsigned int *)0x1000D420 = 0;
        *(volatile unsigned int *)0x1000D400 = 0x105;
        if (r) EI();
    }
}
