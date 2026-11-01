struct BINDING;
extern char D_004520D0[];
extern char D_00452148[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_00255438(void *pv);
void func_00255DE8(void *pv);
}
struct CRef { int m_tagk; int m_tag; void Decref(); ~CRef() { Decref(); } };
struct SPM {
    BINDING *p;
    ~SPM();
};
struct BINDING {
    int cref;
    int x4;
    int sym;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { func_00255438(pv); func_0011CD98(D_004520D0, pv); }
};
struct SPB {
    BINDING *p;
    SPB() {}
    SPB(const SPB &);
    ~SPB() { BINDING *pb = p; if (pb && --pb->cref == 0) delete pb; }
    void operator=(BINDING *pb);
};
struct SB {
    int cref;
    int x4;
    SPB pbinding;
    void SetBinding(BINDING *pb) { pbinding = pb; }
    void operator delete(void *pv) { func_00255DE8(pv); func_0011CD98(D_00452148, pv); }
};
struct SPS {
    SB *p;
    void Set(SB *psb);
    SPS() {}
    SPS(SB *psb) { Set(psb); }
    SPS(const SPS &);
    ~SPS() { SB *psb = p; if (psb && --psb->cref == 0) delete psb; }
    SB *operator->() { return p; }
};
extern "C" {
SB *func_00257D80();
SPB func_0012B0E8(BINDING *pb, void *p, void *q);
SPS func_0012AB78(SB *psbSrc, void *q);
}
/** @sly2-annot addr=0x12AB78
 * Purpose: New sidebag (0x257D80) with cloned binding list (0x12B0E8)
 * Info: proposed name CSidebag_CloneTo (T1); class CSidebag; TU sidebag
 * Notes: agent qualifier: (returns clone)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#sidebag-0x12a980-0x12ae18
 * @end-sly2-annot */
SPS func_0012AB78(SB *psbSrc, void *q)
{
    SPS sp;
    sp.Set(func_00257D80());
    if (psbSrc->pbinding.p)
        sp->SetBinding(func_0012B0E8(psbSrc->pbinding.p, 0, q).p);
    return SPS(sp.p);
}
