struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; };
void *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int id, int b);
struct LO *PloAcquireProxy(void *pg, int n);
void LockProxyPlo(void *pg, struct LO *plo);
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
