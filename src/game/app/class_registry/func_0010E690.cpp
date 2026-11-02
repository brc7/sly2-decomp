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
extern "C" { void func_001F8AD8(void *p, int a, int b, void *ps); CRef func_0010E690(void *p, int c, void *aref); }
struct BIG { qword aq[4]; };
/** @sly2-annot addr=0x10E690
 * Purpose: Splice option thunk: optid 1061 on CID149 (g_aeopid[1229] thunkFn, returns Void, crefReq 3);
 *   unpacks 3 args (0x100D, Int, 0x4) -> Void via RefSetArgsFromSplice then calls SetXfhActuator1Mat
 *   (0x1F8AD8)
 * Info: proposed name RefThunk_SetXfhActuator1Mat (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x1F8AD8 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_0010E690(void *p, int c, void *aref)
{
    int ak[3] = {0x100D, 5, 4};
    BIG s;
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(3, aref, ak, &a, &b, &s);
    ref.Decref();
    ref.InitTag(0x24);
    func_001F8AD8(p, a, b, &s);
    return ref;
}
