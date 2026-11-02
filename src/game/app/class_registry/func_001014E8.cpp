typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(void *pv);
};
extern "C" {
CRef RefSetArgsFromSplice__FiP4CRefP4OTYPe(int c, void *aref, int *ak, void *a0, void *a1);
void *func_001F8510(void *p, int a, int b);
CRef func_001014E8(void *p, int c, void *aref);
}
/** @sly2-annot addr=0x1014E8
 * Purpose: Splice option thunk: optid 194 on SO (g_aeopid[77] thunkFn, returns 0x2095, crefReq 2); unpacks 2
 *   args (Oid, 0x104C) -> 0x2095 via RefSetArgsFromSplice then calls PxfhNewSo (0x1F8510)
 * Info: proposed name RefThunk_PxfhNewSo (T2); TU crt0_splicewrap
 * Notes: T2 forced: thunk name is only as good as its native (0x1F8510 T2)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r0_thunks+table-verified(g_aeopid
 *   D_002AC880)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
CRef func_001014E8(void *p, int c, void *aref)
{
    int ak[2] = {6, 0x104c};
    int a, b;
    CRef ref = RefSetArgsFromSplice__FiP4CRefP4OTYPe(2, aref, ak, &a, &b);
    ref = func_001F8510(p, a, b);
    return ref;
}
