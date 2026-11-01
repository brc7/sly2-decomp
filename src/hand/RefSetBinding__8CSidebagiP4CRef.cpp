struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct BINDING { int a, b; int sym; CRef ref; BINDING *pnext; };
class CSidebag {
public:
    int a, b;
    BINDING *pbindingFirst;
    CRef RefSetBinding(int sym, CRef *pref);
};
/** @sly2-annot addr=0x12AA70
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/splice-scripting.md#sidebag-0x12a980-0x12ae18
 * @end-sly2-annot */
CRef CSidebag::RefSetBinding(int sym, CRef *pref)
{
    BINDING *pb;
    for (pb = pbindingFirst; pb; pb = pb->pnext) {
        if (pb->sym == sym) {
            CRef ref;
            pb->ref = *pref;
            ref.Decref();
            ref.InitTag(0x24);
            return ref;
        }
    }
    {
        CRef ref;
        ref.Decref();
        ref.InitTag(0x24);
        return ref;
    }
}
