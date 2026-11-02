typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(bool f);
};
extern "C" {
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *a0);
int FCheckVolPoint(void *p, qword q);
CRef RefThunkVolCHECK_POINT(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x104718
 * Purpose: Splice option thunk: optid 108 on VOL (g_aeopid[366] thunkFn, returns Bool, crefReq 1); unpacks 1
 *   args (Vector) -> Bool via RefSetArgsFromSplice then calls FCheckVolPoint (0x227C90)
 * Info: proposed name RefThunkVolCHECK_POINT (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunkVolCHECK_POINT(void *p, int c, void *aref)
{
    int ak[1] = {2};
    union { qword q; } u;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, ak, &u);
    ref = FCheckVolPoint(p, u.q) != 0;
    return ref;
}
