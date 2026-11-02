struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    CRef(struct NODE *p);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
    CRef &operator=(NODE *p);
    CRef &operator=(struct LO *p);
};
struct NODE;
extern char D_004520A8[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void GcPair_Unlink(void *pv);
NODE *PvNewPair();
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
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
    int pad[3];
};
struct LO;
struct SW { char pad[0x3C]; int n3C; };
struct OW { char pad[0xC]; SW *psw; };
struct G { char pad[0x3F0C]; int n3F0C; };
extern G D_003DD490;
extern "C" LO *PloFindSwObject_15E958(G *pg, int kind, int n, void *pv, int n2);
extern "C" CRef RefOpFindObjectsFromOids(void *pbasic, CRef *aref);
/** @sly2-annot addr=0x1242B8
 * Purpose: Maps a list of OIDs to a list of objects via 0x15E958 (same root/optional-arg handling as
 *   find-object)
 * Info: proposed name RefOpFindObjectsFromOids (T1); TU bif
 * Notes: splice builtin 114 find-objects-from-oids~ -> find-objects-from-oids~ (0.75): Body maps a list of
 *   oids to objects with PloFindSwObject (same root/optional context arg as find-object). Scripts:
 *   (find-objects-from-oids~ '(60278 60279 60280)
 * Confidence: mid (draft) [T1 0.75] - evidence: semantic review; sources: r1_116918+top50_round(splice
 *   builtin 114)
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
CRef RefOpFindObjectsFromOids(void *pbasic, CRef *aref) { CRef res; if (aref[0].m_tagk == 0) return aref[0];
 void *pv = aref[1].u.pv; SW *psw; if (aref[2].m_tagk != 0) psw = ((OW *)aref[2].u.pv)->psw; else psw = (SW *)pv;
 SPN spRes; NODE *pnR = PvNewPair(); spRes.Set(pnR);
 NODE *pTail; NODE *pn = (NODE *)aref[0].u.pv; while (pn) { SPN spNew; NODE *pnNew = PvNewPair(); spNew.Set(pnNew);
   int n = pn->ref.u.n; spNew->ref = PloFindSwObject_15E958(&D_003DD490, pv ? 1 : 5, n, pv, psw ? psw->n3C : D_003DD490.n3F0C);
   if (spRes->ref.m_tagk != 0x1F) spRes->ref = spNew.p; else pTail->pnext = spNew.p;
   NODE *p = spNew.p; pn = pn->pnext.p; pTail = p; } int unused[4]; res = CRef((NODE *)spRes->ref.u.pv); return res; }
