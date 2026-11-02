// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct { char p[0x24]; int server; } RpcCD;
typedef struct { int port; int slot; int flags; int size; int offset; char name[0x400]; } McSend;
extern RpcCD D_00450880;
extern McSend D_00450930;
extern int D_00451E40[];
extern int D_002C8354;
extern int D_002C8350;
void func_00243ED8(void *);
int func_00234A70(int);
int func_00234A40(int);
char *func_00233638(char *, const char *, unsigned int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int sceMcDelete(int port, int slot, const char *name)
{
    if (D_00450880.server == 0)
        return -100;
    if (func_00234A70(D_002C8354) < 0)
        return -200;
    if (name == 0 || name[0] == 0) {
        func_00234A40(D_002C8354);
        return -210;
    }
    func_00233638(D_00450930.name, name, 0x3FF);
    D_00450930.port = port;
    D_00450930.slot = slot;
    D_00450930.name[0x3FF] = 0;
    D_00450930.flags = 0;
    if (func_00237288(&D_00450880, 0xF, 1, &D_00450930, 0x414, D_00451E40, 4, func_00243ED8, 0) == 0) {
        D_002C8350 = 0xF;
        return 0;
    }
    func_00234A40(D_002C8354);
    return -91;
}
