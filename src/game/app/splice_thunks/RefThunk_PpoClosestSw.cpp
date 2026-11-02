typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(void *pv);
};
extern "C" {
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *an);
void *PpoClosestSw(void *p, qword q);
CRef RefThunk_PpoClosestSw(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x10BF70
 * Purpose: Splice option thunk: optid 847 on SW (g_aeopid[929] thunkFn, returns 0x2032, crefReq 1); unpacks 1
 *   args (Vector) -> 0x2032 via RefSetArgsFromSplice then calls PpoClosestSw (0x1C4AF8)
 * Info: proposed name RefThunk_PpoClosestSw (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_PpoClosestSw(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    ref = PpoClosestSw(p, u.q);
    return ref;
}
