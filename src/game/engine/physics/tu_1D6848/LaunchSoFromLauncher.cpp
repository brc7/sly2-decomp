// LaunchSoFromLauncher (wave-3 lane 10): launch an object (pso) from a launcher (plaunch). The launch point is the target point in
// the source's frame, optionally advanced by a predicted time (clqPredict over the distance, clamped to lmPredict), then
// moved by clqOffset(distance) along the direction to it. Depending on the object's class it gets a speed vector (0x52),
// a velocity toward that point at its own launch speed plus a matching orientation (0x51), or a ballistic launch velocity
// (func_001D6848); a random spin from lmSpin; and class 0x50 objects are told their target.
// Matching notes (all found on the -da dumps):
//  * matParent / mat / matPredict reproduce the frame; mat and matPredict live in an inner block, so their slots are freed and
//    every later temporary (VU_FLOAT, VECTOR, VECTOR4) lands at sp+0x40 like the game's dead stores. posStart, posOrigin,
//    posAim, posLaunch and the 0x51 velocity are spilled pseudos (sp+0xC0..0x100, in declaration order).
//  * posOrigin is a VV copy of posStart: passed by value into AddScaled it survives gcse's copy propagation (a plain qword copy
//    would be merged into posStart and lose the second spill slot).
//  * Point takes the vector by value: the parameter copy is loaded first, which gives local-alloc the game's vf1..vf5 order.
//  * One `dist` variable for both lengths (a global pseudo, so the polynomial's multiplies are not tied to it).
//  * The 0x51 direction reads its length through an integer word (qmfc2 + mtc1) and Normalize2 lists posAim twice in the
//    subtraction (allocation-only operand: VU preference, so posAim is spilled rather than kept in $s4).
//  * The spin direction uses NormalizeTo, whose fallback arm comes after the division (goto form), and an explicit qmtc2 of the
//    packed VECTOR, which keeps the packed value in a GPR (the game's dead `sq v1`).
typedef int qword __attribute__((mode(TI)));
union MATRIX4 { qword r[4]; float m[4][4]; };
struct MATRIX3 { qword r[3]; };
struct XF { MATRIX4 mat; MATRIX4 matWorld; };
struct CLQ { float g0, g1, g2, gUnused; };
struct LM { float gMin, gMax; };
struct VECTOR4 { qword q; VECTOR4(const qword &v) { q = v; } VECTOR4(const VECTOR4 &v) { q = v.q; } };
struct SO;
struct BASIC {
    void *pframe, *psidebag, *prefnodeFirst;
    virtual void V01(); virtual void V02(); virtual void V03(); virtual void V04(); virtual void V05(); virtual void V06();
};
struct LO : BASIC {
    virtual void Init(); virtual void Add(); virtual void V09(); virtual void V10(); virtual void V11(); virtual void V12();
    virtual void V13(); virtual void V14(); virtual void V15(); virtual void V16(); virtual void V17(); virtual void V18();
    virtual void V19(); virtual void V20(); virtual void V21(); virtual void V22(); virtual void V23(); virtual void V24();
    virtual void V25(); virtual void V26(); virtual void V27(); virtual void V28(); virtual void V29();
    virtual void RotateToMat(MATRIX3 *pmat);                    // 30
    virtual void V31();
    virtual XF *PxfLocal(MATRIX4 *pmatLocal);                   // 32
    virtual void V33(); virtual void V34();
    virtual void SetVelocity(qword v);                           // 35
    virtual void SetAngularVelocity(qword w);                    // 36
    virtual void V37(); virtual void V38(); virtual void V39(); virtual void V40(); virtual void V41(); virtual void V42();
    virtual void V43(); virtual void V44(); virtual void V45(); virtual void V46(); virtual void V47(); virtual void V48();
};
struct SO : LO {
    char pad10[0x250];
    float svLaunch;                                              // 0x260
    virtual void V49(); virtual void V50(); virtual void V51(); virtual void V52(); virtual void V53(); virtual void V54();
    virtual void V55(); virtual void V56(); virtual void V57(); virtual void V58(); virtual void V59(); virtual void V60();
    virtual void V61(); virtual void V62(); virtual void V63(); virtual void V64(); virtual void V65(); virtual void V66();
    virtual void V67(); virtual void V68(); virtual void V69(); virtual void V70(); virtual void V71(); virtual void V72();
    virtual void V73(); virtual void V74(); virtual void V75(); virtual void V76(); virtual void V77();
    virtual void SetTarget(SO *psoSource, const VECTOR4 &posTarget); // 78
};
struct LAUNCH {
    char pad0[0x50];
    qword posTarget;            // 0x50
    SO *psoSource;              // 0x60
    char pad64[0xC];
    CLQ clqPredict;             // 0x70
    LM lmPredict;               // 0x80
    char pad88[8];
    CLQ clqOffset;              // 0x90
    LM lmOffset;                // 0xA0
    LM lmSpin;                  // 0xA8
    float pitch;                // 0xB0
};
extern XF D_002C44F0;           // identity frame
extern qword D_002A2DA0;        // default direction
extern char D_003DD490[];
extern "C" {
qword func_001A62E8(SO *);                                       // PosLoWorld
MATRIX4 *__ml__FRC7MATRIX4T0(MATRIX4 *, MATRIX4 *, MATRIX4 *);
void *func_002333B4(void *, int, int);                           // memset
void LoadIdentityMatrix4(XF *, int, MATRIX4 *, MATRIX4 *, float);
int FIsBasicDerivedFrom__FP5BASIC3CID(SO *, int);                                    // FIsBasicDerivedFrom
void func_0013BC60(SO *, LAUNCH *);
void RecalcSwOxfFilterForObject(void *, SO *);
void SetSoCnstrForce(SO *, void *);
void SetSoCnstrTorque(SO *, void *);
void SetLoParent_1A58D0(SO *, int, int);
void func_001F4898(SO *, qword);
qword GetSoDvGravity(SO *);
qword func_001D6848(float, qword, qword, qword);                 // CalculateLaunchVelocity
void LoadLookAtMatrix(MATRIX3 *, qword);
float GRandInRange__Fff(float, float);                               // GRandInRange
}
static inline float GRandInRange(const LM &lm) { return GRandInRange__Fff(lm.gMin, lm.gMax); }
struct VV { qword q; VV() {} VV(const qword &v) { q = v; } operator qword() const { return q; } };
struct VU_FLOAT {
    qword q;
    VU_FLOAT() {}
    VU_FLOAT(float f) { __asm__("qmtc2.ni %1,%0" : "=j"(q) : "r"(f)); }
};
// a VU float straight from a float in memory (loaded as an integer word)
static inline VU_FLOAT VuFloatFromWord(const float &f) { VU_FLOAT g; __asm__("qmtc2.ni %1,%0" : "=j"(g.q) : "r"(*(const int *)&f)); return g; }
struct VECTOR {
    qword q;
    VECTOR(float x, float y, float z)
    {
        qword t;
        __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0" : "=&r"(q), "=&r"(t) : "f"(x), "f"(y), "f"(z));
    }
};
static inline qword QwToVf(qword v) { qword r; __asm__("qmtc2.ni %1,%0" : "=j"(r) : "r"(v)); return r; }
static inline qword QwFromVf(qword v) { qword r; __asm__("qmfc2.ni %0,%1" : "=r"(r) : "j"(v)); return r; }
static inline float GFromVf(qword v) { float g; __asm__("mtc1 %1,%0" : "=f"(g) : "r"(v)); return g; }
static inline float GFromVfBits(qword v)
{
    int n;
    float g;
    __asm__("qmfc2.ni %0,%1" : "=r"(n) : "j"(v));
    __asm__("mtc1 %1,%0" : "=f"(g) : "r"(n));
    return g;
}
static inline qword Sub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static inline qword Point(const MATRIX4 &m, qword v)
{
    qword r;
    __asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w"
            : "=j"(r) : "j"(m.r[0]), "j"(m.r[1]), "j"(m.r[2]), "j"(m.r[3]), "j"(v));
    return r;
}
static inline qword Length(qword v)
{
    qword d, one, l;
    __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "=j"(d), "=&j"(one) : "j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q" : "=j"(l) : "j"(d));
    return l;
}
static inline qword Div(qword v, qword l) { qword r; __asm__("vdiv $Q,$vf0w,%2x\n\tvwaitq\n\tvmulq.xyzw %0,%1,$Q" : "=j"(r) : "j"(v), "j"(l)); return r; }
// unit vector along v (computed in place of its length), or the default direction when v is too short
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
// unit vector from posFrom to posTo
static inline qword Normalize2(qword posTo, qword posFrom)
{
    qword v;
    __asm__("vsub.xyzw %0,%1,%2" : "=j"(v) : "j"(posTo), "j"(posFrom), "j"(posTo)); // posTo again: allocation only
    qword fallback = D_002A2DA0;
    qword l = Length(v);
    if (GFromVfBits(l) < 0.0001f)
        l = fallback;
    else
        l = Div(v, l);
    return l;
}
static inline qword NormalizeTo(qword v)
{
    qword fallback = D_002A2DA0;
    qword l = Length(v);
    if (GFromVf(l) < 0.0001f)
        goto tiny;
    return Div(v, l);
tiny:
    return fallback;
}
// base + v * g
static inline qword AddScaled(VV base, VU_FLOAT g, qword v)
{
    qword r;
    __asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x" : "=j"(r) : "j"(base.q), "j"(v), "j"(g.q));
    return r;
}
static inline qword operator*(qword v, VU_FLOAT g) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
static inline qword operator*(VU_FLOAT g, qword v) { qword r; __asm__("vmulx.xyzw %0,%1,%2x" : "=j"(r) : "j"(v), "j"(g.q)); return r; }
static inline float GFromClq(const CLQ &clq, float g) { return clq.g0 + g * (clq.g1 + g * clq.g2); }
static inline float GClamp(float g, float gMin, float gMax) { if (g < gMin) g = gMin; else if (gMax < g) g = gMax; return g; }
static inline float GClampR(float g, float gMin, float gMax) { if (g < gMin) return gMin; if (gMax < g) return gMax; return g; }
static inline void LoadIdentity(MATRIX4 *pm) { func_002333B4(pm, 0, sizeof(*pm)); pm->m[0][0] = pm->m[1][1] = pm->m[2][2] = pm->m[3][3] = 1.0f; }

