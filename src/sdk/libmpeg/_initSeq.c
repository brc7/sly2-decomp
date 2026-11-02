// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int addr; int w; int h; int mbw; int mbh; char p14[0x68 - 0x14]; } FB;
typedef struct { int x0[4]; } HEAP;
typedef struct {
    char p0[0x10C]; int x10C; int x110; int x114; HEAP heap;
    char p128[0x134 - 0x128]; int hsize; int vsize; int mb_width; int mb_height;
    char p144[0x14C - 0x144]; int progressive_sequence; int x150; int x154;
    char p158[0x184 - 0x158]; int picture_structure; int x188; int x18C;
    char p190[0x198 - 0x190]; int x198;
    char p19C[0x1F8 - 0x19C]; FB fb[9];
    char p5A0[0x858 - 0x5A0]; int mpeg2;
    char p85C[0x87C - 0x85C]; int x87C;
} D;
typedef struct { int w; int h; char p8[0x40 - 8]; D *d; } SEQ;
void _sceMpegHeapInit(HEAP *h);
int _sceMpegHeapAlloc(D *d, HEAP *h, unsigned int size, int align);

static inline void _setFB(FB *f, int w, int h)
{
    f->w = w;
    f->h = h;
    f->mbw = w >> 4;
    f->mbh = h >> 4;
}

void _initSeq(SEQ *s)
{
    D *d = s->d;
    int m2 = d->mpeg2;
    int w, h, mbh, t;
    unsigned int size;

    if (!m2) {
        d->picture_structure = 3;
        d->progressive_sequence = 1;
        d->x150 = 1;
        d->x198 = 1;
        d->x18C = 1;
        d->x154 = 5;
    }
    d->mb_width = (d->hsize + 15) >> 4;
    if (m2 && !d->progressive_sequence)
        mbh = ((d->vsize + 31) >> 5) << 1;
    else
        mbh = (d->vsize + 15) >> 4;
    d->mb_height = mbh;
    h = mbh << 4;
    w = d->mb_width << 4;
    if (w == s->w && h == s->h)
        return;
    s->w = w;
    s->h = h;
    t = h * 0x180;
    size = (unsigned int)(w * t) >> 8;
    _sceMpegHeapInit(&d->heap);
    d->x10C = _sceMpegHeapAlloc(d, &d->heap, size, 64);
    __asm__ volatile(""); /* Sony scheduler keeps the result store next to the call */
    d->x110 = _sceMpegHeapAlloc(d, &d->heap, size, 64);
    __asm__ volatile(""); /* Sony scheduler keeps the result store next to the call */
    d->x114 = _sceMpegHeapAlloc(d, &d->heap, size, 64);
    if (d->x87C == 0) {
        int a = (d->x10C & 0x0FFFFFFF) | 0x20000000;
        int b = (d->x110 & 0x0FFFFFFF) | 0x20000000;
        int c = (d->x114 & 0x0FFFFFFF) | 0x20000000;
        int off = s->w * s->h / 512 * 0x180;
        d->fb[0].addr = a;
        d->fb[1].addr = b;
        d->fb[2].addr = c;
        d->fb[3].addr = a;
        d->fb[4].addr = b;
        d->fb[5].addr = c;
        d->fb[6].addr = a + off;
        d->fb[7].addr = b + off;
        d->fb[8].addr = c + off;
    } else if (d->x87C == 1) {
        int a = d->x10C;
        int b = d->x110;
        int c = d->x114;
        int off = s->w * s->h / 512 * 0x180;
        d->fb[0].addr = a;
        d->fb[1].addr = b;
        d->fb[2].addr = c;
        d->fb[3].addr = a;
        d->fb[4].addr = b;
        d->fb[5].addr = c;
        d->fb[6].addr = a + off;
        d->fb[7].addr = b + off;
        d->fb[8].addr = c + off;
    }
    _setFB(&d->fb[0], w, h);
    _setFB(&d->fb[1], w, h);
    _setFB(&d->fb[2], w, h);
    _setFB(&d->fb[3], w, h / 2);
    _setFB(&d->fb[4], w, h / 2);
    _setFB(&d->fb[5], w, h / 2);
    _setFB(&d->fb[6], w, h / 2);
    _setFB(&d->fb[7], w, h / 2);
    _setFB(&d->fb[8], w, h / 2);
}
