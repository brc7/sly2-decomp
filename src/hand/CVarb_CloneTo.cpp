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
extern char D_004520D0[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_00255438(void *pv);
W *func_00257340();
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
    int x8;
    CRef ref;
    SPM pnext;
    void SetNext(W *pw) { pnext = pw; }
    void operator delete(void *pv) { func_00255438(pv); func_0011CD98(D_004520D0, pv); }
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
extern "C" SPW CVarb_CloneTo(W *src, void *a, void *b);
/** @sly2-annot addr=0x12B0E8
 * Purpose: Clones a binding (varb) chain: new binding (0x257340) per entry with RefClone
 * Info: proposed name CVarb_CloneTo (T1); class CVarb; TU varb
 * Notes: agent qualifier: (returns clone)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138,
 *   docs/reference/functions/splice-scripting.md#sidebag-0x12a980-0x12ae18
 * @end-sly2-annot */
SPW CVarb_CloneTo(W *src, void *a, void *b)
{
    SPW sp;
    sp.Set(func_00257340());
    sp->x8 = src->x8;
    sp->ref = CRef_CloneTo(&src->ref, a, b);
    if (src->pnext.p)
        sp->SetNext(CVarb_CloneTo(src->pnext.p, a, b).p);
    return SPW(sp.p);
}
