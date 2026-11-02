// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct {
    char p0[0x12C]; int err;
    char p130[0x13C - 0x130]; int mb_width;
    char p140[0x1C0 - 0x140]; int x1C0; int qscale;
    char p1C4[0x878 - 0x1C8]; int stop;
} D;
unsigned int _sceMpegNextBit(D *d, int n);
unsigned int _sceMpegPeepBit(D *d, int n);
void func_002544C0(D *d, int n);
void func_00254620(D *d);
void flushByteBoundary(D *d);
void _sceMpegError(D *d, const char *msg);
void _sceMpegError1(D *d, const char *fmt, unsigned int v);
int _sceMpegMbAddressIncrement(D *d);
int _sceMpegSliceA0(D *d, void *unused, int *mbaddr, int *mbinc, int *pmv)
{
    unsigned int code;
    int inc;

    d->err = 0;
    flushByteBoundary(d);
    while (_sceMpegPeepBit(d, 24) != 1 && !d->stop)
        func_002544C0(d, 8);
    code = _sceMpegPeepBit(d, 32);
    if (code - 0x101 >= 0xAF) {
        _sceMpegError1(d, "slice_start_code(0x%08x) out of range", code);
        return 2;
    }
    func_00254620(d);
    d->qscale = _sceMpegNextBit(d, 5);
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegNextBit(d, 1);
        func_002544C0(d, 7);
        while (_sceMpegNextBit(d, 1))
            func_002544C0(d, 8);
    }
    inc = _sceMpegMbAddressIncrement(d);
    *mbinc = inc;
    if (!d->err) {
        *mbaddr = ((code & 0xFF) - 1) * d->mb_width + inc - 1;
        *mbinc = 1;
        d->x1C0 = 1;
        pmv[5] = 0;
        pmv[4] = 0;
        pmv[1] = 0;
        pmv[0] = 0;
        pmv[7] = 0;
        pmv[6] = 0;
        pmv[3] = 0;
        pmv[2] = 0;
    return 0;
    }
    _sceMpegError(d, "_sceMpegSliceA0(): error happens");
    return 1;
}
