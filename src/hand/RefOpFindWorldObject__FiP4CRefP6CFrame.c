typedef struct CRef { int m_tagk; int m_tag; } CRef;
void Decref__4CRef(CRef *pref);
CRef *__4CRefRC4CRef(CRef *pref, CRef *prefSrc);
void CRef_SetBasic(CRef *pref, void *p);
extern char D_003DD490[];
/** @sly2-annot addr=0x124640
 * Purpose: Splice builtin "find-world-object": CRef(BASIC*) of &D_003DD490 (g_sw)
 * Info: proposed name RefOpFindWorldObject (T1); TU bif
 * Notes: names_v2 C proposed RefOpFindPlayerObject__FiP4CRefP6CFrame (0.200)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef *RefOpFindWorldObject__FiP4CRefP6CFrame(CRef *ret)
{
    CRef ref;
    ref.m_tagk = -1;
    CRef_SetBasic(&ref, D_003DD490);
    __4CRefRC4CRef(ret, &ref);
    Decref__4CRef(&ref);
    return ret;
}
