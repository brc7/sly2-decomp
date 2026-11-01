struct CRef { int m_tagk; void *m_p; };
struct OBJ;
struct PRM { int a; int k; int rest[8]; };
extern "C" {
extern char D_004520A8[];
CRef *func_0012A0B0(CRef *pref, int tagk);
void *func_002333B4(void *p, int c, int n);
OBJ *func_00256B48(void);
void func_001FFD98(CRef *ret, void *b, PRM *prm);
CRef *func_00129658(CRef *pref, CRef *prefSrc); /* CRef::operator= returns *this */
void func_00129EC8(CRef *pref);
void func_002550F8(OBJ *p);
void func_0011CD98(void *pool, OBJ *p);
void func_00129480(CRef *pref, OBJ *p);
CRef *func_00200F58(CRef *ret, int c, int x, void *pb);
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
void func_00256C58(SP *pp, OBJ *p);
void func_00256D40(SP *p, int n);
}
struct CREFA { CRef r; } __attribute__((aligned(16)));
struct OBJ { int cref; int pad; CRef ref; SP next; };
inline SP::SP(OBJ *q) { func_00256C38(this, q); }
inline void SP::Set(OBJ *q) { func_00256C58(this, q); }
inline SP::~SP()
{
    OBJ *q = p;
    if (q) {
        if (--q->cref == 0) {
            func_00256D40(&q->next, 2);
            func_00129EC8(&q->ref);
            func_002550F8(q);
            func_0011CD98(D_004520A8, q);
        }
    }
}

/** @sly2-annot addr=0x200F58
 * Purpose: Same as RefSetArgListFromPvs with one otype for all values
 * Info: proposed name RefSetArgListFromPvsUniform (T2); TU spliceobj
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
CRef *func_00200F58(CRef *ret, int c, int x, void *pb)
{
    if (c == 0) {
        func_0012A0B0(ret, 0);
        return ret;
    }
    PRM prm;
    SP list;
    func_002333B4(&prm, 0, sizeof(prm));
    prm.a = x;
    prm.k = 2;
    list.Set(0);
    SP *ppTail = &list;
    for (int i = 0; i < c; i++) {
        prm.rest[0] = i * 4;
        SP nw(func_00256B48());
        CRef *pr = &nw.p->ref;
        CREFA ref;
        func_001FFD98(&ref.r, pb, &prm);
        func_00129658(pr, &ref.r);
        func_00129EC8(&ref.r);
        ppTail->Set(nw.p);
        ppTail = &nw.p->next;
    }
    func_00129480(ret, list.p);
    return ret;
}
