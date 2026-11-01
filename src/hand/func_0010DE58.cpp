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
extern "C" { void func_00212828(void *p, CRef *pref); CRef func_0010DE58(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10DE58
 * Purpose: Splice option thunk: optid 984 on WIDGET (g_aeopid[1132] thunkFn, returns Void, crefReq 1);
 *   unpacks 1 args (0x8010) -> Void via RefSetArgsFromSplice then calls ShowWidgetArgsFromSplice (0x212828)
 * Info: proposed name RefThunk_ShowWidgetArgsFromSplice (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010DE58(void *p, int c, void *aref)
{
    int k = 0x8010;
    CRef r;
    CRef ref = func_00201360(1, aref, &k, &r);
    ref.Decref();
    ref.InitTag(0x24);
    func_00212828(p, &r);
    return ref;
}
