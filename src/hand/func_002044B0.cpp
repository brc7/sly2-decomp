// func_002044B0: a STEP's contact-velocity adjustment (vtable slot 56; Sly 1 has AdjustStepXpVelocity(STEP *, XP *, int)).
// After the base version (func_001F4FC0), for a kind-0xE contact: the closing speed along the contact normal is smoothed
// with GSmooth (func_0011D018) toward two limits (SMPs D_002C3C98 / D_002C3CA8), the change is limited to dt * 9000 up and
// dt * -4500 down, and this side's velocity gets normal * -speed plus (matTarget * vUnk190 without its part along the
// normal) * gUnk180. Finally the contact distance moves by gUnk210 - gUnk248.
// Matched with one asm statement per VU0 operation, no register pins: the local pmat gives the target's pointer/row registers,
// the two smoothing outputs are separate address-taken floats (frame slots after the VU_FLOAT home at 0($sp)).
typedef int qword __attribute__((mode(TI)));

struct VU_FLOAT
{
    qword qw;
};

struct M3
{
    qword mat[3];
};

/* one side of a contact: 0x40 bytes, its velocity at +0x30 */
struct XPD
{
    char pad0[0x30];
    qword v;
};

struct XP
{
    struct XPD axpd[2];
    qword normal;
    float gDist;
    int unk94[2];
    int xpk;
};

struct STEP
{
    char pad0[0x180];
    float gUnk180;
    char pad184[0xC];
    qword vUnk190;
    char pad1a0[0x10];
    struct M3 matTarget;
    char pad1e0[0x30];
    float gUnk210;
    char pad214[0x34];
    float gUnk248;
};

struct CLOCK
{
    float t;
    float dt;
};

extern struct CLOCK D_002DDED0;
extern float D_002C3C98[];
extern float D_002C3CA8[];

void func_001F4FC0(struct STEP *pstep, struct XP *pxp, int ixpd);
float func_0011D018(float gCur, float gTarget, float dt, float *psmp, float *pdgNext);

static inline qword QwSub(qword a, qword b)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(a), "j"(b));
    return qw;
}

static inline qword QwDot(qword a, qword b)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(a), "j"(b));
    return qw;
}

static inline float FloatOfVf(qword qw)
{
    float g;
    __asm__("mtc1 %1, %0" : "=f"(g) : "r"(qw));
    return g;
}

static inline qword QwRotate(struct M3 *pmat, qword v)
{
    qword qw;
    __asm__("vmulax.xyz $ACC, %2, %1x\n\tvmadday.xyz $ACC, %3, %1y\n\tvmaddz.xyz %0, %4, %1z"
            : "=j"(qw) : "j"(v), "j"(pmat->mat[0]), "j"(pmat->mat[1]), "j"(pmat->mat[2]));
    return qw;
}

static inline qword QwScale(qword v, qword s)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(v), "j"(s));
    return qw;
}

static inline struct VU_FLOAT VuFloat(float g)
{
    struct VU_FLOAT vu;
    __asm__("qmtc2.ni %1, %0" : "=j"(vu.qw) : "r"(g));
    return vu;
}

static inline struct VU_FLOAT VuFloatFromMemWord(int n)
{
    struct VU_FLOAT vu;
    __asm__("qmtc2.ni %1, %0" : "=j"(vu.qw) : "rV"(n));
    return vu;
}

static inline qword QwAddScaled2(qword v, qword a, qword sa, qword b, qword sb)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %3x\n\tvmaddx.xyzw %0, %4, %5x"
            : "=j"(qw) : "j"(v), "j"(a), "j"(sa), "j"(b), "j"(sb));
    return qw;
}

/** @sly2-annot addr=0x2044B0
 * Purpose: STEP vt57: for base contacts (kind 0xE) smooth closing speed (GSmooth D_002C3C98/D_002C3CA8)
 *   limited by dt*9000/-4500 and add the desired tangential velocity matTarget*v190*g180
 * Info: proposed name AdjustStepXpVelocity (T1); class STEP slot57; TU step
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
extern "C" void func_002044B0(struct STEP *pstep, struct XP *pxp, int ixpd)
{
    func_001F4FC0(pstep, pxp, ixpd);
    if (pxp->xpk != 0xE)
        return;

    float gVn = FloatOfVf(QwDot(QwSub(pxp->axpd[0].v, pxp->axpd[1].v), pxp->normal));
    float gVnMin;
    float gVnTarget;
    func_0011D018(pxp->gDist, pstep->gUnk210, D_002DDED0.dt, D_002C3C98, &gVnMin);
    func_0011D018(pxp->gDist, 0.0f, D_002DDED0.dt, D_002C3CA8, &gVnTarget);
    gVn = gVn > gVnMin ? gVn : gVnMin;
    if (gVn < gVnTarget)
    {
        float dg = gVnTarget - gVn;
        float dgMax = D_002DDED0.dt * 9000.0f;
        gVn += dg < dgMax ? dg : dgMax;
    }
    else
    {
        float dg = gVnTarget - gVn;
        float dgMin = D_002DDED0.dt * -4500.0f;
        gVn += dg > dgMin ? dg : dgMin;
    }

    struct XPD *pxpd = &pxp->axpd[ixpd];
    struct M3 *pmat = &pstep->matTarget;
    qword v = QwRotate(pmat, pstep->vUnk190);
    qword normal = pxp->normal;
    qword vT = QwSub(v, QwScale(normal, QwDot(v, normal)));
    struct VU_FLOAT vuVn = VuFloat(-gVn);
    struct VU_FLOAT vuK = VuFloatFromMemWord(*(int *)&pstep->gUnk180);
    pxpd->v = QwAddScaled2(pxpd->v, normal, vuVn.qw, vT, vuK.qw);
    pxp->gDist += pstep->gUnk210 - pstep->gUnk248;
}
