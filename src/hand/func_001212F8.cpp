struct NODE;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; NODE *pnode; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    CRef(NODE *p);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(NODE *p);
};
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_002550F8(void *pv);
NODE *func_00256B48();
}
struct SPM {
    NODE *p;
    ~SPM();
    void operator=(NODE *pn);
};
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { func_002550F8(pv); func_0011CD98(D_004520A8, pv); }
};
struct SP {
    NODE *p;
    SP() { p = 0; }
    SP(const SP &);
    ~SP() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    void operator=(NODE *pn);
    NODE *operator->() { return p; }
};
extern "C" SP func_00128CB8(NODE *src, void *a, void *b);
extern "C" CRef func_001212F8(void *pbasic, CRef *aref);
/** @sly2-annot addr=0x1212F8
 * Purpose: Splice builtin "append": concatenates list args (copies all but last)
 * Info: proposed name RefOpAppend (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_B+verify_side
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_001212F8(void *pbasic, CRef *aref) { if (aref[0].m_tagk == 0) return aref[0];
 SP spRes; NODE *pTail;
 for (NODE *pn = aref[0].u.pnode; pn; pn = pn->pnext.p) {
   if (pn->ref.m_tagk != 0x1F) continue;
   SP spCopy;
   if (pn->pnext.p) { SP t = func_00128CB8(pn->ref.u.pnode, 0, 0); spCopy = t.p; } else spCopy = pn->ref.u.pnode;
   if (!spRes.p) { spRes = func_00256B48(); spRes->ref = spCopy.p; } else pTail->pnext = spCopy.p;
   pTail = spCopy.p;
   while (pTail->pnext.p) pTail = pTail->pnext.p;
 }
 if (!spRes.p) return CRef(0);
 return CRef(spRes->ref.u.pnode); }
