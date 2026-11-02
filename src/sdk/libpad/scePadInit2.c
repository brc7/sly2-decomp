// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int a, b, c, d, e, f, g; } PadSlot;
extern PadSlot D_004505D0[2][4];
extern char D_00450580[];
extern int D_004507C0[];
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int func_00239450(void);
int func_002394A0(void);
void func_0023ADA8(int, void *, void *);
int _pad_intr_hdr(void *);
int scePadInit2(int mode)
{
    int i;
    int s;
    for (i = 0; i < 4; i++) {
        D_004505D0[0][i].e = 0;
        D_004505D0[0][i].g = 0;
        D_004505D0[0][i].f = 0;
        D_004505D0[1][i].e = 0;
        D_004505D0[1][i].g = 0;
        D_004505D0[1][i].f = 0;
    }
    D_004507C0[0] = 0x10;
    D_004507C0[4] = 0;
    if (func_00237288(D_00450580, 1, 0, D_004507C0, 0x80, D_004507C0, 0x80, 0, 0) < 0)
        return 0;
    s = func_00239450();
    func_0023ADA8(0x80000019, _pad_intr_hdr, 0);
    if (s == 1) func_002394A0();
    return D_004507C0[3];
}
