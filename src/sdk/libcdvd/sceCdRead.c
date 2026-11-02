// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { unsigned char trycount, spindlctrl, datapattern, pad; } sceCdRMode;
typedef struct {
    unsigned int lbn;
    unsigned int sectors;
    void *buf;
    sceCdRMode mode;
    void *eedest;
    void *eeaddr;
} CdReadArg;
extern volatile int D_002C6778;
extern int D_002C6750;
extern volatile int D_002C67A0;
extern volatile int D_002C6774;
extern volatile int D_002C6768;
extern CdReadArg D_002C6840;
extern char D_002C7840[];
extern int D_002C7900;
extern char D_002C7910[];
extern char D_002AB608[];
extern char D_002AB620[];
void func_00241978(void *);
int func_00241DF0(void);
int func_00241C98(int);
void func_0023B170(void *, int);
int func_00236960(const char *, ...);
int func_00234A40(int);
int sceSifCallRpc(void *, int, unsigned int, void *, int, void *, int, void *, void *);
int sceCdRead(unsigned int lbn, unsigned int sectors, void *buf, sceCdRMode *mode)
{
    CdReadArg *rd = &D_002C6840;
    int size;
    if (!(D_002C6778 & 1)) {
        if (func_00241DF0() == 6)
            return 0;
    }
    if (func_00241C98(4) == 0)
        return 0;
    rd->lbn = lbn;
    rd->sectors = sectors;
    rd->buf = buf;
    rd->mode.trycount = mode->trycount;
    rd->mode.spindlctrl = mode->spindlctrl;
    rd->mode.datapattern = mode->datapattern;
    rd->eedest = D_002C7840;
    rd->eeaddr = &D_002C7900;
    switch (mode->datapattern) {
    case 1:
        size = sectors * 2328;
        break;
    case 2:
        size = sectors * 2340;
        break;
    case 0:
    default:
        size = sectors << 11;
        break;
    }
    D_002C7900 = 0;
    if (!(D_002C6778 & 2))
        func_0023B170(buf, size);
    func_0023B170(D_002C7840, 0x90);
    func_0023B170(rd, 0x18);
    func_0023B170(&D_002C7900, 4);
    if (D_002C6750 > 0)
        func_00236960(D_002AB608);
    D_002C67A0 = 1;
    D_002C6774 = 1;
    if (sceSifCallRpc(D_002C7910, 1, 1, rd, 0x18, 0, 0, func_00241978, D_002C7840) < 0) {
        D_002C67A0 = 0;
        D_002C6774 = 0;
        func_00234A40(D_002C6768);
        return 0;
    }
    if (D_002C6750 > 0)
        func_00236960(D_002AB620);
    return 1;
}
