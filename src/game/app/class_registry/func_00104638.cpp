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
extern "C" { void func_0020B060(void *p, int, float, float, float, float); CRef func_00104638(void *p, int c, void *aref); }
/** @sly2-annot addr=0x104638
 * Purpose: Splice option thunk: optid 371 on CID120 (g_aeopid[365] thunkFn, returns Void, crefReq 5); unpacks
 *   5 args (Oid, Float, Float, Float, Float) -> Void via RefSetArgsFromSplice then calls
 *   SetCid120AttackBindingK2Resolve (0x20B060)
 * Info: proposed name RefThunk_SetCid120AttackBindingK2Resolve (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x20B060 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00104638(void *p, int c, void *aref)
{
    int ak[5] = {0x6, 0x1, 0x1, 0x1, 0x1};
    int v0;
    float v1;
    float v2;
    float v3;
    float v4;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(5, aref, ak, &v0, &v1, &v2, &v3, &v4);
    ref.Decref();
    ref.InitTag(0x24);
    func_0020B060(p, v0, v1, v2, v3, v4);
    return ref;
}
