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
extern "C" { void SetPoChoices(void *p, CRef *pref); CRef RefThunk_SetPoChoices(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10F648
 * Purpose: Splice option thunk: optid 1154 on CID70 (g_aeopid[1339] thunkFn/thunkUser, returns Void, crefReq
 *   1); unpacks 1 args (0x8006) -> Void via RefSetArgsFromSplice then calls SetPoChoices (0x1D7780)
 * Info: proposed name RefThunk_SetPoChoices (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_SetPoChoices(void *p, int c, void *aref)
{
    int k = 0x8006;
    CRef r;
    CRef ref = func_00201360(1, aref, &k, &r);
    ref.Decref();
    ref.InitTag(0x24);
    SetPoChoices(p, &r);
    return ref;
}
