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
void GcBinding_Unlink(void *pv);
BINDING *PvNewBinding();
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
    void operator delete(void *pv) { GcBinding_Unlink(pv); func_0011CD98(D_004520D0, pv); }
};
struct SPB {
    BINDING *p;
    void Set(BINDING *pb);
    ~SPB() { BINDING *pb = p; if (pb && --pb->cref == 0) delete pb; }
    BINDING *operator->() { return p; }
    operator BINDING *() { return p; }
};
struct FRAME { char pad[0x1C]; SPM pbinding; };
extern "C" CRef CFrame_RefAddBinding(FRAME *pframe, int sym, CRef *pref);
/** @sly2-annot addr=0x127C00
 * Purpose: Prepends a new BINDING {sym, ref} (pool alloc 0x257340) to the frame binding list; returns Void
 * Info: proposed name CFrame_RefAddBinding (T1); class CFrame; TU frame
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#eval-0x126438-0x127b18,
 *   docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
CRef CFrame_RefAddBinding(FRAME *pframe, int sym, CRef *pref)
{
    SPM *ppbFirst = &pframe->pbinding;
    SPB sp;
    sp.Set(PvNewBinding());
    sp->sym = sym;
    sp->ref = *pref;
    sp->pnext = ppbFirst->p;
    *ppbFirst = sp;
    return CRef(0x24);
}
