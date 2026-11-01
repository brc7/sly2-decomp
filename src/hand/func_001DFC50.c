typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
extern struct CLOCK { float a[20]; } D_002DDED0;
extern DLI *D_002DD2FC;
typedef struct {
    char pad[0x40];
    unsigned int x40;
    char pad44[0x5C - 0x44];
    DL dl;
    char pad68[0x70 - 0x68];
    float t;
    int x74;
} OBJ;
void func_001A5E28(OBJ *);
void func_001E1C40(void *);
int FIsDlEmpty__FP2DL(DL *);
void func_001DFEB8(OBJ *);
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
void func_001DFC50(OBJ *p)
{
    DLI dli;
    void *pv;
    unsigned int fl;
    func_001A5E28(p);
    if (p->x74 == 0)
        return;
    fl = p->x40;
    if ((fl ^ 1) & 1)
        fl = *(unsigned int *)fl;
    if (fl & 0x100)
        p->t += D_002DDED0.a[16];
    else
        p->t += D_002DDED0.a[1];
    InitDli(&dli, &p->dl);
    while ((pv = PvNextDli(&dli)) != 0)
        func_001E1C40(pv);
    if (FIsDlEmpty__FP2DL(&p->dl))
        func_001DFEB8(p);
    D_002DD2FC = dli.pdliNext;
}
