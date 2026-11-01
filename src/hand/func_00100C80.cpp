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
extern "C" CRef func_00201360(int c, void *aref, int *ak, void *an);
struct VT { short d; short pad; void *pfn; };
struct LO { char pad[0xC]; VT *pvt; };
extern "C" CRef func_00100C80(LO *p, int c, void *aref);
/** @sly2-annot addr=0x100C80
 * Purpose: Splice option thunk: optid 108 on SO (g_aeopid[43] thunkFn, returns Bool, crefReq 1); unpacks 1
 *   args (Vector) -> Bool via RefSetArgsFromSplice then calls virtual SO slot 73 -> FPosInSo (0x1F72F8)
 * Info: proposed name RefThunkSoPOINT_IN_BSP_QUICK (T1); TU crt0_splicewrap
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00100C80(LO *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 2;
    CRef ref = func_00201360(1, aref, &k, &u);
    VT *pvt = &p->pvt[73];
    ref = ((int (*)(void *, qword))pvt->pfn)((char *)p + pvt->d, u.q) != 0;
    return ref;
}
