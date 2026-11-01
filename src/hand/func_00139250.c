typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;     /* g_pdliFirst */
extern char D_002DEAF0[];
struct S { char pad[0x10]; int f10; char pad2[0x448 - 0x14]; DL dl; };
void func_00139098(void *);
void func_001A5848(struct S *);
void func_001A57A8(struct S *);
void func_001A53E0(struct S *);
void AppendDlEntry__FP2DLPv(void *, struct S *);
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
void func_00139250(struct S *p) {
    DLI dli;
    void *pv;
    InitDli(&dli, &p->dl);
    while ((pv = PvNextDli(&dli)) != 0) func_00139098(pv);
    func_001A5848(p);
    func_001A57A8(p);
    if (!p->f10) func_001A53E0(p);
    AppendDlEntry__FP2DLPv(D_002DEAF0, p);
    D_002DD2FC = dli.m_pdliNext;
}
