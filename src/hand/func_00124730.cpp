struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
extern char D_003DD490[];
extern "C" {
int func_0015E9C8(void *pg, int k, void *a, int b, int c, void **ap);
CRef func_00124078(int c, void **ap);
CRef func_00124730(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x124730
 * Purpose: Splice builtin "find-class-objects": 0x15E9C8 by class + RefPairFromAplo
 * Info: proposed name RefOpFindClassObjects (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00124730(void *pbasic, CRef *aref)
{
    CRef ref;
    void *ap[0x180];
    int b = aref[1].u.n;
    int c = func_0015E9C8(D_003DD490, b ? 0x401 : 0x405, aref[0].u.pv, b, 0x180, ap);
    ref = func_00124078(c, ap);
    return ref;
}
