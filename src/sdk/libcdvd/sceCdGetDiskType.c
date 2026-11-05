// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int sceCdGetDiskType2(void);
int sceCdGetDiskType(void)
{
    int r = sceCdGetDiskType2();
    return (r == -1) ? 0 : r;
}
