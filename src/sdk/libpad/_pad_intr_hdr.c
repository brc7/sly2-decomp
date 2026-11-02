// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern int (*D_00450844)(void *);
extern void *D_00450840;
int _pad_intr_hdr(void **p)
{
    void *gp;
    int (*f)(void *) = D_00450844;
    if (f) {
        __asm__ volatile("move %0, $gp\n\tlw $gp, %1" : "=&r"(gp) : "m"(D_00450840));
        f(p[3]);
        __asm__ volatile("move $gp, %0" : : "r"(gp));
    }
    __asm__ volatile("sync.l\n\tei");
}
