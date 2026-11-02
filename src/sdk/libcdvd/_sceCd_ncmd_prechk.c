// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { char p[0x24]; int server; } RpcCD;
extern RpcCD D_002C7910;
extern volatile int D_002C6768;
extern int D_002C6750;
extern int D_002C675C;
extern int D_002C677C;
extern char D_002AB4D0[];
extern char D_002AB4F8[];
void func_00241A30(void);
int func_00234A70(int);
int func_00234A40(int);
int func_00236960(const char *, ...);
int func_00241E88(int);
void func_00236A60(int);
int func_002370A8(void *, unsigned int, int);
static __inline__ void nopdelay(void)
{
    int i = 0x100000;
    while (i--) ;
}
int _sceCd_ncmd_prechk(int cmd)
{
    int i;
    func_00241A30();
    if (D_002C6768 != func_00234A70(D_002C6768)) {
        if (D_002C6750 > 0) func_00236960(D_002AB4D0, cmd, D_002C675C);
        return 0;
    }
    D_002C675C = cmd;
    if (func_00241E88(1)) {
        func_00234A40(D_002C6768);
        return 0;
    }
    func_00236A60(0);
    if (D_002C677C < 0) {
        while (1) {
            if (func_002370A8(&D_002C7910, 0x80000595, 0) < 0) {
                if (D_002C6750 > 0) func_00236960(D_002AB4F8);
                nopdelay();
                continue;
            }
            if (D_002C7910.server != 0) break;
            nopdelay();
        }
        D_002C677C = 0;
    }
    return 1;
}
