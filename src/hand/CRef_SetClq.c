typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VR { TI vec; int cref; } VR;
extern char D_004521B8[];
VR *PvAllocSlotheapClear(void *pslotheap);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129808
 * Purpose: Decref, tag 0x1C
 * Info: proposed name CRef_SetClq (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_SetClq(CRef *pref, TI *pvec)
{
    VR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1C;
    p = PvAllocSlotheapClear(D_004521B8);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((VR *)pref->m_p)->vec = *pvec;
    return pref;
}
