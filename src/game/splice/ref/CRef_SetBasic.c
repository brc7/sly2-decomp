typedef struct CRef { int m_tagk; void *m_p; } CRef;
typedef struct VT { short d; short pad; void (*fn)(void *); } VT;
typedef struct O { int pad[3]; VT *vt; } O;
void Decref__4CRef(CRef *pref);
/** @sly2-annot addr=0x129A10
 * Purpose: Object vtable Incref (slot 1), Decref old, tag 0x21
 * Info: proposed name CRef_SetBasic (T1); class CRef; TU ref
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef *CRef_SetBasic(CRef *pref, O *p)
{
    if (p)
        p->vt[1].fn((char *)p + p->vt[1].d);
    Decref__4CRef(pref);
    pref->m_p = p;
    pref->m_tagk = 0x21;
    return pref;
}
