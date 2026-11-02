struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct W;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void GcPair_Unlink(void *pv);
W *PvNewPair();
CRef CRef_CloneTo(CRef *pref, void *a, void *b);
}
struct SPM {
    W *p;
    ~SPM();
    SPM &operator=(W *pw);
};
struct W {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void SetNext(W *pw) { pnext = pw; }
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPW {
    W *p;
    void Set(W *pw);
    SPW() {}
    SPW(W *pw) { Set(pw); }
    SPW(const SPW &);
    ~SPW() { W *pw = p; if (pw && --pw->cref == 0) delete pw; }
    W *operator->() { return p; }
};
extern "C" SPW CPair_CloneTo(W *src, void *a, void *b);
/** @sly2-annot addr=0x128CB8
 * Purpose: Deep-copies a list: new nodes (0x256B48) with RefClone of each element
 * Info: proposed name CPair_CloneTo (T1); class CPair; TU pair
 * Notes: agent qualifier: (returns clone)
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#pair-0x128cb8-0x128f70,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
SPW CPair_CloneTo(W *src, void *a, void *b)
{
    SPW sp;
    sp.Set(PvNewPair());
    sp->ref = CRef_CloneTo(&src->ref, a, b);
    if (src->pnext.p)
        sp->SetNext(CPair_CloneTo(src->pnext.p, a, b).p);
    return SPW(sp.p);
}
