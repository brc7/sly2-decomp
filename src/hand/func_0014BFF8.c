typedef struct DLI { void *m_pdl; void **m_ppv; int m_ibDle; struct DLI *m_pdliNext; } DLI;
struct X { int a; int x4; void (*pfn)(void *); void *pv; int x10; int x14; };
struct H { struct X *pxFirst; struct X *pxLast; int ibDle; float gC; };
extern DLI *D_002DD2F8;
extern float D_002DDF0C;
void RemoveSlEntry(struct H *ph, struct X *px);
void *func_001FBB90(int n);
void func_001FB2F8(void *p, int n);
void func_0014BF18(struct H *ph);
static __inline__ void InitDli(DLI *pdli, struct H *pdl)
{
    pdli->m_pdl = pdl;
    pdli->m_ppv = (void **)pdl;
    pdli->m_ibDle = pdl->ibDle;
    pdli->m_pdliNext = D_002DD2F8;
    D_002DD2F8 = pdli;
}
static __inline__ void *PvNextDli(DLI *pdli)
{
    void *pv = *pdli->m_ppv;
    pdli->m_ppv = (void **)((char *)pv + pdli->m_ibDle);
    return pv;
}
void func_0014BFF8(struct H *ph)
{
    DLI dli;
    struct X *px, *pxBest;
    if (ph->gC == 0.0f) return;
    if (ph->gC <= D_002DDF0C) {
        InitDli(&dli, ph);
        pxBest = 0;
        while ((px = PvNextDli(&dli)) != 0) {
            if (px->x14) {
                RemoveSlEntry(ph, px);
                px->x10 = 0;
                px->x14 = 0;
            } else {
                int fReady = px->x10 >= px->x4;
                if (fReady) {
                    if (!pxBest || pxBest->x4 < px->x4) pxBest = px;
                }
            }
        }
        if (pxBest) {
            func_001FB2F8(func_001FBB90(0x34), 0);
            pxBest->pfn(pxBest->pv);
        }
        func_0014BF18(ph);
        ph->gC = 0.0f;
        D_002DD2F8 = dli.m_pdliNext;
    }
}
