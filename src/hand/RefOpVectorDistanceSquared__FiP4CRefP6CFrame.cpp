typedef int qword __attribute__((mode(TI)));
struct CRef {
    int m_tagk; union { int n; float g; void *pv; } u;
    CRef() { m_tagk = -1; }
    CRef(int tagk) { InitTag(tagk); }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(float g);
};
struct W { char pad[0xC]; qword *pv; };
extern qword D_002A2DD0;
extern "C" {
CRef RefOpVectorDistanceSquared__FiP4CRefP6CFrame(void *pbasic, CRef *aref);
}
static inline float SDistSq(qword *pa, qword *pb)
{
    register qword a __asm__("vf1") = *pa;
    register qword b __asm__("vf2") = *pb;
    register qword t __asm__("vf3");
    int n;
    float g;
    __asm__ ("vsub.xyzw %0, %0, %2\n"
        "vmul.xyz %0, %0, %0\n"
        "vaddw.x %1, $vf0, $vf0w\n"
        "vadday.x ACC, %0, %0y\n"
        "vmaddz.x %0, %1, %0z" : "+j"(a), "=j"(t) : "j"(b));
    __asm__ ("qmfc2.ni %0, %1" : "=r"(g) : "j"(a));
    return g;
}
CRef RefOpVectorDistanceSquared__FiP4CRefP6CFrame(void *pbasic, CRef *aref) { CRef ref; if (aref[1].m_tagk == 0) ref = SDistSq((qword *)aref[0].u.pv, &D_002A2DD0); else { register W *pw __asm__("$5") = (W *)aref[1].u.pv; ref = SDistSq((qword *)aref[0].u.pv, pw->pv);} return ref; }
