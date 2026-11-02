// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder-db
typedef struct {
    int width, height, frameCount;
    long pts, dts;
    unsigned long flags;
    long pts2nd, dts2nd;
    unsigned long flags2nd;
    void *sys;
} sceMpeg;
typedef struct { int (*func)(); void *data; void *gp; } CB;
#define CBS ((CB *)(d + 0xC))
#define W(o) (*(int *)((char *)d + (o)))
#define L(o) (*(long *)((char *)d + (o)))
#define P(o) (*(void **)((char *)d + (o)))
void *func_002333B4(void *, int, int);
void func_0024FA38(void *, const char *, int);
void func_0024FA90(void *, const char *, int);
void func_0024B8F8(void *, void *, int);
int func_0024B930(void *, void *, int, int);
void func_0024A908(void *);
void func_0024B590(sceMpeg *);
int func_0024B7A8(sceMpeg *);
void func_0024B910(void *);
int func_0024B9A0();
int func_0024B9C8();
extern char D_002AC2F8[], D_002AC2A8[], D_002AC2D0[];
int sceMpegCreate(sceMpeg *mp, unsigned char *work, int size)
{
    char *d;
    void *bs;
    int rem;
    int r;
    func_002333B4(work, 0, size);
    d = (char *)((((unsigned int)work + 3) >> 2) << 2);
    rem = size - (d - (char *)work);
    if (rem < 0x19A0) {
        func_0024FA38(d, D_002AC2F8, rem);
        return 0;
    }
    bs = d + 0x118;
    mp->sys = d;
    func_0024B8F8(bs, d + 0x19A0, rem - 0x19A0);
    mp->width = 0;
    mp->height = 0;
    mp->frameCount = 0;
    mp->pts = -1;
    mp->dts = -1;
    mp->flags = 0;
    mp->pts2nd = -1;
    mp->dts2nd = -1;
    mp->flags2nd = 0;
    W(0xC8) = 0; W(0xCC) = 0; W(0xD0) = 0; W(0xD4) = 0;
    W(0xD8) = 0; W(0xDC) = 0; W(0xE0) = 0; W(0xE4) = 0;
    W(0xE8) = 0; W(0xEC) = 0; W(0xF0) = 0; W(0xF4) = 0;
    W(0xF8) = 0; W(0xFC) = 0; W(0x108) = 0;
    CBS[0].func = 0; CBS[1].func = 0; CBS[4].func = 0; CBS[5].func = 0; CBS[6].func = 0;
    L(0x100) = -1;
    CBS[2].func = func_0024B9A0;
    CBS[3].func = func_0024B9C8;
    r = func_0024B930(d, bs, 0x800, 8);
    W(0x64) = 0;
    W(0x10C) = 0; W(0x110) = 0; W(0x114) = 0;
    W(0x8C) = 0;
    L(0x90) = 0;
    W(0x98) = -1;
    L(0xA0) = 0;
    W(0x9C) = 0;
    W(0xC0) = 0;
    W(0xA8) = -1; W(0xAC) = -1; W(0xB0) = -1;
    P(0x868) = mp;
    W(0x60) = r;
    W(0xC4) = 1;
    func_0024A908(d);
    func_0024B590(mp);
    func_0024B7A8(mp);
    P(0x1C8) = d + 0x1F8;
    P(0x1CC) = d + 0x260;
    P(0x1D4) = d + 0x2C8;
    P(0x1D8) = d + 0x330;
    P(0x1DC) = d + 0x398;
    P(0x1E4) = d + 0x400;
    P(0x1E8) = d + 0x468;
    P(0x1EC) = d + 0x4D0;
    P(0x1F4) = d + 0x538;
    func_0024B910(bs);
    W(0x860) = -1;
    W(0x85C) = 0;
    W(0x864) = 0;
    if ((rem = (unsigned int)(d + 0x140) & 63)) {
        func_0024FA90(d, D_002AC2A8, rem);
        return 0;
    }
    if ((unsigned int)(d + 0x880) & 63) {
        func_0024FA90(d, D_002AC2D0, (unsigned int)(d + 0x880) & 63);
        return 0;
    }
    return 1;
}
