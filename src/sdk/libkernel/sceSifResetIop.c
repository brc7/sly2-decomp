// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    unsigned int psize:8;
    unsigned int dsize:24;
    unsigned int dest;
    int cid;
    unsigned int opt;
} SifCmdHeader;
typedef struct {
    SifCmdHeader hdr;
    int size;
    int flag;
    char arg[80];
} SifCmdResetData;
typedef struct {
    void *src;
    void *dest;
    int size;
    int attr;
} SifDmaTransfer;
extern SifCmdResetData D_0044C280 __attribute__((aligned(64)));
void func_0023A920(int, int);
void func_00234CF0(void);
unsigned int func_00234E30(unsigned int);
unsigned int func_00234E20(unsigned int, unsigned int);
void func_0023B170(void *, int);
unsigned int func_00234DE0(SifDmaTransfer *, int);
int sceSifResetIop(const char *arg, int mode)
{
    SifDmaTransfer dmat;
    void *dest;
    int i;

    func_0023A920(0, 0);
    func_0023A920(1, 0);
    func_00234CF0();
    dest = (void *)func_00234E30(0x80000000);
    D_0044C280.flag = mode;
    for (i = 0; arg[i] != 0; i++)
        D_0044C280.arg[i] = arg[i];
    D_0044C280.hdr.dest = 0;
    D_0044C280.hdr.dsize = 0;
    D_0044C280.size = i;
    D_0044C280.hdr.cid = 0x80000003;
    D_0044C280.hdr.psize = sizeof(SifCmdResetData);
    dmat.src = &D_0044C280;
    dmat.dest = dest;
    dmat.size = sizeof(SifCmdResetData);
    dmat.attr = 0x44;
    func_0023B170(&D_0044C280, sizeof(SifCmdResetData));
    func_00234E20(4, 0x40000);
    if (func_00234DE0(&dmat, 1) != 0) {
        func_00234E20(4, 0x10000);
        func_00234E20(4, 0x20000);
        func_00234E20(0x80000002, 0);
        func_00234E20(0x80000000, 0);
        return 1;
    }
    return 0;
}

