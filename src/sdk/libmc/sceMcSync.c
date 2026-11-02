// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// (Claude agent) matches with ee-gcc 2.9 (old 2.95.3-114 near miss)
extern int D_002C8350;
extern int D_002C8354;
extern int D_002C8358;
extern int D_00451E40;
int func_00234A70(int);
int func_00234A60(int);
int func_00234A40(int);
int sceMcSync(int mode, int *cmd, int *result)
{
    int r;
    if (!D_002C8350) return -1;
    if (func_00234A70(D_002C8358) < 0) r = 1; else r = 0;
    if (mode == 0 && r) {
        func_00234A60(D_002C8358);
        r = 0;
    }
    r = !r;
    if (cmd) *cmd = D_002C8350;
    if (r) {
        D_002C8350 = 0;
        if (result) *result = D_00451E40;
        func_00234A40(D_002C8354);
    }
    return r;
}
