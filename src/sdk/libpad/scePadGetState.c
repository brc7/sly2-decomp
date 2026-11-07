// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char *buf; int x4; int x8; int xC; int open; int x14; int x18; } PadSt;
extern PadSt D_004505D0[2][4];
unsigned char *scePadGetDmaStr(int port, int slot);
// libpad: returns the state of the pad on port/slot, or 0 when that port/slot is not open. The state is byte 0x71
// of the pad's DMA receive buffer (scePadGetDmaStr).
// (This comment keeps the five lines of the draft block it replaced: the file uses SLY2_ASM gnu-reorder-db, which
// cannot be built in every environment, so its line layout is left exactly as it was matched.)
//
int scePadGetState(int port, int slot)
{
    if (D_004505D0[port][slot].open == 0) return 0;
    return scePadGetDmaStr(port, slot)[0x71];
}
