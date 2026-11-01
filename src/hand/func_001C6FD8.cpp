// func_001C6FD8 (wave-3 lane 10): spray up to cRequested objects of a kind (0x5B / 0x5D, made by func_001C6CE8) out of a
// point. The seed is dropped to the ground (posOrig), snapped onto the region (posSnap, height from posOrig or from the
// nearest object) and raised again; then up to 50 candidate landing points are tried on a widening spiral around posSnap.
// A candidate is used when it is blocked from posSnap and its sideways-pushed partner, and a landing spot is found near it
// among the objects in the box around posSnap; the object is launched ballistically from posOrig to land there after a
// time from func_00196928 (staggered per object). Objects that found no spot are put back into state 2.
// Matching notes (found on the -da dumps):
//  * PvAllocFrame keeps its ALLOCREQ in the inline's own block, so the slot is freed and posMin reuses it (sp+0x30) like
//    the game; posLand is declared in the loop body after the first temporaries so it shares sp+0x50 with them.
//  * The z of posSnap is stored in both arms (cross-jumped later): sched1 then sees the join block without the store, which
//    gives the game's load order, and cse1 no longer carries &posSnap into the else arm (gcse makes the $a2/$s1 copy).
//  * The box corners: posMin first (its VU_FLOAT temporary is stored to sp+0x50 first); AddScaled takes (VV base, scalar,
//    vector), so the scalar is converted before the Normalize branch and the base is a separate pseudo (the vmove).
//  * Scale lists the dot product first (local-alloc ties the projection to it); GClamp passes both bounds by value.
//  * posBase is read before the SetVectorCylind call, so it is a spilled pseudo at sp+0x80 like the game.
typedef int qword __attribute__((mode(TI)));
union VECTOR4 { qword q; float f[4]; };
struct CLQ { float g0, g1, g2, gUnused; };
struct LM { float gMin, gMax; };
struct FRAME { int n; FRAME(); ~FRAME(); };
struct ALLOCREQ { int cb, cbAlign; ALLOCREQ(int c, int a) { cb = c; cbAlign = a; } };
struct REGION;
struct LINK { char pad0[0x10]; REGION *pregion; };
struct EXT { char pad0[0xDC]; LINK *plink; };
struct CANDIDATE;
struct BASIC {
    void *pv0, *pv4, *pv8;
    virtual void V01(); virtual void V02(); virtual void V03(); virtual void V04(); virtual void V05(); virtual void V06();
};
struct LO : BASIC {
    char pad10[0xE8 - 0x10];
    EXT *pext;                      // 0xE8
    char padEC[0x1D8 - 0xEC];
    float sRadius;                  // 0x1D8
    virtual void V07(); virtual void V08(); virtual void V09(); virtual void V10(); virtual void V11(); virtual void V12();
    virtual void V13(); virtual void V14(); virtual void V15(); virtual void V16(); virtual void V17(); virtual void V18();
    virtual void V19(); virtual void V20(); virtual void V21(); virtual void V22(); virtual void V23(); virtual void V24();
    virtual void V25(); virtual void V26(); virtual void V27(); virtual void V28(); virtual void V29(); virtual void V30();
    virtual void V31(); virtual void V32(); virtual void V33(); virtual void V34(); virtual void V35(); virtual void V36();
    virtual void V37(); virtual void V38(); virtual void V39(); virtual void V40(); virtual void V41(); virtual void V42();
    virtual void V43(); virtual void V44(); virtual void V45(); virtual void V46(); virtual void V47(); virtual void V48();
    virtual void V49(); virtual void V50(); virtual void V51(); virtual void V52(); virtual void V53(); virtual void V54();
    virtual void V55(); virtual void V56(); virtual void V57(); virtual void V58(); virtual void V59(); virtual void V60();
    virtual void V61(); virtual void V62(); virtual void V63(); virtual void V64(); virtual void V65(); virtual void V66();
    virtual void V67(); virtual void V68(); virtual void V69(); virtual void V70(); virtual void V71(); virtual void V72();
    virtual void V73(); virtual void V74(); virtual void V75();
    virtual void SetState(int state);                  // 76
};
extern char D_003DD490[];
extern qword D_002A2DA0, D_002A2DC0, D_002A2DE0, D_002C0D60;
extern float D_002A775C, D_002A7760, D_002C0D70;
extern float D_002C0E80, D_002C0E84, D_002C0E88, D_002C0E8C, D_002C0E90, D_002C0E94;
extern CLQ D_002C0EA0;
extern LM D_002C0EB0;
extern float D_002C0EB8, D_002C0EBC, D_002C0EC0, D_002C0EC4;
extern "C" {
int func_001C6DE8(float, float, float, qword, int, CANDIDATE **, qword *);
LO *PpoHero(void *);
REGION *func_001C0D70(qword, int);
int func_001C0940(REGION *, qword, qword *, int);
int func_001C0980(REGION *, qword, qword, int);
qword func_001A62E8(LO *);
void *func_001AE318(ALLOCREQ *);
int func_001C6CE8(void *, int, int, LO **);
int func_001C5570(void *, CANDIDATE *);
void IntersectSwBoundingBox(void *, qword, qword, int (*)(void *, CANDIDATE *), void *, int *, CANDIDATE ***);
float GRandInRange__Fff(float, float);
float RadNormalize__Ff(float);
qword SetVectorCylind(float, float, float);
float func_00196928(qword, qword, float, float, float, float);
void func_001C6BD0(LO *, qword, qword, float, float);
}
struct VU_FLOAT {
    qword q;
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
static inline void *PvAllocFrame(int cb, int cbAlign) { ALLOCREQ req(cb, cbAlign); return func_001AE318(&req); }
static inline float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static inline float GFromVf(qword v) { float g; __asm__("mtc1 %1,%0" : "=f"(g) : "r"(v)); return g; }
static inline qword Add(qword a, qword b) { qword r; __asm__("vadd.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Dot(qword a, qword b)
{
    qword d, one;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(a), "j"(b));
    return d;
}
static inline qword Length(qword v)
{
    qword d, one, l;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(l) : "j"(d));
    return l;
}
static inline qword LengthXY(qword v)
{
    qword d, l;
    __asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y" : "=j"(d) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(l) : "j"(d));
    return l;
}
static inline qword Div(qword v, qword l) { qword r; __asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "=j"(r) : "j"(v), "j"(l)); return r; }
static inline qword Normalize(qword v)
{
    qword fallback = D_002A2DA0;
    qword l = Length(v);
    if (GFromVf(l) < 0.0001f)
        l = fallback;
    else
        l = Div(v, l);
    return l;
}
static inline qword Scale(qword g, qword v) { qword r; __asm__("vmulx.xyzw %0,%2,%1x" : "=j"(r) : "j"(g), "j"(v)); return r; }
// base + v * g
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
static inline qword AddScaled(VV base, VU_FLOAT g, qword v)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base.q), "j"(v), "j"(g.q));
    return r;
}
// a * ga + b * gb
static inline qword Combine(qword a, VU_FLOAT ga, qword b, VU_FLOAT gb)
{
    qword r;
    __asm__("vmulax.xyzw $ACC,%1,%2x\n\tvmaddx.xyzw %0,%3,%4x" : "=j"(r) : "j"(a), "j"(ga.q), "j"(b), "j"(gb.q));
    return r;
}
static inline float GFromClq(const CLQ &clq, float g) { return clq.g0 + g * (clq.g1 + g * clq.g2); }
static inline float GClampR(float g, float gMin, float gMax) { if (g < gMin) return gMin; if (gMax < g) return gMax; return g; }
static inline float GClamp(float g, const LM &lm) { return GClampR(g, lm.gMin, lm.gMax); }
static inline REGION *PregionFromLo(LO *plo)
{
    EXT *pext = plo->pext;
    LINK *plink = pext ? pext->plink : 0;
    return plink ? plink->pregion : 0;
}
static inline int FFartherThan(float g, float gMax) { return gMax < g; }

