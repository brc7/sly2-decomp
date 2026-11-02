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
extern "C" { int KCalculateDetectorTarget(void *p, int, int); CRef RefThunk_KCalculateDetectorTarget(void *p, int c, void *aref); }
/** @sly2-annot addr=0x104188
 * Purpose: Splice option thunk: optid 466 on DETECTOR (g_aeopid[347] thunkFn, returns 0x106B, crefReq 2);
 *   unpacks 2 args (0x2002, 0x106B) -> 0x106B via RefSetArgsFromSplice then calls KCalculateDetectorTarget
 *   (0x1541D8)
 * Info: proposed name RefThunk_KCalculateDetectorTarget (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_KCalculateDetectorTarget(void *p, int c, void *aref)
{
    int ak[2] = {0x2002, 0x106b};
    int v0;
    int v1;
    CRef ref = func_00201360(2, aref, ak, &v0, &v1);
    ref = KCalculateDetectorTarget(p, v0, v1);
    return ref;
}
