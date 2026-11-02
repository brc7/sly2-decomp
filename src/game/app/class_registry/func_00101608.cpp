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
extern "C" { void func_001F82B8(void *p, int, void *); CRef func_00101608(void *p, int c, void *aref); }
/** @sly2-annot addr=0x101608
 * Purpose: Splice option thunk: optid 196 on SO (g_aeopid[79] thunkFn/thunkUser, returns Void, crefReq 2);
 *   unpacks 2 args (0x104D, Lm) -> Void via RefSetArgsFromSplice then calls SetSoSideRecordParams (0x1F82B8)
 * Info: proposed name RefThunk_SetSoSideRecordParams (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x1F82B8 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_00101608(void *p, int c, void *aref)
{
    int ak[2] = {0x104d, 0xa};
    union { qword q; } v1;
    int v0;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &v0, &v1);
    ref.Decref();
    ref.InitTag(0x24);
    func_001F82B8(p, v0, &v1);
    return ref;
}
