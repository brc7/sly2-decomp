typedef struct CLT { int a, b, c; } CLT;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct TR { CLT v; int cref; } TR;
extern char D_004521E8[];
TR *PvAllocSlotheapClear(void *pslotheap);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129408
 * Purpose: Constructor: tag 0x1E (SMP, 12 B box, pool D_004521E8)
 * Info: proposed name CRef_ctor_SMP (T1); class CRef; TU ref
 * Notes: C++ signature hint (const SMP *) | COLLISION: CRef_ctor already used (same name proposed for
 *   0x129248); renamed to CRef_ctor_SMP
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_ctor_SMP(CRef *pref, CLT *pv)
{
    TR *p;

    pref->m_tagk = 0x1E;
    p = PvAllocSlotheapClear(D_004521E8);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((TR *)pref->m_p)->v = *pv;
    return pref;
}
