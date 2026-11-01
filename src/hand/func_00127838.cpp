struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
};
struct NODE { int x0, x4; CRef ref; NODE *px10; };
extern "C" {
CRef func_00126438(NODE *p, CRef *aref);
CRef func_00126490(NODE *p, CRef *aref);
CRef func_00126540(NODE *p, CRef *aref);
CRef func_001265F0(NODE *p, CRef *aref);
CRef func_001266C8(NODE *p, CRef *aref);
CRef func_00126758(NODE *p, CRef *aref);
CRef func_00126840(NODE *p, CRef *aref);
CRef func_00126928(NODE *p, CRef *aref);
CRef func_00126A18(NODE *p, CRef *aref);
CRef func_00126BA0(NODE *p, CRef *aref);
CRef func_00126DB8(NODE *p, CRef *aref);
CRef func_00126F78(NODE *p, CRef *aref);
CRef func_00127228(NODE *p, CRef *aref);
CRef func_00127068(NODE *p, CRef *aref);
CRef func_001272E0(NODE *p, CRef *aref);
CRef func_00127838(NODE *p, CRef *aref);
}
/** @sly2-annot addr=0x127838
 * Purpose: Core evaluator: self-evaluating atoms, symbols, special-form dispatch on head TAGK (1..18), else
 *   apply; import (17) returns Nil, quote (18) returns datum
 * Info: proposed name RefEval (T1); TU eval
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: r1_116918
 * See: docs/engine/overview.md#6-subsystem-map, docs/engine/splice-scripting.md#3-the-evaluator,
 *   docs/engine/splice-scripting.md#7-key-functions
 * @end-sly2-annot */
CRef func_00127838(NODE *p, CRef *aref)
{
    switch (p->ref.m_tagk) {
    default:
        return p->ref;
    case 0x18:
        return func_00126438(p, aref);
    case 0x1F: {
        NODE *pn = (NODE *)p->ref.u.pv;
        switch (pn->ref.m_tagk) {
        case 1: return func_00126490(p, aref);
        case 2: return func_00126540(p, aref);
        case 5: return func_001265F0(p, aref);
        case 6: return func_001266C8(p, aref);
        case 7: return func_00126758(p, aref);
        case 8: return func_00126840(p, aref);
        case 9: return func_00126928(p, aref);
        case 10: return func_00126A18(p, aref);
        case 12: return func_00126BA0(p, aref);
        case 13: return func_00126DB8(p, aref);
        case 14: return func_00126F78(p, aref);
        case 16: return func_00127228(p, aref);
        case 15: return func_00127068(p, aref);
        case 18: return pn->px10->ref;
        case 17: return CRef(-1);
        }
        return func_001272E0(p, aref);
    }
    }
}
