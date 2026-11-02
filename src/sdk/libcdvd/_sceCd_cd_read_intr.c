// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int size1; int size2; char *dst1; char *dst2; char buf1[0x40]; char buf2[0x40]; } FixAlign;
extern char D_002C67A0[];
void func_002418D0(void *);
void _sceCd_cd_read_intr(void *data)
{
    FixAlign *p = (FixAlign *)((unsigned int)data | 0x20000000);
    int i;
    if (p->size1 > 0) {
        char *d = p->dst1;
        char *du = (char *)((unsigned int)d | 0x20000000);
        for (i = 0; i < p->size1; i++) {
            d[i] = du[i] = p->buf1[i];
        }
    }
    if (p->size2 > 0) {
        char *d = p->dst2;
        char *du = (char *)((unsigned int)d | 0x20000000);
        for (i = 0; i < p->size2; i++) {
            d[i] = du[i] = p->buf2[i];
        }
    }
    func_002418D0(D_002C67A0);
}
