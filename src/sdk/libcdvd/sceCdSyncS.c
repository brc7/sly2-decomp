// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern int D_002C6750;
extern char D_002AB520[];
extern char D_002C82C0[];
int func_00236960(const char *, ...);
int func_00235518(int);
int func_00237488(void *);
int sceCdSyncS(int mode)
{
    if (mode == 0) {
        if (D_002C6750 > 0) func_00236960(D_002AB520);
        while (func_00237488(D_002C82C0)) func_00235518(0xFA0);
        return 0;
    }
    return func_00237488(D_002C82C0);
}
