// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef int u_long128 __attribute__((mode(TI)));
typedef struct { u_long NLOOP:15, EOP:1, pad16:16, id:14, PRE:1, PRIM:11, FLG:2, NREG:4;
                 u_long REGS0:4, REGS1:4, REGS2:4, REGS3:4, REGS4:4, REGS5:4, REGS6:4, REGS7:4,
                        REGS8:4, REGS9:4, REGS10:4, REGS11:4, REGS12:4, REGS13:4, REGS14:4, REGS15:4; } sceGifTag;
typedef struct { u_long FBP:9, FBW:6, PSM:5, p0:12, DBX:11, DBY:11, p1:10; } sceGsDispfb;
typedef struct { u_long FBP:9, p0:7, FBW:6, p1:2, PSM:6, p2:2, FBMSK:32; } sceGsFrame;
typedef struct { u_long pmode, smode2; sceGsDispfb dispfb; u_long display, bgcolor; } sceGsDispEnv;
typedef struct { sceGsFrame frame1; u_long frame1addr; u_long d[14]; } sceGsDrawEnv1;
typedef struct { u_long d[12]; } sceGsClear;
typedef struct {
    sceGsDispEnv disp[2];
    sceGifTag giftag0;
    sceGsDrawEnv1 draw0;
    sceGsClear clear0;
    sceGifTag giftag1;
    sceGsDrawEnv1 draw1;
    sceGsClear clear1;
} sceGsDBuff;
typedef struct { short sceGsInterMode, sceGsOutMode, sceGsFFMode, sceGsVersion; } __attribute__((aligned(8))) sceGsGParam;
sceGsGParam *sceGsGetGParam(void);
void sceGsSetDefDispEnv(sceGsDispEnv *, short, short, short, short, short);
int sceGsSetDefDrawEnv(sceGsDrawEnv1 *, short, short, short, short, short);
int sceGsSetDefClear(sceGsClear *, short, short, short, short, short, unsigned char, unsigned char, unsigned char, unsigned char, u_int);
int func_00240FB0(short, short, short);
void sceGsSetDefDBuff(sceGsDBuff *dp, short psm, short w, short h, short ztest, short zpsm, short clear)
{
    sceGsGParam *gp = sceGsGetGParam();
    int fbp;

    sceGsSetDefDispEnv(&dp->disp[0], psm, w, h, 0, 0);
    sceGsSetDefDispEnv(&dp->disp[1], psm, w, h, 0, 0);
    sceGsSetDefDrawEnv(&dp->draw0, psm, w, h, ztest, zpsm);
    sceGsSetDefDrawEnv(&dp->draw1, psm, w, h, ztest, zpsm);
    if (clear) {
        sceGsSetDefClear(&dp->clear0, ztest, 2048 - (w >> 1), 2048 - (h >> 1), w, h, 0, 0, 0, 0, 0);
        sceGsSetDefClear(&dp->clear1, ztest, 2048 - (w >> 1), 2048 - (h >> 1), w, h, 0, 0, 0, 0, 0);
    }
    *(u_long128 *)&dp->giftag0 = 0;
    *(u_long128 *)&dp->giftag1 = 0;
    dp->giftag0.NLOOP = clear ? 14 : 8;
    dp->giftag0.EOP = 1;
    dp->giftag0.NREG = 1;
    dp->giftag0.REGS0 = 0xE;
    dp->giftag1.NLOOP = clear ? 14 : 8;
    dp->giftag1.EOP = 1;
    dp->giftag1.NREG = 1;
    dp->giftag1.REGS0 = 0xE;
    fbp = func_00240FB0(psm, w, h);
    if ((gp->sceGsInterMode == 1 && gp->sceGsFFMode == 1) || gp->sceGsInterMode == 0) {
        short s = fbp >> 1;
        dp->disp[1].dispfb.FBP = s;
        dp->draw0.frame1.FBP = fbp >> 1;
    }
}
