struct VECTOR; struct MATRIX4; struct V16; struct V8; struct V12; struct OBJ;
struct NODE; struct O;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_002550F8(void *pv);
}
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    CRef(const VECTOR *pv);
    CRef(const MATRIX4 *pm);
    CRef(const V16 *p);
    CRef(const V8 *p);
    CRef(const V12 *p);
    CRef(NODE *p);
    CRef(O *p);
    CRef(OBJ *p);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct SPM {
    NODE *p;
    ~SPM();
};
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    SPN(const SPN &);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
};
struct O {
    int cref;
    ~O();
};
struct SPO {
    O *p;
    SPO(const SPO &);
    ~SPO() { O *po = p; if (po && --po->cref == 0) delete po; }
};
extern "C" {
SPN func_00128CB8(NODE *pn, void *a, void *b);
SPO func_00128F70(O *po, void *a);
CRef func_00129BC8(CRef *pref, void *a, void *b);
}
/** @sly2-annot addr=0x129BC8
 * Purpose: Deep copy: boxes re-allocated, pairs cloned via 0x128CB8, procs via 0x128F70, objects
 *   re-referenced
 * Info: proposed name CRef_CloneTo (T1); class CRef; TU ref
 * Notes: agent qualifier: (returns clone)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#ref-0x1291a8-0x12a3d8
 * @end-sly2-annot */
CRef func_00129BC8(CRef *pref, void *a, void *b)
{
    switch (pref->m_tagk) {
    case 0x1A: return CRef((const VECTOR *)pref->u.pv);
    case 0x1B: return CRef((const MATRIX4 *)pref->u.pv);
    case 0x1C: return CRef((const V16 *)pref->u.pv);
    case 0x1D: return CRef((const V8 *)pref->u.pv);
    case 0x1E: return CRef((const V12 *)pref->u.pv);
    case 0x1F: return CRef(func_00128CB8((NODE *)pref->u.pv, a, b).p);
    case 0x20: return CRef(func_00128F70((O *)pref->u.pv, a).p);
    case 0x21: return CRef((OBJ *)pref->u.pv);
    }
    return CRef(*pref);
}
