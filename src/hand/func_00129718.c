typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VR { TI vec; int cref; } VR;
extern char D_00452188[];
VR *func_0011CCC0(void *pslotheap);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129718
 * Purpose: Same as SetVector but vector passed by value
 * Info: proposed name CRef_SetVector_VU_VECTOR (T1); class CRef; TU ref
 * Notes: C++ signature hint (VU_VECTOR) | COLLISION: CRef_SetVector already used (same name proposed for
 *   0x1296A8); renamed to CRef_SetVector_VU_VECTOR
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_00129718(CRef *pref, TI vec)
{
    VR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1A;
    p = func_0011CCC0(D_00452188);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((VR *)pref->m_p)->vec = vec;
    return pref;
}
