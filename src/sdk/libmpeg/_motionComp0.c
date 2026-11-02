// (Claude agent) libmpeg _motionComp0: intra-path pointer setup in do{}while(0) (loop-depth ref weighting fixes s-reg priority),
// non-intra pointers set before _getAllRefs; first IPU wait is a goto loop (unpadded, address rematerialised).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int x0; int p4[3]; int x10; } FRM;
typedef struct {
    char p0[0x12C]; int x12C;
    char p130[0x13C - 0x130]; int mb_width;
    char p140[0x184 - 0x140]; int picture_structure;
    char p188[0x1D0 - 0x188]; FRM *x1D0;
    char p1D4[0x1E0 - 0x1D4]; FRM *x1E0;
    char p1E4[0x1F0 - 0x1E4]; FRM *x1F0;
    char p1F4[0x6C8 - 0x1F4]; int x6C8; int x6CC; int x6D0; int x6D4; int x6D8;
    char p6DC[0x820 - 0x6DC]; int x820;
} D;
#define R(a) (*(volatile unsigned int *)(a))
#define MB(p) (*(int *)((char *)(p) + d->x820 * 0x140))
void _sceMpegError1(D *d, const char *msg, int v);
void _getAllRefs(D *d, int x, int y, int mbtype, int mtype);
void dmaRefImage(D *d);
extern const char D_002AC570[];

int _motionComp0(D *d, int addr, int fs, int mbtype, int mtype)
{
    int *p0, *p8, *pC, *p10;
    int intra = mbtype & 1;
    int y = addr / d->mb_width;
    int x = addr % d->mb_width;
    int x16 = x << 4;
    int y16 = y << 4;
    FRM *f;

    if (intra) {
        int i = d->x820;
        do { p10 = &d->x6D8; pC = &d->x6D4; p8 = &d->x6D0; p0 = &d->x6C8; } while (0);
    wait:
        if ((R(0x1000D400) >> 8) & 1) goto wait;
        *(int *)((char *)p10 + i * 0x140) = 0;
    } else {
        if (mtype - 1 >= 3u) {
            _sceMpegError1(d, D_002AC570, mtype);
            d->x12C = 1;
            return 0;
        }
        p10 = &d->x6D8; pC = &d->x6D4; p8 = &d->x6D0; p0 = &d->x6C8;
        _getAllRefs(d, x16, y16, mbtype, mtype);
        while ((R(0x1000D400) >> 8) & 1) ;
        dmaRefImage(d);
        MB(p10) = 1;
    }
    if (fs == 1 && (mbtype & 2))
        MB(pC) = fs;
    else
        MB(pC) = 0;
    MB(p8) = intra;
    if (d->picture_structure == 3) {
        MB(p0) = d->x1D0->x0 + (x * d->x1D0->x10 + y) * 0x180;
    } else {
        if (d->picture_structure == 2)
            f = d->x1F0;
        else
            f = d->x1E0;
        MB(p0) = f->x0 + (x * f->x10 + y) * 0x180;
    }
    return 1;
}
