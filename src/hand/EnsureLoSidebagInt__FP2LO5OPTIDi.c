/* EnsureLoSidebagInt (Sly 1 name); C++ original inlined CRef ctor/Set*, ~CRef -> Decref (func_00129EC8) */
struct LO;
struct CRef { int m_tagk; union { int m_n; float m_g; } m_tag; };
void RefEnsureOption(struct CRef *pref, struct LO *pbasic, int optid, struct CRef *prefValue);
void func_00129EC8(struct CRef *);
void EnsureLoSidebagInt__FP2LO5OPTIDi(struct LO *plo, int optid, int v)
{
    struct CRef ref;
    struct CRef refT;
    ref.m_tagk = 0x15;
    ref.m_tag.m_n = v;
    RefEnsureOption(&refT, plo, optid, &ref);
    func_00129EC8(&refT);
    func_00129EC8(&ref);
}
