typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(void *pv);
    CRef &operator=(qword q);
    CRef &operator=(bool f);
    CRef &operator=(int n);
    CRef &operator=(float g);
};
extern "C" CRef func_00201360(int c, void *aref, int *ak, ...);
extern "C" { void AddRelayApplySm(void *p, int, int, int, int, int); CRef RefThunkRelaySET_SM_GOAL(void *p, int c, void *aref); }
/** @sly2-annot addr=0x107220
 * Purpose: Splice option thunk: optid 526 on RELAY (g_aeopid[638] thunkFn/thunkUser, returns Void, crefReq
 *   5); unpacks 5 args (Oid, Oid, Oid, Bool, Bool) -> Void via RefSetArgsFromSplice then calls
 *   AddRelayApplySm (0x1E05A0)
 * Info: proposed name RefThunkRelaySET_SM_GOAL (T1); TU crt0_splicewrap
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunkRelaySET_SM_GOAL(void *p, int c, void *aref)
{
    int ak[5] = {0x6, 0x6, 0x6, 0x0, 0x0};
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    CRef ref = func_00201360(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayApplySm(p, v0, v1, v2, v3, v4);
    return ref;
}
