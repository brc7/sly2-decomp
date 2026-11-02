// WIP (Claude agent): 2 diffs - lui v0/li v1 scheduling order at entry (tie); tried volatile/struct/local m/strict-aliasing/other 2.95 bundles
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern volatile int D_002C4A98;
extern char D_00443268[];
void *func_002333B4(void *, int, int);
int sceSifLoadFileReset(void)
{
    D_002C4A98 = -1; func_002333B4(D_00443268, 0, 4);
    return 0;
}
