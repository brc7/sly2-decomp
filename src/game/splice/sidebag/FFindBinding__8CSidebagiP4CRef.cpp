struct CRef { int m_tagk; int m_tag; };
void func_00129658(CRef *pref, CRef *prefSrc) asm("func_00129658");
struct BINDING { int pad[2]; int sym; CRef ref; BINDING *pnext; };
class CSidebag {
public:
    int pad[2];
    BINDING *m_pbindingFirst;
    int FFindBinding(int sym, CRef *pref);
};
/** @sly2-annot addr=0x12AB28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/splice-scripting.md#sidebag-0x12a980-0x12ae18
 * @end-sly2-annot */
int CSidebag::FFindBinding(int sym, CRef *pref)
{
    for (BINDING *p = m_pbindingFirst; p; p = p->pnext) {
        if (p->sym == sym) {
            if (pref)
                func_00129658(pref, &p->ref);
            return 1;
        }
    }
    return 0;
}
