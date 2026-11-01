struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct Q { char pad[0x3C]; void *pv; };
struct W { char pad[0xC]; Q *pq; };
struct G { char pad[0x3F0C]; void *pv; };
extern G D_003DD490;
extern "C" {
int func_0015E678(G *pg, int k, void *a, Q *b, void *c, int n, void **ap);
CRef func_00124078(int c, void **ap);
CRef func_00124578(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x124578
 * Purpose: Splice builtin "find-nearest-objects": 0x15E678 k=4 + RefPairFromAplo
 * Info: proposed name RefOpFindNearestObjects (T1); TU bif
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00124578(void *pbasic, CRef *aref)
{
    CRef ref;
    void *ap[0x40];
    Q *pq = (Q *)aref[1].u.pv;
    Q *pqT;
    if (aref[2].m_tagk)
        pqT = ((W *)aref[2].u.pv)->pq;
    else
        pqT = pq;
    void *p0 = aref[0].u.pv;
    G *pg = &D_003DD490;
    void *pv;
    if (pqT)
        pv = pqT->pv;
    else
        pv = pg->pv;
    int c = func_0015E678(pg, 4, p0, pq, pv, 0x40, ap);
    ref = func_00124078(c, ap);
    return ref;
}
