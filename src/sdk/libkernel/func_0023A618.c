// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
// 2/19 diffs: loop body `sw v1,0(v0)` / `addiu a0,-1` scheduled in the opposite order. (2.95.3: 10 diffs, $a1 copy)
typedef struct E { struct E *next; int a, b, c; } E;
extern E D_0044E300[64];
extern E *D_0044E700;
int func_0023A618(void)
{
    int i;
    D_0044E700 = D_0044E300;
    for (i = 0; i < 64; i++) D_0044E300[i].next = &D_0044E300[i + 1];
    D_0044E300[63].next = 0;
    return 0;
}
