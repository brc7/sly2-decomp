// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// NOTE: exact MATCH when compiled WITH delayed-branch filling + GNU as reorder (see notes/245000.md round 9); not with current gnu-reorder ().
int sceIpuSync(int mode, unsigned short timeout)
{
    int r = 0;
    switch (mode) {
    case 0:
        while ((int)*(volatile unsigned int *)0x10002010 < 0)
            ;
        r = 0;
        break;
    case 1:
        r = *(volatile unsigned int *)0x10002010 >> 31;
        break;
    }
    return r;
}
