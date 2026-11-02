// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;
typedef struct {
    u_char sts, std, mfd, rcyc;
    u_short express, notify;
    u_short sqwc, tqwc;
    void *rbadr;
    u_int rbmsk;
} sceDmaEnv;
typedef struct { u_int chcr; u_int p0[3]; u_int madr; u_int p1[3]; u_int qwc; u_int p2[3]; u_int tadr; u_int p3[3];
    u_int asr0; u_int p4[3]; u_int asr1; u_int p5[3]; u_int p6[8]; u_int sadr; } sceDmaChan;
#define D_CTRL (*(volatile u_int *)0x1000E000)
#define D_STAT (*(volatile u_int *)0x1000E010)
extern int D_002AB380[];
extern sceDmaChan *D_002C6710[];
void func_00241408(void *p, int n);
int sceDmaPutEnv(sceDmaEnv *env);
int sceDmaReset(int mode)
{
    sceDmaEnv env;
    int i;
    int ret = D_CTRL & 1;
    for (i = 0; i < 10; i++) {
        if (D_002AB380[i]) {
            volatile sceDmaChan *d = D_002C6710[i];
            d->sadr = 0;
            d->chcr = 0;
            d->tadr = 0;
            d->madr = 0;
            d->asr1 = 0;
            d->asr0 = 0;
        }
    }
    D_STAT = 0xFF1F;
    D_STAT = D_STAT & 0xFF1F0000;
    func_00241408(&env, sizeof(env));
    sceDmaPutEnv(&env);
    if (mode == 1) D_CTRL |= 1;
    return ret;
}
