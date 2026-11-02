// (Claude agent) MATCHES only with the empty asm barriers after each 'd->x = call()' statement: emulates Sony's scheduler keeping
// the return-value consumer right after the call (gcc3 in_post_call_group_p-like). Without them 24/121 differ (sw moves into jal slot).
// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct {
    char p0[0xE8]; int xE8;
    char pEC[0x174 - 0xEC]; int f_code[4]; int picture_structure; int top_field_first;
    int frame_pred_frame_dct; int concealment_motion_vectors; int repeat_first_field; int progressive_frame;
} D;
unsigned int _sceMpegNextBit(D *d, int n);
#define IPU_CTRL (*(volatile unsigned int *)0x10002010)
void _pictureCodingExtension(D *d)
{
    unsigned int ps;

    d->f_code[0] = _sceMpegNextBit(d, 4);
    __asm__ volatile("");
    d->f_code[1] = _sceMpegNextBit(d, 4);
    __asm__ volatile("");
    d->f_code[2] = _sceMpegNextBit(d, 4);
    __asm__ volatile("");
    d->f_code[3] = _sceMpegNextBit(d, 4);
    __asm__ volatile("");
    IPU_CTRL = (IPU_CTRL & 0xFFFCFFFF) | (_sceMpegNextBit(d, 2) << 16);
    ps = _sceMpegNextBit(d, 2);
    d->picture_structure = ps;
    if (d->xE8 == 0)
        d->xE8 = ps;
    d->top_field_first = _sceMpegNextBit(d, 1);
    __asm__ volatile("");
    d->frame_pred_frame_dct = _sceMpegNextBit(d, 1);
    __asm__ volatile("");
    d->concealment_motion_vectors = _sceMpegNextBit(d, 1);
    __asm__ volatile("");
    IPU_CTRL = (IPU_CTRL & 0xFFBFFFFF) | (_sceMpegNextBit(d, 1) << 22);
    IPU_CTRL = (IPU_CTRL & 0xFFDFFFFF) | (_sceMpegNextBit(d, 1) << 21);
    IPU_CTRL = (IPU_CTRL & 0xFFEFFFFF) | (_sceMpegNextBit(d, 1) << 20);
    d->repeat_first_field = _sceMpegNextBit(d, 1);
    __asm__ volatile("");
    _sceMpegNextBit(d, 1);
    d->progressive_frame = _sceMpegNextBit(d, 1);
    __asm__ volatile("");
    if (_sceMpegNextBit(d, 1)) {
        _sceMpegNextBit(d, 1);
        _sceMpegNextBit(d, 3);
        _sceMpegNextBit(d, 1);
        _sceMpegNextBit(d, 7);
        _sceMpegNextBit(d, 8);
    }
}
