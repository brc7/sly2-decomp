// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// (Claude agent) near miss 10/171: only the cb.type store vs 'a1 = &cb' order before the two inlined Dispatch(type=2) calls (same as _sceMpegErrorBdec).
typedef struct { int type; int pad[7]; } CB;
typedef struct {
    char p0[0xE8]; int xE8;
    char pEC[0x134 - 0xEC]; int hsize; int vsize;
    char p13C[0x144 - 0x13C]; int bit_rate; int vbv_size;
    char p14C[0x828 - 0x14C]; int x828; unsigned int cmd;
    char p830[0x850 - 0x830]; int load_intra; int load_nonintra;
    char p858[0x868 - 0x858]; void *mp;
} D;
#define R(a) (*(volatile unsigned int *)(a))
extern unsigned char D_002C84C0[];
extern unsigned char D_002C8500[];
unsigned int _sceMpegNextBit(D *d, int n);
void _sceMpegError(D *d, const char *msg);
int _sceMpegDispatchMpegCallback(void *mp, CB *cb);
void _waitIpuIdle(D *d);
int DIntr(void);
int func_002394A0(void);
void _extensionAndUserData(D *d);
void _initSeq(void *mp);
static inline void _loadQM(D *d, unsigned char *qm, unsigned int cmd)
{
    CB cb;
    int e;

    cb.type = 2;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
    _waitIpuIdle(d);
    R(0x10002000) = 0;
    _waitIpuIdle(d);
    e = DIntr();
    R(0x1000B410) = (unsigned int)qm & 0x0FFFFFFF;
    R(0x1000B420) = 4;
    R(0x1000B400) = 0x101;
    if (e)
        func_002394A0();
    R(0x10002000) = cmd;
    d->cmd = cmd & 0xF0000000;
    d->x828 = 1;
    _waitIpuIdle(d);
    cb.type = 3;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
}
void _sequenceHeader(D *d)
{
    unsigned int v;
    int vs;

    d->xE8 = 0;
    v = _sceMpegNextBit(d, 32);
    vs = (v >> 8) & 0xFFF;
    d->hsize = v >> 20;
    d->vsize = vs;
    if (vs > 2800)
        _sceMpegError(d, "vertical size > 2800");
    v = _sceMpegNextBit(d, 30);
    d->bit_rate = v >> 12;
    d->vbv_size = (v >> 1) & 0x3FF;
    if ((d->load_intra = _sceMpegNextBit(d, 1))) {
        _waitIpuIdle(d);
        R(0x10002000) = 0x50000000;
        d->cmd = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle(d);
    } else {
        _loadQM(d, D_002C84C0, 0x50000000);
    }
    if ((d->load_nonintra = _sceMpegNextBit(d, 1))) {
        _waitIpuIdle(d);
        R(0x10002000) = 0x58000000;
        d->cmd = 0x50000000;
        d->x828 = 1;
        _waitIpuIdle(d);
    } else {
        _loadQM(d, D_002C8500, 0x58000000);
    }
    _extensionAndUserData(d);
    _initSeq(d->mp);
}
