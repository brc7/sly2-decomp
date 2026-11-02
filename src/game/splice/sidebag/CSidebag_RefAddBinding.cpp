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
struct BINDING;
extern char D_004520D0[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_00255438(void *pv);
BINDING *func_00257340();
}
struct SPM {
    BINDING *p;
    ~SPM();
    SPM &operator=(BINDING *pb);
};
struct BINDING {
    int cref;
    int x4;
    int sym;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { func_00255438(pv); func_0011CD98(D_004520D0, pv); }
};
struct SPB {
    BINDING *p;
    void Set(BINDING *pb);
    ~SPB() { BINDING *pb = p; if (pb && --pb->cref == 0) delete pb; }
    BINDING *operator->() { return p; }
    operator BINDING *() { return p; }
};
struct FRAME { char pad[0x8]; SPM pbinding; };
extern "C" CRef CSidebag_RefAddBinding(FRAME *pframe, int sym, CRef *pref);
/** @sly2-annot addr=0x12A980
 * Purpose: Prepends BINDING {n, ref} to sidebag list
 * Info: proposed name CSidebag_RefAddBinding (T1); class CSidebag; TU sidebag
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#sidebag-0x12a980-0x12ae18,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
CRef CSidebag_RefAddBinding(FRAME *pframe, int sym, CRef *pref)
{
    SPM *ppbFirst = &pframe->pbinding;
    SPB sp;
    sp.Set(func_00257340());
    sp->sym = sym;
    sp->ref = *pref;
    sp->pnext = ppbFirst->p;
    *ppbFirst = sp;
    return CRef(0x24);
}
