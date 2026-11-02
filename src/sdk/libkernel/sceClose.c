// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
struct iob { int fd, busy, x8, xC; };
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
typedef struct { int sema; int *res; int rsize; int fd; int idx; } CloseArg;
extern int D_002C4A7C;
extern CloseArg D_0044A680;
extern struct iob D_0044BB80[];
extern char D_0044BD80[];
extern int D_0044B2C0[];
extern char D_002AAAC8[];
struct iob *get_iob(int);
int func_00237B50(int);
void func_00237B80(void);
int func_00234A20(struct SemaParam *);
int func_00234A30(int);
int func_00234A60(int);
int func_002375E0(void *, int, int, void *, int, void *, int, void *, void *, int);
int sceClose(int fd)
{
    struct SemaParam sp;
    int res;
    struct iob *io;
    CloseArg *arg;
    int sema, r;

    arg = &D_0044A680;
    io = get_iob(fd);
    func_00237B50(1);
    if (D_002C4A7C == 0 || io == 0 || io->busy == 0) {
        func_00237B80();
        return -9;
    }
    arg->fd = io->fd;
    arg->idx = io - D_0044BB80;
    sp.maxCount = 1;
    sp.option = (unsigned int)D_002AAAC8;
    sp.initCount = 0;
    sema = func_00234A20(&sp);
    D_0044A680.sema = sema;
    arg->res = &res;
    arg->rsize = 4;
    if (func_002375E0(D_0044BD80, 1, 0, arg, 0x14, D_0044B2C0, 4, 0, 0, 0) < 0) {
        func_00234A30(sema);
        func_00237B80();
        return -11;
    }
    io->busy = 0;
    r = *(volatile int *)((unsigned int)D_0044B2C0 | 0x20000000);
    func_00237B80();
    if (r == 0) {
        func_00234A30(sema);
        return -11;
    }
    func_00234A60(sema);
    func_00234A30(sema);
    if (res > -1) return 0;
    return res;
}
