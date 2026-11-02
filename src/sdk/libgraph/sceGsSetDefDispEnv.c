// sceGsSetDefDispEnv (libgraph, 0x240C38): fill a sceGsDispEnv (PMODE/SMODE2/DISPFB/DISPLAY/BGCOLOR) for the
// current video mode (NTSC 2 / PAL 3, interlaced or not, 480p 0x50).
// The non-interlaced arms divide by w three times: gcc keeps one div but all three divide-by-zero traps; the two
// later traps print nothing (div_trap outputs "" when anti-dependent on an earlier trap) yet still gate the ox/oy
// stack loads in the scheduler, which is why the target computes h-1 right after the div (T1587).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    short sceGsInterMode;
    short sceGsOutMode;
    short sceGsFFMode;
    short sceGsVersion;
} sceGsGParam;
typedef struct {
    unsigned long pmode;
    unsigned long smode2;
    unsigned long dispfb;
    unsigned long display;
    unsigned long bgcolor;
} sceGsDispEnv;
extern char D_002AB330[];
sceGsGParam *sceGsGetGParam(void);
int checkModelVersion(void);
void func_00240C28(int, int *, int *, int *, int *);
int func_00236960(const char *, ...);
void sceGsSetDefDispEnv(sceGsDispEnv *disp, short psm, short w, short h, short dx, short dy)
{
    sceGsGParam *gp = sceGsGetGParam();
    int ox, oy, ow, oh;

    if (gp->sceGsOutMode != 2 && gp->sceGsOutMode != 3 && checkModelVersion()) {
        func_00240C28(gp->sceGsOutMode, &ox, &oy, &ow, &oh);
    } else {
        ox = oy = ow = oh = 0;
    }
    disp->pmode = 0x66;
    if (gp->sceGsInterMode != 0) {
        if (gp->sceGsFFMode)
            disp->smode2 = 3;
        else
            disp->smode2 = 1;
    } else
        disp->smode2 = 2;
    disp->dispfb = ((unsigned long)(psm & 0xF) << 15) | ((unsigned long)(((w + 63) >> 6) & 0x3F) << 9);
    if (gp->sceGsOutMode == 2) {
        if (gp->sceGsInterMode == 1) {
            int magh = (w + 0x9FF) / w;
            unsigned long yy = (unsigned long)((dy + oy + 0x32) & 0xFFF) << 12;
            long xo = (long)ox + 0x27CL;
            int mw = magh * w;
            int md = dx * magh;
            unsigned long mm = (unsigned long)(magh - 1) << 23;
            unsigned long ww = (unsigned long)(mw - 1) << 32;
            long xx = (xo + md) & 0xFFF;
            unsigned long d;

            if (gp->sceGsFFMode) {
                unsigned long mw2 = mm | ww;
                unsigned long xh = (unsigned long)xx | ((unsigned long)(h * 2 - 1) << 44);
                unsigned long t = mw2 | xh;
                d = t | yy;
            } else {
                unsigned long mw2 = mm | ww;
                unsigned long xh = (unsigned long)xx | ((unsigned long)(h - 1) << 44);
                unsigned long t = mw2 | xh;
                d = t | yy;
            }
            disp->display = d;
        } else {
            int magh = (w + 0x9FF) / w;
            unsigned long mm = (unsigned long)((w + 0x9FF) / w - 1) << 23;
            unsigned long ww = (unsigned long)(((w + 0x9FF) / w * w) - 1) << 32;
            unsigned long yy = (unsigned long)((dy + oy + 0x19) & 0xFFF) << 12;
            long xo = ox + 0x27CL;
            long xx = (xo + dx * magh) & 0xFFF;
            unsigned long mw = mm | ww;
            unsigned long xh = (unsigned long)xx | ((unsigned long)(h - 1) << 44);
            unsigned long t = mw | xh;
            disp->display = t | yy;
        }
    } else if (gp->sceGsOutMode == 3) {
        if (gp->sceGsInterMode == 1) {
            int magh = (w + 0x9FF) / w;
            unsigned long yy = (unsigned long)((dy + oy + 0x48) & 0xFFF) << 12;
            long xo = (long)ox + 0x290L;
            int mw = magh * w;
            int md = dx * magh;
            unsigned long mm = (unsigned long)(magh - 1) << 23;
            unsigned long ww = (unsigned long)(mw - 1) << 32;
            long xx = (xo + md) & 0xFFF;
            unsigned long d;

            if (gp->sceGsFFMode) {
                unsigned long mw2 = mm | ww;
                unsigned long xh = (unsigned long)xx | ((unsigned long)(h * 2 - 1) << 44);
                unsigned long t = mw2 | xh;
                d = t | yy;
            } else {
                unsigned long mw2 = mm | ww;
                unsigned long xh = (unsigned long)xx | ((unsigned long)(h - 1) << 44);
                unsigned long t = mw2 | xh;
                d = t | yy;
            }
            disp->display = d;
        } else {
            int magh = (w + 0x9FF) / w;
            unsigned long mm = (unsigned long)((w + 0x9FF) / w - 1) << 23;
            unsigned long ww = (unsigned long)(((w + 0x9FF) / w * w) - 1) << 32;
            unsigned long yy = (unsigned long)((dy + oy + 0x24) & 0xFFF) << 12;
            long xo = ox + 0x290L;
            long xx = (xo + dx * magh) & 0xFFF;
            unsigned long mw = mm | ww;
            unsigned long xh = (unsigned long)xx | ((unsigned long)(h - 1) << 44);
            unsigned long t = mw | xh;
            disp->display = t | yy;
        }
    } else if (gp->sceGsOutMode == 0x50) {
        long cx = (long)((0x2D0 - w) / 2 * 2) + ox;
        long xo = dx * 2 + 0xE8L;
        unsigned long hh = (unsigned long)(h - 1) << 44;
        unsigned long hw = hh | ((unsigned long)(w * 2 - 1) << 32);
        unsigned long xm = (unsigned long)((cx + xo) & 0xFFF) | ((unsigned long)1 << 23);
        unsigned long t = hw | xm;
        disp->display = t | ((unsigned long)((dy + oy + 0x23) & 0xFFF) << 12);
    } else {
        func_00236960(D_002AB330);
    }
    disp->bgcolor = 0;
}
