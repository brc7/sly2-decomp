typedef struct CLQ { int a, b; } CLQ;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct LR { CLQ v; int cref; } LR;
extern char D_004521D0[];
LR *func_0011CCC0(void *pslotheap);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129878
 * Purpose: Decref, tag 0x1D
 * Info: proposed name CRef_SetLm (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_00129878(CRef *pref, CLQ *pv)
{
    LR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1D;
    p = func_0011CCC0(D_004521D0);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((LR *)pref->m_p)->v = *pv;
    return pref;
}
