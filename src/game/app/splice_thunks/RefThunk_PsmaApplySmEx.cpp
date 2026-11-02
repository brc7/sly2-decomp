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
extern "C" { void * PsmaApplySmEx(void *p, int, int, int, int); CRef RefThunk_PsmaApplySmEx(void *p, int c, void *aref); }
/** @sly2-annot addr=0x1048F0
 * Purpose: Splice option thunk: optid 477 on SM (g_aeopid[369] thunkFn, returns 0x2088, crefReq 4); unpacks 4
 *   args (0x2030, Oid, 0x100B, 0x1054) -> 0x2088 via RefSetArgsFromSplice then calls PsmaApplySmEx (0x1F03A8)
 * Info: proposed name RefThunk_PsmaApplySmEx (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_PsmaApplySmEx(void *p, int c, void *aref)
{
    int ak[4] = {0x2030, 0x6, 0x100b, 0x1054};
    int v0;
    int v1;
    int v2;
    int v3;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(4, aref, ak, &v0, &v1, &v2, &v3);
    ref = PsmaApplySmEx(p, v0, v1, v2, v3);
    return ref;
}
