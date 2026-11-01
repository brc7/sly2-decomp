typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;     /* g_pdliFirst */
extern char D_002DEAF0[];
struct S { char pad[0x10]; int f10; char pad2[0x448 - 0x14]; DL dl; };
void func_00139098(void *);
void func_001A5848(struct S *);
void func_001A57A8(struct S *);
void func_001A53E0(struct S *);
void func_0011A220(void *, struct S *);
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
/** @sly2-annot addr=0x139250
 * Purpose: BLIPG vt12: remove all blips, base remove, queue group for freeing
 * Info: proposed name OnBlipgRemove (T1); class BLIPG slot12; TU blip
 * Notes: slot 12 is RemoveHierarchy by position, but the body matches Sly 1 OnBlipgRemove; Sly 1 name kept
 * Confidence: high [T1 0.85] - evidence: Sly 1 / Lilly reference; static naming pass; sources:
 *   r2b_rest+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#blip-0x138f08-0x13a9e8
 * @end-sly2-annot */
void func_00139250(struct S *p) {
    DLI dli;
    void *pv;
    InitDli(&dli, &p->dl);
    while ((pv = PvNextDli(&dli)) != 0) func_00139098(pv);
    func_001A5848(p);
    func_001A57A8(p);
    if (!p->f10) func_001A53E0(p);
    func_0011A220(D_002DEAF0, p);
    D_002DD2FC = dli.m_pdliNext;
}
