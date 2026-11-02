typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VR { TI vec; int cref; } VR;
extern char D_00452188[];
VR *PvAllocSlotheapClear(void *pslotheap);
/** @sly2-annot addr=0x129248
 * Purpose: Constructor: tag 0x1A from a vector passed by value in a qword register
 * Info: proposed name CRef_ctor (T1); class CRef; TU ref
 * Notes: C++ signature hint (VU_VECTOR)
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_ctor(CRef *pref, TI vec)
{
    VR *p;
    pref->m_tagk = 0x1A;
    p = PvAllocSlotheapClear(D_00452188);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((VR *)pref->m_p)->vec = vec;
    return pref;
}
