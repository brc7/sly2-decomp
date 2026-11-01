struct DL { void *head; void *tail; int ibDle; };
struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; DLI *m_pdliNext; };
extern DLI *D_002DD2FC;
struct RIP;
struct RIPG { char pad[0x10]; int x10; char p1[0xEC - 0x14]; void *xEC; char p2[0x15C - 0xF0]; DL dl; };
struct SW { char pad[0x403C]; char pool[0x30]; RIPG *x406C; };
extern SW D_003DD490;
extern "C" {
void func_001CE980(RIP *prip);
void func_001F2D48(RIPG *p);
void func_001337D0(SW *psw, void *pv);
void func_001A57A8(RIPG *p);
void *func_0015E3A8(SW *psw, int n);
void RemoveSlEntry(void *pl, RIPG *p);
void func_001A53E0(RIPG *p);
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
void OnRipgRemove(RIPG *pripg)
{
    DLI dli;
    RIP *prip;
    SW *psw;
    InitDli(&dli, &pripg->dl);
    while ((prip = (RIP *)PvNextDli(&dli)) != 0) func_001CE980(prip);
    psw = &D_003DD490;
    if (pripg == psw->x406C) psw->x406C = 0;
    func_001F2D48(pripg);
    if (pripg->xEC) {
        func_001337D0(psw, pripg->xEC);
        pripg->xEC = 0;
    }
    func_001A57A8(pripg);
    RemoveSlEntry(func_0015E3A8(psw, 0x57), pripg);
    if (!pripg->x10) func_001A53E0(pripg);
    func_0011CD98(psw->pool, pripg);
    D_002DD2FC = dli.m_pdliNext;
}
