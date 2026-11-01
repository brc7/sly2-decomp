// STEP::VUnk81 (0x205008, KB name VUnk81__4STEPiP2LOUxUxUx): add contacts (PxpAdd 0x1A7C18) for a point
// against the three axes of the frame BuildOrthonormalMatrixZ(direction, g_normalZ) builds. The contacts go
// to the link list entry that joins this object to the zap target of plo (or of its nearest attached
// ancestor); with no target they go to this object's own list.
typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
struct XP;
struct SOPART;
struct SO;
struct LO;
struct LINK { XP *pxpFirst; LINK *plinkNext; XP **ppxpOut; SO *pso; };
struct ATT { char pad0[0x34]; SOPART *psopart; char pad38[0x60 - 0x38]; SO *pso; LINK *plinkFirst; XP *pxpFirst; };
struct SO { char pad0[0x10]; SO *psoParent; char pad14[0xEC - 0x14]; ATT *patt; char padF0[0x258 - 0xF0]; SOPART *psopart; };
extern char D_003DD490[];  // g_sw
extern qword D_002A2DC0;   // g_normalZ
extern "C" {
SO *PsoFromLo(LO *);                 // PsoZapTarget
M3 BuildOrthonormalMatrixZ(qword, qword);          // BuildOrthonormalMatrixZ
void func_001A7C18(void *, SOPART *, SOPART *, qword, qword, float, int, void *, void *, XP **);  // PxpAdd
}
static inline qword Sub(const qword &a, const qword &b) {
    qword r;
    __asm__ __volatile__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b));
    return r;
}
// Transform v into the frame of m (one dot product per row); rows 1 and 2 are loaded by the asm itself.
static inline qword Rotate(const M3 &m, qword v) {
    qword one, a, b, c, r;
    __asm__("lqc2 $vf2,0x20(%7)\n\tlqc2 $vf1,0x10(%7)\n\tvmul.xyzw %1,%5,%6\n\tvmaxw.xyzw %0,$vf0,$vf0w\n\tvmul.xyzw %2,$vf1,%6\n\tvmul.xyzw %3,$vf2,%6\n\tvadday.x ACC,%1,%1y\n\tvmaddz.x %4,%0,%1z\n\tvaddax.y ACC,%2,%2x\n\tvmaddz.y %4,%0,%2z\n\tvaddax.z ACC,%3,%3x\n\tvmaddy.z %4,%0,%3y"
        : "=&j"(one), "=&j"(a), "=&j"(b), "=&j"(c), "=&j"(r) : "j"(m.r[0]), "j"(v), "r"(&m) : "vf1", "vf2");
    return r;
}
static inline float X(qword q) { float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(q)); return f; }
static inline float Y(qword q) { float f; __asm__("prot3w %0,%1" : "=r"(f) : "r"(q)); return f; }
static inline float Z(qword q) { float f; __asm__("pextuw %0,$zero,%1" : "=r"(f) : "r"(q)); return f; }

extern "C" void AddStepAxisXps(SO *pso, int grf, LO *plo, qword posOther, qword direction, qword pos)
{
    XP **ppxpFirst, **ppxpOut;
    SOPART *psopartOther;
    SO *psoTarget = PsoFromLo(plo);
    ATT *patt, *pattThis;
    if (psoTarget) {
        patt = psoTarget->patt;
        pattThis = pso->patt;
        if (!patt) {
            while ((psoTarget = psoTarget->psoParent) != 0) {
                patt = psoTarget->patt;
                if (patt) break;
            }
        }
    }
    if (psoTarget) {
        SO *psoOwner = patt->pso;
        LINK *plink = pattThis->plinkFirst;
        while (plink && plink->pso != psoOwner) plink = plink->plinkNext;
        ppxpFirst = &plink->pxpFirst;
        ppxpOut = plink->ppxpOut;
        psopartOther = psoOwner->patt->psopart;
    } else {
        psopartOther = 0;
        ppxpFirst = ppxpOut = &pso->patt->pxpFirst;
    }
    M3 mat = BuildOrthonormalMatrixZ(direction, D_002A2DC0);
    qword posLocal = Rotate(mat, Sub(pos, posOther));
    if ((grf & 1) || ((grf & 8) && X(posLocal) < 0.0001f))
        func_001A7C18(D_003DD490, pso->psopart, psopartOther, pos, mat.r[0], X(posLocal), (grf & 1) ? 11 : 10, 0, 0, ppxpFirst);
    if ((grf & 2) || ((grf & 0x10) && Y(posLocal) < 0.0001f))
        func_001A7C18(D_003DD490, pso->psopart, psopartOther, pos, mat.r[1], Y(posLocal), (grf & 2) ? 11 : 10, 0, 0, ppxpFirst);
    if ((grf & 4) || ((grf & 0x20) && Z(posLocal) < 0.0001f))
        func_001A7C18(D_003DD490, pso->psopart, psopartOther, pos, mat.r[2], Z(posLocal), (grf & 4) ? 11 : 14, 0, 0, ppxpFirst);
    *ppxpOut = *ppxpFirst;
}
