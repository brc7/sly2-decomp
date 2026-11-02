// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
extern char D_0044BDA8[];
extern char D_002C49E4[];
extern void *D_002C4A90;
int func_00233178(const void *, const void *, unsigned int);
int func_00237DE0(void)
{
    char *b = D_002C49E4;
    char *a = D_0044BDA8;
    int r = 0;
    if (func_00233178(a, b, 4) && func_00233178(a, D_002C4A90, 4))
        r = func_00233178(b, D_002C4A90, 4) != 0;
    return r;
}
