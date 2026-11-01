typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
CRef *CRef_ctor(CRef *pref, qword v);
CRef *func_001291A8(CRef *pref, CRef *prefSrc);
void func_00129EC8(CRef *pref);
CRef *RefOpVectorCrossProduct__FiP4CRefP6CFrame(CRef *ret, void *pbasic, CRef *aref)
{
    CRef ref;
    register qword a __asm__("vf1") = *(qword *)aref[0].m_tag;
    register qword b __asm__("vf2") = *(qword *)aref[1].m_tag;
    register qword v __asm__("vf3");
    __asm__ ("vopmula.xyz ACC, %1, %2\n"
        "vopmsub.xyz %0, %2, %1" : "=j"(v) : "j"(a), "j"(b));
    CRef_ctor(&ref, v);
    func_001291A8(ret, &ref);
    func_00129EC8(&ref);
    return ret;
}
