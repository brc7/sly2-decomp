struct DL { void *head; void *tail; int ibDle; };
struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
extern DLI *D_002DD2FC;
struct RIP;
struct RIPG { char pad[0x10]; int x10; char p1[0xEC - 0x14]; void *xEC; char p2[0x15C - 0xF0]; DL dl; };
struct SW { char pad[0x403C]; char pool[0x30]; RIPG *x406C; };
extern SW D_003DD490;
extern "C" {
void func_001CE980(RIP *prip);
void RemoveSoHierarchy(RIPG *p);
void RemoveSwAaobrObject(SW *psw, void *pv);
void SnipLo(RIPG *p);
void *PdlFromSwCid(SW *psw, int n);
void RemoveSlEntry(void *pl, RIPG *p);
void FreeLoroot(RIPG *p);
void func_0011CD98(void *pool, void *pv);
}
static inline void InitDli(DLI *pdli, DL *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->ibDle;
    pdli->m_pdliNext = D_002DD2FC;
    D_002DD2FC = pdli;
}
static inline void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
/** @sly2-annot addr=0x1CDE80
 * Purpose: RIPG vt slot 12 (OnRemove): release rips / clear SW default RIPG
 * Info: proposed name OnRipgRemove__FP4RIPG (EXISTING); class RIPG slot12; TU rip
 * Confidence: high [EXISTING 0.90] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void OnRipgRemove(RIPG *pripg)
{
    DLI dli;
    RIP *prip;
    SW *psw;
    InitDli(&dli, &pripg->dl);
    while ((prip = (RIP *)PvNextDli(&dli)) != 0) func_001CE980(prip);
    psw = &D_003DD490;
    if (pripg == psw->x406C) psw->x406C = 0;
    RemoveSoHierarchy(pripg);
    if (pripg->xEC) {
        RemoveSwAaobrObject(psw, pripg->xEC);
        pripg->xEC = 0;
    }
    SnipLo(pripg);
    RemoveSlEntry(PdlFromSwCid(psw, 0x57), pripg);
    if (!pripg->x10) FreeLoroot(pripg);
    func_0011CD98(psw->pool, pripg);
    D_002DD2FC = dli.m_pdliNext;
}
