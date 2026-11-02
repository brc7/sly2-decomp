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
CRef func_00201360(int c, void *aref, int *ak, int *an);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int n);
CRef RefThunkBasicDERIVED_FROMQ(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x100228
 * Purpose: Splice option thunk (just BEFORE the requested span): optid 3 on BASIC (g_aeopid[2] thunkFn,
 *   returns Bool, crefReq 1); unpacks 1 arg (Cid) and returns FIsBasicDerivedFrom(pbasic, cid) via
 *   CRef_SetBool
 * Info: proposed name RefThunkBasicDERIVED_FROMQ (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunkBasicDERIVED_FROMQ(void *p, int c, void *aref)
{
    int k = 7;
    int n;
    CRef ref = func_00201360(1, aref, &k, &n);
    ref = FIsBasicDerivedFrom__FP5BASIC3CID(p, n) != 0;
    return ref;
}
