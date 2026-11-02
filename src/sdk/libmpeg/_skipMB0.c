// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p0[0x6DC]; int skipped; char p6E0[0x140 - 0x6E0 + 0x6DC]; } MBX;
typedef struct {
    char p0[0x160]; int picture_coding_type;
    char p164[0x184 - 0x164]; int picture_structure;
    char p188[0x1C0 - 0x188]; int x1C0;
    char p1C4[0x820 - 0x1C4]; int idx;
} D;
void _sceMpegError(D *d, const char *msg);
int _skipMB0(D *d, int *pmv, int *mvfs, int *mvs, int *flags)
{
    int r = 1;

    *(int *)((char *)d + d->idx * 0x140 + 0x6DC) = 1;
    d->x1C0 = 1;
    if (d->picture_coding_type == 2) {
        pmv[5] = 0;
        pmv[4] = 0;
        pmv[1] = 0;
        pmv[0] = 0;
    }
    if (d->picture_structure == 3) {
        *mvfs = 2;
    } else {
        *mvfs = 1;
        mvs[0] = mvs[1] = d->picture_structure == 2;
    }
    if (d->picture_coding_type == 1) {
        _sceMpegError(d, "skiped macroblock in I picure is not allowed");
        r = 0;
    }
    *flags &= ~1;
    return r;
}
