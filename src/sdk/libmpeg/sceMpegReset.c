// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
typedef struct { int x0, x4, x8; char p0[0x98 - 0xC]; int x98; char p1[0xC0 - 0x9C]; int xC0; char p2[0x128 - 0xC4]; int x128; char p3[0x858 - 0x12C]; int x858; char p4[0x878 - 0x85C]; int x878; } D;
typedef struct { int x0, x4, x8; char p0[0x40 - 0xC]; D *sys; } sceMpeg;
void func_0024AC10(D *);
int sceMpegReset(sceMpeg *mp)
{
    D *d = mp->sys;
    d->x878 = 0;
    d->x0 = 0;
    d->x4 = 0;
    d->x8 = 0;
    mp->x8 = 0;
    d->xC0 = 0;
    d->x98 = -1;
    func_0024AC10(d);
    d->x128 = 0;
    d->x858 = 0;
    *(volatile unsigned int *)0x10002010 = (*(volatile unsigned int *)0x10002010 & 0xFF7FFFFF) | 0x800000;
    return 1;
}
