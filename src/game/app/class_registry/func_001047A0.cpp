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
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *a0, void *a1);
int func_00227D20(void *p, qword q, float g);
CRef func_001047A0(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x1047A0
 * Purpose: Splice option thunk: optid 474 on VOL (g_aeopid[367] thunkFn, returns Bool, crefReq 2); unpacks 2
 *   args (Vector, Float) -> Bool via RefSetArgsFromSplice then calls FCheckVolSphere (0x227D20)
 * Info: proposed name RefThunk_FCheckVolSphere (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x227D20 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_001047A0(void *p, int c, void *aref)
{
    int ak[2] = {2, 0x16};
    union { qword q; } u;
    float g;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &u, &g);
    ref = func_00227D20(p, u.q, g) != 0;
    return ref;
}
