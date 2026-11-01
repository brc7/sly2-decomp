/* EnsureLoSidebag* with a 128-bit by-value argument (VECTOR/QW?) passed to func_00129248 (CRef setter/ctor) */
struct LO;
typedef int ti128 __attribute__((mode(TI)));
struct CRef { int m_tagk; union { int m_n; float m_g; } m_tag; };
void RefEnsureOption(struct CRef *pref, struct LO *pbasic, int optid, struct CRef *prefValue);
void func_00129EC8(struct CRef *);
void func_00129248(struct CRef *, ti128);
/** @sly2-annot addr=0x132DE0
 * Purpose: RefEnsureOption with a vector CRef (passed by value as qword)
 * Info: proposed name EnsureLoSidebagVector (T1); TU basic
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
void func_00132DE0(struct LO *plo, int optid, ti128 v)
{
    struct CRef ref;
    struct CRef refT;
    func_00129248(&ref, v);
    RefEnsureOption(&refT, plo, optid, &ref);
    func_00129EC8(&refT);
    func_00129EC8(&ref);
}
