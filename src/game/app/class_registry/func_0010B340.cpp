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
extern "C" { void func_0019EAA0(void *p, qword a, qword b); CRef func_0010B340(void *p, int c, void *aref); }
/** @sly2-annot addr=0x10B340
 * Purpose: Splice option thunk: optid 749 on STEPPLAYER (g_aeopid[766] thunkFn/thunkUser, returns Void,
 *   crefReq 2); unpacks 2 args (Vector, Vector) -> Void via RefSetArgsFromSplice then calls SetJtPipeVectors
 *   (0x19EAA0)
 * Info: proposed name RefThunk_SetJtPipeVectors (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x19EAA0 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010B340(void *p, int c, void *aref)
{
    int ak[2] = {2, 2};
    union { qword q; } u;
    union { qword q; } v;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &u, &v);
    ref.Decref();
    ref.InitTag(0x24);
    func_0019EAA0((char *)p + 0xF50, u.q, v.q);
    return ref;
}
