// (Claude agent) MATCHES only with the empty asm barrier after the first call (same post-call-group issue). Without: 3/81 differ.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char p0[0x160]; int type; int full_pel_fwd; int fwd_f_code; int full_pel_bwd; int bwd_f_code;
    char p174[0x1BC - 0x174]; int tr;
    char p1C0[0x85C - 0x1C0]; int base; int maxtr; int first;
} D;
unsigned int _sceMpegNextBit(D *d, int n);
void func_002544C0(D *d, int n);
int _extensionAndUserData(D *d);
void _pictureHeader(D *d)
{
    int tr;
    int f, p;
    int t;

    tr = _sceMpegNextBit(d, 10);
    __asm__ volatile ("");
    d->type = _sceMpegNextBit(d, 3);
    _sceMpegNextBit(d, 16);
    t = d->type;
    if (t == 2 || t == 3) {
        d->full_pel_fwd = _sceMpegNextBit(d, 1);
        d->fwd_f_code = _sceMpegNextBit(d, 3);
        t = d->type;
    }
    if (t == 3) {
        d->full_pel_bwd = _sceMpegNextBit(d, 1);
        d->bwd_f_code = _sceMpegNextBit(d, 3);
    }
    while (_sceMpegNextBit(d, 1))
        func_002544C0(d, 8);
    _extensionAndUserData(d);
    f = 0;
    p = 0;
    if (d->type != 3 && tr != 0) {
        if (tr < 0) f = (d->first == 0);
        d->first = 0;
        p = tr;
    }
    d->tr = d->base + tr;
    if (f && !(p < tr)) d->tr = d->base + tr + 0x400;
    d->maxtr = d->maxtr < d->tr ? d->tr : d->maxtr;
}
