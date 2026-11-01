struct CRef;
extern "C" void func_00129EC8(CRef *);
extern "C" void func_001291A8(CRef *, const CRef *);
extern "C" void func_00129658(CRef *, const CRef *);
extern "C" void func_0012A0B0(CRef *, int);
struct CRef {
    int tagk; int tag;
    CRef() { tagk = -1; }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    ~CRef() { func_00129EC8(this); }
    CRef &operator=(const CRef &r) { func_00129658(this, &r); return *this; }
};
struct O { int cref; char pad[0x24 - 4]; int n24; };
extern "C" void func_00257078(O *, int);
struct SP {
    O *p;
    SP(const SP &r);
    ~SP() { O *q = p; if (q && --q->cref == 0) func_00257078(q, 3); }
};
struct E { int a; O *b; };
struct Q { int pad[2]; E *ae; };
extern "C" {
int func_00256DD0(void);
SP func_00257058(int);
void func_00259A88(O *);
CRef func_00127838(int, O *);
}
/** @sly2-annot addr=0x1FFA70
 * Purpose: Build one splice table entry: new frame, CRef InitTag, bind
 * Info: proposed name LoadSpliceEntryFromBrx (T1); TU splicemap
 * Notes: splice_scripts format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r7_1F8DD8+format-doc:splice_scripts
 * See: docs/formats/splice-scripts.md#1-where-the-scripts-live-cstatic--cparse,
 *   docs/reference/functions/splice-scripting.md#splicemap-0x1ff7e8-0x1ffcf8
 * @end-sly2-annot */
extern "C" CRef func_001FFA70(void *p, Q *pq, int i)
{
    CRef r;
    if (pq->ae[i].a && !pq->ae[i].b) {
        int x = func_00256DD0();
        SP sp = func_00257058(x);
        func_00259A88(sp.p);
        sp.p->n24 = 1;
        r = func_00127838(pq->ae[i].a, sp.p);
        sp.p->cref++;
        pq->ae[i].b = sp.p;
    }
    func_00129EC8(&r);
    func_0012A0B0(&r, 0x24);
    return r;
}
