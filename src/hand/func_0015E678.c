typedef struct LO LO;
struct LO {
    char pad[0x10];
    LO *pParent;
    char pad14[4];
    int oid;
    LO *pNextHash;
    char pad20[0xC];
    signed char ips;
    char pad2D[0xF];
    void *pch;
};
typedef struct ENT {
    int x0;
    void *pch;
    int oid;
    struct ENT *pNext;
    LO *plo;
} ENT;
typedef struct ST {
    int ips;
    LO *plo;
    int f;
} ST;
extern void *D_003E139C;
extern LO **PdlFromSwOid(void *psw, int oid);
extern ENT **PdlFromSwPxrOid(void *psw, int oid);
extern void MatchSwObject(void *pv, LO *plo, void *pch, unsigned int grf, LO *ctx, void *pchList, int cMax, int *pc, LO **aplo, int *pips);
/** @sly2-annot addr=0x15E678
 * Purpose: Core object search over the oid hash buckets (and second table), with an extra Sly 2 parameter
 *   between ploContext and cploMax
 * Info: proposed name CploFindSwObjects (T1); TU find
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5
 * See: docs/engine/object-model.md#9-key-functions, docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/object-model.md#find-0x15e380-0x15ef00
 * @end-sly2-annot */
int func_0015E678(void *psw, int grf, int oid, LO *ploCtx, void *pchList, int cMax, LO **aplo)
{
    volatile int unused; /* unreferenced stack slot at 0x10 in the original */
    int cplo;
    int ips;
    LO *plo;
    int f;
    LO *p;
    ENT *pe;
    if (oid == -1 || (!ploCtx && (grf & 0xFF) != 5))
        return 0;
    if (!pchList)
        pchList = ploCtx ? ploCtx->pch : D_003E139C;
    cplo = 0;
    switch (grf & 0xFF) {
    case 1:
    case 3:
    case 4: {
        int i = 0;
        p = ploCtx;
        plo = p;
        f = grf & 0x200;
        for (; p; p = p->pParent)
            p->ips = ++i;
        ips = 0x7FFFFFFF;
        break;
    }
    default:
        plo = 0;
        f = grf & 0x200;
        break;
    }
    PdlFromSwOid(psw, oid);
    for (p = *PdlFromSwOid(psw, oid); p; p = p->pNextHash) {
        if (p->oid == oid)
            MatchSwObject(psw, p, p->pch, grf, ploCtx, pchList, cMax, &cplo, aplo, &ips);
    }
    PdlFromSwPxrOid(psw, oid);
    for (pe = *PdlFromSwPxrOid(psw, oid); pe; pe = pe->pNext) {
        LO *ploE;
        if (pe->oid != oid)
            continue;
        ploE = pe->plo;
        if (!ploE)
            continue;
        if (ploE->oid == oid) {
            int i;
            for (i = 0; i < cplo; i++)
                if (aplo[i] == pe->plo)
                    break;
            if (i < cplo)
                continue;
        }
        MatchSwObject(psw, pe->plo, pe->pch, grf, ploCtx, pchList, cMax, &cplo, aplo, &ips);
    }
    for (p = plo; p; p = p->pParent)
        p->ips = 0;
    if (!f && cMax < cplo)
        cplo = cMax;
    return cplo;
}