/** @sly2-annot addr=0x1D6B00
 * Purpose: Launch an object from a launcher: predicted aim point, class-dependent velocity (0x50/0x51/0x52
 *   CIDs), random spin
 * Info: proposed name LaunchSoFromLauncher (T1); TU tu_1D6848
 * Notes: runtime5a #13: darts launched via the launcher CID112 (owner +0x200)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r6_1D5150+runtime
 * See: docs/engine/guards-ai.md#4-combat-ko-and-takedowns,
 *   docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode,
 *   docs/engine/player.md#11-key-functions
 * @end-sly2-annot */
extern "C" void LaunchSoFromLauncher(LAUNCH *plaunch, SO *pso)
{
    MATRIX4 matParent;
    qword posStart = func_001A62E8(pso);
    VV posOrigin(posStart);
    SO *psoSource = plaunch->psoSource;
    XF *pxf;
    if (psoSource)
        pxf = psoSource->PxfLocal(&matParent);
    else {
        pxf = &D_002C44F0;
        LoadIdentity(&matParent);
    }
    qword dpos;
    float dist;
    {
        MATRIX4 mat;
        __ml__FRC7MATRIX4T0(&mat, &pxf->matWorld, &matParent);
        dpos = Sub(Point(mat, plaunch->posTarget), posStart);
        dist = GFromVf(Length(dpos));
        float t = GClamp(GFromClq(plaunch->clqPredict, dist), plaunch->lmPredict.gMin, plaunch->lmPredict.gMax);
        if (t != 0.0f) {
            MATRIX4 matPredict;
            LoadIdentityMatrix4(pxf, 0, &mat, 0, t);
            __ml__FRC7MATRIX4T0(&matPredict, &mat, &matParent);
            dpos = Sub(Point(matPredict, plaunch->posTarget), posStart);
        }
    }
    dist = GFromVf(Length(dpos));
    float offset = GClampR(GFromClq(plaunch->clqOffset, dist), plaunch->lmOffset.gMin, plaunch->lmOffset.gMax);
    qword posAim;
    qword posLaunch = AddScaled(posOrigin, VU_FLOAT(offset), Normalize(dpos));
    posAim = QwFromVf(posLaunch);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x4F)) {
        func_0013BC60(pso, plaunch);
        RecalcSwOxfFilterForObject(D_003DD490, pso);
    }
    pso->Add();
    SetSoCnstrForce(pso, 0);
    SetSoCnstrTorque(pso, 0);
    SetLoParent_1A58D0(pso, 0, 3);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x52)) {
        func_001F4898(pso, VECTOR(pso->svLaunch, 0.0f, 0.0f).q);
    } else if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x51)) {
        qword v = VuFloatFromWord(pso->svLaunch) * Normalize2(posAim, posStart);
        pso->SetVelocity(v);
        MATRIX3 mat;
        LoadLookAtMatrix(&mat, v);
        pso->RotateToMat(&mat);
    } else {
        qword vAccel = GetSoDvGravity(pso);
        qword v = func_001D6848(plaunch->pitch, posStart, posLaunch, vAccel);
        pso->SetVelocity(v);
    }
    if (plaunch->lmSpin.gMax > 0.0f) {
        float g = GRandInRange(plaunch->lmSpin);
        float x = GRandInRange__Fff(-1.0f, 1.0f);
        float y = GRandInRange__Fff(-1.0f, 1.0f);
        float z = GRandInRange__Fff(-1.0f, 1.0f);
        qword w = NormalizeTo(QwToVf(VECTOR(x, y, z).q)) * VU_FLOAT(g);
        pso->SetAngularVelocity(w);
    }
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pso, 0x50))
        pso->SetTarget(plaunch->psoSource, VECTOR4(plaunch->posTarget));
}
