typedef unsigned long ulong;
typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *pvt; char pad2[0x28 - 0x10]; ulong fl; };
struct E { char pad[0xDC]; int nDC; };
struct P { char pad[0x64]; DL dl; char pad2[0xE8 - 0x70]; struct E *pe; char pad3[0x12C - 0xEC]; float f12C; int n130; };
struct K { int a, b; };
struct G;
extern struct G D_003DD490;
extern struct K D_002A8270;
void RemoveLoHierarchy(struct P *);
void RemoveSoParts(struct P *);
void ClearSwCallbacks(struct G *, int, struct P *, struct K, int);
void func_001F37C8(struct P *, int);
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
void RemoveSoHierarchy(struct P *p)
{
    DLI dli;
    struct O *po;
    int n;
    RemoveLoHierarchy(p);
    RemoveSoParts(p);
    p->n130 = 0;
    p->f12C = 1.0f;
    n = 0;
    if (p->pe) n = p->pe->nDC;
    if (n) {
        ClearSwCallbacks(&D_003DD490, 3, p, D_002A8270, 0);
        func_001F37C8(p, 0);
    }
    InitDli(&dli, &p->dl);
    while ((po = PvNextDli(&dli)) != 0 && !(po->fl & (1L << 56)))
        po->pvt[12].pfn((char *)po + po->pvt[12].d);
    D_002DD2FC = dli.m_pdliNext;
}
