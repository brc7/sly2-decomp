typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
extern DLI *D_002DD2FC;
extern int D_002DD318;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *); } a[16]; } VT;
typedef struct { char pad[0xC]; VT *vt; } LO;
extern char D_003D9E80[], D_003D9ED0[], D_002E1E60[], D_003DA0F0[], D_003DA150[], D_003DA0C0[], D_003DA3C0[];
extern char D_002E6550[], D_002E65A0[], D_003DA4F0[], D_003DA530[];
void func_001D9B30(void *);
void func_001AE970(void *);
void func_001A3070(void *);
void func_001DAA48(void *);
void func_001DB7F0(void *);
void func_001DA320(void *);
void func_001DCCD8(void *);
void func_001ACB38(void *);
void func_001DA7B0(void *);
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->pdl = pdl;
    pdli->ppv = (void **)pdl;
    pdli->ibDle = pdl->ibDle;
    pdli->pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->ppv;
    pdli->ppv = (void **)((char *)pv + pdli->ibDle);
    return pv;
}
/** @sly2-annot addr=0x1D9408
 * Purpose: Call each global HUD screen's Reset (slot-13 implementations
 *   0x1D9B30/0x1DAA48/0x1DB7F0/0x1DA320/0x1DCCD8/0x1DA7B0) and walk the active list
 * Info: proposed name ResetScreens (T2); TU screen
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/hud-ui.md#screen-0x1d93d8-0x1de2a0
 * @end-sly2-annot */
void func_001D9408(DL *pdl)
{
    DLI dli;
    LO *plo;
    InitDli(&dli, pdl);
    while ((plo = (LO *)PvNextDli(&dli)) != 0)
        plo->vt->a[11].pfn((char *)plo + plo->vt->a[11].d);
    D_002DD318 = 0;
    func_001D9B30(D_003D9E80);
    func_001AE970(D_003D9ED0);
    func_001A3070(D_002E1E60);
    func_001DAA48(D_003DA0F0);
    func_001DB7F0(D_003DA150);
    func_001DA320(D_003DA0C0);
    func_001DCCD8(D_003DA3C0);
    func_001ACB38(D_002E6550);
    func_001DA7B0(D_002E65A0);
    func_001DA7B0(D_003DA4F0);
    func_001DA7B0(D_003DA530);
    D_002DD2FC = dli.pdliNext;
}
