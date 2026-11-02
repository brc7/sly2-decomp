// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int _sceCd_mode; extern int _sceCd_readpos;
int sceCdGetReadPos(void)
{
    if (_sceCd_mode != 1) return 0;
    return *(int *)((unsigned)&_sceCd_readpos | 0x20000000);
}
