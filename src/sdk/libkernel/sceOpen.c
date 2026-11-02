// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define va_arg(ap, T) (*(T *)((ap += 8) - 8))
struct iob { int fd, flags, x8, xC; };
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
typedef struct { int sema; int *res; int rsize; int flags; int mode; char name[0x400]; int idx; } OpenArg;
extern int D_002C4A7C;
extern int D_002C4A88;
extern OpenArg D_0044A680;
extern struct iob D_0044BB80[];
extern char D_0044BD80[];
extern int D_0044B2C0[];
extern char D_002AAAB0[];
int func_00237B50(int);
void func_00237B80(void);
void func_00237BD8(void);
int func_00237DE0(void);
struct iob *func_00237558(void);
int func_00234A20(struct SemaParam *);
int func_00234A30(int);
int func_00234A40(int);
int func_00234A60(int);
int func_002375E0(void *, int, int, void *, int, void *, int, void *, void *, int);
int sceOpen(const char *name, int flags, ...)
{
    struct SemaParam sp;
    int res;
    struct iob *io;
    OpenArg *arg;
    int sema, r, i, mode, idx;
    va_list ap;

    va_start(ap, flags);
    arg = &D_0044A680;
    func_00237B50(0);
    if (D_002C4A7C == 0)
        func_00237BD8();
    if (func_00237DE0()) {
        func_00237B80();
        return -0x10004;
    }
    io = func_00237558();
    if (io == 0) {
        func_00237B80();
        return -19;
    }
    mode = va_arg(ap, int);
    for (i = 0; i < 0x400; i++) {
        if ((arg->name[i] = name[i]) == 0) break;
    }
    if (i == 0x400)
        arg->name[0x3FF] = 0;
    idx = io - D_0044BB80;
    arg->flags = flags & 0x7FFFFFFF;
    arg->mode = mode;
    sp.maxCount = 1;
    sp.option = (unsigned int)D_002AAAB0;
    arg->idx = idx;
    sp.initCount = 0;
    sema = func_00234A20(&sp);
    arg->res = &res;
    arg->sema = sema;
    arg->rsize = 4;
    if (func_002375E0(D_0044BD80, 0, 0, &D_0044A680, 0x418, D_0044B2C0, 4, 0, 0, 0) < 0) {
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
    func_00234A60(sema);
    func_00234A30(sema);
    if (res < 0) {
        func_00234A60(D_002C4A88);
        io->flags = 0;
        func_00234A40(D_002C4A88);
        return res;
    }
    r = idx;
    func_00234A60(D_002C4A88);
    io->fd = res;
    io->flags |= flags;
    func_00234A40(D_002C4A88);
    return r;
}
