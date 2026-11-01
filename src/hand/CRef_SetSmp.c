typedef struct CLT { int a, b, c; } CLT;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct TR { CLT v; int cref; } TR;
extern char D_004521E8[];
TR *PvAllocSlotheapClear(void *pslotheap);
void Decref__4CRef(CRef *pref);
CRef *CRef_SetSmp(CRef *pref, CLT *pv)
{
    TR *p;
    Decref__4CRef(pref);
    pref->m_tagk = 0x1E;
    p = PvAllocSlotheapClear(D_004521E8);
    p->cref = 0;
    pref->m_p = p;
    p->cref++;
    ((TR *)pref->m_p)->v = *pv;
    return pref;
}
