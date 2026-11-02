// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char *buf; int x4; int x8; int xC; int open; int x14; int x18; } PadSt;
extern PadSt D_004505D0[2][4];
unsigned char *scePadGetDmaStr(int port, int slot);
int scePadGetReqState(int port, int slot)
{
    if (D_004505D0[port][slot].open == 0) return 0;
    return scePadGetDmaStr(port, slot)[0x71];
}
