// LoadSwFromBrx = LoadSwFromBrx(SW *psw, CBinaryInputStream *pbis) (Claude agent for Lilly, T1681, from the v2/v3 drafts).
// Reads the level body: the world-entry table (0x6C-byte WE records at psw+0x3F08, count psw+0x3F04), sound section, save vars,
// level table, language strings, cells, options of g_sw / g_cm, then per world entry its objects (LoadSwObjectsFromBrx into a
// CLoadCollectorArray) and the clones of them.
// Callee names (KB): 0x12AE18 BeginSpliceLoad, 0x117D50/0x117DB8 S16Read/S32Read, 0x117968 Read, 0x117988 Align,
// 0x1AE1D0/0x1AE2A0/0x1AE318 PvAllocSw/PvAllocSwClear/PvAllocStack Impl(SWALLOC *), 0x1C9058/90/98 CLoadSpin Begin/SetName/End,
// 0x16F960 BuildClearGifs(QW *, const RGBA &, int), 0x1703F8 InitGsb, 0x258248 InitFloc, 0x1848A8/0x184950 CBinaryInputStream
// ctor/dtor, 0x1AE478/0x1AE4A0 STACKSCOPE ctor/dtor, 0x13F888 CLoadCollectorArray ctor, 0x13FB40 LoadSwObjectsFromBrx,
// 0x13E510 PloCloneLo, 0x14B690 PchzFidName. D_003DD040 = g_sndmgr, D_003DD490 = g_sw, D_002DDF40 = g_cm.
// Matching notes: the alloc helpers take cbAlign as a parameter (its copy lets loop.c hoist the loop's 0x10, which reload then
// rematerialises); the black clear colour is stored as `unsigned int`.
// STAND-IN (same trick as LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream.cpp / func_00219400.cpp): the five spilled gcse reaching pseudos (&gsb1, &frame, &bis,
// &gsb0, &frameT) get their spill slots in the order of the gcse expression hash-table size S = max_cuid/2|1. This source has
// S = 171 after cse1; the game's slot order needs S = 201/203 (~60 more insns). `if (fNever)` is dead code (cprop folds it,
// jump deletes it; the final code is unchanged) that only supplies those insns. What the game really had there is unknown.
extern "C" {
struct BIS;
struct AREQ { int cb; int cbAlign; };
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void *func_001AE1D0(AREQ *preq);
void *func_001AE2A0(AREQ *preq);
void *func_001AE318(AREQ *preq);
void func_001AE5A0(void *pvDst, void *pvSrc, int cb);
void *func_002333B4(void *pv, int b, int cb);
void StartupSplice(void);
int func_00117D50(BIS *pbis);
int func_00117DB8(BIS *pbis);
void func_00117968(BIS *pbis, int cb, void *pv);
void func_00117988(BIS *pbis, int cb);
void func_001C9058(void *p);
void func_001C9090(void *p, void *pv);
void func_001C9098(void *p);
void func_001FD0E8(void *p, BIS *pbis);
void func_001FD5E8(void *p);
void func_001D7B88(void *p, BIS *pbis);
void LoadLevelTableFromBrx(void *psw, BIS *pbis);
void LoadSwLangstrsFromBrx__FP2SWP18CBinaryInputStream(void *psw, BIS *pbis);
void LoadSwCellsFromBrx__FP2SWP18CBinaryInputStream(void *psw, BIS *pbis);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(void *p, BIS *pbis);
void func_001FF7E8(void *psw);
void ResolveSwSaveVars__FP2SW(void *psw);
void UpdateSwIllum(void *psw);
void func_0016F960(void *p, void *pv, int c);
void func_001703F8(void *pzpr, int a, int b);
void func_001ED3A8(void);
void func_001ED448(void);
void func_001EA9F0(void *psw, void *pzpr);
void func_00258248(void *p);
void *func_0014B690(void *p);
void func_001848A8(void *p, int k, void *pv, int n);
void func_00184950(void *p, int n);
void LoadSwSharedHeaderFromBrx(int n, BIS *pbis);
void LoadSwSpliceFromBrx(void *psw, int n, BIS *pbis);
void func_001EB748(void *pa, void *pb, BIS *pbis);
void LoadSwZoneBspFromBrx(void *psw, BIS *pbis);
struct LO;
struct LOL { void *pvt; int x4; int c; LO **apo; };
LOL *CLoadCollectorArray_ctor(LOL *p);
void LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(int n, BIS *pbis, LOL *p);
void func_001EC7A0(void);
LO *PloCloneLo__FP2LOP2SO(LO *po, int n);
extern char D_002A4BD0[];
extern char D_003DD040[];
extern char D_003DD490[];
extern char D_002DDF40[];
extern char D_002DEDC0[];
extern int D_003E1C34;
}
struct VTE { short d; short pad; void (*pfn)(); };
struct LO { int pad[3]; VTE *pvt; };
struct WE {
    int c0; LO **apo; int c8; int cC; int x10; int x14; void *pv18; char ab1C[0x40]; int x5C; int x60; int pad64[2];
};
struct RGBA { unsigned char r, g, b, a; };
struct SW {
    char pad0[0x39D0]; void *p39D0; int x39D4; void *p39D8; int x39DC; void *p39E0; void *p39E4; int x39E8;
    char pad39EC[0x3E00 - 0x39EC]; RGBA rgba; char pad3E04[0x3E38 - 0x3E04]; int x3E38;
    char pad3E3C[0x3F04 - 0x3E3C]; int cwe; WE *awe; int pad3F0C; int iwe;
};
class FRAME { public: FRAME(); ~FRAME(); int a[4]; };
struct ZPR { char ab[0x630]; };
struct ODD { ODD() { ab[2] = 2; } int ab[0x110 / 4]; };
class OBJ { public: OBJ(int k, void *pv, int n); ~OBJ(); char ab[0x70]; };

