// g_pfrmOpen: Sly 1 name of the current-frame pointer (symbol_addrs g_pfrmOpen, set in OpenFrame); D_002DEA4C in config.
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
typedef struct { qword r[4]; } M4;
typedef struct { float x, y, z; } V3;
typedef struct { V3 v; float w; } V4;
typedef struct RPL { int k; float z; int a8; int aC; } RPL;
typedef struct VIFS { int pad[3]; qword *pqw; int pad2[4]; int f20; } VIFS;
typedef struct FR { char pad[0x24]; qword *pqw24; } FR;
typedef struct CM { char pad[0x4C]; float sNear; float sFar; char pad2[0x60 - 0x54]; M4 m60; char pad3[0x140 - 0xA0]; M4 m140; } CM;
typedef struct GAME { char pad[0x3E28]; int a3E28[4]; int f3E38; int f3E3C; char pad2[0x3EA8 - 0x3E40]; int f3EA8; } GAME;
typedef void (*PFN)(VIFS *, int, int);
typedef struct RPLK { PFN pfnBegin; PFN pfnEnd; } RPLK;
extern RPL D_003B3980[];
extern RPL *D_003D4A50;
extern VIFS D_002DEA28;
extern FR *g_pfrmOpen;
extern FR D_002DEA50;
extern char D_002DF800[], D_002DF940[], D_002DFA80[], D_002DEDC0[];
extern char D_002DEA00[];
extern GAME D_003DD490;
extern CM D_002DDF40;
extern V3 D_002C1F00, D_002C1F10;
extern int D_002DE5A4;
extern int D_002BDD20[];
extern RPLK D_002BDD88[];
int func_00157AE0(int c, RPL *arpl, RPL **aprpl, int *aiFirst, int *ac);
void func_00157598(VIFS *p, void *pv);
void func_001574B8(VIFS *p);
void func_002270C8(VIFS *p, int n, void *pv, int m);
void func_00226D10(VIFS *p);
void func_00226CA8(VIFS *p, int a, int b);
void func_00226F28(VIFS *p, int a, int b, void *pv, int n);
void func_0014F768(CM *p, int k, void *pv, M4 *pm0, M4 *pm1);
void func_001576F0(VIFS *p);
void func_001574A0(void *p);
void func_001EA9F0(GAME *p, int n);

/** @sly2-annot addr=0x157FA0
 * Purpose: Builds the frame VIF1 chain: partitions RPLs, emits frame/camera VU constants, then for each RPL
 *   kind-change calls the pass table begin/end hooks D_002BDD88[D_002BDD20[kind]] and emits the RPL DMA CALL
 *   tags; flushes VIFS/GIFS
 * Info: proposed name DrawSw (T1); class SW slot9; TU draw
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#5-per-frame-update-and-render, docs/engine/overview.md#6-subsystem-map
 * @end-sly2-annot */
void func_00157FA0(GAME *pgame)
{
    M4 mat0;
    M4 mat1;
    RPL *aprpl[0x2000];
    int aiFirst[26];
    int ac[26];
    V4 v0, v1;
    int c;
    int kPrev, kindPrev;
    int i;

    c = func_00157AE0(D_003D4A50 - D_003B3980, D_003B3980, aprpl, aiFirst, ac);
    {
        qword *pqw = D_002DEA28.pqw;
        g_pfrmOpen->pqw24 = pqw;
    }
    func_00157598(&D_002DEA28, D_002DF800);
    D_002DEA28.f20 = 1;
    func_001574B8(&D_002DEA28);
    if (pgame->f3E3C || aiFirst[1] == ac[1])
        func_002270C8(&D_002DEA28, 0x25, D_002DEDC0, 0);
    if (D_003DD490.f3E38) {
        func_00226D10(&D_002DEA28);
        if (g_pfrmOpen == &D_002DEA50)
            func_00157598(&D_002DEA28, D_002DF940);
        else
            func_00157598(&D_002DEA28, D_002DFA80);
    }
    func_00226CA8(&D_002DEA28, 0, 0);
    {
        float sNear = D_002DDF40.sNear;
        float sFar = D_002DDF40.sFar;
        float k = (sNear + sFar) / (sNear - sFar);
        float g = sNear * (1.0f - k);
        v0.v = D_002C1F00;
        v0.w = 1.0f / g;
        v1.v = D_002C1F10;
        v1.w = -k / g;
    }
    func_00226F28(&D_002DEA28, 0xC, 2, &v0, 0);
    func_00226F28(&D_002DEA28, 0, 1, &D_002DE5A4, 0xD);
    D_002DE5A4 = 0;
    if (D_003DD490.f3E38) {
        if (g_pfrmOpen == &D_002DEA50)
            func_0014F768(&D_002DDF40, 1, 0, &mat0, &mat1);
        else
            func_0014F768(&D_002DDF40, 2, 0, &mat0, &mat1);
    } else {
        M4 *pm0 = &D_002DDF40.m140;
        M4 *pm1 = &D_002DDF40.m60;
        mat0 = *pm0;
        mat1 = *pm1;
    }
    func_00226F28(&D_002DEA28, 0xC, 4, &mat0, 2);
    func_00226F28(&D_002DEA28, 0xC, 4, &mat1, 6);
    func_00226F28(&D_002DEA28, 0xC, 1, pgame->a3E28, 0xA);
    func_00226CA8(&D_002DEA28, 0x32, 0x1E7);
    func_001576F0(&D_002DEA28);
    kPrev = -1;
    kindPrev = -1;
    for (i = 0; i < c; i++) {
        RPL *prpl = aprpl[i];
        int kind = prpl->k;
        int k = D_002BDD20[kind];
        if (k != kPrev) {
            if (kPrev != -1)
                D_002BDD88[kPrev].pfnEnd(&D_002DEA28, kindPrev, kind);
            D_002BDD88[k].pfnBegin(&D_002DEA28, kind, kindPrev);
            kindPrev = kind;
            kPrev = k;
        }
        if (prpl->a8) {
            *(ulong *)D_002DEA28.pqw = ((ulong)(unsigned int)prpl->a8 << 32) | 0x50000000;
            D_002DEA28.pqw++;
        }
        if (prpl->aC) {
            *(ulong *)D_002DEA28.pqw = ((ulong)(unsigned int)prpl->aC << 32) | 0x50000000;
            D_002DEA28.pqw++;
        }
    }
    if (kPrev != -1)
        D_002BDD88[kPrev].pfnEnd(&D_002DEA28, kindPrev, -1);
    D_003D4A50 = 0;
    func_001574A0(D_002DEA00);
    func_001574A0(&D_002DEA28);
    if (pgame->f3EA8) {
        pgame->f3EA8 = 0;
        func_001EA9F0(pgame, 0);
    }
}
