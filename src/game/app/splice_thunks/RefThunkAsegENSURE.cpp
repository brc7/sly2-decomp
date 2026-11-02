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
extern "C" { void * func_00130598(void *p, int, int, float, float, int); CRef RefThunkAsegENSURE(void *p, int c, void *aref); }
/** @sly2-annot addr=0x101A88
 * Purpose: Splice option thunk: optid 216 on ASEG (g_aeopid[108] thunkFn, returns 0x2086, crefReq 5); unpacks
 *   5 args (0x2030, 0x100C, Float, Float, 0x100B) -> 0x2086 via RefSetArgsFromSplice then calls
 *   PasegaEnsureAseg__FP4ASEGP3ALO4SEEKffi (0x130598)
 * Info: proposed name RefThunkAsegENSURE (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunkAsegENSURE(void *p, int c, void *aref)
{
    int ak[5] = {0x2030, 0x100c, 0x1, 0x1, 0x100b};
    int v0;
    int v1;
    float v2;
    float v3;
    int v4;
    CRef ref = func_00201360(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref = func_00130598(p, v0, v1, v2, v3, v4);
    return ref;
}
