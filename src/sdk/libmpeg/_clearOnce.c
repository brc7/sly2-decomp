/* libmpeg _clearOnce. The 0x70000000 scratchpad base is held in a local (Sony constant-in-local pattern). */
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
#define R(a) (*(volatile unsigned int *)(a))
typedef struct {
    char p0[0x5A0]; unsigned int x5A0; unsigned int x5A4;
    char p1[0x6E0 - 0x5A8]; unsigned int x6E0; unsigned int x6E4;
    char p2[0x820 - 0x6E8]; int x820; char p3[0xC]; unsigned int x830;
    char p4[0x87C - 0x834]; int x87C;
    char b0[0x300]; char b1[0x300]; char b2[0x300];
} MpegD;
extern char D_002AC290[];
void func_0024FA38(MpegD *, const char *);
void _clearOnce(MpegD *d)
{
    R(0x10002010) = (R(0x10002010) & ~0x800000) | 0x800000;
    if (d->x87C == 0) {
        unsigned int z = 0x70000000;
        d->x830 = 0x70003600;
        d->x5A0 = z;
        d->x5A4 = 0x70001800;
        d->x6E0 = 0x70001B00;
        d->x6E4 = 0x70003300;
        d->x820 = 0;
    } else if (d->x87C == 1) {
        d->x830 = (unsigned int)d->b2;
        d->x5A4 = ((unsigned int)d->b0 & 0x0FFFFFFF) | 0x30000000;
        d->x6E4 = ((unsigned int)d->b1 & 0x0FFFFFFF) | 0x30000000;
        d->x5A0 = 0;
        d->x6E0 = 0;
        d->x820 = 0;
    } else {
        func_0024FA38(d, D_002AC290);
    }
}
