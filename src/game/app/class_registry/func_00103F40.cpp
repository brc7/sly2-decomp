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
extern "C" { void func_0020A100(void *p, int n, qword q); CRef func_00103F40(void *p, int c, void *aref); }
/** @sly2-annot addr=0x103F40
 * Purpose: Splice option thunk: optid 446 on STEPGUARD (g_aeopid[322] thunkFn, returns Void, crefReq 2);
 *   unpacks 2 args (0x2002, Vector) -> Void via RefSetArgsFromSplice then calls SetStepguardRootBhvPoint
 *   (0x20A100)
 * Info: proposed name RefThunk_SetStepguardRootBhvPoint (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x20A100 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00103F40(void *p, int c, void *aref)
{
    int ak[2] = {0x2002, 2};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    func_0020A100(p, n, u.q);
    return ref;
}
