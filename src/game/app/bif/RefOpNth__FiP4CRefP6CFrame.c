typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef struct PAIR { int a; int b; CRef ref; struct PAIR *pnext; } PAIR;
void SetTag__4CRef4TAGK(CRef *pref, int tagk);
void func_00129658(CRef *pref, CRef *prefSrc);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x120D78
 * Purpose: Splice builtin "nth": walk list to index a1
 * Info: proposed name RefOpNth (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verify_side
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpNth__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    SetTag__4CRef4TAGK(&ref, 0);
    if (aref[0].m_tagk == 0x1F) {
        PAIR *p = (PAIR *)aref[0].m_tag;
        CRef *pn = &aref[1];
        int i;
        for (i = 0; p; p = p->pnext, i++) {
            if (i == pn->m_tag) {
                func_00129658(&ref, &p->ref);
                break;
            }
        }
    }
    func_001291A8(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
