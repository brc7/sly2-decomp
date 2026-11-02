// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
extern char D_002C7910[];
extern int D_002C67C0[];
extern int D_002C6768;
int func_00241C98(int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int func_00234A40(int);
int sceCdNcmdDiskReady(void)
{
    int r;
    if (func_00241C98(2) == 0)
        return 0;
    if (func_00237288(D_002C7910, 14, 0, 0, 0, D_002C67C0, 4, 0, 0) >= 0) {
        r = *(int *)((unsigned int)D_002C67C0 | 0x20000000);
        func_00234A40(D_002C6768);
        return r;
    }
    func_00234A40(D_002C6768);
    return 0;
}
