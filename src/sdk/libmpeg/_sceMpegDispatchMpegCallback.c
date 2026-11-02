// (Claude agent) callback table is a struct {int x0,x4,x8; CB cb[];} indexed t->cb[type] (not (t+type)[1]).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int type; } sceMpegCbData;
typedef int (*sceMpegCallback)(void *mp, sceMpegCbData *cbd, void *data);
typedef struct { sceMpegCallback func; void *data; void *gp; } CB;
typedef struct { int x0, x4, x8; CB cb[8]; } CBT;
typedef struct { char p0[0x40]; CBT *cb; } sceMpeg;
static inline void *SetGp(void *gp)
{
    void *old;
    __asm__ volatile("move %0, $28\n\tmove $28, %1" : "=&r"(old) : "r"(gp) : "memory");
    return old;
}
int _sceMpegDispatchMpegCallback(sceMpeg *mp, sceMpegCbData *cbd)
{
    int ret = 0; CBT *t; sceMpegCallback f;
    if (mp && (t = mp->cb) && (f = t->cb[cbd->type].func)) {
        void *old = SetGp(t->cb[cbd->type].gp);
        ret = f(mp, cbd, t->cb[cbd->type].data);
        SetGp(old);
    }
    return ret;
}
