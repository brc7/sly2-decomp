/* EnsureLoSidebagBool (Sly 1 name); C++ original inlined CRef ctor/Set*, ~CRef -> Decref (func_00129EC8) */
struct LO;
struct CRef { int m_tagk; union { int m_n; float m_g; } m_tag; };
void RefEnsureOption(struct CRef *pref, struct LO *pbasic, int optid, struct CRef *prefValue);
void func_00129EC8(struct CRef *);
/** @sly2-annot addr=0x132BC8
 * Purpose: RefEnsureOption(plo, optid, bool CRef (tagk 0x17))
 * Info: proposed name EnsureLoSidebagBool (T1); TU basic
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#9-key-functions, docs/engine/splice-scripting.md#5-objects-and-scripts,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void EnsureLoSidebagBool__FP2LO5OPTIDi(struct LO *plo, int optid, int v)
{
    struct CRef ref;
    struct CRef refT;
    ref.m_tagk = 0x17;
    ref.m_tag.m_n = v != 0;
    RefEnsureOption(&refT, plo, optid, &ref);
    func_00129EC8(&refT);
    func_00129EC8(&ref);
}
