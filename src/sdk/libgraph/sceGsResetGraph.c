// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    short sceGsInterMode;
    short sceGsOutMode;
    short sceGsFFMode;
    short sceGsVersion;
    int vsync_id;
    int handler_id;
} sceGsGParam;
sceGsGParam *sceGsGetGParam(void);
int sceGsPutIMR(unsigned long);
int DisableIntc(int);
int RemoveIntcHandler(int, int);
void SetGsCrt(short, short, short);
void sceGsResetGraph(short mode, short inter, short omode, short ffmode)
{
    sceGsGParam *gp;
    switch (mode) {
    case 0:
        gp = sceGsGetGParam();
        *(volatile unsigned long *)0x12001000 = 0x200;
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = omode;
        gp->sceGsVersion = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        sceGsPutIMR(0xFF00);
        gp->sceGsFFMode = (ffmode != 0);
        if (gp->vsync_id) {
            DisableIntc(2);
            RemoveIntcHandler(2, gp->handler_id);
            gp->vsync_id = 0;
            gp->handler_id = 0;
        }
        SetGsCrt(inter & 1, omode & 0xFF, ffmode & 1);
        break;
    case 1:
        *(volatile unsigned long *)0x12001000 = 0x100;
        break;
    case 5:
        gp = sceGsGetGParam();
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = omode;
        gp->sceGsFFMode = (ffmode != 0);
        gp->sceGsVersion = (*(volatile unsigned long *)0x12001000 >> 16) & 0xFF;
        SetGsCrt(inter & 1, omode & 0xFF, ffmode & 1);
        break;
    }
}
