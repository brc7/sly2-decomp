struct DB { union { int w; unsigned char b[4]; } u; int pad[9]; };
extern char D_002DFB40[];
extern struct DB D_002DEB80[2];
extern char D_002DEDC0[], D_002DF260[], D_002DF010[], D_002DF4B0[], D_002DF700[], D_002DF720[];
extern int D_002DFB4C, D_002DFB50;
void InitCritSect__FP8CRITSECT(void *p);
void sceGsResetGraph(int a, int b, int c, int d);
void sceGsSetDefDBuff(struct DB *p, int a, int b, int c, int d, int e, int f);
void BuildClearGifs__FP2QWG4RGBAi(void *p, int *pc, int n);
void BuildFbaGifs__FiP2QWl(int a, void *p, int n);
void RebuildFrameGifs__Fv(void);
int SemaCreate__Fii(int a, int b);
void VBlankS_Interrupt__Fi(void);
void ResetGs__Fv(void);
void AddIntcHandler(int a, void (*pfn)(void), int b);
void func_00235178(int a);
void GS_Interrupt__Fi(void);
/** @sly2-annot addr=0x1700D0
 * Purpose: GS startup: crit-sect, sceGsResetGraph, double buffers, build clear packets and FBA packets, frame
 *   context packets, semaphores, VBlank/GS interrupt handlers
 * Info: proposed name StartupGs (T1); TU gs
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/overview.md#1-boot, docs/engine/rendering.md#1-frame-structure,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
void StartupGs__Fv(void) {
    int i;
    int c;
    InitCritSect__FP8CRITSECT(D_002DFB40);
    sceGsResetGraph(0, 1, 2, 1);
    sceGsSetDefDBuff(D_002DEB80, 0, 0x200, 0xE0, 3, 0x30, 0);
    for (i = 0; i < 2; i++) {
        D_002DEB80[i].u.w &= ~1;
        D_002DEB80[i].u.b[1] = 0x80;
        D_002DEB80[i].u.w |= 0x20;
    }
    c = 0x80000000;
    BuildClearGifs__FP2QWG4RGBAi(D_002DEDC0, &c, 3);
    c = 0x80000000;
    BuildClearGifs__FP2QWG4RGBAi(D_002DF260, &c, 3);
    c = 0x80000000;
    BuildClearGifs__FP2QWG4RGBAi(D_002DF010, &c, 2);
    c = 0x80000000;
    BuildClearGifs__FP2QWG4RGBAi(D_002DF4B0, &c, 1);
    BuildFbaGifs__FiP2QWl(2, D_002DF700, 1);
    BuildFbaGifs__FiP2QWl(2, D_002DF720, 0);
    RebuildFrameGifs__Fv();
    D_002DFB4C = SemaCreate__Fii(0, 1);
    D_002DFB50 = SemaCreate__Fii(0, 1);
    AddIntcHandler(2, VBlankS_Interrupt__Fi, 0);
    func_00235178(2);
    AddIntcHandler(0, ResetGs__Fv, 0);
    func_00235178(0);
    GS_Interrupt__Fi();
    *(volatile int *)0x10003010 = 0;
}