static inline void *AllocZ(AREQ *preq, int cb, int cbAlign) { preq->cb = cb; preq->cbAlign = cbAlign; return func_001AE2A0(preq); }
static inline void *Alloc(AREQ *preq, int cb, int cbAlign) { preq->cb = cb; preq->cbAlign = cbAlign; return func_001AE1D0(preq); }
static inline void *AllocS(AREQ *preq, int cb, int cbAlign) { preq->cb = cb; preq->cbAlign = cbAlign; return func_001AE318(preq); }

/** @sly2-annot addr=0x13DA18
 * Purpose: Top-level level-body reader: proxy-source table, sound, save vars, level table, langstrs, cells,
 *   g_sw/g_cm options, then per proxy source: refs+shader/texture blob+object list, cloned ccopy-1 times
 * Info: proposed name LoadSwFromBrx (T1); TU brx_sw
 * Notes: names_v2 C proposed LoadOptionFromBrx__FPvP5EOPIDP18CBinaryInputStream (0.438) | level_parser
 *   CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.97] - evidence: semantic review; static naming pass; sources:
 *   r2_131F70+parse-verified
 * See: docs/engine/core-library.md#6-source-files, docs/engine/overview.md#2-the-frame-loop-exact-order,
 *   docs/engine/overview.md#3-world--level-lifecycle
 * @end-sly2-annot */
