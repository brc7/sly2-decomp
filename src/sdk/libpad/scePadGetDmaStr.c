// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char *buf; int x4; int x8; int xC; int open; int x14; int x18; } PadSt;
extern PadSt D_004505D0[2][4];
typedef struct { char x[0x58]; int frame; char y[0x24]; } PadData;
void func_00234F48(void *, void *);
unsigned char *scePadGetDmaStr(int port, int slot)
{
    PadData *p = (PadData *)D_004505D0[port][slot].buf;
    func_00234F48(p, (char *)p + 0xFF);
    { int id = 0; if (p[0].frame < p[1].frame) id = 1; return (unsigned char *)(p + id); }
}
