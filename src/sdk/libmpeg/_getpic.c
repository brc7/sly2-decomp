// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* (Claude agent) The first printf call is made through a non-varargs prototype: the call RTL then differs from the
   abort printf, so jump.c does not cross-jump the two "printf; return -1" tails (the target keeps them separate). */
typedef struct {
    int x0; char p4[0xA8 - 0x4]; int xA8; int xAC; int xB0; int xB4; int xB8; int xBC;
    char pC0[0xE8 - 0xC0]; int xE8; unsigned int xEC;
    char pF0[0x184 - 0xF0]; int picture_structure;
    char p188[0x834 - 0x188]; int x834;
    char p838[0x858 - 0x838]; int mpeg1;
    char p85C[0x878 - 0x85C]; int stop;
} D;
typedef struct { char p0[0x40]; D *sys; } sceMpeg;
void _sceMpegError1(D *d, const char *fmt, unsigned int v);
int func_00236960(const char *fmt, ...);
int _sceMpegNextHeader(D *d);
int _sceMpegFlush(sceMpeg *mp);
int _decodeOrSkip(sceMpeg *mp, int n, int limit);
int _getpic(sceMpeg *mp)
{
    D *d = mp->sys;
    int r = 0;
    int type = 1;

    d->x0 = 0;
    if (d->xEC & 0x3F) {
        _sceMpegError1(d, "image buffer needs to be aligned to 64byte boundary(0x%08x)", d->xEC);
        return -1;
    }
    if (d->stop) {
        ((int (*)(const char *))func_00236960)("Need to re-setup libipu since sceMpegGetPicture was aborted\n");
        return -1;
    abort:
        func_00236960("sceMpegGetPicture is aborted\n");
        return -1;
    }
    d->x834 = 0;
    for (;;) {
        if (r != -1) {
            do {
                type = _sceMpegNextHeader(d);
                if (type < 0)
                    return -1;
            } while (type != 0 && d->picture_structure != d->xE8 && d->mpeg1);
        }
        switch (type) {
        case 0:
            _sceMpegFlush(mp);
            d->x0 = 1;
            break;
        case 1:
            d->xBC = 0;
            d->xB8 = 0;
            d->xB4 = 0;
            r = _decodeOrSkip(mp, 0, d->xA8);
            d->xB4++;
            break;
        case 2:
            r = _decodeOrSkip(mp, d->xB8, d->xAC);
            d->xB8++;
            break;
        case 3:
        case 4:
            r = _decodeOrSkip(mp, d->xBC, d->xB0);
            d->xBC++;
            break;
        }
        if (d->stop)
            goto abort;
        if (d->x834 || d->x0)
            return 1;
    }
}
