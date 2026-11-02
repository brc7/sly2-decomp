// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
typedef struct { char p[0x24]; int server; } RpcCD;
extern RpcCD D_00450880;
extern int D_00450900[];
extern int D_00451E40[];
extern int D_002C8354;
extern int D_002C8350;
void func_00243ED8(void *);
int func_00234A70(int);
int func_00234A40(int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int sceMcFormat(int port, int slot)
{
    if (D_00450880.server == 0)
        return -100;
    if (func_00234A70(D_002C8354) < 0)
        return -200;
    D_00450900[1] = port;
    D_00450900[2] = slot;
    if (func_00237288(&D_00450880, 0x10, 1, D_00450900, 0x30, D_00451E40, 4, func_00243ED8, 0) == 0) {
        D_002C8350 = 0x10;
        return 0;
    }
    func_00234A40(D_002C8354);
    return -91;
}
