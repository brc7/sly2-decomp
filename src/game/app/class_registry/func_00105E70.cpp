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
extern "C" CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *an);
extern "C" {
void func_00170BA0(void *p, void *pv);
CRef func_00105E70(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x105E70
 * Purpose: Splice option thunk: optid 624 on CID113 (g_aeopid[597] thunkFn/thunkUser, returns Void, crefReq
 *   1); unpacks 1 args (Lm) -> Void via RefSetArgsFromSplice then calls AddCid113CmdSetK2D4 (0x170BA0)
 * Info: proposed name RefThunk_AddCid113CmdSetK2D4 (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x170BA0 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00105E70(void *p, int c, void *aref)
{
    union { qword q; } u;
    int k = 0xA;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(1, aref, &k, &u);
    ref.Decref();
    ref.InitTag(0x24);
    func_00170BA0(p, &u);
    return ref;
}
