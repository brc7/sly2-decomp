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
extern "C" CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, ...);
extern "C" { void AddRelayStepCmd29(void *p, int, float, float, int, int); CRef RefThunk_AddRelayStepCmd29(void *p, int c, void *aref); }
/** @sly2-annot addr=0x1085F0
 * Purpose: Splice option thunk: optid 675 on RELAY (g_aeopid[667] thunkFn/thunkUser, returns Void, crefReq
 *   5); unpacks 5 args (Oid, Float, Float, Bool, Oid) -> Void via RefSetArgsFromSplice then calls
 *   AddRelayStepCmd29 (0x1E0DB8)
 * Info: proposed name RefThunk_AddRelayStepCmd29 (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_AddRelayStepCmd29(void *p, int c, void *aref)
{
    int ak[5] = {0x6, 0x1, 0x1, 0x0, 0x6};
    int v0;
    float v1;
    float v2;
    int v3;
    int v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    AddRelayStepCmd29(p, v0, v1, v2, v3, v4);
    return ref;
}
