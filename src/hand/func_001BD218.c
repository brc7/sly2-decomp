typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;
struct VE { short d; short pad; void (*pfn)(); };
struct S { char pad[0xC]; struct VE *vt; char pad1[0x1C4 - 0x10]; DL dl; char pad2[0x1D8 - 0x1D0]; int x1D8; };
extern char D_003E1A4C[], D_003E1A40[];
void RemoveDlEntry__FP2DLPv(DL *pdl, struct S *pv);
int FFindDlEntry__FP2DLPv(void *pdl, struct S *pv);
void AppendDlEntry__FP2DLPv(void *pdl, struct S *pv);
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
void func_001BD218(struct S *p)
{
    DLI dli;
    struct S *pv;
    InitDli(&dli, &p->dl);
    while ((pv = PvNextDli(&dli)) != 0) {
        RemoveDlEntry__FP2DLPv(&p->dl, pv);
        pv->x1D8 = 0;
        if (!FFindDlEntry__FP2DLPv(D_003E1A4C, pv)) AppendDlEntry__FP2DLPv(D_003E1A4C, pv);
    }
    ((void (*)(void *))p->vt[9].pfn)((char *)p + p->vt[9].d);
    AppendDlEntry__FP2DLPv(D_003E1A40, p);
    D_002DD2FC = dli.m_pdliNext;
}
