// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
extern int D_002C4A94;
extern char D_0044BE80[];
extern int D_0044BF00[];
extern int D_0044BEC0[];
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int func_00238CC8(void *addr)
{
    if (D_002C4A94 < 0)
        return 0;
    D_0044BF00[0] = (int)addr;
    if (func_00237288(D_0044BE80, 2, 0, D_0044BF00, 4, D_0044BEC0, 4, 0, 0) < 0)
        return -1;
    return D_0044BEC0[0];
}
