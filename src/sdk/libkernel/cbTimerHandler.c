// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* cbTimerHandler (libkernel timer.c, Claude agent): T2 compare interrupt; runs expired alarms. The callback's $gp is
   loaded from the counter (asm), the inline GetTime is iGetTimerSystemTime; ovf/count volatile as in the target. */
typedef struct Counter {
    struct Counter *next;
    int x4;
    int id;
    unsigned int mode;
    unsigned long base;
    unsigned long x18;
    unsigned long sched;
    unsigned long (*handler)(int, unsigned long, unsigned long, void *, void *);
    void *gp;
    void *arg;
} Counter;
typedef struct {
    volatile unsigned long ovf;  /* 0x00 */
    int pad8[2];
    volatile int count; /* 0x10 */
    Counter *free;      /* 0x14 */
    Counter *head;      /* 0x18 */
    int cur;            /* 0x1C */
} TimerSys;
extern TimerSys D_002C5A30;
#define T_COUNT (*(volatile unsigned int *)0x10001000)
#define T_MODE  (*(volatile unsigned int *)0x10001010)
void func_00239AC0(unsigned int);
Counter *func_00239EF0(Counter *);
void func_00239E68(Counter *);
void func_00239CD8(unsigned long);

static inline unsigned long GetTime(void)
{
    unsigned int cnt, mode;
    cnt = T_COUNT;
    mode = T_MODE;
    if (mode & 0x800) {
        D_002C5A30.ovf++;
        func_00239AC0(mode & ~0x400);
        cnt = T_COUNT;
    }
    return ((D_002C5A30.ovf << 16) | cnt) << ((mode & 3) * 4);
}

int cbTimerHandler(int cause, void *arg, void *addr)
{
    Counter *p, *next;
    unsigned long now, target, r;

    if (T_MODE & 0x400) {
        p = D_002C5A30.head;
        while (p) {
            target = p->sched + p->base - p->x18;
            now = GetTime();
            if (now < target)
                break;
            next = func_00239EF0(p);
            {
                unsigned long dt = now + p->x18 - p->base;
                D_002C5A30.cur = ((unsigned int)p << 4) | p->id;
                __asm__ __volatile__("lw $28, %0" : : "m"(p->gp));
                r = p->handler(((unsigned int)p << 4) | p->id, p->sched, dt, p->arg, addr);
            }
            if (r == 0) {
                p->mode &= ~2;
            } else if (r == (unsigned long)-1) {
                p->id = 0;
                p->mode = 0;
                p->next = D_002C5A30.free;
                D_002C5A30.free = p;
                D_002C5A30.count--;
            } else {
                if (r < 0x3999)
                    r = 0x3999;
                p->sched += r;
                func_00239E68(p);
            }
            p = next;
        }
    }
    D_002C5A30.cur = -1;
    func_00239CD8(GetTime());
    GetTime();
    __asm__ __volatile__("sync\n\tei");
    return 0;
}
