// main.cpp -- the game's main(): startup (IOP modules, VU microcode, the staged init with a progress bar) and the
// frame loop.
// crt0's _start calls GameMain, which never returns: Startup runs the 19 boot stages (table D_002BFCE0), then each
// frame does world transitions, input, disc poll, camera, clock, world update, UI update, render and draw.
// Entry points: GameMain, Startup, StartupSif, StartupVU0, StartupVU1.
// TU 0x1A7518..0x1A7C18 (7 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_main
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_main
#include "sly2/slotheap.h"
#include "sly2/clock.h"
#include "sly2/cm.h"
#include "sly2/joy.h"
#include "sly2/sw.h"

// 0x1A7AC0 StaticInit_main and GlobalConstructors_main: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

struct JOY; struct CM;
enum PHASE { PHASE_1 = 1 };

int D_002E2050, D_002E2054;             /* argc, argv (main's .bss, address order) */
extern void *D_002DD644;
extern int D_002BE538;                  /* frame count */
extern char D_003D9E70[];
extern "C" {
void StartupGame(void);
void ExecuteSwTransition(void *);
void MarkClockTick(void *, int);
void UpdateSw(void *);
void UpdateScreens(void *);
int IsFrameCaptureInactive(void *);
void RenderSw(void *);
void DrawSw(void *);
void DrawUi(void *);
void DrawFrameCaptureScreen(void *);
}
/* Callees with mangled symbols, declared as the C++ functions that mangle to them. */
void __main();
void Startup();
void UpdateJoy(JOY *pjoy);
void UpdateCd();
void SetupCm(CM *pcm);
void OpenFrame();
void CloseFrame();
int IsPs2DvdInDrive();
void SetCdMmode();
void CdPath(char *pchzDst, char *pchzSrc, int f);
int IsCdCommandComplete(int f);
void SetPhase(PHASE phase);
void StartupGs();

// The program's main (crt0 calls it; naming it main would make g++ insert a second __main call, so it keeps
// another name). Saves argc/argv, runs the boot stages and the game start-up, then loops forever, one iteration per
// frame: pending world transition, pad, disc poll, camera setup, clock tick, world update, UI update (D_003D9E70),
// then render and draw (skipped when the object at D_002DD644 exists and func_001477B0 says no; it is notified after
// each frame) and the frame counter.
extern "C" void GameMain(int argc, int argv)
{
    __main();
    D_002E2050 = argc;
    D_002E2054 = argv;
    Startup();
    StartupGame();
    for (;;) {
        ExecuteSwTransition(&g_sw);
        UpdateJoy(&g_joy);
        UpdateCd();
        SetupCm(&g_cm);
        MarkClockTick(&g_clock, 0);
        UpdateSw(&g_sw);
        UpdateScreens(D_003D9E70);
        OpenFrame();
        if (!D_002DD644 || IsFrameCaptureInactive(D_002DD644)) {
            RenderSw(&g_sw);
            DrawSw(&g_sw);
            DrawUi(D_003D9E70);
        }
        if (D_002DD644)
            DrawFrameCaptureScreen(D_002DD644);
        CloseFrame();
        D_002BE538++;
    }
}

/* An IOP module image (table D_004E5D80..D_004E5E38) and a SIF DMA transfer. */
struct E { int bytes; unsigned int source; int argumentBytes; const char *arguments; int unknown10; };
struct DMA { unsigned int source; unsigned int destination; int bytes; int attributes; };
extern struct E *D_004E5D80;
extern struct E *D_004E5E38;
int D_002E2058;     /* main's .bss: skip the IOP reboot */
extern const char D_002A6A48[] = "ioprp300.img";
extern "C" {
void func_00236A60(int n) __asm__("sceSifInitRpc");
int sceSifLoadFileReset(void);
int sceCdInit(int n);
int sceFsReset(void);
int sceSifRebootIop(const char *p);
int sceSifSyncIop(void);
int sceCdDiskReady_2425F0(int n);
int sceSifInitIopHeap(void);
void *sceSifAllocIopHeap(int cb);
void func_00234CA0(int n) __asm__("FlushCache");
int sceSifSetDma(struct DMA *p, int n);
int sceSifLoadModuleBuffer(void *h, int argumentBytes, const char *arguments);
int sceSifFreeIopHeap(void *h);
}
// Copy value through a register move (keeps the original daddu).
static inline int CopyLimit(int value)
{
    int copy;
    __asm__("daddu %0,%1,$zero" : "=r"(copy) : "r"(value));
    return copy;
}

