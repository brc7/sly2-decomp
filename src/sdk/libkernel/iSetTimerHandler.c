// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int x0; int x4; int id; unsigned int mode; long base; long x18; long sched; void *handler; void *gp; void *arg; } Counter;
typedef struct { char pad[0x1C]; volatile int cur; } TimerSys;
extern TimerSys D_002C5A30;
long func_0023A1D0(void);
void func_00239E68(Counter *);
void func_00239EF0(Counter *);
void func_00239CD8(long);
int iSetTimerHandler(int id, long sched, void *handler, void *arg)
{
    Counter *p = (Counter *)(((unsigned int)id >> 10) << 6);
    TimerSys *ts;
    unsigned int mode;
    if (id < 0 || (id & 0x3FF) != p->id)
        return 0x80008002;
    ts = &D_002C5A30;
    if (ts->cur == id)
        return 0x80000010;
    mode = p->mode;
    if (mode & 2) {
        func_00239EF0(p);
        mode = p->mode;
    }
    p->handler = handler;
    if (handler == 0) {
        p->mode = mode & ~2;
    } else {
        register void *gp __asm__("$28");
        p->sched = sched;
        p->mode = mode | 2;
        p->gp = gp;
        p->arg = arg;
        if (*(volatile unsigned int *)&p->mode & 1)
            func_00239E68(p);
    }
    func_00239CD8(func_0023A1D0());
    return 0;
}
