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
extern "C" { void EnsureLoSidebagClq__FP2LO5OPTIDP3CLQ(void *p, int n, void *pu); CRef RefThunkBasicCUSTOM_CLQ_PROPERTY(void *p, int c, void *aref); }
/** @sly2-annot addr=0x1004A0
 * Purpose: Splice option thunk: optid 7 on BASIC (g_aeopid[6] thunkFn/thunkUser, returns Void, crefReq 2);
 *   unpacks 2 args (Basic?, Clq) -> Void via RefSetArgsFromSplice then calls EnsureLoSidebagClq (0x132CC8)
 * Info: proposed name RefThunkBasicCUSTOM_CLQ_PROPERTY (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunkBasicCUSTOM_CLQ_PROPERTY(void *p, int c, void *aref)
{
    int ak[2] = {0x15, 9};
    union { qword q; } u;
    int n;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &n, &u);
    ref.Decref();
    ref.InitTag(0x24);
    EnsureLoSidebagClq__FP2LO5OPTIDP3CLQ(p, n, &u);
    return ref;
}
