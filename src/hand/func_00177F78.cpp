// func_00177F78 (Claude agent, T1690, 2026-10-25): MATCH. Curve follower step: closest point on the curve(s), steering
// velocity toward the next point, avoidance of other bodies (loop), integration and collision clamp.
// Built on lilly15 (aligned 11, T1686). What finished it (each measured, see notes/170000.md):
//  - accel = VecCombo(VU_FLOAT(&D_002BE658), vGoal - p->v, VU_FLOAT(&D_002BE65C), next - p->pos): the game's own
//    VecCombo (0x258480, matched as src/hand/VecCombo__FG8VU_FLOATG9VU_VECTORT0T1.cpp) inlined with by-value params and
//    the weight evaluated first. No "memory" clobber is needed; the old QwBlendF(dv, w1, dp, w2) order could not give
//    the target lqc2 p->v / lui / lw / vsub / qmtc2 / lqc2 p->pos / sqc2 / lui schedule.
//  - dpos = QwSub(posActor, posOther) (plain, not the VSub VV wrapper): the posActor/pctx spill slots 0x90/0xA0.
//  - dirNow length through QwLength2 (the vmul.xyz reads its input as two "j" operands, allocation only): the extra
//    refs give the VLength copy a higher global.c priority than the projected dpos, so the copy takes vf5 and dpos vf7.
// Earlier keys (lilly15):
//  - func_001E7380 takes the float right after the position (pcrv, pos, s, pv, ...): arg-move order f12 before a2.
//  - Top and dirNow normalizes as `NormalizeSafeN(VV v, VV fallback)` / `DivSafe(v, len, fallback)` (return style,
//    by-value VV fallback): the fallback lq/qmtc2 lands in the branch delay slots with the target registers.
//  - "rV" on the VU_FLOAT(const float *) qmtc2 input: the lw reuses the dying lui register (reload-made load).
//  - The two `1.0f - x` sites share ONE float g (the target keeps both results in f5); site 1 is written
//    `gFar = 1.0f; gFar -= x; g = gFar;` so loop.c does not combine and hoist the two 1.0f loads.
//  - Loop helpers take/return the VV class by value (VSub, VAddScaled, VProjectOut, VCross, VDot, VLength): the extra
//    16-byte copies raise the loop to ~339 RTL insns, so loop.c no longer hoists hi(D_002BE664); that drops a0 from the
//    reload spill set and the later reload registers (a2/v0/v1) match.
//  - plain-C GMax (`a < b ? b : a`) instead of a max.s asm; qmfc2 asm for the GPR copy of vGoal (fInit store).
// One register pin: qwActor (s2) - the target copies the spilled posActor into s2 with a plain lq in a delay slot,
// which an asm copy cannot give (reorg stops at asms) and a plain copy is propagated away (without it: 285 differ).
typedef int qword __attribute__((mode(TI)));
union VEC
{
    qword qw;
    float f[4];
};
struct VTENT
{
    short delta, pad;
    void *pfn;
};
struct CURVE
{
    char pad[0xE4];
    int fWrap;
};
struct BODY;
struct CONTEXT
{
    char pad[0x118];
    BODY *pbodyFirst;
};
struct BODY
{
    void *pobj;
    float dz;
    float sRadius;
    float gMass;
    CONTEXT *pctx;
    char pad14[0x4C];
    BODY *pbodyNext;
};
struct GROUP
{
    char pad[0xDC];
    BODY *pbody;
};
struct OBJECT
{
    char pad[0xC];
    VTENT *pvt;
    char pad10[0xD8];
    GROUP *pgroup;
    char padEC[0xC4];
    qword axis;
    char pad1C0[0xD4];
    CURVE *pcrvPrimary;
};
struct S
{
    char pad[0x10];
    OBJECT *pactor;
    char pad14[0x1C];
    CURVE *pcrv;
    float s;
    int fReverse;
    int fHold;
    int fNoCollide;
    char pad44[8];
    int mode;
    char pad50[4];
    CURVE *pcrvExtra;
    int modeExtra;
    float sExtra;
    char pad60[0x10];
    float sSeparation;
    char pad74[0xC];
    qword pos;
    qword v;
    int fInit;
};
struct CLQ
{
    float g0, g1, g2;
};
struct LM
{
    float gMin, gMax;
};
struct CLOCK
{
    float t, dt;
};
extern "C" {
extern char D_003DD020[];
extern CLOCK D_002DDED0;
extern qword D_002A2DA0, D_002A2DC0, D_002A2DD0;
extern float D_002BE630, D_002BE634, D_002BE658, D_002BE65C, D_002BE660, D_002BE664, D_002BE668, D_002BE66C, D_002BE670, D_002BE674, D_002BE690;
extern CLQ D_002BE640, D_002BE680;
extern LM D_002BE650, D_002BBB60;
qword func_001A62E8(void *pobj);
float func_001E66C8(CURVE *pcrv, float s);
float func_001E6648(CURVE *pcrv);
void func_001E7380(CURVE *pcrv, qword pos, float s, void *pv, VEC *ppos, VEC *pdir, float *pg, float *ps);
void func_001E6F10(CURVE *pcrv, float s, VEC *ppos, VEC *pdir);
int func_00178D00(S *p);
void FindClosestPointOnLineSegment(qword a, qword b, qword c, VEC *pv, float *pg0, float *pg1);
int func_001C0B50(CONTEXT *pctx, qword pos, VEC *pv);
}
struct VU_FLOAT
{
    qword qw;
    VU_FLOAT(float g) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "r"(g)); }
    VU_FLOAT(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "rV"(*(const int *)pg)); }

};
static inline float GXOfQw(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw));
    return x;
}
static inline float GXOfQwJ(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw)); /* allocation-only "j"(qw): the vector stays in a VF register */
    return x;
}
static inline float GXOfQwA0A1(qword qw)
{
    float x;
    __asm__("mtc1 %1, %0" : "=f"(x) : "r"(qw), "j"(qw) : "$4", "$5"); /* allocation-only "j"(qw); clobbers of the dead a0/a1 */
    return x;
}
struct VUF
{
    qword qw;
    VUF(const float *pg) { __asm__("qmtc2.ni %1, %0" : "=j"(qw) : "rV"(*(const int *)pg)); }
};
static inline qword QwSub(qword qw0, qword qw1)
{
    qword qw;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(qw) : "j"(qw0), "j"(qw1));
    return qw;
}
static inline qword QwDot(const qword &a, const qword &b)
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
static inline qword QwLength(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v));
    qword r;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(qw));
    return r;
}
/* length of v; copy receives v itself (allocation-only tied output, never written); the "r"(v) operands load v through a GPR */
#define QW_LENGTH_COPY(len, copy, v) \
    { \
        qword qwSq_, vfOne_; \
        __asm__("vmul.xyz %0, %3, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" \
                : "=j"(qwSq_), "=j"(vfOne_), "=j"(copy) : "2"(v), "r"(v), "r"(v)); \
        __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(len) : "j"(qwSq_)); \
    }
