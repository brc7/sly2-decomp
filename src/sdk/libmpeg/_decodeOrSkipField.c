// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int type; int pad[7]; } sceMpegCbData;
typedef int (*sceMpegCallback)(void *mp, sceMpegCbData *cbd, void *data);
typedef struct { sceMpegCallback func; void *data; void *gp; } CB;
typedef struct {
    int x0; int x4; int x8; char pC[0xC0 - 0xC]; int xC0; char pC4[0xE8 - 0xC4]; int xE8;
    char pEC[0x128 - 0xEC]; int x128; char p12C[0x130 - 0x12C]; int second_field;
    char p134[0x184 - 0x134]; int picture_structure;
    char p188[0x878 - 0x188]; int stop;
} D;
typedef struct { int x0, x4, x8; char p0[0x40 - 0xC]; D *sys; } sceMpeg;
static inline void *SetGp(void *gp)
{
    void *old;
    __asm__ volatile("move %0, $28\n\tmove $28, %1" : "=&r"(old) : "r"(gp) : "memory");
    return old;
}
static inline int _sceMpegDispatchMpegCbNodata(sceMpeg *mp)
{
    sceMpegCbData cbd;
    CB *t;
    sceMpegCallback f;
    cbd.type = 1;
    if (mp && (t = ((CB **)mp)[16]) && (f = (t + cbd.type)[1].func)) {
        void *old = SetGp(((void **)(t + cbd.type))[5]);
        f(mp, &cbd, (t + cbd.type)[1].data);
        SetGp(old);
    }
    return 1;
}
int _updateRefImage(D *d, int second);
int _decPicture(D *d);
void _sceMpegOutputFrame(D *d, int a, int b);
int _sceMpegNextHeader(D *d);
void func_0024C360(sceMpeg *mp);
int _decodeOrSkipField(sceMpeg *mp, int n, int limit)
{
    D *d = mp->sys;
    int decode = 0;
    int r, ok, one, exp;

    d->second_field = 0;
    if (limit == -1 || n < limit)
        decode = 1;
    if (d->x8 == 0) {
        mp->x8 = 0;
        d->x8 = 1;
    }
    if (_updateRefImage(d, 0) && decode)
        _decPicture(d);
    if (d->stop)
        return 0;
    one = 1;
    d->second_field = one;
    if (!_sceMpegNextHeader(d)) {
        func_0024C360(mp);
        d->x0 = one;
        return 0;
    }
    exp = 2;
    if (d->xE8 ^ 1)
        exp = one;
    if (d->picture_structure != exp)
        return -1;
    ok = 0;
    if (_updateRefImage(d, 1))
        ok = one;
    r = 0;
    if (ok && decode) {
        if (_decPicture(d))
            r = one;
    }
    if (d->stop)
        return 0;
    _sceMpegOutputFrame(d, d->x128, d->x4);
    d->second_field = 0;
    mp->x8 = d->x128 - d->xC0;
    d->x128++;
    d->x4++;
    if (!decode)
        _sceMpegDispatchMpegCbNodata(mp);
    return r;
}
