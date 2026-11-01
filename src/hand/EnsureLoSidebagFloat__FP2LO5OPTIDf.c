/* EnsureLoSidebagFloat (Sly 1 name); C++ original inlined CRef ctor/SetF32, ~CRef -> Decref (func_00129EC8) */
struct LO;
struct CRef { int m_tagk; union { int m_n; float m_g; } m_tag; };
void RefEnsureOption(struct CRef *pref, struct LO *pbasic, int optid, struct CRef *prefValue);
void func_00129EC8(struct CRef *);
void EnsureLoSidebagFloat__FP2LO5OPTIDf(struct LO *plo, int optid, float g)
{
    struct CRef ref;
    struct CRef refT;
    ref.m_tagk = 0x16;
    ref.m_tag.m_g = g;
    RefEnsureOption(&refT, plo, optid, &ref);
    func_00129EC8(&refT);
    func_00129EC8(&ref);
}