extern "C" void LoadSwFromBrx__FP2SWP18CBinaryInputStream(SW *psw, BIS *pbis)
{
    union { AREQ req; RGBA rgba; struct { int pad[2]; int c; LO **apo; } lol; } u;
    int i;
    int fNever = 0;
    StartupSplice();
    psw->cwe = func_00117D50(pbis);
    psw->awe = (WE *)AllocZ(&u.req, psw->cwe * 0x6C, 0x10);
    int h;
    func_001C9058(&h);
    func_001C9090(&h, D_002A4BD0);
    for (i = 0; i < psw->cwe; i++) {
        WE *pwe = &psw->awe[i];
        func_00117968(pbis, 0x40, pwe->ab1C);
        pwe->x5C = func_00117DB8(pbis);
        pwe->x60 = func_00117DB8(pbis);
        pwe->x10 = func_00117DB8(pbis);
        pwe->x14 = func_00117DB8(pbis);
        pwe->c8 = func_00117DB8(pbis);
        pwe->pv18 = Alloc(&u.req, pwe->c8 * 4, 0x10);
        func_002333B4(pwe->pv18, -1, pwe->c8 * 4);
    }
    if (fNever) { // STAND-IN, see the header
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
        func_00117968(pbis, 0x40, psw);
    }
    func_001FD0E8(D_003DD040, pbis);
    func_001D7B88((char *)psw + 0x3A18, pbis);
    LoadLevelTableFromBrx(psw, pbis);
    LoadSwLangstrsFromBrx__FP2SWP18CBinaryInputStream(psw, pbis);
    LoadSwCellsFromBrx__FP2SWP18CBinaryInputStream(psw, pbis);
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(D_003DD490, pbis);
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(D_002DDF40, pbis);
    func_00117988(pbis, 0x10);
    func_001FF7E8(psw);
    ResolveSwSaveVars__FP2SW(psw);
    UpdateSwIllum(psw);
    if (psw->x3E38)
        *(unsigned int *)&psw->rgba = 0x80000000;
    u.rgba = psw->rgba;
    func_0016F960(D_002DEDC0, &u.rgba, 3);
    psw->p39E4 = psw->p39E0 = AllocS(&u.req, 0x10000, 0x10);
    psw->p39D0 = AllocS(&u.req, 0x6000, 0x10);
    psw->x39D4 = 0;
    psw->p39D8 = AllocS(&u.req, 0x3000, 0x10);
    psw->x39DC = 0;
    FRAME frame;
    ZPR zpr0;
    ZPR zpr1;
    func_001703F8(&zpr0, 0x1800, 0x4000);
    func_001ED3A8();
    func_001703F8(&zpr1, 0x1000, 0x1800);
    func_001EA9F0(psw, &zpr0);
    for (i = 0; i < psw->cwe; ) {
        WE *pwe = &psw->awe[i];
        int iNext = i + 1;
        ODD odd;
        char ab[0x4040];
        func_00258248(&odd);
        func_001C9090(&h, func_0014B690(pwe->ab1C));
        psw->iwe = pwe->x5C ? -1 : i;
        OBJ obj(0x403F, ab, 0);
        LoadSwSharedHeaderFromBrx(1, pbis);
        D_003E1C34 = func_00117DB8(pbis);
        LoadSwSpliceFromBrx(psw, pwe->x10, pbis);
        func_001EB748(&zpr0, &zpr1, pbis);
        LoadSwZoneBspFromBrx(psw, pbis);
        {
            FRAME frameT;
            int j, k;
            LO **ppo;
            AREQ req2;
            CLoadCollectorArray_ctor((LOL *)&u.lol);
            LoadSwObjectsFromBrx__FP2SOP18CBinaryInputStreamP14CLoadCollector(0, pbis, (LOL *)&u.lol);
            func_00117988(pbis, 0x10);
            func_0014B690(pwe->ab1C);
            func_001EC7A0();
            psw->x39E8 = 0;
            pwe->c0 = u.lol.c;
            pwe->apo = (LO **)AllocZ(&req2, pwe->c0 * pwe->c8 * 4, 0x10);
            func_001AE5A0(pwe->apo, u.lol.apo, pwe->c0 * 4);
            ppo = &pwe->apo[pwe->c0];
            for (j = 1; j < pwe->c8; j++)
                for (k = 0; k < pwe->c0; k++)
                    *ppo++ = PloCloneLo__FP2LOP2SO(pwe->apo[k], 0);
            if (pwe->x5C) {
                pwe->cC++;
            } else {
                for (k = 0; k < pwe->c8 * pwe->c0; k++) {
                    LO *po = pwe->apo[k];
                    ((void (*)(LO *))po->pvt[9].pfn)((LO *)((char *)po + po->pvt[9].d));
                }
            }
        }
        i = iNext;
    }
    func_001ED448();
    psw->iwe = -2;
    func_001FD5E8(D_003DD040);
    func_001C9090(&h, 0);
    func_001C9098(&h);
}
