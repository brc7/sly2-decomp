// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p0[0x828]; int x828; unsigned int x82C; char p1[0x848 - 0x830]; int x848; int x84C; } D;
void _waitIpuIdle(D *d);
long _sceMpegWaitIpuIdle64(D *d);
void _sceMpegFlushBuf(D *d, unsigned int n)
{
    _waitIpuIdle(d);
    n |= 0x40000000;
    *(volatile unsigned int *)0x10002000 = n;
    n &= 0xF0000000;
    d->x82C = n;
    if (n == 0x20000000 || n == 0x30000000 || n == 0x40000000)
        d->x828 = 0;
    else
        d->x828 = 1;
    d->x848 = _sceMpegWaitIpuIdle64(d);
    d->x84C = 32;
}
