// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char pad0[0xEC];
    unsigned int dst;
    int padF0;
    int width;
    int fbw;
    char padFC[0x184 - 0xFC];
    int structure;
} MPI;
typedef struct {
    unsigned int src;
    int pad4, pad8;
    int nmb;
    int h;
} IMGI;
int DI(void);
void EI(void);
void _cpr8(MPI *mp, IMGI *img)
{
    int stride, size, qwc, nfield;
    unsigned int src = img->src & 0x0FFFFFFF;
    unsigned int dst = mp->dst & 0x0FFFFFFF;
    int f, i;

    if (mp->structure == 3 || mp->width == 0) {
        size = img->h * 0x180;
        qwc = size >> 4;
        if (mp->width)
            stride = (mp->width >> 4) * 0x180;
        else
            stride = size;
        nfield = 1;
    } else {
        stride = (mp->width >> 4) * 0xC0;
        size = (img->h >> 1) * 0x180;
        nfield = 2;
        qwc = size >> 4;
    }
    for (f = 0; f < nfield; f++) {
        unsigned int d = dst;
        for (i = 0; i < img->nmb; i++) {
            int r = DI();
            *(volatile unsigned int *)0x1000D480 = 0;
            *(volatile unsigned int *)0x1000D410 = src;
            *(volatile unsigned int *)0x1000D420 = qwc;
            *(volatile unsigned int *)0x1000D400 = 0x101;
            if (r) EI();
            while (*(volatile unsigned int *)0x1000D400 & 0x100);
            r = DI();
            *(volatile unsigned int *)0x1000D080 = 0;
            *(volatile unsigned int *)0x1000D010 = d;
            *(volatile unsigned int *)0x1000D020 = qwc;
            *(volatile unsigned int *)0x1000D000 = 0x100;
            if (r) EI();
            while (*(volatile unsigned int *)0x1000D000 & 0x100);
            while (*(volatile unsigned int *)0x1000D020);
            d += stride;
            src += size;
        }
        dst += mp->fbw * 0xC0;
    }
}
