typedef struct DL { void *head; void *tail; int ibDle; } DL;
typedef struct DLI { DL *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
extern DLI *D_002DD2FC;     /* g_pdliFirst */
struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { int grf; struct O *po; };
struct Q { char pad[0x40]; unsigned int x40; };
struct A { char pad[0x14]; struct Q *pq; };
struct S { char pad[0x3D88]; DL dl; char pad2[0x3F14 - 0x3D94]; int c; int x3F18; int *pi; struct E *ae; };
void func_00130918(struct A *);
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
static __inline__ unsigned int GrfQ(struct Q *pq) { unsigned int x = pq->x40; return ((x ^ 1) & 1) ? *(unsigned int *)x : x; }
/** @sly2-annot addr=0x2267C0
 * Purpose: iterates the SW object DL (0x3D88) calling each object update / asega 0x130918
 * Info: proposed name UpdateSwObjects (T1); TU update
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/world-sw.md#10-key-functions,
 *   docs/reference/functions/world-sw.md#update-0x226550-0x226ab8
 * @end-sly2-annot */
void func_002267C0(struct S *p)
{
    DLI dli;
    int i;
    struct A *pa;
    p->pi = &i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if ((pe->grf & 0x102) == 0x102) {
            struct O *po = pe->po;
            ((void (*)(void *))po->vt[16].pfn)((char *)po + po->vt[16].d);
        }
    }
    p->pi = 0;
    InitDli(&dli, &p->dl);
    while ((pa = PvNextDli(&dli)) != 0) {
        if (GrfQ(pa->pq) & 0x100)
            func_00130918(pa);
    }
    p->pi = &i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if ((pe->grf & 0x124) == 0x104) {
            struct O *po = pe->po;
            ((void (*)(void *))po->vt[17].pfn)((char *)po + po->vt[17].d);
        }
    }
    p->pi = 0;
    D_002DD2FC = dli.m_pdliNext;
}
