// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p[0x24]; int server; } RpcCD;
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
extern RpcCD D_00450880;
extern char D_00450900[];
extern int D_00451E40[];
extern int D_002C8354;
extern int D_002C8358;
extern char D_002AB7B0[], D_002AB7C0[], D_002AB7D8[], D_002AB7F0[], D_002AB818[];
int func_00234A20(struct SemaParam *);
int func_00234A30(int);
int func_00234A40(int);
int func_00234A60(int);
void func_00244450(int, int, int);
void func_00236A60(int);
int func_00236960(const char *, ...);
int func_002370A8(void *, unsigned int, int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
static inline int bindmc(void)
{
    if (func_002370A8(&D_00450880, 0x80000400, 0) < 0) {
        func_00236960(D_002AB7D8);
        while (1) ;
    }
    return D_00450880.server;
}
int sceMcInit(void)
{
    struct SemaParam sp;
    int i = 0;
    int j;
    int r;

    if (D_002C8354 < 0) {
        int m1 = -1;
        sp.initCount = 1;
        sp.option = (unsigned int)D_002AB7B0;
        sp.maxCount = 1;
        D_002C8354 = func_00234A20(&sp);
        if (D_002C8354 == m1)
            return -101;
        sp.maxCount = 1;
        sp.initCount = 0;
        sp.option = (unsigned int)D_002AB7C0;
        r = func_00234A20(&sp);
        D_002C8358 = r;
        if (r == m1) {
            func_00234A30(D_002C8354);
            D_002C8354 = r;
            return -101;
        }
    }
    func_00244450(0, 0, 0);
    func_00234A60(D_002C8354);
    func_00236A60(0);
    while (bindmc() == 0) {
        j = 0x100000;
        while (--j) ;
        if (++i >= 0x200) {
            func_00236960(D_002AB7D8);
            return -91;
        }
    }
    r = func_00237288(&D_00450880, 0xFE, 0, D_00450900, 0x30, D_00451E40, 0xC, 0, 0);
    func_00234A40(D_002C8354);
    if (r < 0) {
        D_00450880.server = 0;
        return -91;
    }
    if (D_00451E40[1] < 0x20A) {
        func_00236960(D_002AB7F0);
        D_00450880.server = 0;
        return -120;
    }
    if (D_00451E40[2] < 0x20E) {
        func_00236960(D_002AB818);
        D_00450880.server = 0;
        return -121;
    }
    return D_00451E40[0];
}
