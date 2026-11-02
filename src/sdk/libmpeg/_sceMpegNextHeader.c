// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int type; int pad; long pts; long dts; } CBTS;
typedef struct {
    char p0[0x160]; int picture_coding_type;
    char p164[0x838 - 0x164]; long pts; long dts;
    char p848[0x868 - 0x848]; void *mp;
    char p86C[0x878 - 0x86C]; int stop;
} D;
unsigned int _sceMpegNextBit(D *d, int n);
unsigned int _sceMpegPeepBit(D *d, int n);
void func_002544C0(D *d, int n);
void flushByteBoundary(D *d);
void _sequenceHeader(D *d);
void _groupOfPicturesHeader(D *d);
void _pictureHeader(D *d);
int _sceMpegDispatchMpegCallback(void *mp, void *cb);
int _sceMpegNextHeader(D *d)
{
    unsigned int code;
    CBTS cb;

    for (;;) {
        if (d->stop)
            return -1;
        flushByteBoundary(d);
        while (_sceMpegPeepBit(d, 24) != 1 && !d->stop)
            func_002544C0(d, 8);
        code = _sceMpegNextBit(d, 32);
        switch (code) {
        case 0x1B3:
            _sequenceHeader(d);
            break;
        case 0x1B8:
            _groupOfPicturesHeader(d);
            break;
        case 0x100:
            _pictureHeader(d);
            cb.type = 5;
            cb.pts = -1;
            cb.dts = -1;
            _sceMpegDispatchMpegCallback(d->mp, &cb);
            d->pts = cb.pts;
            d->dts = cb.dts;
            return d->picture_coding_type;
        case 0x1B7:
            return 0;
        }
    }
}
