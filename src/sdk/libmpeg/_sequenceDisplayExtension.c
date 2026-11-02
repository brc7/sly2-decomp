// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p0[0x154]; int matrix; int hsize; int vsize; } D;
unsigned int _sceMpegNextBit(D *d, int n);
void _sequenceDisplayExtension(D *d)
{
    _sceMpegNextBit(d, 3);
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegNextBit(d, 8);
        _sceMpegNextBit(d, 8);
        d->matrix = _sceMpegNextBit(d, 8);
    }
    d->hsize = _sceMpegNextBit(d, 14);
    ((void (*)(D *, int))_sceMpegNextBit)(d, 1);
    d->vsize = _sceMpegNextBit(d, 14);
}
