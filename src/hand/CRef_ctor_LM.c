typedef struct CLQ { int a, b; } CLQ;
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct LR { CLQ v; int cref; } LR;
extern char D_004521D0[];
LR *PvAllocSlotheapClear(void *pslotheap);
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
