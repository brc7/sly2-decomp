// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int size1; int size2; char *dst1; char *dst2; char buf1[0x40]; char buf2[0x40]; } FixAlign;
extern int D_002C8358;
int func_00234A50(int);
void mceIntrReadFixAlign(void *data)
{
    FixAlign *p = (FixAlign *)((unsigned int)data | 0x20000000);
    int i;
    char *d, *du;
    if (p->size1) {
        d = p->dst1;
        du = (char *)((unsigned int)d | 0x20000000);
        for (i = 0; i < p->size1; i++) {
            *du++ = *d++ = p->buf1[i];
        }
    }
    if (p->size2) {
        d = p->dst2;
        du = (char *)((unsigned int)d | 0x20000000);
        for (i = 0; i < p->size2; i++) {
            *du++ = *d++ = p->buf2[i];
        }
    }
    func_00234A50(D_002C8358);
}
