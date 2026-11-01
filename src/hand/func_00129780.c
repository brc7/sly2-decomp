typedef struct MATRIX4 { float m[4][4]; } __attribute__((aligned(16))) MATRIX4;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct MR { MATRIX4 mat; int cref; } MR;
extern char D_004521A0[];
MR *func_0011CCC0(void *pslotheap);
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129780
 * Purpose: Decref, tag 0x1B, new box, copy
 * Info: proposed name CRef_SetMatrix (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *func_00129780(CRef *pref, MATRIX4 *pmat)
{
    MR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1B;
    p = func_0011CCC0(D_004521A0);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((MR *)pref->m_p)->mat = *pmat;
    return pref;
}
