// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p[0x24]; int server; } RpcCD;
extern RpcCD D_002C82C0;
extern volatile int D_002C676C;
extern int D_002C6750;
extern int D_002C6758;
extern int D_002C6794;
extern char D_002AB530[];
extern char D_002AB558[];
void func_00241A30(void);
int func_00234A70(int);
int func_00234A40(int);
int func_00236960(const char *, ...);
int func_00241F28(int);
void func_00236A60(int);
int func_002370A8(void *, unsigned int, int);
static __inline__ void nopdelay(void)
{
    int i = 0x100000;
    while (i--) ;
}
int _sceCd_scmd_prechk(int cmd)
{
    int i;
    func_00241A30();
    if (D_002C676C != func_00234A70(D_002C676C)) {
        if (D_002C6750 > 0) func_00236960(D_002AB530, cmd, D_002C6758);
        return 0;
    }
    D_002C6758 = cmd;
    if (func_00241F28(1)) {
        func_00234A40(D_002C676C);
        return 0;
    }
    func_00236A60(0);
    if (D_002C6794 < 0) {
        while (1) {
            if (func_002370A8(&D_002C82C0, 0x80000593, 0) < 0) {
                if (D_002C6750 > 0) func_00236960(D_002AB558);
                nopdelay();
                continue;
            }
            if (D_002C82C0.server != 0) break;
            nopdelay();
        }
        D_002C6794 = 0;
    }
    return 1;
}
