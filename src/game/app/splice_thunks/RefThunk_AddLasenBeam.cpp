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
extern "C" { void AddLasenBeam(void *p, int, float, float, float, int, int, float, float, float, int); CRef RefThunk_AddLasenBeam(void *p, int c, void *aref); }
/** @sly2-annot addr=0x1056A0
 * Purpose: Splice option thunk: optid 541 on CID85 (g_aeopid[455] thunkUser, returns Void, crefReq 10);
 *   unpacks 10 args (Oid, Float, Float, Float, 0x100B, Oid, Float, Float, Float, 0x100B) -> Void via
 *   RefSetArgsFromSplice then calls AddLasenBeam (0x1DF908)
 * Info: proposed name RefThunk_AddLasenBeam (T1); TU crt0_splicewrap
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef RefThunk_AddLasenBeam(void *p, int c, void *aref)
{
    int ak[10] = {0x6, 0x1, 0x1, 0x1, 0x100b, 0x6, 0x1, 0x1, 0x1, 0x100b};
    int v0;
    float v1;
    float v2;
    float v3;
    int v4;
    int v5;
    float v6;
    float v7;
    float v8;
    int v9;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(10, aref, ak, &v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9);
    ref.Decref();
    ref.InitTag(0x24);
    AddLasenBeam(p, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    return ref;
}
