struct AREQ { int cb; int cbAlign; };
extern "C" void *func_001AE2A0(AREQ *preq);
struct CBase {
    void *operator new(unsigned int cb) { AREQ req; req.cb = cb; req.cbAlign = 0x10; return func_001AE2A0(&req); }
    virtual void F();
};
struct C0 : CBase { int a[3]; virtual void F(); };
struct C1 : CBase { int a[2]; virtual void F(); };
struct C2 : CBase { int a[2]; virtual void F(); };
struct C3 : CBase { int a[11]; virtual void F(); };
struct C4 : CBase { int a[11]; virtual void F(); };
struct C5 : CBase { int a[7]; virtual void F(); };
extern "C" CBase *func_0012B3B0(int k);
/** @sly2-annot addr=0x12B3B0
 * Purpose: Factory: new animation-curve object of kind 0..5 (six C++ curve classes)
 * Info: proposed name PacNewKind (T1); TU act
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#6-source-files, docs/engine/animation.md#7-key-functions,
 *   docs/engine/animation.md#open-questions
 * @end-sly2-annot */
CBase *func_0012B3B0(int k)
{
    switch (k) {
    case 0: return new C0;
    case 1: return new C1;
    case 2: return new C2;
    case 3: return new C3;
    case 4: return new C4;
    case 5: return new C5;
    }
    return 0;
}
