// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char p0[0x12C]; int err;
    char p130[0x828 - 0x130]; int x828; unsigned int cmd;
    char p830[0x848 - 0x830]; int x848; int x84C;
} D;
#define R(a) (*(volatile unsigned int *)(a))
void _waitIpuIdle(D *d);
long _sceMpegWaitIpuIdle64(D *d);
unsigned int _sceMpegNextBit(D *d, int n);

static inline void _setCmd(unsigned int c) { R(0x10002000) = c; }

#define _getVlc(d, c) ({                                  \
    long r_;                                              \
    unsigned int bp_;                                     \
    long top_;                                            \
    _waitIpuIdle(d);                                      \
    _setCmd(c);                                           \
    (d)->x828 = 0;                                        \
    (d)->cmd = 0x30000000;                                \
    r_ = _sceMpegWaitIpuIdle64(d);                        \
    bp_ = R(0x10002020);                                  \
    top_ = *(volatile long *)0x10002030;                  \
    (d)->x848 = top_;                                     \
    if (top_ < 0)                                         \
        (d)->x84C = -(bp_ & 0x1F) & 0x1F;                 \
    else                                                  \
        (d)->x84C = 32;                                   \
    (d)->err = (int)r_ == 0;                              \
    (short)r_;                                            \
})

static inline void _decodeMotionVector(int *pred, int r_size, int motion_code, int motion_residual, int full_pel_vector)
{
    int lim, vec;

    lim = 16 << r_size;
    vec = full_pel_vector ? (*pred >> 1) : *pred;
    if (motion_code > 0) {
        vec += ((motion_code - 1) << r_size) + motion_residual + 1;
        if (vec >= lim)
            vec -= lim + lim;
    } else if (motion_code < 0) {
        vec -= ((-motion_code - 1) << r_size) + motion_residual + 1;
        if (vec < -lim)
            vec += lim + lim;
    }
    *pred = full_pel_vector ? (vec << 1) : vec;
}

void _motionVector(D *d, int *PMV, int *dmvector, int h_r_size, int v_r_size, int dmv, int mvscale, int full_pel_vector)
{
    int motion_code, motion_residual;

    motion_code = _getVlc(d, 0x38000000);
    if (h_r_size != 0 && motion_code != 0)
        motion_residual = _sceMpegNextBit(d, h_r_size);
    else
        motion_residual = 0;
    _decodeMotionVector(&PMV[0], h_r_size, motion_code, motion_residual, full_pel_vector);
    if (dmv)
        dmvector[0] = _getVlc(d, 0x3C000000);

    motion_code = _getVlc(d, 0x38000000);
    if (v_r_size != 0 && motion_code != 0)
        motion_residual = _sceMpegNextBit(d, v_r_size);
    else
        motion_residual = 0;
    if (mvscale)
        PMV[1] >>= 1;
    _decodeMotionVector(&PMV[1], v_r_size, motion_code, motion_residual, full_pel_vector);
    if (mvscale)
        PMV[1] <<= 1;
    if (dmv)
        dmvector[1] = _getVlc(d, 0x3C000000);
}
