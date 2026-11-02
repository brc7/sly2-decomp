// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
void _sceMpegWaitIpuIdle(void *mp);
void _waitIpuIdle(void *mp)
{
    if ((*(volatile unsigned int *)0x10002010 & 0x80004000) == 0x80000000)
        _sceMpegWaitIpuIdle(mp);
}
