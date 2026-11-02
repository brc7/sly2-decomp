// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned long u_long;
typedef struct { u_long pmode; u_long smode2; u_long dispfb; u_long display; u_long bgcolor; } sceGsDispEnv;
typedef struct { short sceGsInterMode, sceGsOutMode, sceGsFFMode, sceGsVersion; } sceGsGParam;
sceGsGParam *sceGsGetGParam(void);
#define REG(a) (*(volatile u_long *)(a))
void sceGsPutDispEnv(sceGsDispEnv *disp)
{
    if (sceGsGetGParam()->sceGsVersion == 1) {
        REG(0x12000000) = disp->pmode;
        REG(0x12000070) = disp->dispfb;
        REG(0x12000080) = disp->display;
        REG(0x120000C0) = disp->bgcolor;
    } else {
        REG(0x12000000) = disp->pmode;
        REG(0x12000020) = disp->smode2;
        REG(0x12000090) = disp->dispfb;
        REG(0x120000A0) = disp->display;
        REG(0x120000E0) = disp->bgcolor;
    }
}
