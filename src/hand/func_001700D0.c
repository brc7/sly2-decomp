struct DB { union { int w; unsigned char b[4]; } u; int pad[9]; };
extern char D_002DFB40[];
extern struct DB D_002DEB80[2];
extern char D_002DEDC0[], D_002DF260[], D_002DF010[], D_002DF4B0[], D_002DF700[], D_002DF720[];
extern int D_002DFB4C, D_002DFB50;
void func_0021F028(void *p);
void func_00240460(int a, int b, int c, int d);
void func_002407A8(struct DB *p, int a, int b, int c, int d, int e, int f);
void func_0016F960(void *p, int *pc, int n);
void func_0016FAF8(int a, void *p, int n);
void func_00170048(void);
int func_0021EFD0(int a, int b);
void func_0016F468(void);
void func_0016F868(void);
void func_00234700(int a, void (*pfn)(void), int b);
void func_00235178(int a);
void func_0016F788(void);
/** @sly2-annot addr=0x1700D0
 * Purpose: GS startup: crit-sect, sceGsResetGraph, double buffers, build clear packets and FBA packets, frame
 *   context packets, semaphores, VBlank/GS interrupt handlers
 * Info: proposed name StartupGs (T1); TU gs
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/overview.md#1-boot, docs/engine/rendering.md#1-frame-structure,
 *   docs/engine/rendering.md#13-key-functions
 * @end-sly2-annot */
void func_001700D0(void) {
    int i;
    int c;
    func_0021F028(D_002DFB40);
    func_00240460(0, 1, 2, 1);
    func_002407A8(D_002DEB80, 0, 0x200, 0xE0, 3, 0x30, 0);
    for (i = 0; i < 2; i++) {
        D_002DEB80[i].u.w &= ~1;
        D_002DEB80[i].u.b[1] = 0x80;
        D_002DEB80[i].u.w |= 0x20;
    }
    c = 0x80000000;
    func_0016F960(D_002DEDC0, &c, 3);
    c = 0x80000000;
    func_0016F960(D_002DF260, &c, 3);
    c = 0x80000000;
    func_0016F960(D_002DF010, &c, 2);
    c = 0x80000000;
    func_0016F960(D_002DF4B0, &c, 1);
    func_0016FAF8(2, D_002DF700, 1);
    func_0016FAF8(2, D_002DF720, 0);
    func_00170048();
    D_002DFB4C = func_0021EFD0(0, 1);
    D_002DFB50 = func_0021EFD0(0, 1);
    func_00234700(2, func_0016F468, 0);
    func_00235178(2);
    func_00234700(0, func_0016F868, 0);
    func_00235178(0);
    func_0016F788();
    *(volatile int *)0x10003010 = 0;
}
