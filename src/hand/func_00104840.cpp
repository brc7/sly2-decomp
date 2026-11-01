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
extern "C" { void *func_001F0380(void *p, int a, int b, int c); CRef func_00104840(void *p, int c, void *aref); }
/** @sly2-annot addr=0x104840
 * Purpose: Splice option thunk: optid 214 on SM (g_aeopid[368] thunkFn, returns 0x2088, crefReq 3); unpacks 3
 *   args (0x2030, Oid, 0x100B) -> 0x2088 via RefSetArgsFromSplice then calls PsmaApplySm (0x1F0380)
 * Info: proposed name RefThunkSmAPPLY (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00104840(void *p, int c, void *aref)
{
    int ak[3] = {0x2030, 6, 0x100B};
    int a, b, d;
    CRef ref = func_00201360(3, aref, ak, &a, &b, &d);
    ref = func_001F0380(p, a, b, d);
    return ref;
}
