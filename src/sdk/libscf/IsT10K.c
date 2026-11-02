// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
extern char D_002C8378[];
extern char *GetRomName(void);

int IsT10K(void)
{
    if (D_002C8378[0] == 0)
        GetRomName();
    return D_002C8378[4] == 0x54;
}
