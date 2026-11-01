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
    CRef &operator=(float g);
};
struct OBJ { int a[4]; OBJ(); ~OBJ(); };
extern char D_003DD490[];
extern "C" {
void func_0020FAA0(void *pg, qword a, qword b, void *p3, void *p4, int *pc, void **pap);
void func_0020FDC8(int c, void *ap, int *pc, void **pap);
CRef func_00124078(int c, void *ap);
CRef func_001247D8(void *pbasic, CRef *aref);
}
/** @sly2-annot addr=0x1247D8
 * Purpose: Splice builtin "find-objects-in-bounding-box": 0x20FAA0 box query + 0x20FDC8 filter
 * Info: proposed name RefOpFindObjectsInBoundingBox (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_001247D8(void *pbasic, CRef *aref)
{
    CRef ref;
    OBJ obj;
    int c0;
    void *ap0;
    int c1;
    void *ap1;
    func_0020FAA0(D_003DD490, *(qword *)aref[0].u.pv, *(qword *)aref[1].u.pv, 0, 0, &c0, &ap0);
    func_0020FDC8(c0, ap0, &c1, &ap1);
    ref = func_00124078(c1, ap1);
    return ref;
}
