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
extern "C" { void func_001E1178(void *p, int, int, int, int, float, float, float); CRef func_00108F80(void *p, int c, void *aref); }
/** @sly2-annot addr=0x108F80
 * Purpose: Splice option thunk: optid 231 on RELAY (g_aeopid[680] thunkFn/thunkUser, returns Void, crefReq
 *   7); unpacks 7 args (0xE, Oid, Int, Int, Float, Float, Float) -> Void via RefSetArgsFromSplice then calls
 *   AddRelaySound (0x1E1178)
 * Info: proposed name RefThunk_AddRelaySound (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00108F80(void *p, int c, void *aref)
{
    int ak[7] = {0xe, 0x6, 0x5, 0x5, 0x1, 0x1, 0x1};
    int v0;
    int v1;
    int v2;
    int v3;
    float v4;
    float v5;
    float v6;
    CRef ref = func_00201360(7, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5, &v6);
    ref.Decref();
    ref.InitTag(0x24);
    func_001E1178(p, v0, v1, v2, v3, v4, v5, v6);
    return ref;
}
