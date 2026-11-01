/* (Claude agent) MATCH: parent bit via (int)(x>>60)&1, fNew via if(!(bit56)) fNew=1; else fNew=0; (gives addiu/movn). */
typedef struct ALO ALO;
typedef struct { short d; short pad; void *pfn; } VTE;
struct ALO { int x0, x4, x8; VTE *vt; ALO *x10; ALO **x14; char pad[0x24 - 0x18]; ALO *x24; union { unsigned long u; struct { unsigned long f0 : 56; unsigned long f56 : 1; unsigned long f57 : 3; unsigned long f60 : 1; unsigned long f61 : 3; } b; } x28; };
#define VFN(p, n, T) ((T)(p)->vt[n].pfn)
#define VTHIS(p, n) ((ALO *)((char *)(p) + (p)->vt[n].d))
extern char D_003DD4BC[];
extern char D_003DD490[];
void func_001A5448(ALO *p, void **ppdl, ALO ***pppalo);
void RemoveDlEntry__FP2DLPv(void *pdl, void *p);
void AppendDlEntry__FP2DLPv(void *pdl, void *p);
void InsertDlEntryBefore__FP2DLPvT1(void *pdl, void *pBefore, void *p);
int func_001A61A0(ALO *p);
void func_00148300(void *g, ALO *p);
void func_001A5900(ALO *palo, ALO *paloParent, int a, int b)
{
    unsigned long fOld = palo->x28.u >> 60 & 1;
    unsigned long fNew = 0;
    void *pdlOld;
    ALO **ppaloOld;
    void *pdlNew;
    ALO **ppaloNew;
    ALO *paloOld;
    ALO **apalo;
    int c, i;
    if (!paloParent || ((int)(paloParent->x28.u >> 60) & 1)) {
        if (!(palo->x28.u & 0x0100000000000000UL)) fNew = 1; else fNew = 0;
    }
    func_001A5448(palo->x10, &pdlOld, &ppaloOld);
    func_001A5448(paloParent, &pdlNew, &ppaloNew);
    if (palo->x28.u & 0x0100000000000000UL) {
        if (palo == *ppaloOld)
            *ppaloOld = palo->x24;
        RemoveDlEntry__FP2DLPv(pdlOld, palo);
        AppendDlEntry__FP2DLPv(pdlNew, palo);
        if (!*ppaloNew)
            *ppaloNew = palo;
    } else {
        RemoveDlEntry__FP2DLPv(pdlOld, palo);
        InsertDlEntryBefore__FP2DLPvT1(pdlNew, *ppaloNew, palo);
    }
    if (func_001A61A0(palo)) {
        VFN(palo, 22, void (*)(ALO *))(VTHIS(palo, 22));
        if (*palo->x14 == palo)
            RemoveDlEntry__FP2DLPv(D_003DD4BC, palo->x14);
    }
    paloOld = palo->x10;
    palo->x10 = paloParent;
    VFN(palo, 13, void (*)(ALO *, ALO *, ALO *))(VTHIS(palo, 13), palo, paloOld);
    c = VFN(palo, 28, int (*)(ALO *, ALO ***))(VTHIS(palo, 28), &apalo);
    for (i = 0; i < c; i++) {
        ALO *p = apalo[i];
        VFN(p, 10, void (*)(ALO *, ALO *, int, int))(VTHIS(p, 10), paloParent, a, b);
    }
    if (fNew != fOld) {
        if (fNew)
            VFN(palo, 11, void (*)(ALO *))(VTHIS(palo, 11));
        else
            VFN(palo, 12, void (*)(ALO *))(VTHIS(palo, 12));
    }
    if (func_001A61A0(palo))
        VFN(palo, 23, void (*)(ALO *))(VTHIS(palo, 23));
    else if (!palo->x10 && fNew)
        func_00148300(D_003DD490, palo);
}
