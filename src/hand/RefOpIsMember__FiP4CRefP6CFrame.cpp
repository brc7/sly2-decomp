struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(bool f) { m_tagk = 0x17; u.n = f; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct NODE { int x0, x4; CRef ref; NODE *pnext; };
extern "C" {
CRef RefOpEqual__FiP4CRefP6CFrame(int carg, CRef *aref, void *pctx);
CRef RefOpIsMember__FiP4CRefP6CFrame(int carg, CRef *aref, void *pctx);
}
/** @sly2-annot addr=0x120E10
 * Purpose: Splice builtin "member?": loop calling RefOpEqual on each element
 * Info: proposed name RefOpIsMember (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpIsMember__FiP4CRefP6CFrame(int carg, CRef *aref, void *pctx)
{
    CRef ref;
    CRef arefT[2];
    CRef res(false);
    CRef *pres = &res;
    if (aref[1].m_tagk == 0x1F) {
        NODE *p;
        arefT[0] = aref[0];
        for (p = (NODE *)aref[1].u.pv; p; p = p->pnext) {
            arefT[1] = p->ref;
            ref = RefOpEqual__FiP4CRefP6CFrame(2, arefT, pctx);
            if (ref.m_tagk == 0x17 && ref.u.n) {
                pres->u.n = 1;
                break;
            }
        }
    }
    return res;
}
