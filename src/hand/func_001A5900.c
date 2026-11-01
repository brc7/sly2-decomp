/* (Claude agent) MATCH: parent bit via (int)(x>>60)&1, fNew via if(!(bit56)) fNew=1; else fNew=0; (gives addiu/movn). */
typedef struct ALO ALO;
typedef struct { short d; short pad; void *pfn; } VTE;
struct ALO { int x0, x4, x8; VTE *vt; ALO *x10; ALO **x14; char pad[0x24 - 0x18]; ALO *x24; union { unsigned long u; struct { unsigned long f0 : 56; unsigned long f56 : 1; unsigned long f57 : 3; unsigned long f60 : 1; unsigned long f61 : 3; } b; } x28; };
#define VFN(p, n, T) ((T)(p)->vt[n].pfn)
#define VTHIS(p, n) ((ALO *)((char *)(p) + (p)->vt[n].d))
extern char D_003DD4BC[];
extern char D_003DD490[];
void func_001A5448(ALO *p, void **ppdl, ALO ***pppalo);
void func_0011A2E8(void *pdl, void *p);
void func_0011A220(void *pdl, void *p);
void func_0011A280(void *pdl, void *pBefore, void *p);
int func_001A61A0(ALO *p);
void func_00148300(void *g, ALO *p);
/** @sly2-annot addr=0x1A5900
 * Purpose: LO vt10: move plo between old/new parent child DLs keeping in-world state, freeze if in world, set
 *   psoParent, vt13 UpdateLoroot, recurse to children (vt10), re-add/notify as needed
 * Info: proposed name SetLoParent (T1); class LO slot10; TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface,
 *   docs/engine/object-model.md#4-activation-onadd--onremove-slots-23--22,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
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
        func_0011A2E8(pdlOld, palo);
        func_0011A220(pdlNew, palo);
        if (!*ppaloNew)
            *ppaloNew = palo;
    } else {
        func_0011A2E8(pdlOld, palo);
        func_0011A280(pdlNew, *ppaloNew, palo);
    }
    if (func_001A61A0(palo)) {
        VFN(palo, 22, void (*)(ALO *))(VTHIS(palo, 22));
        if (*palo->x14 == palo)
            func_0011A2E8(D_003DD4BC, palo->x14);
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
