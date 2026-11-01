// SolveDzgFric (Claude agent, T1655): DZG constraint-add loop. Each pass re-solves (func_00158988), picks the free
// contact row whose projected push (the rows' gC-weighted af[j].av[i] vectors minus the normal part) is largest, then
// appends a new row paired with it: its g10 = -|v|, its F gets the push divided by g10 and the SO, its g8/g4 limits come from
// the object's friction data, and func_00158668 (AppendDzgDz) gets the new row and its F.
// Keys to the match: (1) one gLimit variable assigned in both grf&1 arms (a multi-block pseudo, so local-alloc can't tie
// it: the target's f2/f3 choices); (2) pf passed as AppendDzgDz's third argument (the callee reads px in a2, so pf is
// live to the call and global-alloc puts it in a2); (3) the new row pdz computed before the paired row pdzA.
typedef int qword __attribute__((mode(TI)));
struct DZ { int x0; float g4; float g8; float gC; float g10; int pad[2]; DZ *pdzOther; int pad2[2]; };
struct ZZ { char pad[0x88]; float g88; char pad2[0xD0 - 0x8C]; float gD0; };
struct ALO { char pad[0xE8]; ZZ *pzz; };
struct SO { char pad[8]; ALO *palo; char pad2[0x80 - 0xC]; qword normal; char pad3[0x94 - 0x90]; float g94; char pad4[0x9C - 0x98]; int k9C; int padA0[4]; qword vB0; };
struct F { char pad[0x40]; qword v; SO *pso; qword *av; char pad2[0x60 - 0x58]; };
struct DZG { int x0; int cdz; DZ *adz; char pad[0x50 - 0xC]; int cso; SO **apso; int pad58[2]; F *af; };
struct CLOCK { float t; float dt; };
extern "C" {
extern CLOCK D_002DDED0;
extern float D_002BDDE8;
void func_00158988(DZG *pdzg, int grf);
void func_00158668(DZG *pdzg, DZ *pdz, F *pf);
void SolveDzgFric(DZG *pdzg, int grf);
}
struct VU_FLOAT { qword qw; };
static inline VU_FLOAT VuFloat(float g)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j"(v.qw) : "r"(g));
    return v;
}
static inline float GFromVf(qword qw)
{
    float g;
    __asm__("qmfc2 %0, %1" : "=r"(g) : "j"(qw));
    return g;
}
static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}
static inline qword QwProjectOut(qword v, qword n)
{
    qword d = QwDot(v, n);
    qword al, ac;
    __asm__("vmulx.xyzw %0, %2, %1x" : "=j"(al) : "j"(d), "j"(n));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(ac) : "j"(v), "j"(al));
    return ac;
}
static inline qword QwAddScaled(qword qwBase, qword qwVec, VU_FLOAT gScale, const void *p)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(qwBase), "j"(qwVec), "j"(gScale.qw), "r"(p)); /* "r"(p): allocation-only, keeps the j-loop DZ pointer as the giv (target lwc1 0xC(a0), a0 += 0x28) */
    return qw;
}
static inline VU_FLOAT VuFloatFromMemWord(const int *pn)
{
    VU_FLOAT v;
    __asm__("qmtc2 %1, %0" : "=j,j"(v.qw) : "r,V"(*pn)); /* never-chosen "V" alternative: allocation-only (the word stays a MEM until reload) */
    return v;
}
static inline qword QwDivX(qword v, VU_FLOAT g)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, $Q" : "=j"(qw) : "j"(v), "j"(g.qw));
    return qw;
}
static inline float SqrtF(float g) { float r; __asm__ __volatile__("sqrt.s %0, %1" : "=f"(r) : "f"(g)); return r; }
union VEC4 { qword q; float f[4]; };

void SolveDzgFric(DZG *pdzg, int grf)
{
    for (;;) {
        func_00158988(pdzg, grf);
        int iBest = -1;
        float gBest = 0.01f;
        SO *psoBest = 0;
        VEC4 vBest;
        vBest.f[2] = 0.0f;
        vBest.f[1] = 0.0f;
        vBest.f[0] = 0.0f;
        for (int i = 0; i < pdzg->cso; i++) {
            DZ *pdz = &pdzg->adz[i];
            if (pdz->pdzOther)
                continue;
            if (pdz->gC < 0.0001f)
                continue;
            SO *pso = pdzg->apso[i];
            if (pso->g94 < 0.0001f)
                continue;
            VEC4 v;
            v.q = pso->vB0;
            for (int j = 0; j < pdzg->cdz; j++) {
                DZ *pdzJ = &pdzg->adz[j];
                if (pdzJ->gC != 0.0f) {
                    VU_FLOAT s = VuFloat(pdzJ->gC);
                    v.q = QwAddScaled(v.q, pdzg->af[j].av[i], s, pdzJ);
                }
            }
            v.q = QwProjectOut(v.q, pso->normal);
            float g = GFromVf(QwDot(v.q, v.q));
            if (gBest < g) {
                vBest.q = v.q;
                gBest = g;
                iBest = i;
                psoBest = pso;
            }
        }
        if (!psoBest)
            break;
        DZ *pdz = &pdzg->adz[pdzg->cdz];
        DZ *pdzA = &pdzg->adz[iBest];
        F *pf = &pdzg->af[pdzg->cdz];
        pdz->pdzOther = pdzA;
        pdzA->pdzOther = pdz;
        pdz->g10 = -SqrtF(gBest);
        pf->pso = psoBest;
        pf->v = QwDivX(vBest.q, VuFloatFromMemWord((const int *)&pdz->g10));
        pdz->x0 = 1;
        if (psoBest->k9C == 14) {
            ZZ *pzz = psoBest->palo->pzz;
            float gLimit;
            if (grf & 1) {
                gLimit = -pzz->g88 * pzz->gD0 * D_002DDED0.dt;
                float g = __builtin_fabsf(pdz->pdzOther->gC);
                pdz->g8 = psoBest->g94 * (g < gLimit ? gLimit : g);
            } else {
                float gD0 = pzz->gD0;
                gLimit = gD0 * D_002BDDE8;
                float g = psoBest->g94 * __builtin_fabsf(pdz->pdzOther->gC);
                pdz->g8 = g < gLimit ? g : gLimit;
            }
        } else {
            pdz->g8 = __builtin_fabsf(pdz->pdzOther->gC) * psoBest->g94;
        }
        pdz->g4 = -pdz->g8;
        func_00158668(pdzg, pdz, pf);
    }
}
