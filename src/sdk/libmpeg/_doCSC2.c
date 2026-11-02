// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { volatile int cnt; int done; int rem; unsigned int addr; int total; } CSC;
typedef struct { int type; int pad[7]; } CB;
typedef struct { char p0[0x868]; void *mp; } D;
#define R(a) (*(volatile unsigned int *)(a))
#define RS(a) (*(volatile int *)(a))
int _ch3dmaCSC(int ch, void *arg);
int AddDmacHandler(int ch, int (*h)(int, void *), int next, void *arg);
int RemoveDmacHandler(int ch, int id);
int EnableDmac(int ch);
int DisableDmac(int ch);
int DIntr(void);
int func_002394A0(void);
int _sceMpegDispatchMpegCallback(void *mp, CB *cb);
void _sceMpegError(D *d, const char *msg);
void _doCSC2(D *d, unsigned int out, int n)
{
    CB cb;
    CSC c;
    int id;
    int e;

    c.total = n / 1023 + 1;
    c.rem = n;
    c.addr = (out + 0xFFC00) & 0x0FFFFFFF;
    c.done = 0;
    c.cnt = 0;
    while (RS(0x10002010) < 0)
        ;
    id = AddDmacHandler(3, _ch3dmaCSC, 0, &c);
    R(0x1000E010) = 8;
    EnableDmac(3);
    e = DIntr();
    { volatile unsigned int *m = (void *)0x1000B010, *q = (void *)0x1000B020, *c = (void *)0x1000B000;
    *m = out & 0x0FFFFFFF;
    *q = 0xFFC0;
    *c = 0x100; }
    if (e)
        func_002394A0();
    R(0x10002000) = 0x700003FF;
    cb.type = 4;
    _sceMpegDispatchMpegCallback(d->mp, &cb);
    while (c.cnt < c.total)
        ;
    if (c.done)
        _sceMpegError(d, "CSC handler error\n");
    while (RS(0x10002010) < 0)
        ;
    DisableDmac(3);
    RemoveDmacHandler(3, id);
}
