typedef int qword __attribute__((mode(TI)));
typedef struct CRef { int m_tagk; int m_tag; } CRef;
typedef struct VECTOR { float x, y, z, w; } __attribute__((aligned(16))) VECTOR;
typedef struct CM { char pad[0x4C]; float g; char pad2[0x10]; qword mat[4]; } CM;
extern CM D_002DDF40;
CRef *CRef_ctor(CRef *pref, qword v);
static __inline__ void SetVector(VECTOR *pv, float x, float y, float z)
{
    register qword t __asm__("$6");
    register int ix __asm__("$6"), iy __asm__("$4"), iz __asm__("$4");
    __asm__ ("mfc1 %1, %3\n\tmfc1 %2, %4\n\tpextlw %0, %2, %1" : "=r"(t), "=&r"(ix), "=&r"(iy) : "f"(x), "f"(y));
    __asm__ __volatile__ ("mfc1 %0, %1" : "=r"(iz) : "f"(z));
    __asm__ ("pcpyld %0, %1, %2" : "=r"(t) : "r"(iz), "r"(t));
    *(qword *)pv = t;
}
CRef *RefOpNearClipCenter__FiP4CRefP6CFrame(CRef *ret)
{
    VECTOR vec;
    CM *pcm = &D_002DDF40;
    qword *pm = pcm->mat;
    SetVector(&vec, pcm->g, 0.0f, 0.0f);
    {
        register qword v __asm__("vf5") = *(qword *)&vec;
        register qword m0 __asm__("vf4") = pm[0];
        register qword m1 __asm__("vf2") = pm[1];
        register qword m2 __asm__("vf1") = pm[2];
        register qword m3 __asm__("vf3") = pm[3];
        __asm__ ("vmulax.xyz ACC, %1, %0x\n"
            "vmadday.xyz ACC, %2, %0y\n"
            "vmaddaz.xyz ACC, %3, %0z\n"
            "vmaddw.xyz %0, %4, $vf0w" : "+j"(v) : "j"(m0), "j"(m1), "j"(m2), "j"(m3));
        CRef_ctor(ret, v);
    }
    return ret;
}