// Boot stage: brings up the IOP. Initialises SIF RPC and the CD (hanging if the disc is not a PS2 DVD), reboots
// the IOP with cdrom0:\IOPRP300.IMG;1 (unless D_002E2058 is set) and re-initialises, then copies each IRX module
// embedded in the ELF (table D_004E5D80..D_004E5E38) into an IOP heap buffer by SIF DMA in <= 0xFFC0-byte pieces
// and loads it.
void StartupSif()
{
    union { char buf[0x100]; struct DMA dma; } u;
    struct E *module;
    int maxModuleSize;
    void *iopBuffer;
    func_00236A60(0);
    sceSifLoadFileReset();
    sceCdInit(0);
    if (!IsPs2DvdInDrive())
        while (1)
            ;
    SetCdMmode();
    sceFsReset();
    if (!D_002E2058) {
        CdPath(u.buf, (char *)D_002A6A48, 1);
        while (!sceSifRebootIop(u.buf))
            ;
        while (!sceSifSyncIop())
            ;
        func_00236A60(0);
        sceSifLoadFileReset();
        sceCdInit(0);
        if (!IsPs2DvdInDrive())
            while (1)
                ;
        SetCdMmode();
        sceFsReset();
    }
    sceCdDiskReady_2425F0(0);
    (void)IsCdCommandComplete(1);
    sceSifInitIopHeap();
    maxModuleSize = 0;
    {
        struct E *entry;
        for (entry = D_004E5D80; entry < D_004E5E38; entry++)
            if (entry->bytes >= maxModuleSize)
                maxModuleSize = entry->bytes;
    }
    iopBuffer = sceSifAllocIopHeap(maxModuleSize);
    for (module = D_004E5D80; module < D_004E5E38; module++) {
        int bytesLeft = module->bytes;
        int offset = 0;
        while (bytesLeft) {
            register int limit __asm__("$2") = 0xF0000;
            int chunk;
            const int zeroOperation = 0, limitLow = 0xFFC0;
            register unsigned int destination __asm__("$5");
            register int operation __asm__("$4");
            __asm__("ori %0,%0,%1" : "+r"(limit) : "i"(limitLow));
            __asm__("addu %0,%1,%2" : "=r"(destination) : "r"(iopBuffer), "r"(offset));
            chunk = CopyLimit(limit);
            __asm__("slt %0,%1,%0" : "+r"(limit) : "r"(bytesLeft));
            __asm__("movn %0,%1,%2" : "+r"(chunk) : "r"(bytesLeft), "r"(limit));
            u.dma.source = module->source + offset;
            __asm__("daddu %0,%z3,$zero\n\tsw %2,%1"
                    : "=r"(operation), "=m"(u.dma.destination)
                    : "r"(destination), "rJ"(zeroOperation));
            u.dma.bytes = chunk;
            u.dma.attributes = 0;
            func_00234CA0(operation);
            while (!sceSifSetDma(&u.dma, 1))
                ;
            bytesLeft -= chunk;
            offset += chunk;
        }
        while (sceSifLoadModuleBuffer(iopBuffer, module->argumentBytes, module->arguments) < 0)
            ;
    }
    sceSifFreeIopHeap(iopBuffer);
}

