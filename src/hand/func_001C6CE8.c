struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
void *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int id, int b);
struct LO *PloAcquireProxy(void *pg, int n);
void LockProxyPlo(void *pg, struct LO *plo);
/** @sly2-annot addr=0x1C6CE8
 * Purpose: make one prize of kind 0x5B/0x5D from its proxy pool (PloAcquireProxy + LockProxyPlo)
 * Info: proposed name PdprizeNewSpray (T1); TU prize
 * Notes: runtime2: coin/medicine copies moved to break site
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); sources: r5_1AE130+runtime
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
int func_001C6CE8(void *p, int kind, int n, struct LO **aplo)
{
    int id, i, c;
    void *pg;
    struct LO *plo;
    if (!n) return 0;
    switch (kind) {
    case 0x5B: id = 0x2B3; break;
    case 0x5D: id = 0x2B4; break;
    default: return 0;
    }
    pg = PloFindSwObject__FP2SWi3OIDP2LO(p, 5, id, 0);
    if (!pg) return 0;
    c = 0;
    for (i = 0; ; i++) {
        if (i >= n) break;
        plo = PloAcquireProxy(pg, 0);
        if (!plo) break;
        ((void (*)(void *, int))plo->vt[76].pf)((char *)plo + plo->vt[76].d, 2);
        c++;
        LockProxyPlo(pg, plo);
        aplo[i] = plo;
    }
    return c;
}
