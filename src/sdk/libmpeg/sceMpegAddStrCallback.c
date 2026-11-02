// (Claude agent) ee-gcc2.9; table accessed as D_002C83D8[type] (no descriptor ptr); the $gp read asm has a "memory" clobber (nStrCb reloaded).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
typedef unsigned long u_long;
typedef int (*sceMpegCallback)(void *mp, void *cbdata, void *anyData);
typedef struct { u_long id; u_long mask; } StrType;
typedef struct {
    u_long id;
    u_long mask;
    sceMpegCallback func;
    void *data;
    void *gp;
    int pad;
} StrCb;
typedef struct {
    char pad[0x60];
    StrCb *strCb;
    int nStrCb;
} MpegSys;
typedef struct { char pad[0x40]; MpegSys *sys; } sceMpeg;
extern StrType D_002C83D8[];


sceMpegCallback sceMpegAddStrCallback(sceMpeg *mp, unsigned int type, int ch, sceMpegCallback func, void *data)
{
    MpegSys *sys = mp->sys;
    StrCb *cb = sys->strCb;
    sceMpegCallback old = 0;
    u_long x = 0, id;
    int i;
    void *gp;
    int shift;
    int n;

    if (type < 10) {
        switch (D_002C83D8[type].mask) {
        case 0xFFFFFFFFFFUL: shift = 0; break;
        case 0xFFFF000000UL: shift = 24; break;
        case 0xFF00000000UL: shift = 32; break;
        default: id = 0; goto search;
        }
        x = D_002C83D8[type].id | ((u_long)ch << shift);
    }
    id = x;
search:
    n = sys->nStrCb;
    for (i = 0; i < n; i++) {
        if (id == cb[i].id) {
            old = cb[i].func;
            break;
        }
    }
    if (i < 64) {
        cb[i].id = id;
        cb[i].func = func;
        cb[i].mask = D_002C83D8[type].mask;
        cb[i].data = data;
        __asm__ volatile ("move %0, $gp" : "=r"(gp) : : "memory");
        cb[i].gp = gp;
        sys->nStrCb++;
    }
    return old;
}
