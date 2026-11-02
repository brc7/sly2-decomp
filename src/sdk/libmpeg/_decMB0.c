// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char p0[0x12C]; int err;
    char p130[0x160 - 0x130]; int picture_coding_type;
    int full_pel_fwd; int fwd_f_code; int full_pel_bwd; int bwd_f_code;
    int f_code[2][2];
    int picture_structure;
    char p188[0x18C - 0x188]; int frame_pred_frame_dct; int concealment;
    char p194[0x1C0 - 0x194]; int dc_reset; int qscale;
    char p1C8[0x820 - 0x1C8]; int x820;
    char p824[0x828 - 0x824]; int x828; unsigned int cmd;
    char p830[0x848 - 0x830]; int x848; int x84C;
    char p850[0x858 - 0x850]; int mpeg2;
} D;
#define R(a) (*(volatile unsigned int *)(a))
void _waitIpuIdle(D *d);
long _sceMpegWaitIpuIdle64(D *d);
unsigned int _sceMpegNextBit(D *d, int n);
void _sceMpegError(D *d, const char *msg);
void func_00252390(D *d, int n);
void receiveDataFromIPU(int addr, int n);
void _motionVector(D *d, int *PMV, int *dmvector, int h_r_size, int v_r_size, int dmv, int mvscale, int full_pel_vector);
void _motionVectors(D *d, int *PMV, int *dmvector, int *mvfs, int s, int count, int fmt, int h_r_size, int v_r_size, int dmv, int mvscale);

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

static inline void ipuCmd(D *d, unsigned int cmd)
{
    R(0x10002000) = cmd;
    d->cmd = cmd & 0xF0000000;
    if ((cmd & 0xF0000000) == 0x20000000 || (cmd & 0xF0000000) == 0x30000000 || (cmd & 0xF0000000) == 0x40000000)
        d->x828 = 0;
    else
        d->x828 = 1;
}

int _decMB0(D *d, int *mbtype, int *mtype, int *dct, int *pmv, int *fs, int *ms)
{
    int count, fmt, dmv, mvscale;
    int s, t;

    R(0x10002010) = (R(0x10002010) & 0xF8FFFFFF) | (d->picture_coding_type << 24);
    *mbtype = _getVlc(d, 0x34000000);
    if (*mbtype == 0) {
        _sceMpegError(d, "Invalid macroblock_type code: 0");
        d->err = 1;
        return 0;
    }
    if (*mbtype & 0xC) {
        if (d->picture_structure == 3 && d->frame_pred_frame_dct)
            *mtype = 2;
        else
            *mtype = _sceMpegNextBit(d, 2);
    } else if ((*mbtype & 1) && d->concealment) {
        *mtype = d->picture_structure == 3 ? 2 : 1;
    }
    s = d->picture_structure;
    if (s == 3) {
        count = *mtype == 1 ? 2 : 1;
        fmt = *mtype == 2;
    } else {
        fmt = 0;
        count = *mtype == 2 ? 2 : 1;
    }
    dmv = *mtype == 3;
    mvscale = 0;
    if (!fmt)
        mvscale = s == 3;
    *dct = (s == 3 && !d->frame_pred_frame_dct && (*mbtype & 3)) ? _sceMpegNextBit(d, 1) : 0;
    if (*mbtype & 0x10)
        d->qscale = _sceMpegNextBit(d, 5);
    if ((*mbtype & 8) || ((*mbtype & 1) && d->concealment)) {
        if (d->mpeg2)
            _motionVectors(d, pmv, ms, fs, 0, count, fmt, d->f_code[0][0] - 1, d->f_code[0][1] - 1, dmv, mvscale);
        else
            _motionVector(d, pmv, ms, d->fwd_f_code - 1, d->fwd_f_code - 1, 0, 0, d->full_pel_fwd);
    }
    if (d->err)
        return 0;
    if (*mbtype & 4) {
        if (d->mpeg2)
            _motionVectors(d, pmv, ms, fs, 1, count, fmt, d->f_code[1][0] - 1, d->f_code[1][1] - 1, 0, mvscale);
        else
            _motionVector(d, pmv + 2, ms, d->bwd_f_code - 1, d->bwd_f_code - 1, 0, 0, d->full_pel_bwd);
    }
    if (d->err)
        return 0;
    if ((*mbtype & 1) && d->concealment)
        func_00252390(d, 1);
    if (*mbtype & 3) {
        receiveDataFromIPU(*(int *)((char *)d + d->x820 * 0x140 + 0x5A4), 0x300);
        _waitIpuIdle(d);
        {
            unsigned int c = (*dct << 25) | (d->dc_reset << 26);
            ipuCmd(d, ((*mbtype & 1) << 27) | c | 0x20000000 | (d->qscale << 16));
        }
    } else {
        *(int *)((char *)d + d->x820 * 0x140 + 0x6DC) = 1;
    }
    d->dc_reset = 0;
    if (d->err)
        return 0;
    if (!(*mbtype & 1))
        d->dc_reset = 1;
    if ((*mbtype & 1) && !d->concealment) {
        pmv[5] = 0;
        pmv[4] = 0;
        pmv[1] = 0;
        pmv[0] = 0;
        pmv[7] = 0;
        pmv[6] = 0;
        pmv[3] = 0;
        pmv[2] = 0;
    }
    if (d->picture_coding_type == 2 && !(*mbtype & 9)) {
        pmv[5] = 0;
        pmv[4] = 0;
        pmv[1] = 0;
        pmv[0] = 0;
        if (d->picture_structure == 3) {
            *mtype = 2;
        } else {
            *mtype = 1;
            *fs = d->picture_structure == 2;
        }
    }
    return 1;
}
