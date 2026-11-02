// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* libkernel fileio sceIoctl (Claude agent). Keys: `volatile` sema table D_002C49F8 (case-1 loop); the request
   dispatch is written as the switch's own decision tree (==2, <3, ==1, ==3) with the case-1 constant in a local `one`
   set at entry - that is what keeps 1 in $s5 from the prologue; arg->sema is stored before res/rsize (gcc rotates it
   into the jal delay slot). */
struct iob { int fd, flags, x8, xC; };
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
typedef struct { char b[0x400]; } IoctlData;
typedef struct { int sema; int *res; int rsize; int fd; int req; IoctlData data; int x414; int x418; int size; } IoctlArg;
typedef struct { char b[0x440]; } FsStat;
extern int D_002C4A7C;
extern int D_002C4A78;
extern int D_002C4A8C;
extern volatile int D_002C49F8[32];
extern void *D_0044A648;
extern IoctlArg D_0044A680;
extern FsStat D_0044B310[];
extern char D_0044BD80[];
extern int D_0044B2C0[];
extern char D_002AAB28[];
struct iob *func_002376D0(int);
int func_00237B50(int);
void func_00237B80(void);
void func_00237BD8(void);
void func_0023B170(void *, int);
int func_00234A20(struct SemaParam *);
int func_00234A30(int);
int func_00234A40(int);
int func_00234A60(int);
int func_002375E0(void *, int, int, void *, int, void *, int, void *, void *, int);

int sceIoctl(int fd, int req, void *data)
{
    struct SemaParam sp;
    int res;
    struct iob *io;
    IoctlArg *arg;
    int sema, r, i;
    int one = 1;    /* case value held in a register from the prologue, as in the target */

    arg = &D_0044A680;
    io = func_002376D0(fd);
    func_00237B50(5);
    D_0044A648 = data;
    if (D_002C4A7C == 0)
        func_00237BD8();
    if (io == 0 || io->flags == 0) {
        func_00237B80();
        return -9;
    }
    arg->x414 = 0;
    arg->x418 = 0;
    if (req == 2) goto c2;
    if (req < 3) { if (req == one) goto c1; goto dflt; }
    if (req == 3) goto c3;
    goto dflt;
    c1: {
        func_00234A60(D_002C4A8C);
        for (i = 0; i < 32; i++) {
            if (D_002C49F8[i] != -1)
                break;
        }
        if (i == 32)
            *(int *)D_0044A648 = 0;
        else
            *(int *)D_0044A648 = 1;
        func_00234A40(D_002C4A8C);
        func_00237B80();
        return 0;
    }
    c2: {
        *(int *)data = *(int *)((unsigned int)&D_0044B310[D_002C4A78] | 0x20000000);
        func_00237B80();
        return 0;
    }
    c3: {
        *(long *)data = *(long *)((unsigned int)&D_0044B310[D_002C4A78] | 0x20000000);
        func_00237B80();
        return 0;
    }
    dflt:
    arg->fd = io->fd;
    arg->req = req;
    if (data == 0) {
        arg->size = 0;
    } else {
        arg->size = 0x400;
        arg->data = *(IoctlData *)data;
    }
    sp.maxCount = 1;
    sp.option = (unsigned int)D_002AAB28;
    sp.initCount = 0;
    sema = func_00234A20(&sp);
    arg->sema = sema;
    arg->res = &res;
    arg->rsize = 4;
    func_0023B170(&D_0044A680, 0x420);
    if (func_002375E0(D_0044BD80, 5, 0, &D_0044A680, 0x420, D_0044B2C0, 4, 0, 0, 0) < 0) {
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
    return res;
}
