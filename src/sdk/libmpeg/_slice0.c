// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char p0[0x12C]; int err;
    char p130[0x820 - 0x130]; int x820;
    char p824[0x848 - 0x824]; int x848; int x84C;
    char p850[0x868 - 0x850]; void *mp;
    char p86C[0x878 - 0x86C]; int stop;
} D;
#define R(a) (*(volatile unsigned int *)(a))
int _sceMpegSliceA0(D *d, int n, int *mbaddr, int *mbinc, int *pmv);
void _waitIpuIdle(D *d);
int _sceMpegDispatchMpegCbNodata(void *mp);
void _sceMpegStopIpuDma(void);
void _sceMpegErrorBdec(D *d);
unsigned int _sceMpegPeepBit(D *d, int n);
int _sceMpegMbAddressIncrement(D *d);
void _sceMpegError(D *d, const char *msg);
int _decMB0(D *d, int *mbtype, int *mtype, int *dct, int *pmv, int *mvfs, int *mvs);
int _skipMB0(D *d, int *pmv, int *mtype, int *mvfs, int *mbtype);
int _motionComp0(D *d, int addr, int inc, int mbtype, int mtype, int *pmv, int *mvfs, int *mvs);
void _doMC(D *d, int f);
extern const char D_002AC5D0[];

static inline int _waitBdec(D *d)
{
    int ok;

    ok = 1;
    _waitIpuIdle(d);
    if (R(0x1000B020) && !(R(0x10002010) & 0x4000)) {
        do {
            if (R(0x1000B420) == 0 && !(R(0x1000B400) & 0x100))
                _sceMpegDispatchMpegCbNodata(d->mp);
            if (d->stop) {
                _sceMpegStopIpuDma();
                return 0;
            }
            if (R(0x1000B020) == 0)
                break;
        } while (!(R(0x10002010) & 0x4000));
    }
    {
        unsigned int bp = R(0x10002020);
        long top = *(volatile long *)0x10002030;
        int bits;
        d->x848 = top;
        if (top < 0) {
            int k = bp & 0x1F;
            bits = k ? 32 - k : 0;
        } else
            bits = 32;
        d->x84C = bits;
    }
    if (R(0x10002010) & 0x4000) {
        _sceMpegErrorBdec(d);
        ok = 0;
    }
    return ok;
}

int _slice0(D *d, int n)
{
    int pmv[8];
    int mv[2][4];
    int mbaddr, mbinc, mbtype, mtype, dct;
    int r;
    int *fs, *ms;

    mbaddr = 0;
    mbinc = 0;
    if ((r = _sceMpegSliceA0(d, n, &mbaddr, &mbinc, pmv)))
        return r;
    d->err = 0;
    for (;;) {
    if (mbaddr >= n)
        return 0;
        *(int *)((char *)d + d->x820 * 0x140 + 0x6DC) = 0;
        if (!_waitBdec(d))
        {
            if (d->stop)
                return 4;
            return 2;
        }
        if (mbinc == 0) {
            if (!_sceMpegPeepBit(d, 23) || d->err) {
                d->err = 0;
                return 3;
            }
            mbinc = _sceMpegMbAddressIncrement(d);
            if (d->err) {
                d->err = 0;
                return 1;
            }
        }
        if (mbaddr >= n) {
            _sceMpegError(d, D_002AC5D0);
            return 2;
        }
        fs = mv[0];
        if (mbinc == 1) {
            ms = mv[1];
            if (!_decMB0(d, &mbtype, &mtype, &dct, pmv, fs, ms)) {
                d->err = 0;
                return 1;
            }
        } else {
            if (!_skipMB0(d, pmv, &mtype, fs, &mbtype))
                goto err2;
            ms = mv[1];
        }
        if (!_motionComp0(d, mbaddr, mbinc, mbtype, mtype, pmv, fs, ms)) {
        err2:
            d->err = 0;
            return 2;
        }
        if (mbaddr)
            _doMC(d, d->x820 ^ 1);
        mbaddr++;
        d->x820 ^= 1;
        mbinc--;
    }
    return 0;
}