static inline qword QwLengthXY(qword v)
{
    qword qw;
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(qw) : "j"(v));
    __asm__("vsqrt $Q, %0x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(qw) : "0"(qw));
    return qw;
}
static inline qword QwDivLength(qword v, qword qwLen)
{
    qword qw;
    __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %2, $Q" : "=j"(qw) : "j"(qwLen), "j"(v));
    return qw;
}
struct VV
{
    qword q;
    VV() {}
    VV(const qword &v) { q = v; }
    operator qword() const { return q; }
};
static inline VV NormalizeSafeN(VV v, VV fallback)
{
    qword len = QwLength(v);
    if (GXOfQw(len) < 0.0001f)
        return fallback;
    return QwDivLength(v, len);
}
static inline VV DivSafe(VV v, VV len, VV fallback)
{
    if (GXOfQw(len) < 0.0001f)
        return fallback;
    return QwDivLength(v, len);
}
static inline qword QwScaleX(qword v, const VU_FLOAT &g)
{
    qword qw;
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(qw) : "j"(v), "j"(g.qw));
    return qw;
}
static inline qword QwAddScaled(qword a, qword b, const VU_FLOAT &g)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddx.xyzw %0, %2, %3x" : "=j"(qw) : "j"(a), "j"(b), "j"(g.qw));
    return qw;
}
static inline qword QwIntegrate(qword pos, qword v, const VU_FLOAT &g1, qword a, const VU_FLOAT &g2)
{
    qword qw;
    __asm__("vadda.xyzw $ACC, %1, $vf0\n\tvmaddax.xyzw $ACC, %2, %3x\n\tvmaddx.xyzw %0, %4, %5x"
            : "=j"(qw) : "j"(pos), "j"(v), "j"(g1.qw), "j"(a), "j"(g2.qw));
    return qw;
}
static inline qword QwCross(qword a, qword b)
{
    qword qw;
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(qw) : "j"(a), "j"(b));
    return qw;
}
static inline float GZOfQw(qword qw)
{
    float z;
    __asm__("pextuw %0, $zero, %1" : "=r"(z) : "r"(qw));
    return z;
}
static inline float GEvaluateClqLm(CLQ *pclq, LM *plm, float x)
{
    float g = pclq->g0 + x * (pclq->g1 + x * pclq->g2);
    float gMin = plm->gMin;
    float gMax = plm->gMax;
    if (g < gMin)
        g = gMin;
    else if (gMax < g)
        g = gMax;
    return g;
}
static inline float GClamp(float g, float gMin, float gMax)
{
    if (g < gMin)
        return gMin;
    if (gMax < g)
        return gMax;
    return g;
}
static inline float GMax(float a, float b)
{
    return a < b ? b : a;

}
static inline qword QwVelocity(void *pv)
{
    OBJECT *pobj = (OBJECT *)pv;
    VTENT *pvte = &pobj->pvt[33];
    return ((qword (*)(void *))pvte->pfn)((char *)pobj + pvte->delta);
}

