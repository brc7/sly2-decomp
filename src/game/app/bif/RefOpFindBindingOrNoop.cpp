struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(int n, int tagk) { m_tagk = tagk; u.n = n; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
struct O { int pad[2]; int k; O *pparent; int pad2[5]; int f; };
class CFrame { public: CRef *PrefFindBinding(unsigned int sym, int fRecurse); };
extern "C" CRef RefOpFindBindingOrNoop(void *pbasic, CRef *aref, O *po);
/** @sly2-annot addr=0x125510
 * Purpose: Looks up a symbol in the calling object frame chain (CFrame::PrefFindBinding(sym,1)); if unbound
 *   returns Bifk 0x86 (slot 134 no-op builtin)
 * Info: proposed name RefOpFindBindingOrNoop (T1); TU bif
 * Notes: names_v2 B proposed RefOpEmitSmokeCloud__FiP4CRefP6CFrame (0.856) | splice builtin 133
 *   find-binding-or-noop~ -> find-binding-or-noop~ (0.70): Body walks the caller frame chain to the object
 *   frame and returns PrefFindBinding(sym,1), else a CRef of builtin 0x86 = 134 noop~. Scripts call optional
 *   handler
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; static naming pass; sources:
 *   r1_116918+top50_round(splice builtin 133)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpFindBindingOrNoop(void *pbasic, CRef *aref, O *po)
{
    if (po && po->k == 1) {
        while (!po->f) {
            po = po->pparent;
            if (!po || po->k != 1)
                goto fail;
        }
        {
            CRef *pref = ((CFrame *)po->pparent)->PrefFindBinding(aref[0].u.n, 1);
            if (pref)
                return *pref;
        }
    }
fail:
    return CRef(0x86, 0x19);
}
