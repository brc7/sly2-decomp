typedef struct MATRIX4 { float m[4][4]; } __attribute__((aligned(16))) MATRIX4;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct MR { MATRIX4 mat; int cref; } MR;
extern char D_004521A0[];
MR *PvAllocSlotheapClear(void *pslotheap);
void Decref__4CRef(CRef *pref);
CRef *CRef_SetMatrix(CRef *pref, MATRIX4 *pmat)
{
    MR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1B;
    p = PvAllocSlotheapClear(D_004521A0);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((MR *)pref->m_p)->mat = *pmat;
    return pref;
}
