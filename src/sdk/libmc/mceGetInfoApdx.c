// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern int *D_004508A8;
extern int *D_004508AC;
extern int *D_004508B0;
extern int D_002C8358;
int func_00234A50(int);
void mceGetInfoApdx(void *data)
{
    int *p = (int *)((unsigned int)data | 0x20000000);
    if (D_004508A8) *D_004508A8 = p[0];
    if (D_004508AC) *D_004508AC = p[1];
    if (D_004508B0) *D_004508B0 = p[0x24];
    func_00234A50(D_002C8358);
}
