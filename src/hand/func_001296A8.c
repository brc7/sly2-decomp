typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VR { TI vec; int cref; } VR;
extern char D_00452188[];
VR *func_0011CCC0(void *pslotheap);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x1296A8
 * Purpose: Decref, tag 0x1A, new box, copy *pv
 * Info: proposed name CRef_SetVector (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_001296A8(CRef *pref, TI *pvec)
{
    VR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1A;
    p = func_0011CCC0(D_00452188);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((VR *)pref->m_p)->vec = *pvec;
    return pref;
}
