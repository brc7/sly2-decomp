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
extern "C" { void func_001F8A78(void *p, int, int, qword); CRef func_0010E5D8(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10E5D8
 * Purpose: Splice option thunk: optid 1060 on CID149 (g_aeopid[1228] thunkFn, returns Void, crefReq 3);
 *   unpacks 3 args (0x100D, Int, Vector) -> Void via RefSetArgsFromSplice then calls SetXfhActuator1World
 *   (0x1F8A78)
 * Info: proposed name RefThunk_SetXfhActuator1World (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x1F8A78 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010E5D8(void *p, int c, void *aref)
{
    int ak[3] = {0x100d, 0x5, 0x2};
    union { qword q; } v2;
    int v0;
    int v1;
    CRef ref = func_00201360(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    func_001F8A78(p, v0, v1, v2.q);
    return ref;
}
