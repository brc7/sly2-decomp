// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int sceCdGetError(void);
int sceCdGetDiskType_242AB8(void)
{
    int r = sceCdGetError();
    return (r == -1) ? 0 : r;
}
