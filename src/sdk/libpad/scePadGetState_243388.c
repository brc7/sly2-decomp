// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char *buf; int x4; int x8; int xC; int open; int x14; int x18; } PadSt;
extern PadSt D_004505D0[2][4];
int DIntr(void);
int EIntr(void);
unsigned char *scePadGetDmaStr(int port, int slot);
int scePadGetState_243388(int port, int slot)
{
    int r;
    unsigned char *p;
    if (D_004505D0[port][slot].open == 0) return 99;
    r = DIntr();
    p = scePadGetDmaStr(port, slot);
    if (p[0x70] == 6 && p[0x71] == 2) {
        if (r == 1) EIntr();
        return 5;
    }
    if (r == 1) EIntr();
    return p[0x70];
}
