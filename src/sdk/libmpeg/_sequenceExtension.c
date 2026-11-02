// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char p0[0x134]; int hsize; int vsize;
    char p13C[0x144 - 0x13C]; int bit_rate; int vbv_size; int progressive_sequence; int chroma_format;
    char p154[0x858 - 0x154]; int mpeg2;
} D;
unsigned int _sceMpegNextBit(D *d, int n);
void _sceMpegError(D *d, const char *msg);
void _sequenceExtension(D *d)
{
    unsigned int v;
    unsigned int profile;
    unsigned int hext, vext, brext, vbvext;

    d->mpeg2 = 1;
    *(volatile unsigned int *)0x10002010 &= 0xFF7FFFFF;
    v = _sceMpegNextBit(d, 28);
    brext = (v >> 1) & 0xFFF;
    vext = (v >> 13) & 3;
    hext = (v >> 15) & 3;
    d->chroma_format = (v >> 17) & 3;
    if (d->chroma_format != 1)
        _sceMpegError(d, "_chroma_format needs to be 1: 420");
    d->progressive_sequence = (v >> 19) & 1;
    profile = v >> 20;
    vbvext = _sceMpegNextBit(d, 16) >> 8;
    if (profile != 0x48 && profile != 0x58 && profile != 0x44)
        _sceMpegError(d, "Unsupported profile/level");
    d->hsize = (hext << 12) | (d->hsize & 0xFFF);
    d->vsize = (vext << 12) | (d->vsize & 0xFFF);
    d->bit_rate += brext << 18;
    d->vbv_size += vbvext << 10;
}
