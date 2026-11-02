/* Script builtin (METHOD pfn(pv,c,aref)): ray-cast aref[1]->aref[2] (func_0014C6A0) vs every SO of each LO in list aref[0].
   pv==0x7B returns a list of (lo pos normal) for all hits, else (lo normal) of the first hit (the original stores &hit.normal there). */
typedef int qword __attribute__((mode(TI)));
struct NODE;
struct LO;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; NODE *pnode; qword *pq; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(NODE *pn);
    CRef &operator=(LO *plo);
    CRef &operator=(qword *pv);
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
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    SPN(NODE *pn) { Set(pn); }
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    void operator=(NODE *pn);
    NODE *operator->() { return p; }
    int pad;
};
struct SO {
    char pad[0x10];
    unsigned long grf;
    char pad18[0xB0 - 0x18];
};
struct LO {
    char pad[0xE0];
    int cso;
    SO *aso;
};
struct HIT {
    qword pos;
    qword normal;
    int pad[16];
};
extern "C" int func_0014C6A0(SO *pso, qword pos0, qword pos1, int f, HIT *phit);

/* Script builtin: ray-cast segment aref[1]->aref[2] against every SO of each LO in list aref[0].
   pv == 0x7B: return list of (lo pos normal) for all hits; otherwise return (lo normal) of the first hit. */
/** @sly2-annot addr=0x1249A8
 * Purpose: Line hit test vs objects (0x14C6A0); BIFK 0x7B returns list of (lo pos normal) for all hits, 0x7C
 *   returns first hit
 * Info: proposed name RefOpHitTestObjectsImpl (T1); TU bif
 * Confidence: high [T1 0.88] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0
 * @end-sly2-annot */
extern "C" CRef RefOpHitTestObjectsImpl__F4BIFKiP4CRefP6CFrame(int pv, int c, CRef *aref)
{
    CRef ref;
    if (aref[0].m_tagk == 0) {
        ref = aref[0];
    } else {
        SPN spHead(0);
        SPN spTail(0);
        NODE *pn;

        for (pn = aref[0].u.pnode; pn; pn = pn->pnext.p) {
            CRef *prefTo = &aref[2];
            LO *plo = (LO *)pn->ref.u.pv;
            int i;
            if (!plo)
                continue;
            for (i = 0; i < plo->cso; i++) {
                SO *pso = &plo->aso[i];
                CRef *prefFrom = &aref[1];
                HIT hit;
                if (pso->grf & (1UL << 50))
                    continue;
                if (func_0014C6A0(pso, *prefFrom->u.pq, *prefTo->u.pq, 1, &hit) <= 0)
                    continue;
                {
                    SPN spLo(func_00256B48());
                    SPN spPos(func_00256B48());
                    spLo->ref = plo;
                    spLo->pnext = spPos.p;
                    if (pv == 0x7B) {
                        SPN spNormal(func_00256B48());
                        SPN spItem(func_00256B48());
                        spPos->ref = &hit.pos;
                        spPos->pnext = spNormal.p;
                        spNormal->ref = &hit.normal;
                        spItem->ref = spLo.p;
                        if (spHead.p)
                            spTail->pnext = spItem.p;
                        else
                            spHead = spItem.p;
                        spTail = spItem.p;
                    } else {
                        spPos->ref = &hit.normal;
                        ref = spLo.p;
                        return ref;
                    }
                }
            }
        }
        if (spHead.p)
            ref = spHead.p;
        else
            ref.m_tagk = 0;
    }
    return ref;
}
