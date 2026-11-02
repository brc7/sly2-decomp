// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct Alarm {
    struct Alarm *next;
    volatile int id;
    void *func;
    void *arg;
} Alarm;
extern Alarm *D_0044E700;
int func_0023A228(void);
int iSetTimerHandler(int, long, void *, void *);
int iStartTimerCounter(int);
int func_0023A668(int, int, int, Alarm *);
static inline Alarm *AllocAlarm(void)
{
    Alarm *p = D_0044E700;
    if (p)
        D_0044E700 = p->next;
    return p;
}
int iSetTimerAlarm(long sched, void *func, void *arg)
{
    Alarm *p;
    int id;
    if (func == 0)
        return 0x80000016;
    p = AllocAlarm();
    if (p == 0)
        return 0x80008005;
    id = func_0023A228();
    if (id >= 0) {
        p->func = func;
        p->arg = arg;
        p->id = id;
        iSetTimerHandler(id, sched, func_0023A668, p);
        iStartTimerCounter(id);
        return ((int)p << 4) | (id & 0xFE) | 1;
    }
    {
        Alarm *h = D_0044E700;
        D_0044E700 = p;
        p->next = h;
        p->id = 0;
    }
    return id;
}