typedef char offset_0_bytes[(unsigned)&((struct E*)0)->bytes==0?1:-1];
typedef char offset_0_source[(unsigned)&((struct E*)0)->source==4?1:-1];
typedef char offset_0_argumentBytes[(unsigned)&((struct E*)0)->argumentBytes==8?1:-1];
typedef char offset_0_arguments[(unsigned)&((struct E*)0)->arguments==12?1:-1];
typedef char offset_0_unknown10[(unsigned)&((struct E*)0)->unknown10==16?1:-1];
typedef char offset_1_source[(unsigned)&((struct DMA*)0)->source==0?1:-1];
typedef char offset_1_destination[(unsigned)&((struct DMA*)0)->destination==4?1:-1];
typedef char offset_1_bytes[(unsigned)&((struct DMA*)0)->bytes==8?1:-1];
typedef char offset_1_attributes[(unsigned)&((struct DMA*)0)->attributes==12?1:-1];
typedef char size_0[sizeof(struct E)==20?1:-1];
typedef char size_1[sizeof(struct DMA)==16?1:-1];
typedef char size_2[sizeof(int)==4?1:-1];
typedef char size_3[sizeof(unsigned int)==4?1:-1];
typedef char size_4[sizeof(void*)==4?1:-1];
typedef char size_5[sizeof(char)==1?1:-1];
typedef char alignment_0[__alignof__(struct E)==4?1:-1];
typedef char alignment_1[__alignof__(struct DMA)==4?1:-1];

extern int *g_pdcVif0;     /* VIF0 DMA channel */
extern int *g_pdcVif1;     /* VIF1 DMA channel */
extern char D_00259DF0[];   /* VU0 microcode upload chain */
extern char D_0025E050[];   /* VU1 microcode upload chain */
extern "C" {
void FlushCache(int n);
void sceDmaSend(int *pch, void *p);
void sceDmaSync(int *pch, int a, int b);
}

// Boot stage: uploads the VU0 microcode by VIF0 DMA (chain D_00259DF0).
void StartupVU0()
{
    *g_pdcVif0 |= 0x40;
    FlushCache(0);
    sceDmaSend(g_pdcVif0, D_00259DF0);
    sceDmaSync(g_pdcVif0, 0, 0);
    *g_pdcVif0 &= ~0x40;
}

// Boot stage: uploads the VU1 microcode by VIF1 DMA (chain D_0025E050).
void StartupVU1()
{
    *g_pdcVif1 |= 0x40;
    FlushCache(0);
    sceDmaSend(g_pdcVif1, D_0025E050);
    sceDmaSync(g_pdcVif1, 0, 0);
    *g_pdcVif1 &= ~0x40;
}

/* The startup progress bar (CProg, 0x70 bytes) and its colours. Its members are called under their linker names
   on a byte view, as the per-function file did (modelling CProg as a class is a separate step). */
struct CPROGV { char data[0x70]; };
struct RGBAV { int c; } __attribute__((aligned(16)));
/* The start-up banner (.rodata) and its two arguments, the region and the build number, with the build date between
 * them (.data, address order; the date is named by no code). The ELF has the three strings far from main's other
 * data, after xf's .data (a build-stamp TU later in the link order, presumably), so they get a section of their own
 * (.data.buildinfo; tu-merging.md "Phase 6: one link mode"). */
extern const char D_002A6A58[] = "Sly 2: Band of Thieves \xa9 2004\n  Sony Computer Entertainment America &\n  Sucker Punch Productions\nP3: %s %s\nprops: %d\n\n";
char D_002C46C0[] __attribute__((section(".data.buildinfo"))) = "america";
char D_002C46C8[] __attribute__((section(".data.buildinfo"))) = "08/13/04 00:32";
char D_002C46D8[] __attribute__((section(".data.buildinfo"))) = "0813.0032";
extern void (*D_002BFCE0[])(void);      /* the 19 startup stages */
extern "C" {
struct CPROGV *__5CProgP4RGBAN31(struct CPROGV *b, struct RGBAV *c0, struct RGBAV *c1, struct RGBAV *c2, struct RGBAV *c3);
int printf(const char *format, ...);
void Begin__5CProg(struct CPROGV *b);
void SetRemain__5CProgi(struct CPROGV *b, int n);
void End__5CProg(struct CPROGV *b);
void ClearPhase(int n);
}

