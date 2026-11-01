typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; void *m_pv; } CRef;
typedef struct SLOTHEAP { int a[6]; } SLOTHEAP;
typedef struct NV { qword v; int cref; } NV;
typedef struct NM { qword m[4]; int cref; } NM;
typedef struct N2 { int a[2]; int cref; } N2;
typedef struct N3 { int a[3]; int cref; } N3;
extern SLOTHEAP D_00452188;
extern SLOTHEAP D_004521A0;
extern SLOTHEAP D_004521B8;
extern SLOTHEAP D_004521D0;
extern SLOTHEAP D_004521E8;
void *PvAllocSlotheapClear(SLOTHEAP *psh);
void InitTag__4CRef(CRef *pref, int tagk)
{
    pref->m_tagk = tagk;
    switch (tagk) {
    case 0x1A: {
        NV *p = (NV *)PvAllocSlotheapClear(&D_00452188);
        p->cref = 0;
        pref->m_pv = p;
        p->cref++;
        break;
    }
    case 0x1B: {
        NM *p = (NM *)PvAllocSlotheapClear(&D_004521A0);
        p->cref = 0;
        pref->m_pv = p;
        p->cref++;
        break;
    }
    case 0x1C: {
        NV *p = (NV *)PvAllocSlotheapClear(&D_004521B8);
        p->cref = 0;
        pref->m_pv = p;
        p->cref++;
        break;
    }
    case 0x1D: {
        N2 *p = (N2 *)PvAllocSlotheapClear(&D_004521D0);
        p->cref = 0;
        pref->m_pv = p;
        p->cref++;
        break;
    }
    case 0x1E: {
        N3 *p = (N3 *)PvAllocSlotheapClear(&D_004521E8);
        p->cref = 0;
        pref->m_pv = p;
        p->cref++;
        break;
    }
    case 0x1F:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    default:
        pref->m_pv = 0;
        break;
    }
}
