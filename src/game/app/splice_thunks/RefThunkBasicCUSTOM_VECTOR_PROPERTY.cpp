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
extern "C" { void EnsureLoSidebagVector(void *p, int n, qword q); CRef RefThunkBasicCUSTOM_VECTOR_PROPERTY(void *p, int c, void *aref); }
/** @sly2-annot addr=0x1006B8
 * Purpose: Splice option thunk: optid 10 on BASIC (g_aeopid[9] thunkFn/thunkUser, returns Void, crefReq 2);
 *   unpacks 2 args (Basic?, Vector) -> Void via RefSetArgsFromSplice then calls EnsureLoSidebagVector
 *   (0x132DE0)
 * Info: proposed name RefThunkBasicCUSTOM_VECTOR_PROPERTY (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunkBasicCUSTOM_VECTOR_PROPERTY(void *p, int c, void *aref)
{
    int ak[2] = {0x15, 2};
    union { qword q; } u;
    int n;
    CRef ref = func_00201360(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    EnsureLoSidebagVector(p, n, u.q);
    return ref;
}
