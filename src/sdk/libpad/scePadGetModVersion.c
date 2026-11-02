// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
extern char D_00450580[];
extern int D_004507C0[];
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int scePadGetModVersion(void)
{
    D_004507C0[0] = 0x12;
    if (func_00237288(D_00450580, 1, 0, D_004507C0, 0x80, D_004507C0, 0x80, 0, 0) >= 0)
        return D_004507C0[3];
    return 0;
}
