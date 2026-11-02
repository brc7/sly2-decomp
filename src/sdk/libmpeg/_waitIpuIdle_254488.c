// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
void func_00251E60(void *mp);
void _waitIpuIdle_254488(void *mp)
{
    if ((*(volatile unsigned int *)0x10002010 & 0x80004000) == 0x80000000)
        func_00251E60(mp);
}