extern "C" void func_001C6FD8(void *pworld, int kind, qword posSeed, void *pvUnused, int cRequested)
{
    VECTOR4 posOrig;
    if (!func_001C6DE8(0.0f, 300.0f, D_002A775C, posSeed, 0, 0, &posOrig.q))
        return;
    LO *plo = PpoHero(pworld);
    REGION *pregion = 0;
    if (plo)
        pregion = PregionFromLo(plo);
    if (!pregion)
        pregion = func_001C0D70(posOrig.q, 1);
    VECTOR4 posSnap;
    if (func_001C0940(pregion, posOrig.q, &posSnap.q, 1))
        posSnap.f[2] = posOrig.f[2];
    else {
        qword pos = func_001A62E8(plo);
        if (!func_001C0940(pregion, pos, &posSnap.q, 1))
            return;
        posSnap.f[2] = GetZ(pos);
    }
    if (FFartherThan(GFromVf(LengthXY(Sub(posOrig.q, posSnap.q))), D_002C0E80))
        return;
    if (!func_001C6DE8(-250.0f, 1500.0f, D_002A775C, posSnap.q, 0, 0, &posSnap.q))
        return;
    FRAME frame;
    LO **aplo = (LO **)PvAllocFrame(cRequested * sizeof(LO *), 0x10);
    int clo = func_001C6CE8(pworld, kind, cRequested, aplo);
    if (!clo)
        return;
    float r = D_002C0E84;
    VECTOR4 posMin, posMax;
    posMin.q = AddScaled(posSnap.q, VU_FLOAT(-r), D_002A2DE0);
    posMax.q = AddScaled(posSnap.q, VU_FLOAT(r), D_002A2DE0);
    int ccand;
    CANDIDATE **apcand;
    IntersectSwBoundingBox(D_003DD490, posMin.q, posMax.q, func_001C5570, 0, &ccand, &apcand);
    int cLaunched = 0;
    float gAngle = GRandInRange__Fff(-3.1415927f, 3.1415927f);
    float gRadius = D_002C0E8C;
    for (int iAttempt = 0; cLaunched < clo;) {
        float g = GClamp(GFromClq(D_002C0EA0, iAttempt * 0.02f), D_002C0EB0);
        qword posBase = posSnap.q;
        qword posTrial = Add(posBase, SetVectorCylind(gAngle, gRadius, 0.0f));
        qword dpos = Sub(posTrial, posOrig.q);
        qword dposPerp = Sub(dpos, Scale(Dot(dpos, D_002A2DC0), D_002A2DC0));
        qword posAdjust = AddScaled(posTrial, VU_FLOAT(g), Normalize(dposPerp));
        int fBlocked = !func_001C0980(pregion, posSnap.q, posTrial, 0) && !func_001C0980(pregion, posTrial, posAdjust, 0);
        VECTOR4 posLand;
        if (fBlocked && func_001C6DE8(-300.0f, 300.0f, 0.7f, posTrial, ccand, apcand, &posLand.q)) {
            qword posTarget = AddScaled(posLand.q, VU_FLOAT(aplo[cLaunched]->sRadius - 1.0f), D_002A2DC0);
            float t = func_00196928(Sub(posTarget, posOrig.q), D_002C0D60, D_002C0EB8, D_002C0EBC, D_002C0EC0, D_002C0EC0);
            t += cLaunched * D_002C0EC4;
            qword v = Combine(Sub(posTarget, posOrig.q), VU_FLOAT(1.0f / t), D_002C0D60, VU_FLOAT(-0.5f * t));
            LO *ploLaunch = aplo[cLaunched++];
            func_001C6BD0(ploLaunch, posOrig.q, v, cLaunched * D_002C0D70, D_002A7760);
        }
        gAngle = RadNormalize__Ff(gAngle + D_002C0E94);
        gRadius += D_002C0E90;
        if (gRadius > D_002C0E84)
            gRadius -= D_002C0E84 - D_002C0E88;
        if (++iAttempt >= 50)
            break;
    }
    for (; cLaunched < clo; cLaunched++)
        aplo[cLaunched]->SetState(2);
}
