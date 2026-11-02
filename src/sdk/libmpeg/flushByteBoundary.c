// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
void _waitIpuIdle_254488(void *d);
void func_002544C0(void *d, int n);
void flushByteBoundary(void *d)
{
    int n;
    _waitIpuIdle_254488(d);
    n = -(*(volatile unsigned int *)0x10002020 & 7) & 7;
    if (n) func_002544C0(d, n);
}
