// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// (Claude agent, -Os sweep) 73 diffs with -Os (was 95 at -O2)
typedef struct { char p[0x24]; int server; } RpcCD;
typedef struct { int fd; int port; int slot; int format; int free; int type; int x18; void *param; } McInfoSend;
extern RpcCD D_00450880;
extern McInfoSend D_00450900;
extern int D_00451E40[];
extern char D_00450D80[];
extern int *D_004508A8;
extern int *D_004508AC;
extern int *D_004508B0;
extern int D_002C8354;
extern int D_002C8350;
void mceGetInfoApdx(void *);
int func_00234A70(int);
int func_00234A40(int);
void func_0023B170(void *, int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int sceMcGetInfo(int port, int slot, int *type, int *free, int *format)
{
    if (D_00450880.server == 0)
        return -100;
    if (func_00234A70(D_002C8354) < 0)
        return -200;
    D_00450900.port = port;
    D_00450900.slot = slot;
    D_00450900.param = D_00450D80;
    if (type) D_00450900.type = 1; else D_00450900.type = 0;
    if (free) D_00450900.free = 1; else D_00450900.free = 0;
    if (format) D_00450900.format = 1; else D_00450900.format = 0;
    D_004508A8 = type;
    D_004508AC = free;
    D_004508B0 = format;
    func_0023B170(D_00450D80, 0xC0);
    if (func_00237288(&D_00450880, 1, 1, &D_00450900, 0x30, D_00451E40, 4, mceGetInfoApdx, D_00450D80) != 0) {
        func_00234A40(D_002C8354);
        return -91;
    }
    D_002C8350 = 1;
    return 0;
}