static inline VV VSub(VV a, VV b) { return QwSub(a, b); }
static inline VV VAddScaled(VV a, VV b, const VU_FLOAT &g) { return QwAddScaled(a, b, g); }
static inline VV VProjectOut(VV v, VV n) { return QwProjectOut(v, n); }
static inline VV VCross(VV a, VV b) { return QwCross(a, b); }
static inline VV VDot(VV a, VV b) { return QwDot(a.q, b.q); }
static inline VV VLength(VV v) { return QwLength(v); }
static inline qword QwLength2(qword v)
{
    qword qw;
    qword vfOne;
    __asm__("vmul.xyz %0, %2, %3\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z"
            : "=j"(qw), "=j"(vfOne) : "j"(v), "j"(v));
    qword r;
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(r) : "j"(qw));
    return r;
}
static inline VV VLength2(VV v) { return QwLength2(v); }
struct VU_VECTOR
{
    qword q;
    VU_VECTOR(const qword &v) { q = v; }
};
static inline qword VecCombo(VUF f1, VU_VECTOR v1, VUF f2, VU_VECTOR v2)
{
    qword r;
    __asm__("vmulax.xyzw $ACC, %2, %1x\n\tvmaddx.xyzw %0, %4, %3x" : "=j"(r) : "j"(f1.qw), "j"(v1.q), "j"(f2.qw), "j"(v2.q));
    return r;
}
extern "C" void func_00177F78(S *p)
{
    VEC point, dir, next, nextDir;
    const qword *pqwZero;
    qword posActor = func_001A62E8(p->pactor);
    register qword qwActor asm("$18") = posActor; /* the one register pin: the target reloads the spilled posActor into s2 (lq in a delay slot) */

    BODY *pbody = 0;
    if (p->pactor->pgroup)
        pbody = p->pactor->pgroup->pbody;
    CONTEXT *pctx = 0;
    if (pbody)
        pctx = pbody->pctx;
    int fReverse = p->fReverse;
    float s;
    if (fReverse)
        s = p->s - D_002BE634;
    else
        s = p->s + D_002BE634;
    float sCurve = func_001E66C8(p->pcrv, s);
    func_001E7380(p->pcrv, qwActor, sCurve, D_003DD020, &point, &dir, 0, &p->s);
    if (p->mode == 1 || p->mode == 4)
    {
        fReverse = p->modeExtra != 2;
        if (fReverse)
            s = p->sExtra - D_002BE634;
        else
            s = p->sExtra + D_002BE634;
        float sCurve = func_001E66C8(p->pcrvExtra, s);
        func_001E7380(p->pcrvExtra, qwActor, sCurve, D_003DD020, &point, &dir, 0, &p->sExtra);
    }
    dir.qw = NormalizeSafeN(QwProjectOut(dir.qw, D_002A2DC0), D_002A2DA0);
    p->sSeparation = GXOfQw(QwLengthXY(QwSub(point.qw, posActor)));
    float ds = GEvaluateClqLm(&D_002BE640, &D_002BE650, p->sSeparation);
    if (fReverse)
        ds = -ds;
    float gSpeed;
    if (func_00178D00(p))
    {
        gSpeed = 0.0f;
        nextDir.qw = D_002A2DA0;
        next.qw = posActor;
    }
    else
    {
        gSpeed = D_002BE630;
        switch (p->mode)
        {
        case 0:
        case 2:
        case 3:
            if (p->pcrv == p->pactor->pcrvPrimary)
            {
                float sMin = -p->s;
                float sMax = func_001E6648(p->pcrv) - p->s;
                float dsClamp = GClamp(ds, sMin, sMax);
                if (ds != dsClamp)
                    gSpeed = 0.0f;
                next.qw = QwAddScaled(point.qw, dir.qw, VU_FLOAT(dsClamp));
                nextDir.qw = dir.qw;
            }
            else
            {
                float sNext;
                if (p->pcrv->fWrap)
                {
                    sNext = func_001E66C8(p->pcrv, p->s + ds);
                }
                else
                {
                    float sMin = -p->s;
                    float sMax = func_001E6648(p->pcrv) - p->s;
                    float dsClamp = GClamp(ds, sMin, sMax);
                    if (ds != dsClamp)
                        gSpeed = 0.0f;
                    sNext = p->s + dsClamp;
                }
                func_001E6F10(p->pcrv, sNext, &next, &nextDir);
            }
            break;
        case 1:
        case 4:
            next.qw = QwAddScaled(point.qw, dir.qw, VU_FLOAT(ds));
            nextDir.qw = dir.qw;
            break;
        }
    }
    if (fReverse)
        nextDir.qw = QwScaleX(nextDir.qw, VU_FLOAT(-1.0f));
    qword vGoal = QwScaleX(nextDir.qw, VU_FLOAT(gSpeed));
    qword vGoalCopy;
    __asm__("qmfc2.ni %0, %1" : "=r"(vGoalCopy) : "j"(vGoal)); /* GPR copy of vGoal */

    if (p->fInit)
    {
        p->v = vGoalCopy;
        p->pos = next.qw;
        p->fInit = 0;
    }
    qword accel = VecCombo(VUF(&D_002BE658), QwSub(vGoal, p->v), VUF(&D_002BE65C), QwSub(next.qw, p->pos));
    if (!p->fNoCollide)
    {
        BODY *pbodySelf = p->pactor->pgroup->pbody;
        for (BODY *pbodyOther = pctx->pbodyFirst; pbodyOther; pbodyOther = pbodyOther->pbodyNext)
        {
            if (pbodyOther->sRadius == 0.0f)
                continue;
            if (pbodyOther->gMass == 0.0f)
                continue;
            void *pobj = pbodyOther->pobj;
            if (pobj == p->pactor)
                continue;
            VEC posOther;
            VEC vecUnused;
            posOther.qw = func_001A62E8(pobj);
            posOther.f[2] += pbodyOther->dz;
            qword dpos = QwSub(posActor, posOther.qw);
            if (__builtin_fabsf(GZOfQw(dpos)) > D_002BE660)
                continue;
            qword vOther = QwVelocity(pobj);
            qword posPredict = VAddScaled(posOther.qw, vOther, VU_FLOAT(&D_002BE664));
            dpos = VProjectOut(dpos, D_002A2DC0);

            qword dposLater = VProjectOut(VSub(p->pos, posPredict), D_002A2DC0);

            VEC normal;
            FindClosestPointOnLineSegment(D_002A2DD0, dpos, dposLater, &normal, 0, 0);
            const qword *pqwZeroHere = &D_002A2DA0;
            qword normalCopy;
            qword qwLen;
            QW_LENGTH_COPY(qwLen, normalCopy, normal.qw);
            if (GXOfQw(qwLen) < 0.0001f)
                normal.qw = *pqwZeroHere;
            else
                normal.qw = QwDivLength(normalCopy, qwLen);
            float sGap = GXOfQwJ(qwLen);
            sGap = GMax(0.0f, sGap - pbodyOther->sRadius - pbodySelf->sRadius);
            float gMass = pbodyOther->gMass / (pbodyOther->gMass + pbodySelf->gMass);
            float g;
            if (sGap < D_002BE668)
            {
                float gFar = 1.0f;
                gFar -= sGap / D_002BE668;
                g = gFar;

                accel = VAddScaled(accel, normal.qw, VU_FLOAT(gMass * g * D_002BE66C));
            }
            if (sGap < D_002BE670)
            {
                g = 1.0f - sGap / D_002BE670;


                pqwZero = &D_002A2DA0;
                qword dirNow = DivSafe(dpos, VLength2(dpos), *pqwZero);

                g *= GEvaluateClqLm(&D_002BE680, &D_002BBB60, GXOfQw(VDot(dirNow, p->pactor->axis)));
                if (g == 0.0f)
                    continue;
                VEC side;
                side.qw = VCross(D_002A2DC0, nextDir.qw);
                if (p->pcrv != p->pactor->pcrvPrimary && GXOfQw(VDot(normal.qw, side.qw)) < 0.0f)
                    g = -g;
                accel = VAddScaled(accel, VCross(normal.qw, D_002A2DC0), VU_FLOAT(gMass * g * D_002BE674));
            }
        }
    }
    float half = 0.5f;
    float dt = D_002DDED0.dt;
    accel = QwProjectOut(accel, D_002A2DC0);
    qword posNext = QwIntegrate(p->pos, p->v, VU_FLOAT(dt), accel, VU_FLOAT(dt * (dt * half)));
    qword vNew = QwAddScaled(p->v, accel, VU_FLOAT(dt));
    qword vNext = vNew;
    if (!p->fNoCollide)
    {
        VEC posHit;
        VEC vecUnused2;
        if (func_001C0B50(pctx, posNext, &posHit))
        {
            VEC normal;
            pqwZero = &D_002A2DA0;
            normal.qw = QwProjectOut(QwSub(posHit.qw, posNext), D_002A2DC0);
            qword qwLen = QwLength(normal.qw);
            if (GXOfQw(qwLen) < 0.0001f)
                normal.qw = *pqwZero;
            else
                normal.qw = QwDivLength(normal.qw, qwLen);
            if (D_002BE690 < GXOfQwA0A1(qwLen))
            {
                float gClose = GXOfQwJ(QwDot(vNew, normal.qw));
                posNext = posHit.qw;
                if (gClose < 0.0f)
                    vNext = QwAddScaled(vNew, normal.qw, VU_FLOAT(-gClose));
            }
        }
    }
    p->pos = posNext;
    p->v = vNext;
}
