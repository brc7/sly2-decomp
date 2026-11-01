typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;
struct CLOCK { float t, dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pf)(); };
struct RIP { char pad[0x14]; float tStart; char p1[0xEC - 0x18]; struct VTE *vt; };
struct RIPG { char pad[0xC]; struct VTE *vt; char p1[0x15C - 0x10]; DL dl; };
void func_001CE9C0(struct RIP *prip);
void func_001CE9D0(struct RIP *prip);
int func_001A61A0(struct RIPG *p);
int func_0011A3B0(DL *pdl);
void func_001CE2E8(struct RIPG *p);
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
/** @sly2-annot addr=0x1CE1C8
 * Purpose: RIPG::Update (vt slot 16): AddRipRef/update each rip via its vtable, ReleaseRip, then
 *   BounceRipgRips if active
 * Info: proposed name UpdateRipg (EXISTING); class RIPG slot16; TU rip
 * Notes: agent/verifier proposes UpdateRipg for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> UpdateRipg (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.75] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r5_1AE130+arbitration
 * See: docs/engine/particles-effects.md#2-rip-particles-ripcpp-0x1cdcd0-0x1d1370,
 *   docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void FUN_0019e420(struct RIPG *pripg)
{
    DLI dli;
    struct RIP *prip;
    InitDli(&dli, &pripg->dl);
    while ((prip = (struct RIP *)PvNextDli(&dli)) != 0) {
        if (D_002DDED0.t <= prip->tStart) continue;
        func_001CE9C0(prip);
        ((void (*)(void *))prip->vt[5].pf)((char *)prip + prip->vt[5].d);
        func_001CE9D0(prip);
    }
    if (!func_001A61A0(pripg) || func_0011A3B0(&pripg->dl))
        ((void (*)(void *))pripg->vt[9].pf)((char *)pripg + pripg->vt[9].d);
    else
        func_001CE2E8(pripg);
    D_002DD2FC = dli.m_pdliNext;
}
