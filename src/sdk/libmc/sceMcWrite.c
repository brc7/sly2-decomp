// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p[0x24]; int server; } RpcCD;
typedef struct { int fd; int x4; int x8; int size; int x10; unsigned head; void *buf; int x1c; unsigned char data[16]; } McRwSend;
extern RpcCD D_00450880;
extern McRwSend D_00450900;
extern int D_00451E40[];
extern int D_002C8354;
extern int D_002C8350;
void func_00243ED8(void *);
int func_00234A70(int);
int func_00234A40(int);
void func_00234CA0(int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int sceMcWrite(int fd, const void *buf, int size)
{
    unsigned i;
    if (D_00450880.server == 0)
        return -100;
    if (func_00234A70(D_002C8354) < 0)
        return -200;
    D_00450900.fd = fd;
    if (size <= 16) {
        D_00450900.head = size;
        D_00450900.size = 0;
        D_00450900.buf = 0;
    } else {
        unsigned lo = (unsigned)buf - 16;
        unsigned n = (((unsigned)buf - 1) & 0xFFFFFFF0u) - lo;
        D_00450900.size = size - n;
        D_00450900.head = n;
        D_00450900.buf = (char *)buf + n;
    }
    for (i = 0; i < D_00450900.head; i++)
        D_00450900.data[i] = ((unsigned char *)buf)[i];
    func_00234CA0(0);
    if (func_00237288(&D_00450880, 6, 1, &D_00450900, 0x30, D_00451E40, 4, func_00243ED8, 0) != 0) {
        func_00234A40(D_002C8354);
        return -91;
    }
    D_002C8350 = 6;
    return 0;
}
