// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* _send_to_iop (libpad): DMA the 0x20-byte pad command to the IOP double buffer (Claude agent). */
typedef struct { int x0; int x4; int x8; unsigned char data[0x14]; } PadCmd;
typedef struct { unsigned char *buf; PadCmd *cmd; unsigned int dest; unsigned int dmaid; int open; int x14; int x18; } PadSt;
typedef struct { void *src; void *dest; int size; int attr; } SifDmaData;
extern PadSt D_004505D0[2][4];
extern int D_002C830C;
extern char D_002AB6D0[];
int func_00234DC0(unsigned int);
void func_00234F48(void *, void *);
unsigned int func_00234DE0(SifDmaData *, int);
void func_00236960(const char *, ...);

int _send_to_iop(int port, int slot)
{
    SifDmaData dma[16];
    PadCmd *c;
    unsigned int id;
    unsigned int dest;
    int n;

    c = D_004505D0[port][slot].cmd;
    if (D_004505D0[port][slot].dmaid != 0 && func_00234DC0(D_004505D0[port][slot].dmaid) >= 0) {
        if (D_002C830C)
            func_00236960(D_002AB6D0);
        return 0;
    }
    n = c->x0 + 1;
    dest = D_004505D0[port][slot].dest + ((n & 1) << 5);
    c->x0 = n;
    func_00234F48(c, (char *)c + 0x1F);
    dma[0].dest = (void *)dest;
    dma[0].src = c;
    dma[0].size = 0x20;
    dma[0].attr = 0;
    id = func_00234DE0(dma, 1);
    if (id == 0) {
        if (D_002C830C)
            func_00236960(D_002AB6D0);
        return 0;
    }
    D_004505D0[port][slot].dmaid = id;
    return 1;
}
