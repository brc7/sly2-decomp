struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct W;
extern char D_004520D0[];
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void func_00255438(void *pv);
W *func_00257340();
CRef func_00129BC8(CRef *pref, void *a, void *b);
}
struct SPM {
    W *p;
    ~SPM();
    SPM &operator=(W *pw);
};
struct W {
    int cref;
    int x4;
    int x8;
    CRef ref;
    SPM pnext;
    void SetNext(W *pw) { pnext = pw; }
    void operator delete(void *pv) { func_00255438(pv); func_0011CD98(D_004520D0, pv); }
};
struct SPW {
    W *p;
    void Set(W *pw);
    SPW() {}
    SPW(W *pw) { Set(pw); }
    SPW(const SPW &);
    ~SPW() { W *pw = p; if (pw && --pw->cref == 0) delete pw; }
    W *operator->() { return p; }
};
extern "C" SPW func_0012B0E8(W *src, void *a, void *b);
struct Z;
struct SPZ { Z *p; ~SPZ(); void operator=(Z *pz); };
struct O {
    int cref;
    int x4;
    int c;
    SPZ apz[4];
    SPM pw;
    ~O();
    void SetW(W *p) { pw = p; }
    void SetZ(int i, Z *p) { apz[i] = p; }
};
struct SPO {
    O *p;
    void Set(O *po);
    SPO() {}
    SPO(O *po) { Set(po); }
    SPO(const SPO &);
    ~SPO() { O *po = p; if (po && --po->cref == 0) delete po; }
    O *operator->() { return p; }
};
extern "C" {
O *func_00256DD0();
SPO func_00127E90(O *src, void *a);
}
/** @sly2-annot addr=0x127E90
 * Purpose: New frame (0x256DD0), copies parent pointers, clones binding list via CVarb clone 0x12B0E8
 * Info: proposed name CFrame_CloneTo (T1); class CFrame; TU frame
 * Notes: agent qualifier: (returns clone)
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/splice-scripting.md#frame-0x127b18-0x128138
 * @end-sly2-annot */
SPO func_00127E90(O *src, void *a)
{
    SPO sp;
    int i;
    sp.Set(func_00256DD0());
    sp->c = src->c;
    for (i = 0; i < src->c; i++)
        (*(SPZ *)((int)(i * 4) + (int)sp.p + 0xC)) = src->apz[i].p;
    if (src->pw.p)
        sp->SetW(func_0012B0E8(src->pw.p, sp.p, a).p);
    return SPO(sp.p);
}
