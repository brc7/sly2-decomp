// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* libkernel fileio sceWrite (Lilly's agent, T1573; same recipe as sceRead, T1568). Keys: `nowait` assigned in a nested
   do{do{}while(0);}while(0) after `i = 0` (loop-depth weight gives the target's s4 nowait > s5 sema > s6 size > s7 flags
   priority); a separate counter j for the head-copy loop; and a dead first statement in each arm (`r = (int)D_002C49F8;`,
   `n = buf & 0xF;`): it computes the arm's `lui`/andi itself, so gcse PRE no longer appends it AFTER `i = 0` / `nowait = 0`. */
struct iob { int fd, flags, x8, xC; };
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
typedef struct { int sema; int *res; int rsize; int fd; void *buf; int size; int nhead; char head[16]; int idx; } WriteArg;
extern int D_002C4A7C;
extern int D_002C4A8C;
extern int D_002C49F8[32];
extern WriteArg D_0044A680;
extern struct iob D_0044BB80[];
extern char D_0044BD80[];
extern int D_0044B2C0[];
extern char D_002AAB10[];
struct iob *func_002376D0(int);
int func_00237B50(int);
void func_00237B80(void);
void func_0023B170(void *, int);
int func_00234A20(struct SemaParam *);
int func_00234A30(int);
int func_00234A40(int);
int func_00234A60(int);
int func_002375E0(void *, int, int, void *, int, void *, int, void *, void *, int);
int sceWrite(int fd, const void *buf, int size)
{
    struct SemaParam sp;
    int res;
    struct iob *io;
    WriteArg *arg;
    int sema, r, flags, nowait, i, n, j;

    arg = &D_0044A680;
    io = func_002376D0(fd);
    func_00237B50(3);
    if (D_002C4A7C == 0 || io == 0 || (flags = io->flags) == 0) {
        func_00237B80();
        return -9;
    }
    arg->fd = io->fd;
    arg->idx = io - D_0044BB80;
    sp.maxCount = 1;
    sp.option = (unsigned int)D_002AAB10;
    arg->size = size;
    arg->buf = (void *)buf;
    sp.initCount = 0;
    sema = func_00234A20(&sp);
    arg->res = &res;
    arg->rsize = 4;
    D_0044A680.sema = sema;
    if (flags & 0x8000) {
        func_00234A60(D_002C4A8C);
        r = (int)D_002C49F8;
        i = 0;
        do { do { nowait = flags & 0x8000; } while (0); } while (0);
        for (; i < 32; i++) {
            if (D_002C49F8[i] == -1) {
                D_002C49F8[i] = arg->sema;
                arg->sema = -arg->sema;
                break;
            }
        }
        func_00234A40(D_002C4A8C);
    } else {
        n = (unsigned int)buf & 0xF;
        nowait = 0;
    }
    if (((unsigned int)buf & 0xF) == 0)
        n = 0;
    else
        n = (unsigned int)buf / 16 * 16 + 16 - (unsigned int)buf;
    if (size < n)
        n = size;
    if (!(flags & 0x20000000))
        func_0023B170((void *)buf, size);
    buf = (void *)((unsigned int)buf | 0x20000000);
    arg->nhead = n;
    for (j = 0; j < n; j++)
        arg->head[j] = ((const char *)buf)[j];
    if (func_002375E0(D_0044BD80, 3, 0, &D_0044A680, 0x30, D_0044B2C0, 4, 0, 0, nowait) < 0) {
        func_00234A30(sema);
        func_00237B80();
        return -11;
    }
    r = *(volatile int *)((unsigned int)D_0044B2C0 | 0x20000000);
    func_00237B80();
    if (r == 0) {
        func_00234A30(sema);
        return -11;
    }
    if (nowait) {
        func_00234A30(sema);
        return 0;
    }
    func_00234A60(sema);
    func_00234A30(sema);
    return res;
}
