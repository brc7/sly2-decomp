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
extern "C" { void func_00185E30(void *p, qword, qword, qword); CRef func_0010B618(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10B618
 * Purpose: Splice option thunk: optid 754 on JT (g_aeopid[771] thunkFn/thunkUser, returns Void, crefReq 3);
 *   unpacks 3 args (Vector, Vector, Vector) -> Void via RefSetArgsFromSplice then calls SetMeshVectors
 *   (0x185E30)
 * Info: proposed name RefThunk_SetMeshVectors (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x185E30 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010B618(void *p, int c, void *aref)
{
    int ak[3] = {0x2, 0x2, 0x2};
    union { qword q; } v0;
    union { qword q; } v1;
    union { qword q; } v2;
    CRef ref = func_00201360(3, aref, ak, &v0, &v1, &v2);
    ref.Decref();
    ref.InitTag(0x24);
    func_00185E30(p, v0.q, v1.q, v2.q);
    return ref;
}
