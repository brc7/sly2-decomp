// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
#define R(a) (*(volatile unsigned int *)(a))
typedef struct { char p0[0x1C0]; int x1C0; char p1[0x828 - 0x1C4]; int x828; } MpegD;
int DIntr(void);
int EIntr(void);
void func_00254BC0(int, int);
void _clearEach(MpegD *d)
{
    int r;
    d->x828 = 1;
    d->x1C0 = 0;
    r = DIntr();
    R(0x1000F590) = R(0x1000F520) | 0x10000;
    R(0x1000B000) = 0;
    R(0x1000B400) = 0;
    R(0x1000D400) = 0;
    R(0x1000F590) = R(0x1000F520) & ~0x10000;
    if (r) EIntr();
    R(0x1000B020) = 0;
    R(0x1000B420) = 0;
    R(0x1000D420) = 0;
    R(0x10002010) = 0x40000000;
    func_00254BC0(0, 0);
}
