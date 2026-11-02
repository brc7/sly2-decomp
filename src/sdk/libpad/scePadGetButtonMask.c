// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char *buf; int x4; int x8; int xC; int open; int x14; int x18; } PadSt;
extern PadSt D_004505D0[2][4];
unsigned char *scePadGetDmaStr(int port, int slot);
int scePadGetButtonMask(int port, int slot)
{
    unsigned char *p;
    if (D_004505D0[port][slot].open == 0) return 0;
    p = scePadGetDmaStr(port, slot);
    if (p[0x72] != 1) return 0;
    if (p[0x64] < 2) return 0;
    if (p[0x66] < 2) return 0;
    return (long)p[0x79] + ((long)p[0x7A] << 8) + ((long)p[0x7B] << 16) + ((long)p[0x7C] << 24);
}