/* Run the 19 initialization stages with CProg color and remaining-count updates. */
// Runs the 19 boot stages in order with a progress bar (shown once the GS is up, i.e. after StartupGs), ticking the
// clock before each; prints the build banner ("P3: 0813.0032 america").
void Startup()
{
    struct CPROGV progressBar;
    struct RGBAV color0, color1, color2, color3;
    unsigned int stage;
    int showProgress;
    {
        /* The bar's four RGBA colours (alpha 0x80). Pinned registers: plain constant stores load them in another
           order (no match). */
        register unsigned int rgba2 __asm__("$5") = 0x80003F3F;
        register unsigned int rgba3 __asm__("$6") = 0x8000003F;
        register unsigned int rgba0 __asm__("$3") = 0x807F0000;
        register unsigned int rgba1 __asm__("$2") = 0x803F3F3F;
        color2.c = rgba2;
        color3.c = rgba3;
        color0.c = rgba0;
        color1.c = rgba1;
    }
    __5CProgP4RGBAN31(&progressBar, &color0, &color1, &color2, &color3);
    printf(D_002A6A58, D_002C46D8, D_002C46C0, 0x2B0);
    SetPhase(PHASE_1);
    Begin__5CProg(&progressBar);
    showProgress = 0;
    for (stage = 0; stage < 19; stage++) {
        MarkClockTick(&g_clock, 1);
        if (showProgress)
            SetRemain__5CProgi(&progressBar, 19 - stage);
        else if (D_002BFCE0[stage] == StartupGs)
            showProgress = 1;
        D_002BFCE0[stage]();
    }
    SetRemain__5CProgi(&progressBar, 0);
    End__5CProg(&progressBar);
    ClearPhase(1);
}

// ---- Data owned by this TU, in address order (docs/guides/tu-merging.md "Data"). Defined after the functions
// so no function sees a definition (keeps the code as matched); the namespaced declarations above refer to these.
/* The 19 startup stages and the terminating 0; the stages are bound to their linker names with asm labels. */
namespace M_main_data {
void StartupDma() __asm__("StartupDma__Fv");
void StartupThread() __asm__("StartupThread__Fv");
void StartupSif() __asm__("StartupSif__Fv");
void StartupGs() __asm__("StartupGs__Fv");
void StartupMemMgr() __asm__("StartupMemMgr__Fv");
void StartupCd() __asm__("StartupCd__Fv");
void SetCdPollStateIdle() __asm__("SetCdPollStateIdle");
void StartupFont() __asm__("StartupFont__Fv");
void StartupJoy() __asm__("StartupJoy__Fv");
void StartupFrame() __asm__("StartupFrame__Fv");
void StartupLoadingScreen() __asm__("StartupLoadingScreen");
void StartupClock() __asm__("StartupClock");
void StartupVU0() __asm__("StartupVU0__Fv");
void StartupVU1() __asm__("StartupVU1__Fv");
void StartupSound() __asm__("StartupSound");
void StartupBlips() __asm__("StartupBlips__Fv");
void StartupHudGifPackets() __asm__("StartupHudGifPackets");
void StartupMemcard() __asm__("StartupMemcard__Fv");
void StartupMpeg() __asm__("StartupMpeg__Fv");
void (*D_002BFCE0[20])(void) __asm__("D_002BFCE0") = {
    StartupDma, StartupThread, StartupSif, StartupGs, StartupMemMgr, StartupCd, SetCdPollStateIdle, StartupFont,
    StartupJoy, StartupFrame, StartupLoadingScreen, StartupClock, StartupVU0, StartupVU1, StartupSound, StartupBlips,
    StartupHudGifPackets, StartupMemcard, StartupMpeg, 0
};
}
