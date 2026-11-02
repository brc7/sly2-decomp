// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
// (Claude agent) MATCH with -fstrict-aliasing (-Os sweep)
typedef struct { char p[0x24]; int server; } RpcCD;
typedef struct { int fd; int x4; int x8; int size; int x10; int x14; void *buf; void *param; } McRwSend;
extern RpcCD D_00450880;
extern McRwSend D_00450900;
extern int D_00451E40[];
extern char D_00450D80[];
extern int D_002C8354;
extern int D_002C8350;
void func_00244110(void *);
int func_00234A70(int);
int func_00234A40(int);
void func_0023B170(void *, int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int sceMcRead(int fd, void *buf, int size)
{
    if (D_00450880.server == 0)
        return -100;
    if (func_00234A70(D_002C8354) < 0)
        return -200;
    D_00450900.fd = fd;
    D_00450900.buf = buf;
    D_00450900.size = size;
    D_00450900.param = D_00450D80;
    func_0023B170(buf, size);
    func_0023B170(D_00450D80, 0xC0);
    if (func_00237288(&D_00450880, 5, 1, &D_00450900, 0x30, D_00451E40, 4, func_00244110, D_00450D80) == 0) {
        D_002C8350 = 5;
        return 0;
    }
    func_00234A40(D_002C8354);
    return -91;
}
