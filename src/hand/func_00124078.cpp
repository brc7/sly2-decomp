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
    CRef &operator=(struct NODE *p);
    CRef &operator=(struct O *po);
};
struct O;
struct NODE { int cref; int pad; CRef ref; NODE *pnext; };
extern "C" {
NODE *func_00256B48();
void func_00256C58(NODE **pp, NODE *p);
CRef func_00124078(int c, O **apo);
}
/** @sly2-annot addr=0x124078
 * Purpose: Builds a Splice list of CRef(BASIC*) from an LO* array
 * Info: proposed name RefPairFromAplo (T1); TU bif
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00124078(int c, O **apo)
{
    if (c == 0)
        return CRef(0);
    CRef ref;
    NODE *pprev = 0;
    int i;
    for (i = 0; i < c; i++) {
        NODE *p = func_00256B48();
        if (pprev)
            func_00256C58(&pprev->pnext, p);
        else
            ref = p;
        p->ref = apo[i];
        pprev = p;
    }
    return ref;
}
