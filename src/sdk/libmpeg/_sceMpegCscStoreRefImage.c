// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* _sceMpegCscStoreRefImage (libmpeg, Claude agent): DMA the reference image to the IPU (toIPU ch4, chunked via the
   _ch4dma handler when > 0xFFFF qw) and CSC it back (fromIPU ch3; _doCSC2 for >= 0x400 MBs). The CSC part is an
   inline function used twice; the IPU command helper sets d->x82C/x828. */
typedef struct { int type; int pad[7]; } CB;
typedef struct { unsigned int size; unsigned int addr; int pad[2]; } ST;
typedef struct { unsigned int addr; int x4; int x8; int w; int h; } P;
typedef struct {
    char p0[0xEC]; unsigned int xEC;
    char pF0[0x828 - 0xF0]; int x828; int x82C;
    char p830[0x868 - 0x830]; void *mp;
} D;
#define R(a) (*(volatile unsigned int *)(a))
#define RS(a) (*(volatile int *)(a))
int _ch4dma(int ch, void *arg);
int func_00234740(int ch, int (*h)(int, void *), int next, void *arg);
int func_00234750(int ch, int id);
int func_00235248(int ch);
int func_002351E0(int ch);
void s1_FlushCache(int);
int func_00239450(void);
int func_002394A0(void);
int _sceMpegDispatchMpegCallback(void *mp, CB *cb);
void _doCSC2(D *d, unsigned int out, int n);

static inline void ipuCmd(D *d, unsigned int cmd)
{
    R(0x10002000) = cmd;
    d->x82C = cmd & 0xF0000000;
    if ((cmd & 0xF0000000) == 0x20000000 || (cmd & 0xF0000000) == 0x30000000 || (cmd & 0xF0000000) == 0x40000000)
        d->x828 = 0;
    else
        d->x828 = 1;
}

static inline void cscStore(D *d, int n)
{
    CB cb2;
    CB *pcb;
    unsigned int out;
    int qwc;
    int e;
    if (n < 0x400) {
        out = d->xEC;
        pcb = &cb2;
        qwc = n << 6;
        while (RS(0x10002010) < 0)
            ;
        e = func_00239450();
        { volatile unsigned int *m = (void *)0x1000B010, *q = (void *)0x1000B020, *c = (void *)0x1000B000;
        *m = out & 0x0FFFFFFF;
        *q = qwc;
        *c = 0x100; }
        if (e)
            func_002394A0();
        ipuCmd(d, n | 0x70000000);
        pcb->type = 4;
        _sceMpegDispatchMpegCallback(d->mp, &cb2);
        while ((R(0x1000B000) >> 8) & 1)
            ;
        while (RS(0x10002010) < 0)
            ;
    } else {
        _doCSC2(d, d->xEC, n);
    }
}

void _sceMpegCscStoreRefImage(D *d, P *p)
{
    CB cb;
    ST st;
    int n, id, e;

    n = p->w * p->h;
    cb.type = 2;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
    if (R(0x10002010) & 0x4000)
        R(0x10002010) = 0x40000000;
    while (RS(0x10002010) < 0)
        ;
    ipuCmd(d, 0);
    while (RS(0x10002010) < 0)
        ;
    s1_FlushCache(0);
    st.addr = p->addr & 0x0FFFFFFF;
    st.size = n * 0x18;
    if (st.size > 0xFFFF) {
        id = func_00234740(4, _ch4dma, 0, &st);
        R(0x1000E010) = 0x10;
        func_00235248(4);
        e = func_00239450();
        { volatile unsigned int *m = (void *)0x1000B410, *q = (void *)0x1000B420, *c = (void *)0x1000B400;
        *m = st.addr;
        *q = 0xFFFF;
        *c = 0x101; }
        if (e)
            func_002394A0();
        st.addr = (st.addr + 0xFFFF0) & 0x0FFFFFFF;
        st.size -= 0xFFFF;
        cscStore(d, n);
        func_002351E0(4);
        func_00234750(4, id);
    } else {
        e = func_00239450();
        { volatile unsigned int *m = (void *)0x1000B410, *q = (void *)0x1000B420, *c = (void *)0x1000B400;
        *m = p->addr & 0x0FFFFFFF;
        *q = st.size;
        *c = 0x101; }
        if (e)
            func_002394A0();
        st.size = 0;
        cscStore(d, n);
    }
    cb.type = 3;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
}
