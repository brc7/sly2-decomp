typedef int TI __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VR { TI vec; int cref; } VR;
extern char D_00452188[];
VR *PvAllocSlotheapClear(void *pslotheap);
CRef *func_00129248(CRef *pref, TI vec)
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
