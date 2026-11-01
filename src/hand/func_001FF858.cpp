struct CRef;
extern "C" void func_00129EC8(CRef *);
extern "C" void func_001291A8(CRef *, const CRef *);
struct CRef {
    int tagk; int tag;
    CRef() { tagk = -1; }
    CRef(const CRef &r) { func_001291A8(this, &r); }
    ~CRef() { func_00129EC8(this); }
};
struct O { int cref; int pad; CRef ref; char sub[0x10]; };
struct Pool;
extern Pool D_004520A8;
extern "C" {
void func_00256D40(void *, int);
void func_002550F8(O *);
void func_0011CD98(Pool *, O *);
}
struct SP {
    O *p;
    SP(const SP &r);
    ~SP() {
        O *q = p;
        if (q && --q->cref == 0) {
            func_00256D40(q->sub, 2);
            func_00129EC8(&q->ref);
            func_002550F8(q);
            func_0011CD98(&D_004520A8, q);
        }
    }
};
struct E { O *p; int n; };
struct T { int a; int c; E *ae; T *pNext; };
struct SZ { int cb; int cbAlign; SZ(int a, int b) { cb = a; cbAlign = b; } };
struct G { char pad[0x39E8]; T *pt; T *ptFirst; };
struct Q;
extern int D_003E1C34;
extern char D_002DD598[];
extern "C" {
int func_00117C80(void *);
T *func_001AE2A0(SZ *);
E *func_001AE1D0(SZ *);
SP func_0012A3D8(void *);
void func_00118420(void *);
CRef func_001FFA70(void *, T *, int);
void func_00128148(void *);
}
/** @sly2-annot addr=0x1FF858
 * Purpose: Read the level Splice section: count, per-entry frames/CRefs (0x1FFA70) into a SW-heap table
 * Info: proposed name LoadSwSpliceFromBrx (T1); TU splicemap
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels |
 *   splice_scripts format doc cites this function by this name (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.80] - evidence: static analysis; sources:
 *   r7_1F8DD8+parse-verified+format-doc:splice_scripts
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/formats/level-objects.md#2-per-world-entry-header-loop-body-of-loadswfrombrx
 * @end-sly2-annot */
extern "C" void func_001FF858(G *pg, int a, void *px)
{
    int c = func_00117C80(px);
    int i;
    T *pt;
    if (!c) return;
    {
        SZ sz(0x10, 0x10);
        pt = func_001AE2A0(&sz);
    }
    pt->a = a;
    CRef r;
    pt->c = c;
    {
        SZ sz(c * 8, 0x10);
        pt->ae = func_001AE1D0(&sz);
    }
    for (i = 0; i < c; i++) {
        E *pe = &pt->ae[i];
        SP sp = func_0012A3D8(px);
        O *q = sp.p;
        if (q) q->cref++;
        pe->p = sp.p;
        pe->n = 0;
        if (D_003E1C34) {
            func_00118420(px);
            func_00118420(px);
            func_00118420(px);
        }
    }
    for (i = 0; i < c; i++) {
        func_001FFA70(pg, pt, i);
    }
    pg->pt = pt;
    pt->pNext = pg->ptFirst;
    pg->ptFirst = pt;
    func_00128148(D_002DD598);
}
