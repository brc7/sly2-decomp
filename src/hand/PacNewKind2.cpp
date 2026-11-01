struct AREQ { int cb; int cbAlign; };
extern "C" void *func_001AE2A0(AREQ *preq);
struct CBase {
    void *operator new(unsigned int cb) { AREQ req; req.cb = cb; req.cbAlign = 0x10; return func_001AE2A0(&req); }
    virtual void F();
};
struct C0 : CBase { int a; virtual void F(); };
struct C1 : CBase { int a, b; virtual void F(); };
struct C2 : CBase { int a, b, c, d; virtual void F(); };
extern "C" CBase *PacNewKind2(int k);
/** @sly2-annot addr=0x12CB98
 * Purpose: Factory: new object of kind 0..2 (second curve family)
 * Info: proposed name PacNewKind2 (T1); TU act
 * Notes: names_v2 C proposed StripAsegChn__FP4ASEGP3CHN (0.638) | level_parser CONFIRMED-parse: payload
 *   layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r1_116918+parse-verified
 * See: docs/engine/animation.md#7-key-functions,
 *   docs/formats/level-objects.md#7-animation-segment-loadasegfrombrx-0x12f2a8-cparse,
 *   docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
CBase *PacNewKind2(int k)
{
    switch (k) {
    case 0: return new C0;
    case 1: return new C1;
    case 2: return new C2;
    }
    return 0;
}
