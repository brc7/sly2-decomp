// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
extern char D_00450580[];
extern int D_004507C0[];
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
void func_00243458(int, int, int);
int scePadSetButtonInfo(int port, int slot, int info)
{
    int r;
    D_004507C0[0] = 0xA;
    D_004507C0[1] = port;
    D_004507C0[2] = slot;
    D_004507C0[3] = info;
    if (func_00237288(D_00450580, 1, 0, D_004507C0, 0x80, D_004507C0, 0x80, 0, 0) < 0)
        return 0;
    r = D_004507C0[4];
    if (r == 1) {
        func_00243458(port, slot, 2);
        r = D_004507C0[4];
    }
    return r;
}
