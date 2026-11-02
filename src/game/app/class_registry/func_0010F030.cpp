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
extern "C" { void func_00163838(void *p, int, void *, int, int); CRef func_0010F030(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10F030
 * Purpose: Splice option thunk: optid 1108 on TASK (g_aeopid[1287] thunkUser, returns Void, crefReq 4);
 *   unpacks 4 args (Oid, 0x9050, Oid, 0xF) -> Void via RefSetArgsFromSplice then calls AddTaskStateLo
 *   (0x163838)
 * Info: proposed name RefThunk_AddTaskStateLo (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x163838 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010F030(void *p, int c, void *aref)
{
    int ak[4] = {0x6, 0x9050, 0x6, 0xf};
    CRef v1;
    int v0;
    int v2;
    int v3;
    CRef ref = func_00201360(4, aref, ak, &v0, &v1, &v2, &v3);
    ref.Decref();
    ref.InitTag(0x24);
    func_00163838(p, v0, &v1, v2, v3);
    return ref;
}
