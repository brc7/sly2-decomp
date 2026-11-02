// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-114
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
// SLY2_SAVES: sd
extern char D_002C82C0[];
extern int D_002C7940[];
extern volatile int D_002C676C;
extern volatile int D_002C67A0;
int func_00241F98(int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int func_00234A40(int);
int sceCdBreak(void)
{
    int r;
    if (func_00241F98(0x1E) == 0)
        return 0;
    D_002C67A0 = 8;
    if (func_00237288(D_002C82C0, 0x16, 0, 0, 0, D_002C7940, 4, 0, 0) >= 0) {
        D_002C67A0 = 0;
        r = *(int *)((unsigned int)D_002C7940 | 0x20000000);
        func_00234A40(D_002C676C);
        return r;
    }
    func_00234A40(D_002C676C);
    D_002C67A0 = 0;
    return 0;
}
