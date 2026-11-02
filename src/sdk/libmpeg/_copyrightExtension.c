// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
unsigned int _sceMpegNextBit(void *d, int n);
void _copyrightExtension(void *d)
{
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 8);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 7);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 20);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 22);
    _sceMpegNextBit(d, 1);
    _sceMpegNextBit(d, 22);
}
