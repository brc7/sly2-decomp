struct CRef { int m_tagk; int m_tag; };
struct BINDING { int a, b; unsigned int sym; CRef ref; BINDING *pnext; };
class CFrame {
public:
    int a, b;
    int cframe;
    CFrame *apframe[4];
    BINDING *pbindingFirst;
    int fBusy;
    CRef *PrefFindBinding(unsigned int sym, int fRecurse);
    CRef *PrefFindLocal(unsigned int sym) { BINDING *pb; for (pb = pbindingFirst; pb; pb = pb->pnext) if (pb->sym == sym) return &pb->ref; return 0; }
};
/** @sly2-annot addr=0x127DB8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
CRef *CFrame::PrefFindBinding(unsigned int sym, int fRecurse)
{
    CRef *pref = 0;
    BINDING *pb;
    int i;
    fBusy = 1;
    for (pb = pbindingFirst; pb; pb = pb->pnext) {
        if (pb->sym == sym) {
            pref = &pb->ref;
            goto done;
        }
    }
    if (!fRecurse)
        goto done;
    for (i = 0; i < cframe; i++) {
        CFrame *pframe = apframe[i];
        if (!pframe->fBusy) {
            pref = pframe->PrefFindBinding(sym, fRecurse);
            if (pref)
                goto done;
        }
    }
done:
    fBusy = 0;
    return pref;
}
