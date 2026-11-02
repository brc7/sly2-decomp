// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p0[0x828]; int x828; unsigned int x82C; char p1[0x848 - 0x830]; unsigned int x848; int x84C; } D;
void _waitIpuIdle(D *d);
long _sceMpegWaitIpuIdle64(D *d);
unsigned int _sceMpegPeepBit(D *d, int n)
{
    if (d->x828 || d->x84C < n) {
        _waitIpuIdle(d);
        *(volatile unsigned int *)0x10002000 = 0x40000000;
        d->x828 = 0;
        d->x82C = 0x40000000;
        d->x848 = _sceMpegWaitIpuIdle64(d);
        d->x84C = 32;
    }
    return d->x848 >> (32 - n);
}
