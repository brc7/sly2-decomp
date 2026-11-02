struct CRef { int m_tagk; void *m_p; };
struct OBJ;
struct PRM { int a; int k; int rest[8]; };
extern "C" {
extern char D_004520A8[];
CRef *SetTag__4CRef4TAGK(CRef *pref, int tagk);
void *func_002333B4(void *p, int c, int n);
OBJ *PvNewPair(void);
void RefGetPeopid(CRef *ret, void *b, PRM *prm);
CRef *func_00129658(CRef *pref, CRef *prefSrc); /* CRef::operator= returns *this */
void Decref__4CRef(CRef *pref);
void GcPair_Unlink(OBJ *p);
void func_0011CD98(void *pool, OBJ *p);
void CRef_ctor_CPair(CRef *pref, OBJ *p);
CRef *func_00200DA0(CRef *ret, int c, int *a, void **b);
}
class SP {
public:
    OBJ *p;
    SP() { p = 0; }
    SP(OBJ *q);
    void Set(OBJ *q);
    void Dtor(int n);
    ~SP();
} __attribute__((aligned(16)));
extern "C" {
void func_00256C38(SP *pp, OBJ *p);
void PairPtr_Assign(SP *pp, OBJ *p);
void PairPtr_Release(SP *p, int n);
}
struct CREFA { CRef r; } __attribute__((aligned(16)));
struct OBJ { int cref; int pad; CRef ref; SP next; };
inline SP::SP(OBJ *q) { func_00256C38(this, q); }
inline void SP::Set(OBJ *q) { PairPtr_Assign(this, q); }
inline SP::~SP()
{
    OBJ *q = p;
    if (q) {
        if (--q->cref == 0) {
            PairPtr_Release(&q->next, 2);
            Decref__4CRef(&q->ref);
            GcPair_Unlink(q);
            func_0011CD98(D_004520A8, q);
        }
    }
}

/** @sly2-annot addr=0x200DA0
 * Purpose: Build a Splice list CRef from c (otype, pv) pairs (empty -> void)
 * Info: proposed name RefSetArgListFromPvs (T1); TU spliceobj
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
CRef *func_00200DA0(CRef *ret, int c, int *a, void **b)
{
    if (c == 0) {
        SetTag__4CRef4TAGK(ret, 0);
        return ret;
    }
    PRM prm;
    SP list;
    func_002333B4(&prm, 0, sizeof(prm));
    prm.k = 2;
    SP *ppTail = &list;
    for (int i = 0; i < c; i++) {
        void *pb = b[i];
        prm.a = a[i];
        SP nw(PvNewPair());
        CRef *pr = &nw.p->ref;
        CREFA ref;
        RefGetPeopid(&ref.r, pb, &prm);
        func_00129658(pr, &ref.r);
        Decref__4CRef(&ref.r);
        ppTail->Set(nw.p);
        ppTail = &nw.p->next;
    }
    CRef_ctor_CPair(ret, list.p);
    return ret;
}
