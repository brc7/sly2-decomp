typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(qword q);
};
extern "C" {
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *an);
qword func_001E77B0(void *p, qword q);
CRef func_0010E550(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x10E550
 * Purpose: Splice option thunk: optid 1057 on SHAPE (g_aeopid[1223] thunkFn, returns Vector, crefReq 1);
 *   unpacks 1 args (Vector) -> Vector via RefSetArgsFromSplice then calls PosFindShapeClosest (0x1E77B0)
 * Info: proposed name RefThunk_PosFindShapeClosest (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x1E77B0 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010E550(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    ref = func_001E77B0(p, u.q);
    return ref;
}
