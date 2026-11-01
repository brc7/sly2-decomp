typedef struct VT { short d; short pad; void (*pfn)(void *); } VT;
typedef struct O { char pad[0xC]; VT *vt; } O;
typedef struct CRef { int m_tagk; union { int *pn; char *pb; O *po; } u; } CRef;
void Incref__4CRef(CRef *pref)
{
    switch (pref->m_tagk) {
    case 0x1B:
        (*(int *)(pref->u.pb + 0x40))++;
        break;
    case 0x1A:
    case 0x1C:
        (*(int *)(pref->u.pb + 0x10))++;
        break;
    case 0x1D:
        (*(int *)(pref->u.pb + 0x8))++;
        break;
    case 0x1E:
        (*(int *)(pref->u.pb + 0xC))++;
        break;
    case 0x21: {
        O *po = pref->u.po;
        if (po)
            po->vt[1].pfn((char *)po + po->vt[1].d);
        break;
    }
    case 0x1F:
    case 0x20:
    case 0x22:
        (*pref->u.pn)++;
        break;
    }
}
