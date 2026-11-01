typedef struct CLQ { int a, b; } CLQ;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct LR { CLQ v; int cref; } LR;
extern char D_004521D0[];
LR *PvAllocSlotheapClear(void *pslotheap);
/** @sly2-annot addr=0x129398
 * Purpose: Constructor: tag 0x1D (LM, 8 B box, pool D_004521D0)
 * Info: proposed name CRef_ctor_LM (T1); class CRef; TU ref
 * Notes: C++ signature hint (const LM *) | COLLISION: CRef_ctor already used (same name proposed for
 *   0x129248); renamed to CRef_ctor_LM
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_ctor_LM(CRef *pref, CLQ *pv)
{
    LR *p;
    pref->m_tagk = 0x1D;
    p = PvAllocSlotheapClear(D_004521D0);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((LR *)pref->m_p)->v = *pv;
    return pref;
}
