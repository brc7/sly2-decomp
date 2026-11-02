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
extern "C" { void *func_001FE5F0(void *p, int a, int b); CRef RefThunk_StartDialog(void *p, int c, void *aref); }
/** @sly2-annot addr=0x102038
 * Purpose: Splice option thunk: optid 264 on CID152 (g_aeopid[137] thunkFn, returns 0x2097, crefReq 2);
 *   unpacks 2 args (0x10, 0x105D) -> 0x2097 via RefSetArgsFromSplice then calls StartDialog (0x1FE5F0)
 * Info: proposed name RefThunk_StartDialog (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_StartDialog(void *p, int c, void *aref)
{
    int ak[2] = {0x10, 0x105D};
    int a, b;
    CRef ref = func_00201360(2, aref, ak, &a, &b);
    ref = func_001FE5F0(p, a, b);
    return ref;
}
