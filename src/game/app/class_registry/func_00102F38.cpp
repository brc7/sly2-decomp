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
extern "C" { void func_00207E08(void *p, void *, int, float, float, float, float); CRef func_00102F38(void *p, int c, void *aref); }
/** @sly2-annot addr=0x102F38
 * Purpose: Splice option thunk: optid 371 on STEPGUARD (g_aeopid[245] thunkFn, returns Void, crefReq 6);
 *   unpacks 6 args (0x9010, Oid, Float, Float, Float, Float) -> Void via RefSetArgsFromSplice then calls
 *   SetStepguardAnimBindingK2Resolve (0x207E08)
 * Info: proposed name RefThunk_SetStepguardAnimBindingK2Resolve (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x207E08 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00102F38(void *p, int c, void *aref)
{
    int ak[6] = {0x9010, 0x6, 0x1, 0x1, 0x1, 0x1};
    CRef v0;
    int v1;
    float v2;
    float v3;
    float v4;
    float v5;
    CRef ref = func_00201360(6, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5);
    ref.Decref();
    ref.InitTag(0x24);
    func_00207E08(p, &v0, v1, v2, v3, v4, v5);
    return ref;
}
