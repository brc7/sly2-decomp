typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VT { short d; short pad; void (*fn)(void *); } VT;
typedef struct O { int pad[3]; VT *vt; } O;
void Decref__4CRef(CRef *pref);
CRef *CRef_SetBasic(CRef *pref, O *p)
{
    if (p)
        p->vt[1].fn((char *)p + p->vt[1].d);
    Decref__4CRef(pref);
    pref->m_p = p;
    pref->m_tagk = 0x21;
    return pref;
}
