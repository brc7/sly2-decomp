// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { int type; int code; } CBERR;
typedef struct { char pad[0xC]; int cb; } CBS;
typedef struct { char pad[0x868]; void *mp; } DEC;
int _sceMpegDispatchMpegCallback(void *, void *);
void func_0024F8C0(int);
void _sceMpegError(DEC *d, int code)
{
    void *mp = d->mp;
    if (mp && d && ((CBS *)d)->cb) {
        CBERR e;
        e.code = code;
        e.type = 0;
        _sceMpegDispatchMpegCallback(mp, &e);
    } else {
        func_0024F8C0(code);
    }
}
