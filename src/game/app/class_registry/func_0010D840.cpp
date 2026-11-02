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
extern "C" { void * func_00156698(void *p, qword, int); CRef func_0010D840(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10D840
 * Purpose: Splice option thunk: optid 214 on CID117 (g_aeopid[1095] thunkFn, returns 0x2093, crefReq 2);
 *   unpacks 2 args (Vector, 0x2002) -> 0x2093 via RefSetArgsFromSplice then calls PnoiseEmit (0x156698)
 * Info: proposed name RefThunkCid117APPLY (T2); TU crt0_splicewrap
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010D840(void *p, int c, void *aref)
{
    int ak[2] = {0x2, 0x2002};
    union { qword q; } v0;
    int v1;
    CRef ref = func_00201360(2, aref, ak, &v0, &v1);
    ref = func_00156698(p, v0.q, v1);
    return ref;
}
