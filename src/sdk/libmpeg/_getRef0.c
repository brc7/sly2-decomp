// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* libmpeg _getRef0 (0x2505B0): queues one motion-compensation request (luma block a[k], chroma block b[k]) on the
   current reference set d->refs[d->cur] and picks its copy routines (fnA/fnB) by half-pel flags; n++.
   Callers: _getAllRefs (src/sdk/libmpeg/_getAllRefs.c), which names the parameters.
   Field types matter for matching: the picture pointers (top/bot/base/p1/p2/addr) are pointers and the rest ints, so
   type-based aliasing lets the scheduler move the pointer stores past the int stores (store order frac/addr vs top/bot). */
typedef struct {
    int addr;               /* 0x00 */
    int frac;               /* 0x04 */
    int n1;                 /* 0x08 */
    int n2;                 /* 0x0C */
    int step;               /* 0x10 */
    unsigned char *p1;      /* 0x14 */
    unsigned char *p2;      /* 0x18 */
} MCB;
typedef struct {
    unsigned char *base;
    int x4;
    unsigned char *top[4];
    unsigned char *bot[4];
    void (*fnA[4])(MCB *);
    void (*fnB[4])(MCB *);
    MCB a[4];
    MCB b[4];
    int x128;
    int n;
    int flag;
    int pad134;
    int enable;
    int flag2;
} REFS;
typedef struct {
    char pad0[0x5A0];
    REFS refs[2];
    int cur;                /* 0x820 */
    char pad824[0x830 - 0x824];
    int x830;
    char pad834[0x87C - 0x834];
    int x87C;
} D;
typedef struct { unsigned char *addr; int x4; int x8; int xC; int w; } PIC;
extern void (*D_002C8478[])(MCB *);
extern void (*D_002C8498[])(MCB *);

void _getRef0(D *d, PIC *ref, int sfield, int dfield, int oy, int h, int bx, int by, int dx, int dy, int fld, int avg)
{
    int ia, xh, yh, oyh;
    unsigned char *bv, *pic, *top, *bot, *base;
    int x, y, mbx, mby, idx, fy, cdx, cdy, ch, v, cmbx, cmby, k, o, c;
    MCB *ma, *mb;
    int w;

    c = d->cur;
    o = d->x830;
    k = d->refs[c].n;
    x = (dx >> 1) + bx;
    ma = &d->refs[c].a[k];
    mb = &d->refs[c].b[k];
    if (fld)
        y = ((dy >> 1) << 1) + oy + (by + sfield);
    else
        y = (dy >> 1) + oy + (by + sfield);
    pic = ref->addr;
    mbx = x >> 4;
    w = ref->w;
    mby = y >> 4;
    idx = mbx * w + mby;
    fy = y - (mby << 4);
    top = pic + idx * 0x180;
    bot = (idx + w) * 0x180 + pic;
    d->refs[c].top[k] = top;
    xh = dx & 1;
    yh = dy & 1;
    d->refs[c].bot[k] = bot;
    ma->frac = x - (mbx << 4);
    ma->addr = o + ((dfield + oy) << 5);
    if (yh) {
        if (fy + (h << fld) >= 16) {
            int n1 = (16 >> fld) - (fy >> fld) - 1;
            ma->n1 = n1;
            ma->n2 = h - n1;
        } else {
            ma->n1 = h;
            ma->n2 = 0;
        }
    } else {
        if (fy + (h << fld) >= 17) {
            int n1 = (16 >> fld) - (fy >> fld);
            ma->n1 = n1;
            ma->n2 = h - n1;
        } else {
            ma->n1 = h;
            ma->n2 = 0;
        }
    }
    if (d->x87C == 0) {
        base = d->refs[d->cur].base + k * 0x600;
        ma->p1 = base + (fy << 4);
        ma->p2 = base + ((fy << 4) + 0x300);
    } else {
        base = top;
        ma->p1 = top + (fy << 4);
        ma->p2 = bot + (fy << 4);
    }
    ma->step = 16 << fld;

    /* chroma: half the motion vector and block height */
    cdx = dx / 2;
    ia = (avg << 2) | (xh << 1) | yh;
    x = (cdx >> 1) + (bx >> 1);
    oyh = oy >> 1;

    ch = h >> 1;
    cdy = dy / 2;
    if (fld)
        y = ((cdy >> 1) << 1) + oyh + ((by >> 1) + sfield);
    else
        y = (cdy >> 1) + oyh + ((by >> 1) + sfield);
    xh = cdx & 1;
    cmby = y >> 3;
    cmbx = x >> 3;
    yh = cdy & 1;
    mb->frac = x - cmbx * 8;
    mb->addr = o + 0x200 + ((dfield + oyh) << 4);
    fy = y - (cmby << 3);
    if (yh) {
        if (fy + (ch << fld) >= 8) {
            int n1 = (8 >> fld) - (fy >> fld) - 1;
            mb->n1 = n1;
            mb->n2 = ch - n1;
        } else {
            mb->n1 = ch;
            mb->n2 = 0;
        }
    } else {
        if (fy + (ch << fld) >= 9) {
            int n1 = (8 >> fld) - (fy >> fld);
            mb->n1 = n1;
            mb->n2 = ch - n1;
        } else {
            mb->n1 = ch;
            mb->n2 = 0;
        }
    }
    v = (((cmbx - mbx) << 1) + (cmby - mby)) * 0x180;
    bv = base + v;
    if (d->x87C == 0) {
        int e1, e2;
        e1 = (fy << 3) + 0x100;
        e2 = (fy << 3) + 0x400;
        mb->p1 = bv + e1;
        mb->p2 = bv + e2;
    } else {
        int e1, e2;
        e1 = (fy << 3) + 0x100;
        if ((unsigned int)(v + e1) < 0x301)
            mb->p1 = (top + v) + e1;
        else
            mb->p1 = ((bot + v) + e1) - 0x300;
        e2 = (fy << 3) + 0x100;
        mb->p2 = (bot + v) + e2;
    }
    mb->step = 8 << fld;
    {
        int c2 = d->cur;
        d->refs[c2].fnA[k] = D_002C8478[ia];
        d->refs[c2].n++;
        d->refs[c2].fnB[k] = D_002C8498[(avg << 2) | (xh << 1) | yh];
    }
}
