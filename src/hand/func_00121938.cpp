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
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
    int pad[3];
};
extern "C" void func_00121578(SPN *psp, CRef **ppa, CRef **ppb);
extern "C" CRef func_00127838(NODE *pn, CRef *aref);
extern "C" CRef func_00121938(void *pbasic, CRef *aref, CRef *arefCall);
/** @sly2-annot addr=0x121938
 * Purpose: Splice builtin "filter": same, keeps elems where result true
 * Info: proposed name RefOpFilter (T1); TU bif
 * Confidence: high [T1 0.93] - evidence: static analysis; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/splice-scripting.md#7-key-functions,
 *   docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef func_00121938(void *pbasic, CRef *aref, CRef *arefCall) { if (aref[1].m_tagk == 0) return aref[1];
 SPN sp; CRef *pa; CRef *pb; func_00121578(&sp, &pa, &pb);
 *pa = aref[0];
 SP spRes; NODE *pTail; NODE *pn = aref[1].u.pnode;
 while (pn) {
   *pb = pn->ref;
   CRef r = func_00127838(sp.p, arefCall);
   if (r.m_tagk == 0x17 && r.u.n) {
     SPN spNew; NODE *pnNew = func_00256B48(); spNew.Set(pnNew);
     spNew->ref = pn->ref;
     if (!spRes.p) { NODE *pnR = func_00256B48(); spRes = pnR; spRes->ref = spNew.p; } else pTail->pnext = spNew.p;
     NODE *p = spNew.p; pTail = p;
   }
   pn = pn->pnext.p;
 }
 if (!spRes.p) return CRef(0);
 return CRef(spRes->ref.u.pnode); }
