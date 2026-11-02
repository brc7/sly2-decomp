typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct DLI { DL *pdl; void **ppv; int ibDle; struct DLI *pdliNext; } DLI;
extern DLI *D_002DD2FC;
typedef struct OBJ OBJ;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *, int, int, int, int, int); } a[8]; } VT;
struct OBJ {
    char pad[0xC];
    VT *vt;
    char pad10[0x40 - 0x10];
    int x40;
    char pad44[0x5C - 0x44];
    DL dl;
    char pad68[0x6C - 0x68];
    int x6C;
    int x70;
    int x74;
    int x78;
};
void RemoveJsg(void *);
void ClearRelayAlarms(OBJ *);
void SetLoBusyk(OBJ *, int);
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
/** @sly2-annot addr=0x1DFEB8
 * Purpose: If firing: clear firer/state, stop every JSG (0x1E5628), clear the alarm list, release LO busy
 *   (0x1A6CA8(p,2)), send itself msg 6
 * Info: proposed name StopRelay (T1); TU relay
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void StopRelay(OBJ *p)
{
    DLI dli;
    void *pv;
    if (p->x74 == 0)
        return;
    p->x6C = 0;
    p->x70 = 0;
    p->x74 = 0;
    InitDli(&dli, &p->dl);
    while ((pv = PvNextDli(&dli)) != 0)
        RemoveJsg(pv);
    ClearRelayAlarms(p);
    if (p->x78) {
        SetLoBusyk(p, 2);
        if ((p->x40 ^ 1) & 1)
            *(int *)p->x40 &= ~2;
        else
            p->x40 &= ~2;
    }
    p->vt->a[3].pfn((char *)p + p->vt->a[3].d, 6, 0, 0, 0, 0);
    D_002DD2FC = dli.pdliNext;
}
