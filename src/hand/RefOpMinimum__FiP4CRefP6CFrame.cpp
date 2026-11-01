// pn reuses the aref parameter variable (if/else, tag != 0 arm first) -> beql s0=0
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
struct NODE { int cref; int x4; CRef ref; NODE *pnext; };
extern "C" CRef RefOpMinimum__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
/** @sly2-annot addr=0x1238B0
 * Purpose: Splice builtin "min": min over varargs
 * Info: proposed name RefOpMinimum (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpMinimum__FiP4CRefP6CFrame(void *pbasic, CRef *aref)
{
    CRef ref(aref[0]);
    if (aref[1].m_tagk != 0) aref = (CRef *)aref[1].u.pv; else aref = 0;
    NODE *pn = (NODE *)aref;
    for (; pn; pn = pn->pnext) {
        if (ref.m_tagk == 0x15) {
            if (pn->ref.m_tagk == 0x15) {
                CRef *p = &pn->ref;
                if (ref.u.n > p->u.n)
                    ref = *p;
            } else {
                CRef *p = &pn->ref;
                if ((float)ref.u.n > p->u.g)
                    ref = *p;
            }
        } else {
            if (pn->ref.m_tagk == 0x15) {
                CRef *p = &pn->ref;
                if ((float)p->u.n < ref.u.g)
                    ref = *p;
            } else {
                CRef *p = &pn->ref;
                if (ref.u.g > p->u.g)
                    ref = *p;
            }
        }
    }
    return ref;
}
